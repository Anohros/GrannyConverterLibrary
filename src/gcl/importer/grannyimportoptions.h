#pragma once

namespace GCL::Importer {

///
/// \brief Enableable / disableable options for the import of a model.
///
struct GrannyImportOptions {
    ///
    /// \brief Sets whether to import animation using deboor animation importer.
    ///
    bool importAnimationDeboor = false;
};

} // namespace GCL::Importer
