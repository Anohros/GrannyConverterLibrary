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
    /// \brief Imports a scale keys from scale curve.
    /// \param animation Animation
    /// \param track Granny transform track
    /// \param granny_transform_track Granny transform track
    ///
    void ImportScaleCurve(
        const Bindings::Track::SharedPtr& track, const GrannyTransformTrack& granny_transform_track
    ) const;

    ///
    /// \brief Imports a position keys from scale curve.
    /// \param animation Animation
    /// \param track Track
    /// \param granny_transform_track Granny transform track
    ///
    void ImportPositionCurve(
        const Bindings::Animation::SharedPtr& animation,
        const Bindings::Track::SharedPtr& track,
        const GrannyTransformTrack& granny_transform_track
    ) const;

    ///
    /// \brief Imports a rotation keys from scale curve.
    /// \param animation Animation
    /// \param track Track
    /// \param granny_transform_track Granny transform track
    ///
    void ImportRotationCurve(
        const Bindings::Animation::SharedPtr& animation,
        const Bindings::Track::SharedPtr& track,
        const GrannyTransformTrack& granny_transform_track
    ) const;
};

}  // namespace GCL::Importer
