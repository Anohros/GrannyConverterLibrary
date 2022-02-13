#pragma once

namespace GCL::Utilities::Logging {

#define DEFAULT_LOG_FILE "grannyconverter.log"

///
/// \brief Logs a message tou stdout and a log file.
/// \param level
/// \param file
/// \param line
/// \param function
/// \param message
///
void log(const char* level, const char* file, int line, const char* function, const char* message);

///
/// \brief Logs a format message.
/// \param file
/// \param line
/// \param function
/// \param format
///
void _debug(const char* file, int line, const char* function, const char* format, ...);

///
/// \brief Logs a format message.
/// \param file
/// \param line
/// \param function
/// \param format
///
void _info(const char* file, int line, const char* function, const char* format, ...);

///
/// \brief Logs a format message.
/// \param file
/// \param line
/// \param function
/// \param format
///
void _warning(const char* file, int line, const char* function, const char* format, ...);

///
/// \brief Logs a format message.
/// \param file
/// \param line
/// \param function
/// \param format
///
void _fatal(const char* file, int line, const char* function, const char* format, ...);

///
/// \brief Logs a format message.
///
#define debug(...) _debug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

///
/// \brief Logs a format message.
///
#define info(...) _info(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

///
/// \brief Logs a format message.
///
#define warning(...) _warning(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

///
/// \brief Logs a format message.
///
#define fatal(...) _fatal(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

} // namespace GCL::Utilities::Logging
