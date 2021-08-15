#include "gcl/importer/grannyimporteranimation_deboor.h"

#include <QDebug>

#include <vector>

namespace GCL::Importer {

using namespace std;

GrannyImporterAnimationDeboor::GrannyImporterAnimationDeboor(Scene::SharedPtr scene)
    : GrannyImporterAnimation(scene)
{
}

GrannyImporterAnimationDeboor::~GrannyImporterAnimationDeboor()
{
}

Track::SharedPtr GrannyImporterAnimationDeboor::importTrack(
    Animation::SharedPtr animation,
    GrannyTransformTrack grannyTransformTrack) const
{
    Track::SharedPtr track = make_shared<Track>(grannyTransformTrack);
    track->setName(grannyTransformTrack.Name);

    importScaleCurve(track, grannyTransformTrack);
    importPositionCurve(animation, track, grannyTransformTrack);
    importRotationCurve(animation, track, grannyTransformTrack);

    return track;
}

void GrannyImporterAnimationDeboor::importScaleCurve(
    Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack) const
{
    if (GrannyCurveGetDimension(&grannyTransformTrack.ScaleShearCurve) == 0) {
        return;
    }

    GrannyCurve2* scaleCurve = GrannyCurveConvertToDaK32fC32f(
        &grannyTransformTrack.ScaleShearCurve,
        GrannyCurveIdentityScaleShear);

    const GrannyCurveDataDAK32fC32f* grannyScaleShearCurve = static_cast<GrannyCurveDataDAK32fC32f*>(
        scaleCurve->CurveData.Object);

    const unsigned grannyKnotCount = static_cast<unsigned>(
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

void GrannyImporterAnimationDeboor::importPositionCurve(
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

    const GrannyCurveDataDAK32fC32f* grannyPositionCurve = static_cast<GrannyCurveDataDAK32fC32f*>(
        positionCurve->CurveData.Object);

    const unsigned grannyKnotCount = static_cast<unsigned>(
        grannyPositionCurve->KnotCount);

    if (!grannyKnotCount) {
        return;
    }

    vector<float> knots;
    knots.reserve(grannyKnotCount);

    for (unsigned i = 0; i < grannyKnotCount; i++) {
        knots.emplace_back(static_cast<float>(i) * (timeStep * 1));
    }

    const unsigned controlCount = static_cast<unsigned>(grannyPositionCurve->ControlCount) / 3;

    vector<FbxDouble3> controls;
    controls.reserve(controlCount);

    for (unsigned i = 0; i < controlCount; i++) {
        controls.emplace_back(
            grannyPositionCurve->Controls[(i * 3)],
            grannyPositionCurve->Controls[(i * 3) + 1],
            grannyPositionCurve->Controls[(i * 3) + 2]);
    }

    unsigned step = 0;
    double time = 0;

    while (time < static_cast<double>(duration)) {
        time = static_cast<double>(static_cast<float>(step) * timeStep);

        auto position = de_boor_position(
            grannyPositionCurve->CurveDataHeader.Degree,
            static_cast<float>(time),
            knots,
            controls);

        CurvePositionKey key(grannyTransformTrack.PositionCurve);
        key.setTime(time);
        key.setValue(position);

        track->addPositionKey(key);

        step++;
    }

    GrannyFreeCurve(positionCurve);
}

void GrannyImporterAnimationDeboor::importRotationCurve(
    Animation::SharedPtr animation,
    Track::SharedPtr track,
    GrannyTransformTrack grannyTransformTrack) const
{
    const float duration = animation->getData()->Duration;
    const float timeStep = animation->getData()->TimeStep;

    if (GrannyCurveGetDimension(&grannyTransformTrack.OrientationCurve) == 0) {
        return;
    }

    GrannyCurve2* orientationCurve = GrannyCurveConvertToDaK32fC32f(
        &grannyTransformTrack.OrientationCurve,
        GrannyCurveIdentityOrientation);

    const GrannyCurveDataDAK32fC32f* grannyOrientationCurve = static_cast<GrannyCurveDataDAK32fC32f*>(
        orientationCurve->CurveData.Object);

    const unsigned grannyKnotCount = static_cast<unsigned>(
        grannyOrientationCurve->KnotCount);

    if (!grannyKnotCount) {
        return;
    }

    vector<float> knots;
    knots.reserve(grannyKnotCount);

    for (unsigned i = 0; i < grannyKnotCount; i++) {
        knots.emplace_back(static_cast<float>(i) * (timeStep * 1));
    }

    const unsigned controlCount = static_cast<unsigned>(grannyOrientationCurve->ControlCount) / 4;

    vector<FbxQuaternion> controls;
    controls.reserve(controlCount);

    for (unsigned i = 0; i < controlCount; i++) {
        controls.emplace_back(
            grannyOrientationCurve->Controls[(i * 4)],
            grannyOrientationCurve->Controls[(i * 4) + 1],
            grannyOrientationCurve->Controls[(i * 4) + 2],
            grannyOrientationCurve->Controls[(i * 4) + 3]);
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
        scaleMultiply = (static_cast<CurveScaleKey>(track->getScaleKeys().at(0))).getValue();
    }

    while (time < static_cast<double>(duration)) {
        time = static_cast<double>(static_cast<float>(step) * timeStep);

        FbxQuaternion quaternion = de_boor_rotation(
            grannyOrientationCurve->CurveDataHeader.Degree,
            static_cast<float>(time),
            knots,
            controls);

        FbxAMatrix transformMatrix;

        transformMatrix.SetQ(quaternion);

        // Check for negative scale multiply.
        // Fbx can not handle negative scaling in same way as granny2 does handle it.
        // Multiply negative scale with rotation matrix to apply negative scaling
        // also to rotation matrix and just not apply it only to scale matrix.
        if (scaleMultiply[0] < 0.0 || scaleMultiply[1] < 0.0 || scaleMultiply[2] < 0.0) {
            transformMatrix.MultSM(FbxVector4(
                abs(scaleMultiply[0]),
                abs(scaleMultiply[1]),
                abs(scaleMultiply[2])));
        }

        CurveRotationKey key(*orientationCurve);
        key.setTime(time);
        key.setValue(transformMatrix.GetR());

        track->addRotationKey(key);

        step++;
    }

    GrannyFreeCurve(orientationCurve);
}

} // namespace GCL::Importer
