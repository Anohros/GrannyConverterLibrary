#pragma once

#include <string>
#include <vector>

#include "gcl/exporter/fbxexportoptions.h"
#include "gcl/importer/grannyimportoptions.h"

namespace GCL {

///
/// \brief The GrannyConverterLibrary class - facade to convert granny files to fbx files.
///
class GrannyConverterLibrary {
public:
    ///
    /// \brief Constructor - initializes the granny library.
    ///
    GrannyConverterLibrary();

    ///
    /// \brief Destructor
    ///
    ~GrannyConverterLibrary();

    ///
    /// \brief Convert a single granny file to an fbx file.
    /// \param input_file_path Full filepath to the importing granny file.
    /// \param output_file_path Full filepath where the fbx file will be exported to.
    /// \param import_options Import options which define the way how a scene needs to be imported.
    /// \param export_options Export options to define what aspects of the scene needs to be
    /// exported.
    /// \return Returns whether the conversion was successful or not.
    ///
    bool ConvertFile(
        const std::string& input_file_path,
        const std::string& output_file_path,
        Importer::GrannyImportOptions import_options = {},
        Exporter::FbxExportOptions export_options = {}
    );

    ///
    /// \brief Convert multiple granny files (e.g. a character and its animation) to a single
    /// fbx file. All granny files are imported into one scene which is then exported.
    /// \param input_file_paths Full filepaths of the importing granny files.
    /// \param output_file_path Full filepath where the fbx file will be exported to.
    /// \param import_options Import options which define the way how a scene needs to be imported.
    /// \param export_options Export options to define what aspects of the scene needs to be
    /// exported.
    /// \return Returns whether the conversion was successful or not. The conversion is aborted
    /// as soon as one granny file could not be imported.
    ///
    bool ConvertFiles(
        const std::vector<std::string>& input_file_paths,
        const std::string& output_file_path,
        Importer::GrannyImportOptions import_options = {},
        Exporter::FbxExportOptions export_options = {}
    );
};

}  // namespace GCL
