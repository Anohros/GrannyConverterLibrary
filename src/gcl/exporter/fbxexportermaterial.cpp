#include "gcl/exporter/fbxexportermaterial.h"

#include <filesystem>

namespace GCL::Exporter {

FbxExporterMaterial::FbxExporterMaterial(Scene::SharedPtr scene, FbxScene* fbxScene)
    : FbxExporterModule(scene, fbxScene)
{
    GCL::Utilities::initializeDevilImageLibrary();
}

FbxExporterMaterial::~FbxExporterMaterial()
{
    GCL::Utilities::shutdownDevilImageLibrary();
}

void FbxExporterMaterial::exportMaterials(string outputFilepath)
{
    for (auto material : m_scene->getMaterials()) {
        if (!material->isExcluded()) {
            exportMaterial(outputFilepath, material);
        }
    }
}

string FbxExporterMaterial::getTextureFilePath(string outputFilepath, GrannyTexture* texture)
{
    auto sourceTextureFilePath = string(texture->FromFileName);
    auto sourceTextureFileName = sourceTextureFilePath;

    auto fileNameBeginsOffset = sourceTextureFilePath.find_last_of('\\');
    if (fileNameBeginsOffset == string::npos) {
        fileNameBeginsOffset = sourceTextureFilePath.find_last_of('/');
    }

    if (fileNameBeginsOffset != string::npos) {
        sourceTextureFileName = sourceTextureFilePath.substr(fileNameBeginsOffset + 1);
    }

    const auto textureFileNameWithoutExtension = sourceTextureFileName.substr(0, sourceTextureFileName.find_first_of('.'));
    const auto textureFileNameWithExtension = sanitizeFileName(textureFileNameWithoutExtension).append(".png");

    if (!ifstream(sourceTextureFilePath.c_str()).good()) {
        if (ifstream(sourceTextureFileName.c_str()).good()) {
            sourceTextureFilePath = sourceTextureFileName;
        } else {
            bool foundTexture = false;

            for (auto& searchPath : m_scene->getSearchPaths()) {
                const auto lookupPath = searchPath + sourceTextureFileName;
                if (ifstream(lookupPath.c_str()).good()) {
                    sourceTextureFilePath = lookupPath;
                    foundTexture = true;
                    break;
                }
            }

            const auto parentImportedPath = filesystem::path(m_scene->getImportedFilePaths().front())
                                                .parent_path()
                                                .parent_path();

            if (!foundTexture) {
                for (const auto& entry : filesystem::directory_iterator(parentImportedPath, filesystem::directory_options::skip_permission_denied)) {
                    const auto lookupPath = (entry.path() / sourceTextureFileName).u8string();
                    if (ifstream(lookupPath.c_str()).good()) {
                        sourceTextureFilePath = lookupPath;
                        foundTexture = true;
                        break;
                    }
                }
            }

            if (!foundTexture) {
                const auto deeperParentImportedPath = parentImportedPath
                                                          .parent_path()
                                                          .parent_path();
                // Do not scan root path and program files as parent paths.
                if (deeperParentImportedPath.root_path().string() != deeperParentImportedPath.string()) {
                    if (!regex_match(deeperParentImportedPath.string(), regex("\\\\(Program Files \\(x86\\)|Program Files)\\\\"))) {
                        for (const auto& entry : filesystem::recursive_directory_iterator(deeperParentImportedPath, filesystem::directory_options::skip_permission_denied)) {
                            const auto lookupPath = (entry.path() / sourceTextureFileName).u8string();
                            if (ifstream(lookupPath.c_str()).good()) {
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
    if (outputFilepathFileSeparator == string::npos) {
        outputFilepathFileSeparator = outputFilepath.find_last_of('/');
    }
    const auto targetTextureFilePath = outputFilepath.substr(0, outputFilepathFileSeparator + 1) + textureFileNameWithExtension;

    if (ifstream(sourceTextureFilePath.c_str()).good()) {
        GCL::Utilities::convertImage(sourceTextureFilePath, targetTextureFilePath);
    } else {
        GCL::Utilities::exportTexture(texture, targetTextureFilePath, true);
    }

    return textureFileNameWithExtension;
}

FbxSurfaceMaterial* FbxExporterMaterial::addMaterial(
    Material::SharedPtr material,
    const string materialName,
    const string outputFilepath,
    const string textureFilePath)
{
    auto texture = FbxFileTexture::Create(m_fbxScene, "Diffuse Texture");
    if (!textureFilePath.empty()) {
        texture->SetFileName(textureFilePath.c_str());
    }
    texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    texture->UVSet.Set("UV1");

    FbxFileTexture* ambientTexture = nullptr;
    if (material->getData()->MapCount > 1) {
        const auto grannyAmbientTexture = GCL::Utilities::getMaterialTexture(material->getData()->Maps[1].Material);
        if (grannyAmbientTexture != nullptr) {
            ambientTexture = FbxFileTexture::Create(m_fbxScene, "Ambient Texture");
            const string ambientTextureFilePath = getTextureFilePath(outputFilepath, grannyAmbientTexture);
            if (!ambientTextureFilePath.empty()) {
                ambientTexture->SetFileName(ambientTextureFilePath.c_str());
            }
            ambientTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
            ambientTexture->UVSet.Set("UV2");
        }
    }

    auto phongMaterial = FbxSurfacePhong::Create(m_fbxScene, materialName.c_str());
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

void FbxExporterMaterial::exportMaterial(string outputFilepath, Material::SharedPtr material)
{
    const auto texture = GCL::Utilities::getMaterialTexture(material->getData());
    const auto materialName = sanitizeMaterialName(string(material->getData()->Name), texture->FromFileName);

    if (material->getData()->Texture) {
        return;
    }

    if (!texture) {
        material->setNode(addMaterial(material, materialName, outputFilepath));
    } else {
        string textureFileNameWithExtension = getTextureFilePath(outputFilepath, texture);
        material->setNode(addMaterial(material, materialName, outputFilepath, textureFileNameWithExtension));
    }
}

} // namespace GCL::Exporter
