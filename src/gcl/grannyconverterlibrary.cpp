#include "gcl/grannyconverterlibrary.h"
#include "gcl/exporter/fbxexporter.h"
#include "gcl/importer/grannyimporter.h"

namespace GCL {

GrannyConverterLibrary::GrannyConverterLibrary()
{
    InitializeGrannyLibrary();
}

GrannyConverterLibrary::~GrannyConverterLibrary()
{
}

} // namespace GCL
