#include "gcl/importer/grannyimporteranimation.h"

#include "gcl/utilities/logging.h"

namespace GCL::Importer {

using namespace GCL::Utilities::Logging;

GrannyImporterAnimation::GrannyImporterAnimation(Bindings::Scene::SharedPtr scene) : scene_(scene) {
}

void GrannyImporterAnimation::ImportAnimations(GrannyFileInfo* granny_file_info) const {
    info("Load animations from granny file.");

    if (granny_file_info->animation_count == 0) {
        warning("Skip load animations because granny file does not have at least one animation.");
        return;
    }

    GrannyAnimation* const granny_animation = granny_file_info->animations[0];
    auto animation = std::make_shared<Bindings::Animation>(granny_animation);

    // Process animation track groups. Only if animation does have at least one animation track
    // group.
    if (granny_animation->track_group_count != 0) {
        for (int i = 0; i < granny_animation->track_group_count; ++i) {
            GrannyTrackGroup* const granny_track_group = granny_animation->track_groups[i];

            for (int j = 0; j < granny_track_group->transform_track_count; ++j) {
                animation->addTrack(
                    ImportTrack(animation, granny_track_group->transform_tracks[j])
                );
            }
        }
    } else {
        warning(
            "Skip load animations because animation does not have at least one animation track."
        );
    }

    scene_->addAnimation(animation);

    info("Added animation to scene.");
}

Bindings::Track::SharedPtr GrannyImporterAnimation::ImportTrack(
    const Bindings::Animation::SharedPtr& animation,
    const GrannyTransformTrack& granny_transform_track
) const {
    auto track = std::make_shared<Bindings::Track>(granny_transform_track);
    track->setName(granny_transform_track.name);

    ImportScaleCurve(track, granny_transform_track);
    ImportPositionCurve(animation, track, granny_transform_track);
    ImportRotationCurve(animation, track, granny_transform_track);

    return track;
}

void GrannyImporterAnimation::ImportScaleCurve(
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

void GrannyImporterAnimation::ImportPositionCurve(
    const Bindings::Animation::SharedPtr& animation,
    const Bindings::Track::SharedPtr& track,
    GrannyTransformTrack granny_transform_track
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

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<const double>(duration)) {
        time = static_cast<const double>(static_cast<float>(step) * time_step);

        float position[3];

        int position_dimension = granny.CurveGetDimension(position_curve);
        granny.EvaluateCurveAtT(
            position_dimension,
            false,
            true,
            position_curve,
            true,
            duration,
            static_cast<float>(time),
            position,
            granny.CurveIdentityPosition()
        );

        Bindings::CurvePositionKey key(*position_curve);
        key.setTime(time);
        key.setValue(FbxDouble3(
            static_cast<double>(position[0]),
            static_cast<double>(position[1]),
            static_cast<double>(position[2])
        ));

        track->addPositionKey(key);

        step++;
    }

    granny.FreeCurve(position_curve);
}

void GrannyImporterAnimation::ImportRotationCurve(
    const Bindings::Animation::SharedPtr& animation,
    const Bindings::Track::SharedPtr& track,
    GrannyTransformTrack granny_transform_track
) const {
    const float duration = animation->getData()->duration;
    const float time_step = animation->getData()->time_step;

    auto& granny = GrannyLibrary::instance();

    GrannyCurve2* scale_curve = nullptr;

    if (granny.CurveGetDimension(&granny_transform_track.scale_shear_curve) != 0) {
        scale_curve = granny.CurveConvertToDaK32fC32f(
            &granny_transform_track.scale_shear_curve, granny.CurveIdentityScaleShear()
        );
    }

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

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<const double>(duration)) {
        time = static_cast<const double>(static_cast<float>(step) * time_step);

        float scale[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

        if (scale_curve) {
            int scale_dimension = granny.CurveGetDimension(scale_curve);
            granny.EvaluateCurveAtT(
                scale_dimension,
                false,
                true,
                scale_curve,
                true,
                duration,
                static_cast<float>(time),
                scale,
                granny.CurveIdentityScaleShear()
            );
        }

        float quaternion[4];

        int orientation_dimension = granny.CurveGetDimension(orientation_curve);
        granny.EvaluateCurveAtT(
            orientation_dimension,
            false,
            true,
            orientation_curve,
            true,
            duration,
            static_cast<float>(time),
            quaternion,
            granny.CurveIdentityOrientation()
        );

        FbxAMatrix transform_matrix;

        transform_matrix.SetQ(FbxQuaternion(
            static_cast<double>(quaternion[0]),
            static_cast<double>(quaternion[1]),
            static_cast<double>(quaternion[2]),
            static_cast<double>(quaternion[3])
        ));

        // Check for negative scale.
        // Fbx can not handle negative scaling in same way as granny2 does handle it.
        // Multiply negative scale with rotation matrix to apply negative scaling
        // also to rotation matrix and just not apply it only to scale matrix.
        if (scale[0] < 0.0f || scale[4] < 0.0f || scale[8] < 0.0f) {
            transform_matrix.MultSM(FbxVector4(
                static_cast<double>(abs(scale[0])),
                static_cast<double>(abs(scale[4])),
                static_cast<double>(abs(scale[8]))
            ));
        }

        Bindings::CurveRotationKey key(*orientation_curve);
        key.setTime(time);
        key.setValue(transform_matrix.GetR());

        track->addRotationKey(key);

        step++;
    }

    granny.FreeCurve(orientation_curve);
}

double GrannyImporterAnimation::CalculateTime(const double time) const {
    const double frame = 1.0 / 30.0;
    const double min = time / frame;
    const double min_2 = floor(min);

    return min_2 * frame;
}

}  // namespace GCL::Importer
