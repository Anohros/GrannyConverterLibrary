#include "gcl/bindings/animation.h"

namespace GCL::Bindings {

Animation::Animation(GrannyAnimation* data) : m_data(data) {
}

GrannyAnimation* Animation::getData() {
    return m_data;
}

std::vector<Track::SharedPtr> Animation::getTracks() {
    return m_tracks;
}

void Animation::addTrack(const Track::SharedPtr& track) {
    m_tracks.push_back(track);
}

}  // namespace GCL::Bindings
