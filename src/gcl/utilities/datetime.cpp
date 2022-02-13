#include "gcl/utilities/datetime.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace GCL::Utilities::Datetime {

using namespace std::chrono;

unsigned nowMs()
{
    system_clock::time_point now = system_clock::now();
    duration<long long, ratio<1, 1000>> nowMs
        = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    return static_cast<unsigned>(nowMs.count());
}

string nowTime(const char* format)
{
    tm nowTimeInfo;
    time_t nowTime = system_clock::to_time_t(system_clock::now());
    localtime_s(&nowTimeInfo, &nowTime);

    ostringstream result;
    result << put_time(&nowTimeInfo, format);

    return result.str();
}

string nowTimeMs(const char* format)
{
    ostringstream result;
    result << nowTime(format)
           << "."
           << nowMs();

    return result.str();
}

} // namespace GCL::Utilities::Datetime
