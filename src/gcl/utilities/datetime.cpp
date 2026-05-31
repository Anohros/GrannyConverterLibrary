#include "gcl/utilities/datetime.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace GCL::Utilities::Datetime {

unsigned nowMs() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<long long, std::ratio<1, 1000>> nowMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    return static_cast<unsigned>(nowMs.count());
}

std::string nowTime(const char* format) {
    tm nowTimeInfo;
    time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    localtime_s(&nowTimeInfo, &nowTime);

    std::ostringstream result;
    result << std::put_time(&nowTimeInfo, format);

    return result.str();
}

std::string nowTimeMs(const char* format) {
    std::ostringstream result;
    result << nowTime(format) << "." << nowMs();

    return result.str();
}

}  // namespace GCL::Utilities::Datetime
