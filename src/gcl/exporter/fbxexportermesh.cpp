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
        vector<FbxCluster*> clusters;
        vector<BoneBinding::SharedPtr> bindings;
        map<string, Bone::SharedPtr> boneMap;

        for (auto bone : model->getBones()) {
            boneMap[bone->getData().Name] = bone;
        }

        for (auto i = 0; i < mesh->getData()->BoneBindingCount; i++) {
            auto boneName = mesh->getData()->BoneBindings[i].BoneName;
            auto cluster = FbxCluster::Create(m_fbxScene, boneName);
            auto binding = make_shared<BoneBinding>(boneMap[boneName], cluster);

            bindings.push_back(binding);
        }

        auto grannyVertices = mesh->getRigidVertices();
        auto grannyVertexCounter = 0;

        if (mesh->isRigid()) {
            for (unsigned i = 0; i < grannyVertices.size(); i++) {
                for (auto binds : bindings) {
                    binds->getCluster()->AddControlPointIndex(grannyVertexCounter, 255.0);
                }

                grannyVertexCounter++;
            }
        } else {
            for (auto vertex : grannyVertices) {
                for (int i = 0; i < 4; i++) {
                    if (bindings[vertex.BoneIndices[i]] && vertex.BoneWeights[i]) {
                        bindings[vertex.BoneIndices[i]]->getCluster()->AddControlPointIndex(grannyVertexCounter, static_cast<double>(vertex.BoneWeights[i] / 255.0));
                    }
                }

                grannyVertexCounter++;
            }
        }

        FbxAMatrix Mat = meshNode->EvaluateGlobalTransform();

        auto skin = FbxSkin::Create(m_fbxScene, "MeshSkin");

        // Set skinning type to rigid.
        skin->SetSkinningType(FbxSkin::EType::eRigid);

        for (auto binding : bindings) {
            auto bone = binding->getBone();
            if (bone) {
                auto boneNode = bone->getNode();
                if (boneNode) {
                    auto cluster = binding->getCluster();

                    // Set cluster bone link and linking mode.
                    cluster->SetLink(boneNode);
                    cluster->SetLinkMode(FbxCluster::ELinkMode::eNormalize);

                    // Set cluster transform matrix.
                    cluster->SetTransformMatrix(Mat);
                    cluster->SetTransformLinkMatrix(boneNode->EvaluateGlobalTransform());

                    skin->AddCluster(cluster);
                }
            }
        }

        fbxMesh->AddDeformer(skin);
    }
}

FbxMesh* FbxExporterMesh::exportFbxMesh(Mesh::SharedPtr mesh)
{
    // Create the mesh and three data sources for the vertex positions, normals and texture coordinates.
    FbxMesh* fbxMesh = FbxMesh::Create(m_fbxScene, mesh->getData()->Name);

    // Get vertices.
    auto grannyVertices = mesh->getRigidVertices();

    // Create control points.
    unsigned verticesCount(grannyVertices.size());
    fbxMesh->InitControlPoints(static_cast<int>(verticesCount));
    FbxVector4* controlPoints = fbxMesh->GetControlPoints();

    for (unsigned i = 0; i < grannyVertices.size(); i++) {
        controlPoints[i] = FbxVector4(
            static_cast<double>(grannyVertices[i].Position[0]),
            static_cast<double>(grannyVertices[i].Position[1]),
            static_cast<double>(grannyVertices[i].Position[2]));
    }

    // Create normal layer.
    FbxLayerElementNormal* normalLayer = fbxMesh->CreateElementNormal();
    normalLayer->SetMappingMode(FbxLayerElement::eByControlPoint);
    normalLayer->SetReferenceMode(FbxLayerElement::eDirect);

    for (unsigned i = 0; i < grannyVertices.size(); i++) {
        normalLayer->GetDirectArray().Add(FbxVector4(
            static_cast<double>(grannyVertices[i].Normal[0]),
            static_cast<double>(grannyVertices[i].Normal[1]),
            static_cast<double>(grannyVertices[i].Normal[2])));
    }

    // Create uv-set 1.
    FbxGeometryElementUV* uv1 = fbxMesh->CreateElementUV("UV1");
    uv1->SetMappingMode(FbxLayerElement::eByControlPoint);
    uv1->SetReferenceMode(FbxLayerElement::eDirect);

    for (unsigned i = 0; i < grannyVertices.size(); i++) {
        uv1->GetDirectArray().Add(FbxVector2(
            static_cast<double>(grannyVertices[i].UV1[0]),
            static_cast<double>(1.0 - grannyVertices[i].UV1[1])));
    }

    // Create uv-set 2.
    FbxGeometryElementUV* uv2 = fbxMesh->CreateElementUV("UV2");
    uv2->SetMappingMode(FbxLayerElement::eByControlPoint);
    uv2->SetReferenceMode(FbxLayerElement::eDirect);

    for (unsigned i = 0; i < grannyVertices.size(); i++) {
        uv2->GetDirectArray().Add(FbxVector2(
            static_cast<double>(grannyVertices[i].UV2[0]),
            static_cast<double>(1.0 - grannyVertices[i].UV2[1])));
    }

    // Create indices.
    const auto indexCount = GrannyGetMeshIndexCount(mesh->getData());
    const auto indexArray = new int[static_cast<unsigned>(indexCount)];
    GrannyCopyMeshIndices(mesh->getData(), 4, indexArray);

    // TODO: Refactor
    vector<Material*> added;

    for (auto i = 0; i < mesh->getData()->MaterialBindingCount; i++) {
        const auto material = mesh->getData()->MaterialBindings[i].Material;

        for (auto sceneMaterial : m_scene->getMaterials()) {
            if (sceneMaterial->getData() == material && find(added.begin(), added.end(), sceneMaterial.get()) == added.end()) {
                added.push_back(sceneMaterial.get());
                mesh->getNode()->AddMaterial(sceneMaterial->getNode());
                break;
            }
        }
    }

    // TODO: Refactor
    for (auto i = 0; i < indexCount; i += 3) {
        auto materialIndex = -1;

        if (mesh->getData()->MaterialBindingCount) {
            for (auto groupIndex = 0; groupIndex < mesh->getData()->PrimaryTopology->GroupCount; groupIndex++) {
                auto group = mesh->getData()->PrimaryTopology->Groups[groupIndex];
                auto groupTriOffset = i / 3;

                if (groupTriOffset >= group.TriFirst && groupTriOffset < (group.TriFirst + group.TriCount)) {
                    FbxSurfaceMaterial* currentMaterial = nullptr;

                    for (auto sceneMaterial : m_scene->getMaterials()) {
                        if (sceneMaterial->getData() == mesh->getData()->MaterialBindings[group.MaterialIndex].Material) {
                            currentMaterial = sceneMaterial->getNode();
                            break;
                        }
                    }

                    for (auto r = 0; r < mesh->getNode()->GetMaterialCount(); r++) {
                        if (mesh->getNode()->GetMaterial(r) == currentMaterial) {
                            materialIndex = r;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        fbxMesh->BeginPolygon(materialIndex);
        fbxMesh->AddPolygon(indexArray[i]);
        fbxMesh->AddPolygon(indexArray[i + 1]);
        fbxMesh->AddPolygon(indexArray[i + 2]);
        fbxMesh->EndPolygon();
    }

    delete[] indexArray;

    mesh->getNode()->SetNodeAttribute(fbxMesh);

    return fbxMesh;
}

string FbxExporterMesh::sanitizeMaterialName(string name)
{
    return sanitizeName(name);
}

} // namespace GCL::Exporter
