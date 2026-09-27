#include "gcl/bindings/track.h"

#include <utility>

namespace GCL::Bindings {

Track::Track(GrannyTransformTrack data) : m_data(data) {
}

std::string Track::getName() {
    return m_name;
}

void Track::setName(std::string name) {
    m_name = std::move(name);
}

std::vector<CurvePositionKey> Track::getPositionKeys() {
    return m_positionKeys;
}

std::vector<CurveRotationKey> Track::getRotationKeys() {
    return m_rotationKeys;
}

std::vector<CurveScaleKey> Track::getScaleKeys() {
    return m_scaleKeys;
}

void Track::addPositionKey(const CurvePositionKey& key) {
    m_positionKeys.push_back(key);
}

void Track::addRotationKey(const CurveRotationKey& key) {
    m_rotationKeys.push_back(key);
}

void Track::addScaleKey(const CurveScaleKey& key) {
    m_scaleKeys.push_back(key);
}

}  // namespace GCL::Bindings
