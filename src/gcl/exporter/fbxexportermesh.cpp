#include "gcl/exporter/fbxexportermesh.h"

#include <unordered_set>
#include <utility>

namespace GCL::Exporter {

void FbxExporterMesh::ExportMeshes(const Bindings::Model::SharedPtr& model, bool export_skeleton) {
    if (!model) {
        return;
    }
    for (const Bindings::Mesh::SharedPtr& mesh : model->getMeshes()) {
        if (!mesh) {
            continue;
        }
        if (!mesh->isExcluded()) {
            ExportMesh(model, mesh, export_skeleton);
        }
    }
}

void FbxExporterMesh::ExportMesh(
    const Bindings::Model::SharedPtr& model,
    const Bindings::Mesh::SharedPtr& mesh,
    bool export_skeleton
) {
    if (!model || !mesh || !mesh->getData()) {
        return;
    }
    FbxNode* mesh_node = FbxNode::Create(fbx_scene_, mesh->getData()->name);
    if (!mesh_node) {
        return;
    }
    mesh->setNode(mesh_node);
    fbx_scene_->GetRootNode()->AddChild(mesh_node);
    FbxMesh* fbx_mesh = ExportFbxMesh(mesh);
    if (export_skeleton && !model->getBones().empty()) {
        CreateBoneWeightsAndApplyDeformation(model, mesh, mesh_node, fbx_mesh);
    }
}

void FbxExporterMesh::CreateBoneWeightsAndApplyDeformation(
    const Bindings::Model::SharedPtr& model,
    const Bindings::Mesh::SharedPtr& mesh,
    FbxNode* mesh_node,
    FbxMesh* fbx_mesh
) {
    if (!model || !mesh || !mesh->getData() || !mesh_node || !fbx_mesh) {
        return;
    }
    AllBones all_bones;
    BoundBones bound_bones;
    BoneBindingList bone_bindings;
    InitializeBoneBindings(model, mesh, &all_bones, &bound_bones, &bone_bindings);
    const bool is_rigid = mesh->isRigid();
    const std::vector<GrannyPWNT34322Vertex> vertices = mesh->getVertices();
    if (is_rigid) {
        ApplyRigidBoneWeights(mesh, bone_bindings, vertices);
    } else {
        ApplyNonRigidBoneWeights(mesh, bone_bindings, vertices);
    }
    MapUnboundBones(&all_bones, &bound_bones, vertices, &bone_bindings);
    CreateMeshDeformation(mesh_node, fbx_mesh, bone_bindings, is_rigid);
}

void FbxExporterMesh::InitializeBoneBindings(
    const Bindings::Model::SharedPtr& model,
    const Bindings::Mesh::SharedPtr& mesh,
    AllBones* all_bones,
    BoundBones* bound_bones,
    BoneBindingList* bone_bindings
) const {
    if (!model || model->getBones().empty() || !mesh || !mesh->getData() || !all_bones ||
        !bound_bones || !bone_bindings) {
        return;
    }
    for (const Bindings::Bone::SharedPtr& bone : model->getBones()) {
        all_bones->data[bone->getData().name] = bone;
    }
    const GrannyMesh* mesh_data = mesh->getData();
    const int bone_binding_count = mesh_data->bone_binding_count;
    bone_bindings->reserve(bone_bindings->size() + static_cast<size_t>(bone_binding_count));
    for (int bone_binding_index = 0; bone_binding_index < bone_binding_count;
         ++bone_binding_index) {
        const char* bone_name = mesh_data->bone_bindings[bone_binding_index].bone_name;
        auto bone_iterator = all_bones->data.find(bone_name);
        if (bone_iterator == all_bones->data.end()) {
            continue;
        }
        const Bindings::Bone::SharedPtr& bone = bone_iterator->second;
        FbxCluster* bone_cluster = FbxCluster::Create(fbx_scene_, bone_name);
        auto bone_binding = std::make_shared<Bindings::BoneBinding>(bone, bone_cluster);
        bound_bones->data[bone_name] = bone;
        bone_bindings->push_back(std::move(bone_binding));
    }
    bone_bindings->shrink_to_fit();
}

void FbxExporterMesh::ApplyRigidBoneWeights(
    const Bindings::Mesh::SharedPtr& mesh,
    const BoneBindingList& bone_bindings,
    const std::vector<GrannyPWNT34322Vertex>& vertices
) {
    if (!mesh || bone_bindings.empty() || vertices.empty()) {
        return;
    }
    constexpr double kFullWeight = 255.0;
    const int vertex_count = static_cast<int>(vertices.size());
    for (const Bindings::BoneBinding::SharedPtr& bone_binding : bone_bindings) {
        FbxCluster* cluster = bone_binding->getCluster();
        if (!cluster) {
            continue;
        }
        for (int vertex_index = 0; vertex_index < vertex_count; ++vertex_index) {
            cluster->AddControlPointIndex(vertex_index, kFullWeight);
        }
    }
}

void FbxExporterMesh::ApplyNonRigidBoneWeights(
    const Bindings::Mesh::SharedPtr& mesh,
    const BoneBindingList& bone_bindings,
    const std::vector<GrannyPWNT34322Vertex>& vertices
) {
    if (!mesh || bone_bindings.empty() || vertices.empty()) {
        return;
    }
    constexpr int kMaxBoneInfluences = 4;
    constexpr double kMaxBoneWeight = 255.0;
    const size_t max_bindings_count = bone_bindings.size();
    int vertex_counter = 0;
    for (const GrannyPWNT34322Vertex& vertex : vertices) {
        for (int weight_index = 0; weight_index < kMaxBoneInfluences; ++weight_index) {
            uint8_t weight = vertex.bone_weights[weight_index];
            if (weight == 0U) {
                continue;
            }
            uint8_t bone_index = vertex.bone_indices[weight_index];
            if (bone_index >= max_bindings_count) {
                continue;
            }
            const Bindings::BoneBinding::SharedPtr& bone_binding = bone_bindings[bone_index];
            if (!bone_binding) {
                continue;
            }
            FbxCluster* cluster = bone_binding->getCluster();
            if (!cluster) {
                continue;
            }
            const double normalized_weight = static_cast<double>(weight) / kMaxBoneWeight;
            cluster->AddControlPointIndex(vertex_counter, normalized_weight);
        }
        ++vertex_counter;
    }
}

void FbxExporterMesh::MapUnboundBones(
    const AllBones* all_bones,
    const BoundBones* bound_bones,
    const std::vector<GrannyPWNT34322Vertex>& vertices,
    BoneBindingList* bone_bindings
) {
    if (!all_bones || !bound_bones || !bone_bindings || vertices.empty()) {
        return;
    }
    const int vertex_count = static_cast<int>(vertices.size());
    bone_bindings->reserve(bone_bindings->size() + all_bones->data.size());
    for (const auto& [bone_name, bone] : all_bones->data) {
        if (bound_bones->data.find(bone_name) == bound_bones->data.end()) {
            FbxCluster* bone_cluster = FbxCluster::Create(fbx_scene_, bone_name.c_str());
            auto bone_binding = std::make_shared<Bindings::BoneBinding>(bone, bone_cluster);
            bone_bindings->push_back(bone_binding);
            FbxCluster* cluster = bone_binding->getCluster();
            if (!cluster) {
                continue;
            }
            for (int vertex_index = 0; vertex_index < vertex_count; ++vertex_index) {
                cluster->AddControlPointIndex(vertex_index, 0.0);
            }
        }
    }
    bone_bindings->shrink_to_fit();
}

void FbxExporterMesh::CreateMeshDeformation(
    FbxNode* mesh_node, FbxMesh* mesh, const BoneBindingList& bone_bindings, bool is_rigid
) {
    if (!mesh_node || !mesh || bone_bindings.empty()) {
        return;
    }
    FbxAMatrix mesh_matrix = mesh_node->EvaluateGlobalTransform();
    FbxSkin* mesh_skin = FbxSkin::Create(fbx_scene_, "MeshSkin");
    mesh_skin->SetSkinningType(is_rigid ? FbxSkin::EType::eRigid : FbxSkin::EType::eLinear);
    for (const Bindings::BoneBinding::SharedPtr& bone_binding : bone_bindings) {
        Bindings::Bone::SharedPtr bone = bone_binding->getBone();
        if (!bone) {
            continue;
        }
        FbxNode* bone_node = bone->getNode();
        if (!bone_node) {
            continue;
        }
        FbxCluster* cluster = bone_binding->getCluster();
        if (!cluster) {
            continue;
        }
        cluster->SetLink(bone_node);
        cluster->SetLinkMode(FbxCluster::ELinkMode::eNormalize);
        cluster->SetTransformMatrix(mesh_matrix);
        FbxAMatrix bone_matrix = bone_node->EvaluateGlobalTransform();
        cluster->SetTransformLinkMatrix(bone_matrix);
        mesh_skin->AddCluster(cluster);
    }
    mesh->AddDeformer(mesh_skin);
}

FbxMesh* FbxExporterMesh::ExportFbxMesh(const Bindings::Mesh::SharedPtr& mesh) {
    if (!mesh || !mesh->getData() || !mesh->getNode()) {
        return nullptr;
    }
    FbxMesh* fbx_mesh = FbxMesh::Create(fbx_scene_, mesh->getData()->name);
    if (!fbx_mesh) {
        return nullptr;
    }
    const std::vector<GrannyPWNT34322Vertex> vertices = mesh->getVertices();
    CreateControlPoints(fbx_mesh, vertices);
    CreateMaterial(fbx_mesh);
    CreateNormal(fbx_mesh, vertices);
    CreateUV(mesh, fbx_mesh, vertices);
    auto& granny = Importer::GrannyLibrary::instance();
    auto index_count = static_cast<size_t>(granny.GetMeshIndexCount(mesh->getData()));
    if (index_count == 0 || (index_count % 3) != 0) {
        return fbx_mesh;
    }
    std::vector<int> index_array(index_count);
    granny.CopyMeshIndices(mesh->getData(), 4, index_array.data());
    BindMaterials(mesh);
    for (size_t index = 0; index < index_count; index += 3) {
        fbx_mesh->BeginPolygon(GetMaterialForIndex(mesh, index));
        fbx_mesh->AddPolygon(index_array[index]);
        fbx_mesh->AddPolygon(index_array[index + 1]);
        fbx_mesh->AddPolygon(index_array[index + 2]);
        fbx_mesh->EndPolygon();
    }
    mesh->getNode()->SetNodeAttribute(fbx_mesh);
    mesh->getNode()->SetShadingMode(FbxNode::eTextureShading);
    return fbx_mesh;
}

void FbxExporterMesh::CreateControlPoints(
    FbxMesh* mesh, const std::vector<GrannyPWNT34322Vertex>& vertices
) {
    if (!mesh || vertices.empty()) {
        return;
    }
    const int vertices_count = static_cast<int>(vertices.size());
    mesh->InitControlPoints(vertices_count);
    FbxVector4* control_points = mesh->GetControlPoints();
    if (!control_points) {
        return;
    }
    int vertex_index = 0;
    for (const GrannyPWNT34322Vertex& vertex : vertices) {
        control_points[vertex_index] = FbxVector4(
            static_cast<double>(vertex.position[0]),
            static_cast<double>(vertex.position[1]),
            static_cast<double>(vertex.position[2])
        );
        ++vertex_index;
    }
}

void FbxExporterMesh::CreateMaterial(FbxMesh* mesh) {
    if (!mesh) {
        return;
    }
    FbxGeometryElementMaterial* material_element = mesh->CreateElementMaterial();
    if (!material_element) {
        return;
    }
    material_element->SetMappingMode(FbxLayerElement::EMappingMode::eByPolygon);
    material_element->SetReferenceMode(FbxLayerElement::EReferenceMode::eIndexToDirect);
    material_element->GetIndexArray().Add(0);
}

void FbxExporterMesh::CreateNormal(
    FbxMesh* mesh, const std::vector<GrannyPWNT34322Vertex>& vertices
) {
    if (!mesh) {
        return;
    }
    FbxGeometryElementNormal* normal_element = mesh->CreateElementNormal();
    if (!normal_element) {
        return;
    }
    normal_element->SetMappingMode(FbxLayerElement::EMappingMode::eByControlPoint);
    normal_element->SetReferenceMode(FbxLayerElement::EReferenceMode::eDirect);
    const int vertices_count = static_cast<int>(vertices.size());
    auto& direct_array = normal_element->GetDirectArray();
    if (!direct_array.Resize(vertices_count)) {
        return;
    }
    for (int vertex_index = 0; vertex_index < vertices_count; ++vertex_index) {
        const GrannyPWNT34322Vertex& vertex = vertices[static_cast<size_t>(vertex_index)];
        direct_array.SetAt(
            vertex_index,
            FbxVector4(
                static_cast<double>(vertex.normal[0]),
                static_cast<double>(vertex.normal[1]),
                static_cast<double>(vertex.normal[2])
            )
        );
    }
}

int FbxExporterMesh::CountTextureCoordinateTypes(const Bindings::Mesh::SharedPtr& mesh) {
    if (!mesh || !mesh->getData() || !mesh->getData()->primary_vertex_data ||
        !mesh->getData()->primary_vertex_data->vertex_type) {
        return 0;
    }
    const int total_type_size = Importer::GrannyLibrary::instance().GetTotalTypeSize(
        mesh->getData()->primary_vertex_data->vertex_type
    );
    const int type_count = static_cast<int>(total_type_size / sizeof(GrannyDataTypeDefinition));
    int texture_coordinate_types_count = 0;
    const size_t match_length = strlen(GrannyVertexTextureCoordinatesName);
    for (int type_index = 0; type_index < type_count; ++type_index) {
        const GrannyDataTypeDefinition& vertex_type =
            mesh->getData()->primary_vertex_data->vertex_type[type_index];
        const char* vertex_type_name = vertex_type.name;
        if (!vertex_type_name) {
            continue;
        }
        const bool is_texture_coordinate_type =
            _strnicmp(vertex_type_name, GrannyVertexTextureCoordinatesName, match_length) == 0;
        if (is_texture_coordinate_type) {
            ++texture_coordinate_types_count;
        }
    }
    return texture_coordinate_types_count;
}

void FbxExporterMesh::CreateUV(
    const Bindings::Mesh::SharedPtr& mesh,
    FbxMesh* fbx_mesh,
    const std::vector<GrannyPWNT34322Vertex>& vertices
) {
    if (!mesh || !fbx_mesh || vertices.empty()) {
        return;
    }
    CreateUV1(fbx_mesh, vertices);
    int texture_coordinate_types_count = CountTextureCoordinateTypes(mesh);
    if (texture_coordinate_types_count < 2) {
        return;
    }
    CreateUV2(fbx_mesh, vertices);
}

void FbxExporterMesh::CreateUV1(
    FbxMesh* fbx_mesh, const std::vector<GrannyPWNT34322Vertex>& vertices
) {
    if (!fbx_mesh || vertices.empty()) {
        return;
    }
    FbxGeometryElementUV* element_uv = fbx_mesh->CreateElementUV("UV1");
    if (!element_uv) {
        return;
    }
    element_uv->SetMappingMode(FbxLayerElement::eByControlPoint);
    element_uv->SetReferenceMode(FbxLayerElement::eDirect);
    const int vertices_count = static_cast<int>(vertices.size());
    auto& direct_array = element_uv->GetDirectArray();
    if (!direct_array.Resize(vertices_count)) {
        return;
    }
    for (int vertex_index = 0; vertex_index < vertices_count; ++vertex_index) {
        const GrannyPWNT34322Vertex& vertex = vertices[static_cast<size_t>(vertex_index)];
        const double u_coord = static_cast<double>(vertex.uv1[0]);
        const double v_coord = 1.0 - static_cast<double>(vertex.uv1[1]);
        direct_array.SetAt(vertex_index, FbxVector2(u_coord, v_coord));
    }
}

void FbxExporterMesh::CreateUV2(
    FbxMesh* fbx_mesh, const std::vector<GrannyPWNT34322Vertex>& vertices
) {
    if (!fbx_mesh || vertices.empty()) {
        return;
    }
    FbxGeometryElementUV* element_uv = fbx_mesh->CreateElementUV("UV2");
    if (!element_uv) {
        return;
    }
    element_uv->SetMappingMode(FbxLayerElement::eByControlPoint);
    element_uv->SetReferenceMode(FbxLayerElement::eDirect);
    const int vertices_count = static_cast<int>(vertices.size());
    auto& direct_array = element_uv->GetDirectArray();
    if (!direct_array.Resize(vertices_count)) {
        return;
    }
    for (int vertex_index = 0; vertex_index < vertices_count; ++vertex_index) {
        const GrannyPWNT34322Vertex& vertex = vertices[static_cast<size_t>(vertex_index)];
        const double u_coord = static_cast<double>(vertex.uv2[0]);
        const double v_coord = 1.0 - static_cast<double>(vertex.uv2[1]);
        direct_array.SetAt(vertex_index, FbxVector2(u_coord, v_coord));
    }
}

void FbxExporterMesh::BindMaterials(const Bindings::Mesh::SharedPtr& mesh) {
    if (!mesh || !mesh->getData() || !mesh->getNode()) {
        return;
    }
    const GrannyMesh* mesh_data = mesh->getData();
    const int material_binding_count = mesh_data->material_binding_count;
    if (material_binding_count == 0) {
        return;
    }
    std::unordered_set<const Bindings::Material*> added_materials;
    added_materials.reserve(static_cast<size_t>(material_binding_count));
    for (int material_binding_index = 0; material_binding_index < material_binding_count;
         ++material_binding_index) {
        const GrannyMaterial* material =
            mesh_data->material_bindings[material_binding_index].material;
        if (!material) {
            continue;
        }
        for (const Bindings::Material::SharedPtr& scene_material : scene_->getMaterials()) {
            if (!scene_material) {
                continue;
            }
            if (scene_material->getData() == material) {
                const Bindings::Material* material_binding = scene_material.get();
                if (added_materials.find(material_binding) == added_materials.end()) {
                    added_materials.insert(material_binding);
                    mesh->getNode()->AddMaterial(scene_material->getNode());
                }
                break;
            }
        }
    }
}

int FbxExporterMesh::GetMaterialForIndex(const Bindings::Mesh::SharedPtr& mesh, size_t index) {
    if (!mesh || !mesh->getData() || !mesh->getNode()) {
        return -1;
    }
    const GrannyMesh* mesh_data = mesh->getData();
    const int material_binding_count = mesh_data->material_binding_count;
    if (material_binding_count == 0 || !mesh_data->primary_topology) {
        return -1;
    }
    FbxNode* mesh_node = mesh->getNode();
    const int group_count = mesh_data->primary_topology->group_count;
    const int group_tri_offset = static_cast<int>(index / 3);
    for (int group_index = 0; group_index < group_count; ++group_index) {
        const GrannyTriMaterialGroup& group = mesh_data->primary_topology->groups[group_index];
        if (group_tri_offset >= group.tri_first &&
            group_tri_offset < (group.tri_first + group.tri_count)) {
            if (group.material_index >= material_binding_count) {
                return -1;
            }
            const GrannyMaterial* target_granny_material =
                mesh_data->material_bindings[group.material_index].material;
            if (!target_granny_material) {
                return -1;
            }
            FbxSurfaceMaterial* current_material = FindSceneMaterial(target_granny_material);
            if (!current_material) {
                return -1;
            }
            return FindMaterialIndexAtNode(mesh_node, current_material);
        }
    }
    return -1;
}

FbxSurfaceMaterial* FbxExporterMesh::FindSceneMaterial(
    const GrannyMaterial* target_material
) const {
    if (!target_material) {
        return nullptr;
    }
    for (const Bindings::Material::SharedPtr& scene_material : scene_->getMaterials()) {
        if (scene_material && scene_material->getData() == target_material) {
            return scene_material->getNode();
        }
    }
    return nullptr;
}

int FbxExporterMesh::FindMaterialIndexAtNode(
    FbxNode* mesh_node, const FbxSurfaceMaterial* target_material
) {
    if (!mesh_node || !target_material) {
        return -1;
    }
    const int node_material_count = mesh_node->GetMaterialCount();
    for (int material_index = 0; material_index < node_material_count; ++material_index) {
        if (mesh_node->GetMaterial(material_index) == target_material) {
            return material_index;
        }
    }
    return -1;
}

std::string FbxExporterMesh::SanitizeMaterialName(std::string name) {
    return Utilities::sanitizeName(std::move(name));
}

}  // namespace GCL::Exporter
