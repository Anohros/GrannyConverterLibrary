#pragma once

#include <string>

namespace GCL::Utilities::Datetime {

using namespace std;

#define DEFAULT_FORMAT "%Y-%m-%e %H:%M:%S"

///
/// \brief Returns the milliseconds of now.
///
unsigned nowMs();

///
/// \brief Returns now time without milliseconds.
/// \param Time format, see: https://en.cppreference.com/w/cpp/io/manip/put_time
///
string nowTime(const char* format = DEFAULT_FORMAT);

///
/// \brief Returns now time with milliseconds.
/// \param Time format, see: https://en.cppreference.com/w/cpp/io/manip/put_time
///
string nowTimeMs(const char* format = DEFAULT_FORMAT);

} // namespace GCL::Utilities::Datetime
