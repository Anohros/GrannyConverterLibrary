#include "gcl/utilities/logging.h"

#include "gcl/utilities/datetime.h"

#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <mutex>

namespace GCL::Utilities::Logging {

using namespace std;
using namespace std::filesystem;

using namespace GCL::Utilities::Datetime;

void log(const char* level, const char* file, int line, const char* function, const char* message)
{
    static mutex logMutex;
    lock_guard<mutex> lockGuard(logMutex);

    ofstream(stdout)
        << nowTimeMs()
        << " "
        << level
        << " "
        << path(file).filename().string()
        << ":"
        << line
        << " "
        << function
        << " "
        << message
        << endl;

    static ofstream logFile(DEFAULT_LOG_FILE);

    if (logFile) {
        logFile
            << message
            << endl;
    }
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

void _debug(const char* file, int line, const char* function, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, 1024, format, args);

    va_end(args);

    log("Debug", file, line, function, message);
}

void _info(const char* file, int line, const char* function, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, 1024, format, args);

    va_end(args);

    log("Info", file, line, function, message);
}

void _warning(const char* file, int line, const char* function, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, 1024, format, args);

    va_end(args);

    log("Warning", file, line, function, message);
}

void _fatal(const char* file, int line, const char* function, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, 1024, format, args);

    va_end(args);

    log("Fatal", file, line, function, message);
}

#ifdef __clang__
#pragma diagnostic pop
#endif

} // namespace GCL::Utilities::Logging
