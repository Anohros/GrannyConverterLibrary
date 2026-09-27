#include "gcl/exporter/fbxexportermodulefactory.h"

namespace GCL::Exporter {

FbxExporterMaterial* FbxExporterModuleFactory::createExporterModuleMaterial(
    Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
) {
    return new FbxExporterMaterial(scene, fbxScene);
}

FbxExporterMesh* FbxExporterModuleFactory::createExporterModuleMesh(
    Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
) {
    return new FbxExporterMesh(scene, fbxScene);
}

FbxExporterSkeleton* FbxExporterModuleFactory::createExporterModuleSkeleton(
    Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
) {
    return new FbxExporterSkeleton(scene, fbxScene);
}

FbxExporterAnimation* FbxExporterModuleFactory::createExporterModuleAnimation(
    Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
) {
    return new FbxExporterAnimation(scene, fbxScene);
}

}  // namespace GCL::Exporter
