#pragma once

#include <vector>

#include "gcl/bindings/abstractcurvekey.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Bindings {

///
/// \brief The CurveRotationKey class.
///
class CurveRotationKey : public AbstractCurveKey {
public:
    ///
    /// \brief Constructer with data initialization.
    /// \param data Granny curve data
    ///
    explicit CurveRotationKey(GrannyCurve2 data);
};

}  // namespace GCL::Bindings
