#include "gcl/importer/grannyimporteranimation_deboor.h"

#include <vector>

namespace GCL::Importer {

GrannyImporterAnimationDeboor::GrannyImporterAnimationDeboor(Bindings::Scene::SharedPtr scene)
    : GrannyImporterAnimation(scene) {
}

Bindings::Track::SharedPtr GrannyImporterAnimationDeboor::importTrack(
    Bindings::Animation::SharedPtr animation, GrannyTransformTrack grannyTransformTrack
) const {
    Bindings::Track::SharedPtr track = std::make_shared<Bindings::Track>(grannyTransformTrack);
    track->setName(grannyTransformTrack.name);

    importScaleCurve(track, grannyTransformTrack);
    importPositionCurve(animation, track, grannyTransformTrack);
    importRotationCurve(animation, track, grannyTransformTrack);

    return track;
}

void GrannyImporterAnimationDeboor::importScaleCurve(
    Bindings::Track::SharedPtr track, GrannyTransformTrack grannyTransformTrack
) const {
    auto& granny = GrannyLibrary::instance();

    if (granny.CurveGetDimension(&grannyTransformTrack.scale_shear_curve) == 0) {
        return;
    }

    GrannyCurve2* scaleCurve = granny.CurveConvertToDaK32fC32f(
        &grannyTransformTrack.scale_shear_curve, granny.CurveIdentityScaleShear()
    );

    const GrannyCurveDataDAK32fC32f* grannyScaleShearCurve =
        static_cast<GrannyCurveDataDAK32fC32f*>(scaleCurve->curve_data.object);

    const unsigned grannyKnotCount = static_cast<unsigned>(grannyScaleShearCurve->knot_count);

    for (unsigned i = 0; i < grannyKnotCount; i++) {
        Bindings::CurveScaleKey key(*scaleCurve);
        key.setTime(static_cast<double>(grannyScaleShearCurve->knots[i]));
        key.setValue(FbxDouble3(
            static_cast<double>(grannyScaleShearCurve->controls[(i * 9)]),
            static_cast<double>(grannyScaleShearCurve->controls[(i * 9) + 4]),
            static_cast<double>(grannyScaleShearCurve->controls[(i * 9) + 8])
        ));

        track->addScaleKey(key);
    }

    granny.FreeCurve(scaleCurve);
}

void GrannyImporterAnimationDeboor::importPositionCurve(
    Bindings::Animation::SharedPtr animation,
    Bindings::Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack
) const {
    auto& granny = GrannyLibrary::instance();

    const float duration = animation->getData()->duration;
    const float timeStep = animation->getData()->time_step;

    if (granny.CurveGetDimension(&grannyTransformTrack.position_curve) == 0) {
        return;
    }

    GrannyCurve2* positionCurve = granny.CurveConvertToDaK32fC32f(
        &grannyTransformTrack.position_curve, granny.CurveIdentityPosition()
    );

    const GrannyCurveDataDAK32fC32f* grannyPositionCurve =
        static_cast<GrannyCurveDataDAK32fC32f*>(positionCurve->curve_data.object);

    const unsigned grannyKnotCount = static_cast<unsigned>(grannyPositionCurve->knot_count);

    if (!grannyKnotCount) {
        return;
    }

    std::vector<float> knots;
    knots.reserve(grannyKnotCount);

    for (unsigned i = 0; i < grannyKnotCount; i++) {
        knots.emplace_back(static_cast<float>(i) * (timeStep * 1));
    }

    const unsigned controlCount = static_cast<unsigned>(grannyPositionCurve->control_count) / 3;

    std::vector<FbxDouble3> controls;
    controls.reserve(controlCount);

    for (unsigned i = 0; i < controlCount; i++) {
        controls.emplace_back(
            grannyPositionCurve->controls[(i * 3)],
            grannyPositionCurve->controls[(i * 3) + 1],
            grannyPositionCurve->controls[(i * 3) + 2]
        );
    }

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<double>(duration)) {
        time = static_cast<double>(static_cast<float>(step) * timeStep);

        auto position = de_boor_position(
            grannyPositionCurve->curve_data_header.degree, static_cast<float>(time), knots, controls
        );

        Bindings::CurvePositionKey key(grannyTransformTrack.position_curve);
        key.setTime(time);
        key.setValue(position);

        track->addPositionKey(key);

        step++;
    }

    granny.FreeCurve(positionCurve);
}

void GrannyImporterAnimationDeboor::importRotationCurve(
    Bindings::Animation::SharedPtr animation,
    Bindings::Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack
) const {
    auto& granny = GrannyLibrary::instance();

    const float duration = animation->getData()->duration;
    const float timeStep = animation->getData()->time_step;

    if (granny.CurveGetDimension(&grannyTransformTrack.orientation_curve) == 0) {
        return;
    }

    GrannyCurve2* orientationCurve = granny.CurveConvertToDaK32fC32f(
        &grannyTransformTrack.orientation_curve, granny.CurveIdentityOrientation()
    );

    const GrannyCurveDataDAK32fC32f* grannyOrientationCurve =
        static_cast<GrannyCurveDataDAK32fC32f*>(orientationCurve->curve_data.object);

    const unsigned grannyKnotCount = static_cast<unsigned>(grannyOrientationCurve->knot_count);

    if (!grannyKnotCount) {
        return;
    }

    std::vector<float> knots;
    knots.reserve(grannyKnotCount);

    for (unsigned i = 0; i < grannyKnotCount; i++) {
        knots.emplace_back(static_cast<float>(i) * (timeStep * 1));
    }

    const unsigned controlCount = static_cast<unsigned>(grannyOrientationCurve->control_count) / 4;

    std::vector<FbxQuaternion> controls;
    controls.reserve(controlCount);

    for (unsigned i = 0; i < controlCount; i++) {
        controls.emplace_back(
            grannyOrientationCurve->controls[(i * 4)],
            grannyOrientationCurve->controls[(i * 4) + 1],
            grannyOrientationCurve->controls[(i * 4) + 2],
            grannyOrientationCurve->controls[(i * 4) + 3]
        );
    }

    unsigned step = 0;
    double time = 0;

    // Set default scale multiply.
    auto scaleMultiply = FbxDouble3(1.0, 1.0, 1.0);

    // Get first scale key as scale multiply for rotation curve.
    // It is required to calculate correct rotation in case
    // that animation track uses scale shear in animation track
    // because fbx does not support scale shear.
    if (!track->getScaleKeys().empty()) {
        scaleMultiply =
            (static_cast<Bindings::CurveScaleKey>(track->getScaleKeys().at(0))).getValue();
    }

    while (time < static_cast<double>(duration)) {
        time = static_cast<double>(static_cast<float>(step) * timeStep);

        FbxQuaternion quaternion = de_boor_rotation(
            grannyOrientationCurve->curve_data_header.degree,
            static_cast<float>(time),
            knots,
            controls
        );

        FbxAMatrix transformMatrix;

        transformMatrix.SetQ(quaternion);

        // Check for negative scale multiply.
        // Fbx can not handle negative scaling in same way as granny2 does handle it.
        // Multiply negative scale with rotation matrix to apply negative scaling
        // also to rotation matrix and just not apply it only to scale matrix.
        if (scaleMultiply[0] < 0.0 || scaleMultiply[1] < 0.0 || scaleMultiply[2] < 0.0) {
            transformMatrix.MultSM(
                FbxVector4(abs(scaleMultiply[0]), abs(scaleMultiply[1]), abs(scaleMultiply[2]))
            );
        }

        Bindings::CurveRotationKey key(*orientationCurve);
        key.setTime(time);
        key.setValue(transformMatrix.GetR());

        track->addRotationKey(key);

        step++;
    }

    granny.FreeCurve(orientationCurve);
}

}  // namespace GCL::Importer
