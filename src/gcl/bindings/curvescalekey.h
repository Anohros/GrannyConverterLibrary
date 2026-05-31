#pragma once

#include <vector>

#include "gcl/bindings/abstractcurvekey.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Bindings {

///
/// \brief The CurveScaleKey class.
///
class CurveScaleKey : public AbstractCurveKey {
public:
    ///
    /// \brief Constructer with data initialization.
    /// \param data Granny curve data
    ///
    explicit CurveScaleKey(GrannyCurve2 data);
};

}  // namespace GCL::Bindings
