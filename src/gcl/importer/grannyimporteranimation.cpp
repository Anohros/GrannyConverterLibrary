#include "gcl/importer/grannyimporteranimation.h"

#include <QDebug>

namespace GCL::Importer {

GrannyImporterAnimation::GrannyImporterAnimation(Scene::SharedPtr scene)
    : m_scene(scene)
{
}

GrannyImporterAnimation::~GrannyImporterAnimation()
{
}

void GrannyImporterAnimation::importAnimations(GrannyFileInfo* grannyFileInfo) const
{
    qInfo("Load animations from granny file.");

    if (!grannyFileInfo->AnimationCount) {
        qWarning("Skip load animations because granny file does not have at least one animation.");
        return;
    }

    const auto grannyAnimation = grannyFileInfo->Animations[0];
    auto animation = make_shared<Animation>(grannyAnimation);

    // Process animation track groups. Only if animation does have at least one animation track group.
    if (grannyAnimation->TrackGroupCount) {
        for (auto i = 0; i < grannyAnimation->TrackGroupCount; i++) {
            const auto grannyTrackGroup = grannyAnimation->TrackGroups[i];

            for (auto i = 0; i < grannyTrackGroup->TransformTrackCount; i++) {
                animation->addTrack(importTrack(animation, grannyTrackGroup->TransformTracks[i]));
            }
        }
    } else {
        qWarning("Skip load animations because animation does not have at least one animation track.");
    }

    m_scene->addAnimation(animation);

    qInfo("Added animation to scene.");
}

Track::SharedPtr GrannyImporterAnimation::importTrack(
    Animation::SharedPtr animation,
    GrannyTransformTrack grannyTransformTrack) const
{
    auto track = make_shared<Track>(grannyTransformTrack);
    track->setName(grannyTransformTrack.Name);

    importScaleCurve(track, grannyTransformTrack);
    importPositionCurve(animation, track, grannyTransformTrack);
    importRotationCurve(animation, track, grannyTransformTrack);

    return track;
}

void GrannyImporterAnimation::importScaleCurve(
    Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack) const
{
    if (GrannyCurveGetDimension(&grannyTransformTrack.ScaleShearCurve) == 0) {
        return;
    }

    GrannyCurve2* scaleCurve = GrannyCurveConvertToDaK32fC32f(
        &grannyTransformTrack.ScaleShearCurve,
        GrannyCurveIdentityScaleShear);

    const auto grannyScaleShearCurve = static_cast<GrannyCurveDataDAK32fC32f*>(
        scaleCurve->CurveData.Object);

    const auto grannyKnotCount = static_cast<unsigned>(
        grannyScaleShearCurve->KnotCount);

    for (unsigned i = 0; i < grannyKnotCount; i++) {
        CurveScaleKey key(*scaleCurve);
        key.setTime(static_cast<double>(grannyScaleShearCurve->Knots[i]));
        key.setValue(FbxDouble3(
            static_cast<double>(grannyScaleShearCurve->Controls[(i * 9)]),
            static_cast<double>(grannyScaleShearCurve->Controls[(i * 9) + 4]),
            static_cast<double>(grannyScaleShearCurve->Controls[(i * 9) + 8])));

        track->addScaleKey(key);
    }

    GrannyFreeCurve(scaleCurve);
}

void GrannyImporterAnimation::importPositionCurve(
    Animation::SharedPtr animation,
    Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack) const
{
    const float duration = animation->getData()->Duration;
    const float timeStep = animation->getData()->TimeStep;

    if (GrannyCurveGetDimension(&grannyTransformTrack.PositionCurve) == 0) {
        return;
    }

    GrannyCurve2* positionCurve = GrannyCurveConvertToDaK32fC32f(
        &grannyTransformTrack.PositionCurve,
        GrannyCurveIdentityPosition);

    const auto grannyPositionCurve = static_cast<GrannyCurveDataDAK32fC32f*>(
        positionCurve->CurveData.Object);

    const auto grannyKnotCount = static_cast<unsigned>(
        grannyPositionCurve->KnotCount);

    if (!grannyKnotCount) {
        return;
    }

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<const double>(duration)) {
        time = static_cast<const double>(static_cast<float>(step) * timeStep);

        float position[3];

        int positionDimension = GrannyCurveGetDimension(positionCurve);
        GrannyEvaluateCurveAtT(
            positionDimension,
            false,
            true,
            positionCurve,
            true,
            duration,
            static_cast<float>(time),
            position,
            GrannyCurveIdentityPosition);

        CurvePositionKey key(*positionCurve);
        key.setTime(time);
        key.setValue(FbxDouble3(
            static_cast<double>(position[0]),
            static_cast<double>(position[1]),
            static_cast<double>(position[2])));

        track->addPositionKey(key);

        step++;
    }

    GrannyFreeCurve(positionCurve);
}

void GrannyImporterAnimation::importRotationCurve(
    Animation::SharedPtr animation,
    Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack) const
{
    const float duration = animation->getData()->Duration;
    const float timeStep = animation->getData()->TimeStep;

    GrannyCurve2* scaleCurve = nullptr;

    if (GrannyCurveGetDimension(&grannyTransformTrack.ScaleShearCurve) != 0) {
        scaleCurve = GrannyCurveConvertToDaK32fC32f(
            &grannyTransformTrack.ScaleShearCurve,
            GrannyCurveIdentityScaleShear);
    }

    if (GrannyCurveGetDimension(&grannyTransformTrack.OrientationCurve) == 0) {
        return;
    }

    GrannyCurve2* orientationCurve = GrannyCurveConvertToDaK32fC32f(
        &grannyTransformTrack.OrientationCurve,
        GrannyCurveIdentityOrientation);

    const auto grannyOrientationCurve = static_cast<GrannyCurveDataDAK32fC32f*>(
        orientationCurve->CurveData.Object);

    const auto grannyKnotCount = static_cast<unsigned>(
        grannyOrientationCurve->KnotCount);

    if (!grannyKnotCount) {
        return;
    }

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<const double>(duration)) {
        time = static_cast<const double>(static_cast<float>(step) * timeStep);

        float scale[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

        if (scaleCurve != nullptr) {
            int scaleDimension = GrannyCurveGetDimension(scaleCurve);
            GrannyEvaluateCurveAtT(
                scaleDimension,
                false,
                true,
                scaleCurve,
                true,
                duration,
                static_cast<float>(time),
                scale,
                GrannyCurveIdentityScaleShear);
        }

        float quaternion[4];

        int orientationDimension = GrannyCurveGetDimension(orientationCurve);
        GrannyEvaluateCurveAtT(
            orientationDimension,
            false,
            true,
            orientationCurve,
            true,
            duration,
            static_cast<float>(time),
            quaternion,
            GrannyCurveIdentityOrientation);

        FbxAMatrix transformMatrix;

        transformMatrix.SetQ(FbxQuaternion(
            static_cast<double>(quaternion[0]),
            static_cast<double>(quaternion[1]),
            static_cast<double>(quaternion[2]),
            static_cast<double>(quaternion[3])));

        // Check for negative scale.
        // Fbx can not handle negative scaling in same way as granny2 does handle it.
        // Multiply negative scale with rotation matrix to apply negative scaling
        // also to rotation matrix and just not apply it only to scale matrix.
        if (scale[0] < 0.0f || scale[4] < 0.0f || scale[8] < 0.0f) {
            transformMatrix.MultSM(FbxVector4(
                static_cast<double>(abs(scale[0])),
                static_cast<double>(abs(scale[4])),
                static_cast<double>(abs(scale[8]))));
        }

        CurveRotationKey key(*orientationCurve);
        key.setTime(time);
        key.setValue(transformMatrix.GetR());

        track->addRotationKey(key);

        step++;
    }

    GrannyFreeCurve(orientationCurve);
}

double GrannyImporterAnimation::calculateTime(const double time) const
{
    const auto frame = 1.0 / 30.0;
    const auto min = time / frame;
    const auto min2 = floor(min);

    return min2 * frame;
}

} // namespace GCL::Importer
