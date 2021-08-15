#pragma once

#include "gcl/bindings/bone.h"
#include "gcl/bindings/scene.h"
#include "gcl/importer/deboor.h"
#include "gcl/importer/grannyformat.h"
#include "gcl/importer/grannyimporteranimation.h"

namespace GCL::Importer {

using namespace GCL::Bindings;

///
/// \brief The GrannyImporterAnimationDeboor class.
///
class GrannyImporterAnimationDeboor : public GrannyImporterAnimation {
public:
    ///
    /// \brief Constructor
    /// \param scene Scene which needs to be exported.
    ///
    GrannyImporterAnimationDeboor(Scene::SharedPtr scene);

    ///
    /// \brief Destructor
    ///
    virtual ~GrannyImporterAnimationDeboor() override;

protected:
    ///
    /// \brief Construct a track.
    /// \param animation Animation
    /// \param grannyTransformTrack Granny transform track
    /// \return Animation tracks of animation.
    ///
    Track::SharedPtr importTrack(Animation::SharedPtr animation, GrannyTransformTrack grannyTransformTrack) const override;

    ///
    /// \brief Imports a scale keys from scale curve.
    /// \param animation Animation
    /// \param track Granny transform track
    /// \param grannyTransformTrack Granny transform track
    ///
    void importScaleCurve(Track::SharedPtr track, GrannyTransformTrack grannyTransformTrack) const;

    ///
    /// \brief Imports a position keys from scale curve.
    /// \param animation Animation
    /// \param track Track
    /// \param grannyTransformTrack Granny transform track
    ///
    void importPositionCurve(Animation::SharedPtr animation, Track::SharedPtr track, GrannyTransformTrack grannyTransformTrack) const;

    ///
    /// \brief Imports a rotation keys from scale curve.
    /// \param animation Animation
    /// \param track Track
    /// \param grannyTransformTrack Granny transform track
    ///
    void importRotationCurve(Animation::SharedPtr animation, Track::SharedPtr track, GrannyTransformTrack grannyTransformTrack) const;
};

} // namespace GCL::Importer
