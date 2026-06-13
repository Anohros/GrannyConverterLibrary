#pragma once

#include <fstream>
#include <map>
#include <regex>
#include <string>
#include <utility>

#include <fbxsdk.h>

#include "gcl/bindings/scene.h"
#include "gcl/exporter/fbxexportermodule.h"
#include "gcl/importer/grannyformat.h"
#include "gcl/utilities/devilimageutility.h"
#include "gcl/utilities/materialutility.h"
#include "gcl/utilities/textureutility.h"

namespace GCL::Exporter {

///
/// \brief The ExporterMaterial class.
///
class FbxExporterMaterial : public FbxExporterModule {
public:
    ///
    /// \brief ExporterModule
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    ///
    FbxExporterMaterial(Bindings::Scene::SharedPtr scene, FbxScene* fbxScene);

    ///
    /// \brief Destructor
    ///
    virtual ~FbxExporterMaterial();

    ///
    /// \brief Export materials to the fbx scene.
    ///
    void exportMaterials(const std::string& outputFilepath);

protected:
    ///
    /// \brief Export all materials of the scene to the fbx scene.
    /// \param outputFilepath Output filepath of current model.
    /// \param material Material which should be exported.
    ///
    void exportMaterial(std::string outputFilepath, const Bindings::Material::SharedPtr& material);

    ///
    /// \brief Returns the file path for a texture.
    /// \param outputFilepath Output filepath of current model.
    /// \param texture Texture of which the file path should be returned.
    ///
    std::string getTextureFilePath(std::string outputFilepath, GrannyTexture* texture);

    ///
    /// \brief Exports a material of the scene to the fbx scene - part of exportMaterial.
    /// \param material Material which should be added.
    /// \param materialName Name of material which should be exported.
    /// \param outputFilepath Output filepath of current model.
    /// \param textureFilePath Filepath of diffuse texture for current material.
    ///
    FbxSurfaceMaterial* addMaterial(
        Bindings::Material::SharedPtr material,
        std::string materialName,
        std::string outputFilepath,
        std::string textureFilePath = ""
    );

    ///
    /// \brief Sanitizes a material name.
    /// \param materialName
    /// \return
    ///
    virtual std::string sanitizeMaterialName(const std::string& materialName) {
        return GCL::Utilities::sanitizeName(materialName);
    }

    ///
    /// \brief Sanitizes a material name using both material and texture names for better
    /// uniqueness.
    /// \param materialName
    /// \param textureName
    /// \return
    ///
    virtual std::string sanitizeMaterialName(
        const std::string& materialName, const std::string& textureName
    ) {
        // Combine material and texture names for better uniqueness in case of duplicates
        std::string combinedName = materialName;
        if (!textureName.empty()) {
            combinedName += "_" + textureName;
        }
        return GCL::Utilities::sanitizeName(std::move(combinedName));
    }

    ///
    /// \brief Sanitizes a file name without path.
    /// \param name
    /// \return
    ///
    virtual std::string sanitizeFileName(std::string name) {
        return GCL::Utilities::sanitizeName(std::move(name));
    }

    ///
    /// \brief Exported materials of the current exporting scene stored by material name.
    ///
    std::map<std::string, FbxSurfacePhong*> m_fbxMaterials;
};

}  // namespace GCL::Exporter
