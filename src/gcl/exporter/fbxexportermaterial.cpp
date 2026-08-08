#include "gcl/exporter/fbxexportermaterial.h"

#include <filesystem>
#include <utility>

namespace GCL::Exporter {

FbxExporterMaterial::FbxExporterMaterial(Bindings::Scene::SharedPtr scene, FbxScene* fbxScene)
    : FbxExporterModule(std::move(scene), fbxScene) {
    GCL::Utilities::initializeDevilImageLibrary();
}

FbxExporterMaterial::~FbxExporterMaterial() {
    GCL::Utilities::shutdownDevilImageLibrary();
}

void FbxExporterMaterial::exportMaterials(const std::string& outputFilepath) {
    for (auto& material : scene_->getMaterials()) {
        if (!material->isExcluded()) {
            exportMaterial(outputFilepath, material);
        }
    }
}

std::string FbxExporterMaterial::getTextureFilePath(
    std::string outputFilepath, GrannyTexture* texture
) {
    auto sourceTextureFilePath = std::string(texture->FromFileName);
    auto sourceTextureFileName = sourceTextureFilePath;

    auto fileNameBeginsOffset = sourceTextureFilePath.find_last_of('\\');
    if (fileNameBeginsOffset == std::string::npos) {
        fileNameBeginsOffset = sourceTextureFilePath.find_last_of('/');
    }

    if (fileNameBeginsOffset != std::string::npos) {
        sourceTextureFileName = sourceTextureFilePath.substr(fileNameBeginsOffset + 1);
    }

    const auto textureFileNameWithoutExtension =
        sourceTextureFileName.substr(0, sourceTextureFileName.find_first_of('.'));
    const auto textureFileNameWithExtension =
        sanitizeFileName(textureFileNameWithoutExtension).append(".png");

    if (!std::ifstream(sourceTextureFilePath.c_str()).good()) {
        if (std::ifstream(sourceTextureFileName.c_str()).good()) {
            sourceTextureFilePath = sourceTextureFileName;
        } else {
            bool foundTexture = false;

            for (auto& searchPath : scene_->getSearchPaths()) {
                const auto lookupPath = searchPath + sourceTextureFileName;
                if (std::ifstream(lookupPath.c_str()).good()) {
                    sourceTextureFilePath = lookupPath;
                    foundTexture = true;
                    break;
                }
            }

            const auto parentImportedPath =
                std::filesystem::path(scene_->getImportedFilePaths().front())
                    .parent_path()
                    .parent_path();

            if (!foundTexture && std::filesystem::exists(parentImportedPath)) {
                for (const auto& entry : std::filesystem::directory_iterator(
                         parentImportedPath,
                         std::filesystem::directory_options::skip_permission_denied
                     )) {
                    const auto lookupPath = (entry.path() / sourceTextureFileName).u8string();
                    if (std::ifstream(lookupPath.c_str()).good()) {
                        sourceTextureFilePath = lookupPath;
                        foundTexture = true;
                        break;
                    }
                }
            }

            if (!foundTexture) {
                const auto deeperParentImportedPath =
                    parentImportedPath.parent_path().parent_path();
                // Do not scan root path and program files as parent paths.
                if (deeperParentImportedPath.root_path().string() !=
                        deeperParentImportedPath.string() &&
                    std::filesystem::exists(deeperParentImportedPath)) {
                    if (!std::regex_match(
                            deeperParentImportedPath.string(),
                            std::regex("\\\\(Program Files \\(x86\\)|Program Files)\\\\")
                        )) {
                        for (const auto& entry : std::filesystem::recursive_directory_iterator(
                                 deeperParentImportedPath,
                                 std::filesystem::directory_options::skip_permission_denied
                             )) {
                            const auto lookupPath =
                                (entry.path() / sourceTextureFileName).u8string();
                            if (std::ifstream(lookupPath.c_str()).good()) {
                                sourceTextureFilePath = lookupPath;
                                foundTexture = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    auto outputFilepathFileSeparator = outputFilepath.find_last_of('\\');
    if (outputFilepathFileSeparator == std::string::npos) {
        outputFilepathFileSeparator = outputFilepath.find_last_of('/');
    }
    const auto targetTextureFilePath =
        outputFilepath.substr(0, outputFilepathFileSeparator + 1) + textureFileNameWithExtension;

    if (std::ifstream(sourceTextureFilePath.c_str()).good()) {
        GCL::Utilities::convertImage(sourceTextureFilePath, targetTextureFilePath);
    } else {
        GCL::Utilities::exportTexture(texture, targetTextureFilePath, true);
    }

    return textureFileNameWithExtension;
}

FbxSurfaceMaterial* FbxExporterMaterial::addMaterial(
    Bindings::Material::SharedPtr material,
    std::string materialName,
    std::string outputFilepath,
    std::string textureFilePath
) {
    auto texture = FbxFileTexture::Create(fbx_scene_, "Diffuse Texture");
    if (!textureFilePath.empty()) {
        texture->SetFileName(textureFilePath.c_str());
    }
    texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    texture->UVSet.Set("UV1");

    FbxFileTexture* ambientTexture = nullptr;
    if (material->getData()->MapCount > 1) {
        const auto grannyAmbientTexture =
            GCL::Utilities::getMaterialTexture(material->getData()->Maps[1].Material);
        if (grannyAmbientTexture != nullptr) {
            ambientTexture = FbxFileTexture::Create(fbx_scene_, "Ambient Texture");
            const std::string ambientTextureFilePath =
                getTextureFilePath(outputFilepath, grannyAmbientTexture);
            if (!ambientTextureFilePath.empty()) {
                ambientTexture->SetFileName(ambientTextureFilePath.c_str());
            }
            ambientTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
            ambientTexture->UVSet.Set("UV2");
        }
    }

    auto phongMaterial = FbxSurfacePhong::Create(fbx_scene_, materialName.c_str());
    if (ambientTexture != nullptr) {
        phongMaterial->Ambient.ConnectSrcObject(ambientTexture);
    }
    phongMaterial->AmbientFactor.Set(1.0);
    phongMaterial->Diffuse.ConnectSrcObject(texture);
    phongMaterial->TransparencyFactor.Set(0.0);
    phongMaterial->ShadingModel.Set("Phong");
    phongMaterial->Shininess.Set(0.0);
    phongMaterial->SpecularFactor.Set(0.0);

    return phongMaterial;
}

void FbxExporterMaterial::exportMaterial(
    std::string outputFilepath, const Bindings::Material::SharedPtr& material
) {
    const auto* texture = GCL::Utilities::getMaterialTexture(material->getData());
    const auto material_name =
        sanitizeMaterialName(material->getData()->Name, texture ? texture->FromFileName : "");

    if (material->getData()->Texture) {
        return;
    }

    if (!texture) {
        material->setNode(addMaterial(material, material_name, outputFilepath));
    } else {
        std::string textureFileNameWithExtension =
            getTextureFilePath(outputFilepath, const_cast<GrannyTexture*>(texture));
        material->setNode(
            addMaterial(material, material_name, outputFilepath, textureFileNameWithExtension)
        );
    }
}

}  // namespace GCL::Exporter
