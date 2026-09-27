#include "gcl/importer/grannyimporteranimation_deboor.h"

#include <vector>

namespace GCL::Importer {

GrannyImporterAnimationDeboor::GrannyImporterAnimationDeboor(Bindings::Scene::SharedPtr scene)
    : GrannyImporterAnimation(scene) {
}

void GrannyImporterAnimationDeboor::ImportScaleCurve(
    const Bindings::Track::SharedPtr& track, const GrannyTransformTrack& granny_transform_track
) const {
    auto& granny = GrannyLibrary::instance();

    if (granny.CurveGetDimension(&granny_transform_track.scale_shear_curve) == 0) {
        return;
    }

    GrannyCurve2* scale_curve = granny.CurveConvertToDaK32fC32f(
        &granny_transform_track.scale_shear_curve, granny.CurveIdentityScaleShear()
    );

    const GrannyCurveDataDAK32fC32f* granny_scale_shear_curve =
        static_cast<GrannyCurveDataDAK32fC32f*>(scale_curve->curve_data.object);

    const unsigned granny_knot_count = static_cast<unsigned>(granny_scale_shear_curve->knot_count);

    for (unsigned i = 0; i < granny_knot_count; i++) {
        Bindings::CurveScaleKey key(*scale_curve);
        key.setTime(static_cast<double>(granny_scale_shear_curve->knots[i]));
        key.setValue(FbxDouble3(
            static_cast<double>(granny_scale_shear_curve->controls[(i * 9)]),
            static_cast<double>(granny_scale_shear_curve->controls[(i * 9) + 4]),
            static_cast<double>(granny_scale_shear_curve->controls[(i * 9) + 8])
        ));

        track->addScaleKey(key);
    }

    granny.FreeCurve(scale_curve);
}

void GrannyImporterAnimationDeboor::ImportPositionCurve(
    const Bindings::Animation::SharedPtr& animation,
    const Bindings::Track::SharedPtr& track,
    const GrannyTransformTrack& granny_transform_track
) const {
    auto& granny = GrannyLibrary::instance();

    const float duration = animation->getData()->duration;
    const float time_step = animation->getData()->time_step;

    if (granny.CurveGetDimension(&granny_transform_track.position_curve) == 0) {
        return;
    }

    GrannyCurve2* position_curve = granny.CurveConvertToDaK32fC32f(
        &granny_transform_track.position_curve, granny.CurveIdentityPosition()
    );

    const GrannyCurveDataDAK32fC32f* granny_position_curve =
        static_cast<GrannyCurveDataDAK32fC32f*>(position_curve->curve_data.object);

    const unsigned granny_knot_count = static_cast<unsigned>(granny_position_curve->knot_count);

    if (granny_knot_count == 0) {
        return;
    }

    std::vector<float> knots;
    knots.reserve(granny_knot_count);

    for (unsigned i = 0; i < granny_knot_count; i++) {
        knots.emplace_back(static_cast<float>(i) * (time_step * 1));
    }

    const unsigned control_count = static_cast<unsigned>(granny_position_curve->control_count) / 3;

    std::vector<FbxDouble3> controls;
    controls.reserve(control_count);

    for (unsigned i = 0; i < control_count; i++) {
        controls.emplace_back(
            granny_position_curve->controls[(i * 3)],
            granny_position_curve->controls[(i * 3) + 1],
            granny_position_curve->controls[(i * 3) + 2]
        );
    }

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<double>(duration)) {
        time = static_cast<double>(static_cast<float>(step) * time_step);

        FbxDouble3 position = de_boor_position(
            granny_position_curve->curve_data_header.degree,
            static_cast<float>(time),
            knots,
            controls
        );

        Bindings::CurvePositionKey key(granny_transform_track.position_curve);
        key.setTime(time);
        key.setValue(position);

        track->addPositionKey(key);

        step++;
    }

    granny.FreeCurve(position_curve);
}

void GrannyImporterAnimationDeboor::ImportRotationCurve(
    const Bindings::Animation::SharedPtr& animation,
    const Bindings::Track::SharedPtr& track,
    const GrannyTransformTrack& granny_transform_track
) const {
    auto& granny = GrannyLibrary::instance();

    const float duration = animation->getData()->duration;
    const float time_step = animation->getData()->time_step;

    if (granny.CurveGetDimension(&granny_transform_track.orientation_curve) == 0) {
        return;
    }

    GrannyCurve2* orientation_curve = granny.CurveConvertToDaK32fC32f(
        &granny_transform_track.orientation_curve, granny.CurveIdentityOrientation()
    );

    const GrannyCurveDataDAK32fC32f* granny_orientation_curve =
        static_cast<GrannyCurveDataDAK32fC32f*>(orientation_curve->curve_data.object);

    const unsigned granny_knot_count = static_cast<unsigned>(granny_orientation_curve->knot_count);

    if (granny_knot_count == 0) {
        return;
    }

    std::vector<float> knots;
    knots.reserve(granny_knot_count);

    for (unsigned i = 0; i < granny_knot_count; i++) {
        knots.emplace_back(static_cast<float>(i) * (time_step * 1));
    }

    const unsigned control_count =
        static_cast<unsigned>(granny_orientation_curve->control_count) / 4;

    std::vector<FbxQuaternion> controls;
    controls.reserve(control_count);

    for (unsigned i = 0; i < control_count; i++) {
        controls.emplace_back(
            granny_orientation_curve->controls[(i * 4)],
            granny_orientation_curve->controls[(i * 4) + 1],
            granny_orientation_curve->controls[(i * 4) + 2],
            granny_orientation_curve->controls[(i * 4) + 3]
        );
    }

    unsigned step = 0;
    double time = 0;

    // Set default scale multiply.
    FbxDouble3 scale_multiply{1.0, 1.0, 1.0};

    // Get first scale key as scale multiply for rotation curve.
    // It is required to calculate correct rotation in case
    // that animation track uses scale shear in animation track
    // because fbx does not support scale shear.
    if (!track->getScaleKeys().empty()) {
        scale_multiply =
            (static_cast<Bindings::CurveScaleKey>(track->getScaleKeys().at(0))).getValue();
    }

    while (time < static_cast<double>(duration)) {
        time = static_cast<double>(static_cast<float>(step) * time_step);

        FbxQuaternion quaternion = de_boor_rotation(
            granny_orientation_curve->curve_data_header.degree,
            static_cast<float>(time),
            knots,
            controls
        );

        FbxAMatrix transform_matrix;

        transform_matrix.SetQ(quaternion);

        // Check for negative scale multiply.
        // Fbx can not handle negative scaling in same way as granny2 does handle it.
        // Multiply negative scale with rotation matrix to apply negative scaling
        // also to rotation matrix and just not apply it only to scale matrix.
        if (scale_multiply[0] < 0.0 || scale_multiply[1] < 0.0 || scale_multiply[2] < 0.0) {
            transform_matrix.MultSM(
                FbxVector4(abs(scale_multiply[0]), abs(scale_multiply[1]), abs(scale_multiply[2]))
            );
        }

        Bindings::CurveRotationKey key(*orientation_curve);
        key.setTime(time);
        key.setValue(transform_matrix.GetR());

        track->addRotationKey(key);

        step++;
    }

    granny.FreeCurve(orientation_curve);
}

}  // namespace GCL::Importer
