#include "gcl/exporter/fbxexportermodule.h"

#include <utility>

namespace GCL::Exporter {

FbxExporterModule::FbxExporterModule(Bindings::Scene::SharedPtr scene, FbxScene* fbxScene)
    : scene_(std::move(scene)), fbx_scene_(fbxScene) {
}

FbxExporterModule::~FbxExporterModule() = default;

}  // namespace GCL::Exporter
