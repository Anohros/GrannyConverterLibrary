#pragma once

#include "gcl/bindings/bone.h"
#include "gcl/bindings/scene.h"
#include "gcl/importer/deboor.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Importer {

///
/// \brief The GrannyImporterAnimation class.
///
class GrannyImporterAnimation {
public:
    ///
    /// \brief Constructor
    /// \param scene Scene which needs to be exported.
    ///
    explicit GrannyImporterAnimation(Bindings::Scene::SharedPtr scene);

    ///
    /// \brief Destructor
    ///
    virtual ~GrannyImporterAnimation() = default;

    ///
    /// \brief Imports all animations from the granny file as scene animation to the scene.
    /// \param grannyFileInfo Granny file info
    ///
    void ImportAnimations(GrannyFileInfo* granny_file_info) const;

public:
    ///
    /// \brief Animation tracks in the scene.
    ///
    std::vector<GrannyTransformTrack> tracks_;

    ///
    /// \brief Animations in the scene.
    ///
    std::vector<GrannyAnimation*> animations_;

protected:
    ///
    /// \brief Construct a track.
    /// \param animation Animation
    /// \param granny_transform_track Granny transform track
    /// \return Animation tracks of animation.
    ///
    [[nodiscard]] virtual Bindings::Track::SharedPtr ImportTrack(
        const Bindings::Animation::SharedPtr& animation,
        const GrannyTransformTrack& granny_transform_track
    ) const;

    ///
    /// \brief Imports a scale keys from scale curve.
    /// \param track Granny transform track
    /// \param grannyTransformTrack Granny transform track
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
        GrannyTransformTrack granny_transform_track
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
        GrannyTransformTrack granny_transform_track
    ) const;

    ///
    /// \brief Calculcates time to a 30 frames per second rate.
    /// \param time
    /// \return Time at a 30 frames per second rate.
    ///
    double CalculateTime(const double time) const;

protected:
    ///
    /// \brief Scene of the importing granny file.
    ///
    Bindings::Scene::SharedPtr scene_;
};

}  // namespace GCL::Importer
