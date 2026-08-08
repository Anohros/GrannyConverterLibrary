#pragma once

#include <memory>
#include <vector>

#include <fbxsdk.h>

#include "gcl/importer/grannyformat.h"

namespace GCL::Importer {

// De Boor's algorithm to evaluate to interpolate keyframes to bézier spline.
// Thanks to https://github.com/Arbos/nwn2mdk/blob/master/mdb2fbx/export_gr2.cpp

///
/// \brief padded_knots
/// \param knots
/// \param degree
/// \return
///
std::vector<float> padded_knots(const std::vector<float>& knots, unsigned degree);

///
/// \brief de_boor_position
/// \param degree
/// \param time
/// \param knots
/// \param controls
/// \return
///
FbxDouble3 de_boor_position(
    unsigned degree,
    float time,
    const std::vector<float>& knots,
    const std::vector<FbxDouble3>& controls
);

///
/// \brief de_boor_position
/// \param degree
/// \param time
/// \param knots
/// \param controls
/// \return
///
FbxDouble3 de_boor_position(
    unsigned degree, float time, std::vector<float>& knots, std::vector<FbxDouble3>& controls
);

///
/// \brief de_boor_rotation
/// \param degree
/// \param time
/// \param knots
/// \param controls
/// \return
///
FbxQuaternion de_boor_rotation(
    unsigned degree,
    float time,
    const std::vector<float>& knots,
    const std::vector<FbxQuaternion>& controls
);

///
/// \brief de_boor_rotation
/// \param degree
/// \param time
/// \param knots
/// \param controls
/// \return
///
FbxQuaternion de_boor_rotation(
    unsigned degree, float time, std::vector<float>& knots, std::vector<FbxQuaternion>& controls
);

}  // namespace GCL::Importer
