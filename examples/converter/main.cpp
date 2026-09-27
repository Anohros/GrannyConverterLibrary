#include "gcl/exporter/fbxexportoptions.h"
#include "gcl/grannyconverterlibrary.h"
#include "gcl/importer/grannyimportoptions.h"

int main() {
    // Initialize library.
    GCL::GrannyConverterLibrary grannyConverterLibrary;

    GCL::Importer::GrannyImportOptions importOptions;

    // Use deboor animation importer.
    // The importer is able to import animations with bones are being mis-positioned.
    // However the resulted animation is might be changed in comparison to its original.
    // importOptions.importAnimationDeboor = true;

    GCL::Exporter::FbxExportOptions exportOptions;

    // Export all skeletons.
    exportOptions.exportSkeleton = true;

    // Export all materials and textures.
    exportOptions.exportMaterials = true;

    // Export animations.
    exportOptions.exportAnimation = true;

    // Load a character and an animation and convert them to a single fbx file.
    const bool success = grannyConverterLibrary.ConvertFiles(
        {"character.gr2", "character_animation.gr2"},
        "character_with_animation.fbx",
        importOptions,
        exportOptions
    );

    // Alternative: convert a single file.
    // const bool success = grannyConverterLibrary.convertFile(
    //     "character.gr2",
    //     "character.fbx",
    //     importOptions,
    //     exportOptions
    // );

    return success ? 0 : 1;
}
