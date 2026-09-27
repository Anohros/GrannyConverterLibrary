#include "gcl/grannyconverterlibrary.h"

#include <utility>

#include "gcl/exporter/fbxexporter.h"
#include "gcl/importer/grannyformat.h"
#include "gcl/importer/grannyimporter.h"

namespace GCL {

GrannyConverterLibrary::GrannyConverterLibrary() {
    Importer::GrannyLibrary::instance().Initialize();
}

GrannyConverterLibrary::~GrannyConverterLibrary() {
}

bool GrannyConverterLibrary::ConvertFile(
    const std::string& input_file_path,
    const std::string& output_file_path,
    Importer::GrannyImportOptions import_options,
    Exporter::FbxExportOptions export_options
) {
    return ConvertFiles(
        {input_file_path}, output_file_path, import_options, std::move(export_options)
    );
}

bool GrannyConverterLibrary::ConvertFiles(
    const std::vector<std::string>& input_file_paths,
    const std::string& output_file_path,
    Importer::GrannyImportOptions import_options,
    Exporter::FbxExportOptions export_options
) {
    if (input_file_paths.empty()) {
        return false;
    }

    // Import all granny files into one scene.
    Importer::GrannyImporter importer(import_options);
    for (const auto& input_file_path : input_file_paths) {
        if (!importer.importFromFile(input_file_path)) {
            return false;
        }
    }

    // Export the imported scene to an fbx file.
    Exporter::FbxExporter exporter(std::move(export_options), importer.getScene());
    return exporter.exportToFile(output_file_path);
}

}  // namespace GCL
