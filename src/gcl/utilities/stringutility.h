#pragma once

#include <string>

namespace GCL::Utilities {

///
/// \brief Uppers case of a string.
/// \param input String
/// \return A uppercase string.
///
void toUpper(std::string& input);

///
/// \brief Lowers case of a string.
/// \param input String
/// \return A lowercase string.
///
void toLower(std::string& input);

///
/// \brief Uppers case of first character of a string.
/// \param input String
/// \return A string with its first character is a uppercase.
///
void toUpperFirst(std::string& input);

///
/// \brief Lowers case of first character of a string.
/// \param input String
/// \return A string with its first character is a lowercase.
///
void toLowerFirst(std::string& input);

}  // namespace GCL::Utilities
