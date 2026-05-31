#pragma once

#include <fstream>
#include <vector>

namespace GCL::Utilities::FileStreamUtility {

///
/// \brief Reads out bytes from file offset to a value.
/// \param file
/// \param offset
/// \return
///
template <typename T>
T binaryread(std::fstream& file, unsigned offset);

template <>
unsigned binaryread<unsigned>(std::fstream& file, unsigned offset);

///
/// \brief Writes bytes to a file at specified offset.
/// \param file
/// \param offset
/// \param bytes
/// \param size
///
void binarywrite(std::fstream& file, unsigned offset, const char* bytes, unsigned size = 0);

///
/// \brief Returns size of a file.
/// \param file
/// \return File size
///
unsigned filesize(std::fstream& file);

///
/// \brief Returns a file as character list.
/// \param file
/// \return File bytes as aharacter list.
///
std::vector<char> binarydata(std::fstream& file);

}  // namespace GCL::Utilities::FileStreamUtility
