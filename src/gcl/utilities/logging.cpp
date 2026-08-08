#include "gcl/utilities/logging.h"

#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <mutex>

#include "gcl/utilities/datetime.h"

namespace GCL::Utilities::Logging {

void log(const char* level, const char* file, int line, const char* function, const char* message) {
    static std::mutex logMutex;
    std::lock_guard<std::mutex> lockGuard(logMutex);

    std::ofstream(stdout) << Datetime::nowTimeMs() << " " << level << " "
                          << std::filesystem::path(file).filename().string() << ":" << line << " "
                          << function << " " << message << '\n';

    static std::ofstream logFile(DEFAULT_LOG_FILE);

    if (logFile) {
        logFile << message << '\n';
    }
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

void _debug(const char* file, int line, const char* function, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, 1024, format, args);

    va_end(args);

    log("Debug", file, line, function, message);
}

void _info(const char* file, int line, const char* function, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, 1024, format, args);

    va_end(args);

    log("Info", file, line, function, message);
}

void _warning(const char* file, int line, const char* function, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, 1024, format, args);

    va_end(args);

    log("Warning", file, line, function, message);
}

void _fatal(const char* file, int line, const char* function, const char* format, ...) {
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

}  // namespace GCL::Utilities::Logging
