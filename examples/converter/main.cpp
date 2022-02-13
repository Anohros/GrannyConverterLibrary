#include "gcl/exporter/fbxexporter.h"
#include "gcl/exporter/fbxexportoptions.h"
#include "gcl/grannyconverterlibrary.h"
#include "gcl/importer/grannyimporter.h"

int main()
{
    // Initialize library.
    GCL::GrannyConverterLibrary grannyConverterLibrary;

    // Initialize importer.
    GCL::Importer::GrannyImporter importer;

    // Load a character and a animation.
    importer.importFromFile("character.gr2");
    importer.importFromFile("character_animation.gr2");

    GCL::Exporter::FbxExportOptions exporterOptions;

    // Export all skeletons.
    exporterOptions.exportSkeleton = true;

    // Export all materials and textures.
    exporterOptions.exportMaterials = true;

    // Export animations.
    exporterOptions.exportAnimation = true;

    // Create exporter instance with the scene to be exported.
    GCL::Exporter::FbxExporter exporter(exporterOptions, importer.getScene());

    // Export the fbx scene to a fbx file.
    exporter.exportToFile("character_with_animation.fbx");

    return 0;
}
