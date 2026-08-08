#pragma once

#include <vector>

#include <fbxsdk.h>

#include "gcl/bindings/binding.h"
#include "gcl/bindings/track.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Bindings {

///
/// \brief The Animation class.
///
class Animation : public Binding<Animation> {
public:
    ///
    /// \brief Constructer with data initialization.
    /// \param data Granny animation data
    ///
    explicit Animation(GrannyAnimation* data);

    ///
    /// \brief Returns the granny animation data.
    /// \return Granny animation data
    ///
    GrannyAnimation* getData();

    ///
    /// \brief Returns the animation tracks of the animation.
    /// \return Animation tracks
    ///
    std::vector<Track::SharedPtr> getTracks();

    ///
    /// \brief Adds an animation track.
    /// \param track Animation track
    ///
    void addTrack(const Track::SharedPtr& track);

protected:
    ///
    /// \brief Granny data of the animation.
    ///
    GrannyAnimation* m_data = nullptr;

    ///
    /// \brief Fbx node of the animation.
    ///
    FbxNode* m_node = nullptr;

    ///
    /// \brief Animation tracks of the animation.
    ///
    std::vector<Track::SharedPtr> m_tracks;
};

}  // namespace GCL::Bindings
