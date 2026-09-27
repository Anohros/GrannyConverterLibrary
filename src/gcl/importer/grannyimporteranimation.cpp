#include "gcl/importer/grannyimporteranimation.h"

#include "gcl/utilities/logging.h"

namespace GCL::Importer {

using namespace GCL::Utilities::Logging;

GrannyImporterAnimation::GrannyImporterAnimation(Bindings::Scene::SharedPtr scene) : scene_(scene) {
}

void GrannyImporterAnimation::importAnimations(GrannyFileInfo* grannyFileInfo) const {
    info("Load animations from granny file.");

    if (grannyFileInfo->animation_count == 0) {
        warning("Skip load animations because granny file does not have at least one animation.");
        return;
    }

    GrannyAnimation* const grannyAnimation = grannyFileInfo->animations[0];
    auto animation = std::make_shared<Bindings::Animation>(grannyAnimation);

    // Process animation track groups. Only if animation does have at least one animation track
    // group.
    if (grannyAnimation->track_group_count != 0) {
        for (int i = 0; i < grannyAnimation->track_group_count; ++i) {
            auto* const grannyTrackGroup = grannyAnimation->track_groups[i];

            for (int j = 0; j < grannyTrackGroup->transform_track_count; ++j) {
                animation->addTrack(importTrack(animation, grannyTrackGroup->transform_tracks[j]));
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

Bindings::Track::SharedPtr GrannyImporterAnimation::importTrack(
    Bindings::Animation::SharedPtr animation, GrannyTransformTrack grannyTransformTrack
) const {
    auto track = std::make_shared<Bindings::Track>(grannyTransformTrack);
    track->setName(grannyTransformTrack.name);

    importScaleCurve(track, grannyTransformTrack);
    importPositionCurve(animation, track, grannyTransformTrack);
    importRotationCurve(animation, track, grannyTransformTrack);

    return track;
}

void GrannyImporterAnimation::importScaleCurve(
    Bindings::Track::SharedPtr track, GrannyTransformTrack grannyTransformTrack
) const {
    auto& granny = GrannyLibrary::instance();

    if (granny.CurveGetDimension(&grannyTransformTrack.scale_shear_curve) == 0) {
        return;
    }

    GrannyCurve2* scaleCurve = granny.CurveConvertToDaK32fC32f(
        &grannyTransformTrack.scale_shear_curve, granny.CurveIdentityScaleShear()
    );

    const auto grannyScaleShearCurve =
        static_cast<GrannyCurveDataDAK32fC32f*>(scaleCurve->curve_data.object);

    const auto grannyKnotCount = static_cast<unsigned>(grannyScaleShearCurve->knot_count);

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

void GrannyImporterAnimation::importPositionCurve(
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

    GrannyCurve2* position_curve = granny.CurveConvertToDaK32fC32f(
        &grannyTransformTrack.position_curve, granny.CurveIdentityPosition()
    );

    const auto grannyPositionCurve =
        static_cast<GrannyCurveDataDAK32fC32f*>(position_curve->curve_data.object);

    const auto grannyKnotCount = static_cast<unsigned>(grannyPositionCurve->knot_count);

    if (!grannyKnotCount) {
        return;
    }

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<const double>(duration)) {
        time = static_cast<const double>(static_cast<float>(step) * timeStep);

        float position[3];

        int positionDimension = granny.CurveGetDimension(position_curve);
        granny.EvaluateCurveAtT(
            positionDimension,
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

void GrannyImporterAnimation::importRotationCurve(
    Bindings::Animation::SharedPtr animation,
    Bindings::Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack
) const {
    const float duration = animation->getData()->duration;
    const float timeStep = animation->getData()->time_step;

    auto& granny = GrannyLibrary::instance();

    GrannyCurve2* scaleCurve = nullptr;

    if (granny.CurveGetDimension(&grannyTransformTrack.scale_shear_curve) != 0) {
        scaleCurve = granny.CurveConvertToDaK32fC32f(
            &grannyTransformTrack.scale_shear_curve, granny.CurveIdentityScaleShear()
        );
    }

    if (granny.CurveGetDimension(&grannyTransformTrack.orientation_curve) == 0) {
        return;
    }

    GrannyCurve2* orientationCurve = granny.CurveConvertToDaK32fC32f(
        &grannyTransformTrack.orientation_curve, granny.CurveIdentityOrientation()
    );

    const auto grannyOrientationCurve =
        static_cast<GrannyCurveDataDAK32fC32f*>(orientationCurve->curve_data.object);

    const auto grannyKnotCount = static_cast<unsigned>(grannyOrientationCurve->knot_count);

    if (!grannyKnotCount) {
        return;
    }

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<const double>(duration)) {
        time = static_cast<const double>(static_cast<float>(step) * timeStep);

        float scale[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

        if (scaleCurve) {
            int scaleDimension = granny.CurveGetDimension(scaleCurve);
            granny.EvaluateCurveAtT(
                scaleDimension,
                false,
                true,
                scaleCurve,
                true,
                duration,
                static_cast<float>(time),
                scale,
                granny.CurveIdentityScaleShear()
            );
        }

        float quaternion[4];

        int orientationDimension = granny.CurveGetDimension(orientationCurve);
        granny.EvaluateCurveAtT(
            orientationDimension,
            false,
            true,
            orientationCurve,
            true,
            duration,
            static_cast<float>(time),
            quaternion,
            granny.CurveIdentityOrientation()
        );

        FbxAMatrix transformMatrix;

        transformMatrix.SetQ(FbxQuaternion(
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
            transformMatrix.MultSM(FbxVector4(
                static_cast<double>(abs(scale[0])),
                static_cast<double>(abs(scale[4])),
                static_cast<double>(abs(scale[8]))
            ));
        }

        Bindings::CurveRotationKey key(*orientationCurve);
        key.setTime(time);
        key.setValue(transformMatrix.GetR());

        track->addRotationKey(key);

        step++;
    }

    granny.FreeCurve(orientationCurve);
}

double GrannyImporterAnimation::calculateTime(const double time) const {
    const auto frame = 1.0 / 30.0;
    const auto min = time / frame;
    const auto min2 = floor(min);

    return min2 * frame;
}

}  // namespace GCL::Importer
