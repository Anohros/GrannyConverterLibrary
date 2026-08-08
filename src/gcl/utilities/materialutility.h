#pragma once

#include <string>

namespace GCL::Utilities {

///
/// \brief Sanitizes a name (e.g. material name) by replacing "-", " ", "#" by "_".
/// \param name Material name for instance.
/// \return Sanitized name
///
std::string sanitizeName(std::string name);

}  // namespace GCL::Utilities
