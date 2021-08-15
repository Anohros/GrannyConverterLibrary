#include "gcl/grannyconverterlibrary.h"
#include "gcl/exporter/fbxexporter.h"
#include "gcl/importer/grannyimporter.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QMutex>

namespace GCL {

void messageHandler(
    QtMsgType type,
    const QMessageLogContext& context,
    const QString& message)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    // Output to stdout.
    ofstream(stdout) << qPrintable(qFormatLogMessage(type, context, message))
                     << endl;

    // Write to log file.
    static ofstream logFile("grannyconverter.log");

    if (logFile) {
        logFile
            << qPrintable(qFormatLogMessage(type, context, message))
            << endl;
    }
}

GrannyConverterLibrary::GrannyConverterLibrary()
{
    qInstallMessageHandler(messageHandler);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} %{category} %{type} %{function} %{line} %{message}");

    // Set filter rules to disable debug log for release builds for instance.
    // QLoggingCategory::setFilterRules("*.debug=false\n*.info=false");

    InitializeGrannyLibrary();
}

GrannyConverterLibrary::~GrannyConverterLibrary()
{
}

} // namespace GCL
