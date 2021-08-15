#include "gcl/bindings/material.h"

namespace GCL::Bindings {

Material::Material(GrannyMaterial* data)
    : m_data(data)
{
}

GrannyMaterial* Material::getData()
{
    return m_data;
}

FbxSurfaceMaterial* Material::getNode()
{
    return m_node;
}

void Material::setData(GrannyMaterial* data)
{
    m_data = data;
}

void Material::setNode(FbxSurfaceMaterial* node)
{
    m_node = node;
}

} // namespace GCL::Bindings
