#pragma once

#include "gcl/bindings/bone.h"
#include "gcl/bindings/scene.h"
#include "gcl/importer/deboor.h"
#include "gcl/importer/grannyformat.h"
#include "gcl/importer/grannyimporteranimation.h"

namespace GCL::Importer {

///
/// \brief The GrannyImporterAnimationDeboor class.
///
class GrannyImporterAnimationDeboor : public GrannyImporterAnimation {
public:
    ///
    /// \brief Constructor
    /// \param scene Scene which needs to be exported.
    ///
    explicit GrannyImporterAnimationDeboor(Bindings::Scene::SharedPtr scene);

protected:
    ///
    /// \brief Construct a track.
    /// \param animation Animation
    /// \param grannyTransformTrack Granny transform track
    /// \return Animation tracks of animation.
    ///
    Bindings::Track::SharedPtr importTrack(
        Bindings::Animation::SharedPtr animation, GrannyTransformTrack grannyTransformTrack
    ) const override;

    ///
    /// \brief Imports a scale keys from scale curve.
    /// \param animation Animation
    /// \param track Granny transform track
    /// \param grannyTransformTrack Granny transform track
    ///
    void importScaleCurve(
        Bindings::Track::SharedPtr track, GrannyTransformTrack grannyTransformTrack
    ) const;

    ///
    /// \brief Imports a position keys from scale curve.
    /// \param animation Animation
    /// \param track Track
    /// \param grannyTransformTrack Granny transform track
    ///
    void importPositionCurve(
        Bindings::Animation::SharedPtr animation,
        Bindings::Track::SharedPtr track,
        GrannyTransformTrack grannyTransformTrack
    ) const;

    ///
    /// \brief Imports a rotation keys from scale curve.
    /// \param animation Animation
    /// \param track Track
    /// \param grannyTransformTrack Granny transform track
    ///
    void importRotationCurve(
        Bindings::Animation::SharedPtr animation,
        Bindings::Track::SharedPtr track,
        GrannyTransformTrack grannyTransformTrack
    ) const;
};

}  // namespace GCL::Importer
