#include "gcl/grannyconverterlibrary.h"

#include "gcl/exporter/fbxexporter.h"
#include "gcl/importer/grannyformat.h"
#include "gcl/importer/grannyimporter.h"

namespace GCL {

GrannyConverterLibrary::GrannyConverterLibrary() {
    Importer::GrannyLibrary::instance().Initialize();
}

GrannyConverterLibrary::~GrannyConverterLibrary() {
}

}  // namespace GCL
