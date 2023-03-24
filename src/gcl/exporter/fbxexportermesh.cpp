#include "gcl/exporter/fbxexportermesh.h"

#include <map>
#include <vector>

namespace GCL::Exporter {

void FbxExporterMesh::exportMeshes(Model::SharedPtr model, bool exportSkeleton)
{
    for (auto mesh : model->getMeshes()) {
        if (!mesh->isExcluded()) {
            exportMesh(model, mesh, exportSkeleton);
        }
    }
}

void FbxExporterMesh::exportMesh(Model::SharedPtr model, Mesh::SharedPtr mesh, bool exportSkeleton)
{
    auto meshNode = FbxNode::Create(m_fbxScene, mesh->getData()->Name);
    mesh->setNode(meshNode);

    m_fbxScene->GetRootNode()->AddChild(meshNode);

    auto fbxMesh = exportFbxMesh(mesh);

    if (exportSkeleton && model->getBones().size() > 0) {
        createBoneWeightsAndApplyDeformation(model, mesh, meshNode, fbxMesh);
    }
}

void FbxExporterMesh::createBoneWeightsAndApplyDeformation(
    Model::SharedPtr model,
    Mesh::SharedPtr mesh,
    FbxNode* meshNode,
    FbxMesh* fbxMesh)
{
    map<string, Bone::SharedPtr> boneMap;
    map<string, Bone::SharedPtr> boneMapBinded;
    vector<BoneBinding::SharedPtr> boneBindings;

    for (auto bone : model->getBones()) {
        boneMap[bone->getData().Name] = bone;
    }

    for (auto boneBindingIndex = 0; boneBindingIndex < mesh->getData()->BoneBindingCount; boneBindingIndex++) {
        auto boneName = mesh->getData()->BoneBindings[boneBindingIndex].BoneName;
        auto boneCluster = FbxCluster::Create(m_fbxScene, boneName);
        auto boneBinding = make_shared<BoneBinding>(boneMap[boneName], boneCluster);
        boneMapBinded[boneName] = boneMap[boneName];
        boneBindings.push_back(boneBinding);
    }

    auto vertices = mesh->getRigidVertices();

    if (mesh->isRigid()) {
        for (unsigned vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++) {
            for (auto bineBinding : boneBindings) {
                bineBinding->getCluster()->AddControlPointIndex(vertexIndex, 255.0);
            }
        }
    } else {
        auto vertexCounter = 0;
        for (auto vertex : vertices) {
            for (int boneIndicesIndex = 0; boneIndicesIndex < 4; boneIndicesIndex++) {
                if (boneBindings[vertex.BoneIndices[boneIndicesIndex]] && vertex.BoneWeights[boneIndicesIndex]) {
                    boneBindings[vertex.BoneIndices[boneIndicesIndex]]
                        ->getCluster()
                        ->AddControlPointIndex(vertexCounter, static_cast<double>(vertex.BoneWeights[boneIndicesIndex] / 255.0));
                }
            }

            vertexCounter++;
        }
    }

    // Map bones without bone binding.
    for (auto bone : boneMap) {
        auto boneName = bone.first;
        if (!boneMapBinded[boneName]) {
            auto boneCluster = FbxCluster::Create(m_fbxScene, boneName.c_str());
            auto boneBinding = make_shared<BoneBinding>(boneMap[boneName], boneCluster);
            boneBindings.push_back(boneBinding);
            for (unsigned vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++) {
                boneBinding->getCluster()->AddControlPointIndex(vertexIndex, 0);
            }
        }
    }

    createMeshDeformation(meshNode, fbxMesh, boneBindings);
}

void FbxExporterMesh::createMeshDeformation(FbxNode* meshNode, FbxMesh* mesh, vector<BoneBinding::SharedPtr> boneBindings)
{
    auto meshMatrix = meshNode->EvaluateGlobalTransform();
    auto meshSkin = FbxSkin::Create(m_fbxScene, "MeshSkin");

    // Set skinning type to rigid.
    meshSkin->SetSkinningType(FbxSkin::EType::eRigid);

    for (auto boneBinding : boneBindings) {
        auto bone = boneBinding->getBone();
        if (bone) {
            auto boneNode = bone->getNode();
            if (boneNode) {
                auto cluster = boneBinding->getCluster();

                // Set cluster bone link and linking mode.
                cluster->SetLink(boneNode);
                cluster->SetLinkMode(FbxCluster::ELinkMode::eNormalize);

                // Set cluster transform matrix.
                cluster->SetTransformMatrix(meshMatrix);
                cluster->SetTransformLinkMatrix(boneNode->EvaluateGlobalTransform());

                meshSkin->AddCluster(cluster);
            }
        }
    }

    mesh->AddDeformer(meshSkin);
}

FbxMesh* FbxExporterMesh::exportFbxMesh(Mesh::SharedPtr mesh)
{
    auto fbxMesh = FbxMesh::Create(m_fbxScene, mesh->getData()->Name);
    auto vertices = mesh->getRigidVertices();

    createControlPoints(fbxMesh, vertices);
    createMaterial(fbxMesh);
    createNormal(fbxMesh, vertices);
    createUV(mesh, fbxMesh, vertices);

    const auto indexCount = GrannyGetMeshIndexCount(mesh->getData());
    const auto indexArray = new int[static_cast<unsigned>(indexCount)];
    GrannyCopyMeshIndices(mesh->getData(), 4, indexArray);

    bindMaterials(mesh);

    for (auto index = 0; index < indexCount; index += 3) {
        fbxMesh->BeginPolygon(getMaterialForIndex(mesh, index));
        fbxMesh->AddPolygon(indexArray[index]);
        fbxMesh->AddPolygon(indexArray[index + 1]);
        fbxMesh->AddPolygon(indexArray[index + 2]);
        fbxMesh->EndPolygon();
    }

    delete[] indexArray;

    mesh->getNode()->SetNodeAttribute(fbxMesh);
    mesh->getNode()->SetShadingMode(FbxNode::eTextureShading);

    return fbxMesh;
}

void FbxExporterMesh::createControlPoints(FbxMesh* mesh, vector<GrannyPWNT34322Vertex> vertices)
{
    auto verticesCount(vertices.size());
    mesh->InitControlPoints(static_cast<int>(verticesCount));

    auto controlPoints = mesh->GetControlPoints();
    for (unsigned vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
        controlPoints[vertexIndex] = FbxVector4(
            static_cast<double>(vertices[vertexIndex].Position[0]),
            static_cast<double>(vertices[vertexIndex].Position[1]),
            static_cast<double>(vertices[vertexIndex].Position[2]));
    }
}

void FbxExporterMesh::createMaterial(FbxMesh* mesh)
{
    auto materialElement = mesh->CreateElementMaterial();
    materialElement->SetMappingMode(FbxLayerElement::eByPolygon);
    materialElement->SetReferenceMode(FbxLayerElement::eIndexToDirect);
    materialElement->GetIndexArray().Add(0);
}

void FbxExporterMesh::createNormal(FbxMesh* mesh, vector<GrannyPWNT34322Vertex> vertices)
{
    auto normalElement = mesh->CreateElementNormal();
    normalElement->SetMappingMode(FbxLayerElement::eByControlPoint);
    normalElement->SetReferenceMode(FbxLayerElement::eDirect);

    for (unsigned vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++) {
        normalElement->GetDirectArray().Add(FbxVector4(
            static_cast<double>(vertices[vertexIndex].Normal[0]),
            static_cast<double>(vertices[vertexIndex].Normal[1]),
            static_cast<double>(vertices[vertexIndex].Normal[2])));
    }
}

void FbxExporterMesh::createUV(Mesh::SharedPtr mesh, FbxMesh* fbxMesh, vector<GrannyPWNT34322Vertex> vertices)
{
    // Create uv-set 1.
    FbxGeometryElementUV* uvSetElement1 = fbxMesh->CreateElementUV("UV1");
    uvSetElement1->SetMappingMode(FbxLayerElement::eByControlPoint);
    uvSetElement1->SetReferenceMode(FbxLayerElement::eDirect);

    for (unsigned vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++) {
        uvSetElement1->GetDirectArray().Add(FbxVector2(
            static_cast<double>(vertices[vertexIndex].UV1[0]),
            static_cast<double>(1.0 - vertices[vertexIndex].UV1[1])));
    }

    auto typeCount = GrannyGetTotalTypeSize(mesh->getData()->PrimaryVertexData->VertexType) / sizeof(GrannyDataTypeDefinition);
    auto textureCoordinateTypesCount = 0;
    for (unsigned typeIndex = 0; typeIndex < typeCount; typeIndex++) {
        const auto typeName = mesh->getData()->PrimaryVertexData->VertexType[typeIndex].Name;
        if (typeName == nullptr) {
            continue;
        }
        const auto isTextureCoordinateType = _strnicmp(
                                                 mesh->getData()->PrimaryVertexData->VertexType[typeIndex].Name,
                                                 GrannyVertexTextureCoordinatesName,
                                                 strlen(GrannyVertexTextureCoordinatesName))
            == 0;
        if (isTextureCoordinateType) {
            textureCoordinateTypesCount++;
        }
    }

    if (textureCoordinateTypesCount < 2) {
        return;
    }

    // Create uv-set 2.
    FbxGeometryElementUV* uvSetElement2 = fbxMesh->CreateElementUV("UV2");
    uvSetElement2->SetMappingMode(FbxLayerElement::eByControlPoint);
    uvSetElement2->SetReferenceMode(FbxLayerElement::eDirect);

    for (unsigned vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++) {
        uvSetElement2->GetDirectArray().Add(FbxVector2(
            static_cast<double>(vertices[vertexIndex].UV2[0]),
            static_cast<double>(1.0 - vertices[vertexIndex].UV2[1])));
    }
}

void FbxExporterMesh::bindMaterials(Mesh::SharedPtr mesh)
{
    // Store already added materials in a unique list to prevent duplications.
    vector<Material*> unique;

    for (auto materialBindingIndex = 0; materialBindingIndex < mesh->getData()->MaterialBindingCount; materialBindingIndex++) {
        const auto material = mesh->getData()->MaterialBindings[materialBindingIndex].Material;

        for (auto sceneMaterial : m_scene->getMaterials()) {
            if (sceneMaterial->getData() == material && find(unique.begin(), unique.end(), sceneMaterial.get()) == unique.end()) {
                unique.push_back(sceneMaterial.get());
                mesh->getNode()->AddMaterial(sceneMaterial->getNode());
                break;
            }
        }
    }
}

int FbxExporterMesh::getMaterialForIndex(Mesh::SharedPtr mesh, int index)
{
    if (mesh->getData()->MaterialBindingCount == 0) {
        return -1;
    }

    for (auto groupIndex = 0; groupIndex < mesh->getData()->PrimaryTopology->GroupCount; groupIndex++) {
        auto group = mesh->getData()->PrimaryTopology->Groups[groupIndex];
        auto groupTriOffset = index / 3;

        if (groupTriOffset >= group.TriFirst && groupTriOffset < (group.TriFirst + group.TriCount)) {
            FbxSurfaceMaterial* currentMaterial = nullptr;

            for (auto sceneMaterial : m_scene->getMaterials()) {
                if (sceneMaterial->getData() == mesh->getData()->MaterialBindings[group.MaterialIndex].Material) {
                    currentMaterial = sceneMaterial->getNode();
                    break;
                }
            }

            for (auto materialIndex = 0; materialIndex < mesh->getNode()->GetMaterialCount(); materialIndex++) {
                if (mesh->getNode()->GetMaterial(materialIndex) == currentMaterial) {
                    return materialIndex;
                }
            }
            break;
        }
    }

    return -1;
}

string FbxExporterMesh::sanitizeMaterialName(string name)
{
    return sanitizeName(name);
}

} // namespace GCL::Exporter
