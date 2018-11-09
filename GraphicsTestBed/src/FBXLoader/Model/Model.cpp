#include "Model.h"
#include "../RoseEngine/Trace/Trace.h"
#pragma warning (disable: 4244)
namespace ntr
{
namespace AssetLoader
{

Mesh::Mesh(int num_verts, int num_indicies) : m_numVertices(num_verts), m_numIndices(num_indicies), m_containsDeformer(false)
{
  m_positions.reserve(num_verts * 3);
  m_indices.reserve(num_indicies * 3);

  m_normals.reserve(num_verts * 3);
  m_uvs.reserve(num_verts * 2);
  m_tangents.reserve(num_verts * 3);
  m_binormals.reserve(num_verts * 3);
}

Mesh::~Mesh()
{

}

void Mesh::LoadNormals(FbxLayerElementNormal *normals)
{

  // Normally, we'd want to check for MappingMode with GetMappingMode(),
  // but we are assuming we only have one normal per vertex, so we don't need to
  switch (normals->GetReferenceMode())
  {
  case FbxGeometryElement::eDirect:
    for (int j = 0; j < static_cast<int>(m_numVertices); ++j)
    {
      m_normals.push_back(((*normals->mDirectArray)[j][0]));
      m_normals.push_back(((*normals->mDirectArray)[j][1]));
      m_normals.push_back(((*normals->mDirectArray)[j][2]));
    }
    break;
  case FbxGeometryElement::eIndexToDirect:
    for (int j = 0; j < normals->GetIndexArray().GetCount(); ++j)
    {
      int index = normals->GetIndexArray().GetAt(j);
      m_normals.push_back(((*normals->mDirectArray)[index][0]));
      m_normals.push_back(((*normals->mDirectArray)[index][1]));
      m_normals.push_back(((*normals->mDirectArray)[index][2]));
    }
    break;
  default:
    traceE << "Error: Invalid Reference Mode" << std::endl;
  }
}

void Mesh::LoadUVS(FbxLayerElementUV *uvs, FbxMesh *mesh)
{
  const int TRIANGLE_SIZE = 3;
  
  if (uvs->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    if (uvs->GetReferenceMode() == FbxGeometryElement::eDirect)
    {
      for (int j = 0; j < static_cast<int>(m_numVertices); ++j) {
        m_uvs.push_back(((*uvs->mDirectArray)[j][0]));
        m_uvs.push_back(((*uvs->mDirectArray)[j][1]));
      }
    }
    else if (uvs->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
      for (int j = 0; j < uvs->GetIndexArray().GetCount(); ++j) {
        int uv_index = uvs->GetIndexArray().GetAt(j);
        m_uvs.push_back(((*uvs->mDirectArray)[uv_index][0]));
        m_uvs.push_back(((*uvs->mDirectArray)[uv_index][1]));
      }
    }
    else
      traceE << "Error: Invalid Reference Mode" << std::endl;
  }

  else if (uvs->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
  {
    m_uvs.resize(m_numVertices * 2);

    if (uvs->GetReferenceMode() == FbxGeometryElement::eDirect) {
      for (int j = 0; j < mesh->GetPolygonCount(); ++j) {
        for (int k = 0; k < TRIANGLE_SIZE; ++k)
        {
          int index = mesh->GetPolygonVertex(j, k);
          m_uvs[index*2    ] = uvs->GetDirectArray()[index][0];
          m_uvs[index*2 + 1] = uvs->GetDirectArray()[index][1];
        }
      }
    }
    else if (uvs->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
    {
      for (int j = 0; j < mesh->GetPolygonCount(); ++j) {
        for (int k = 0; k < TRIANGLE_SIZE; ++k)
        {
          int index = mesh->GetPolygonVertex(j, k);
          int uv_index = uvs->GetIndexArray().GetAt(index);
          m_uvs[index * 2] = uvs->GetDirectArray()[uv_index][0];
          m_uvs[index * 2 + 1] = uvs->GetDirectArray()[uv_index][1];
        }
      }
    }
    else
      traceE << "Error: Invalid Reference Mode" << std::endl;
  }
  else
    traceE << "Error: Invalid Mapping Mode" << std::endl;
}

void Mesh::LoadTangents(FbxLayerElementTangent *tangents)
{
  // Normally, we'd want to check for MappingMode with GetMappingMode(),
// but we are assuming we only have one normal per vertex, so we don't need to
  switch (tangents->GetReferenceMode())
  {
  case FbxGeometryElement::eDirect:
    for (int j = 0; j < static_cast<int>(m_numVertices); ++j)
    {
      m_tangents.push_back(((*tangents->mDirectArray)[j][0]));
      m_tangents.push_back(((*tangents->mDirectArray)[j][1]));
      m_tangents.push_back(((*tangents->mDirectArray)[j][2]));
    }
    break;
  case FbxGeometryElement::eIndexToDirect:
    for (int j = 0; j < tangents->GetIndexArray().GetCount(); ++j)
    {
      int index = tangents->GetIndexArray().GetAt(j);
      m_tangents.push_back(((*tangents->mDirectArray)[index][0]));
      m_tangents.push_back(((*tangents->mDirectArray)[index][1]));
      m_tangents.push_back(((*tangents->mDirectArray)[index][2]));
    }
    break;
  default:
    traceE << "Error: Invalid Reference Mode" << std::endl;
  }
}


void Mesh::LoadBinormals(FbxLayerElementBinormal *binormals)
{
  // Normally, we'd want to check for MappingMode with GetMappingMode(),
  // but we are assuming we only have one normal per vertex, so we don't need to
  switch (binormals->GetReferenceMode())
  {
  case FbxGeometryElement::eDirect:
    for (int j = 0; j < static_cast<int>(m_numVertices); ++j)
    {
      m_binormals.push_back(((*binormals->mDirectArray)[j][0]));
      m_binormals.push_back(((*binormals->mDirectArray)[j][1]));
      m_binormals.push_back(((*binormals->mDirectArray)[j][2]));
    }
    break;
  case FbxGeometryElement::eIndexToDirect:
    for (int j = 0; j < binormals->GetIndexArray().GetCount(); ++j)
    {
      int index = binormals->GetIndexArray().GetAt(j);
      m_binormals.push_back(((*binormals->mDirectArray)[index][0]));
      m_binormals.push_back(((*binormals->mDirectArray)[index][1]));
      m_binormals.push_back(((*binormals->mDirectArray)[index][2]));
    }
    break;
  default:
    traceE << "Error: Invalid Reference Mode" << std::endl;
  }
}

Model::~Model()
{

}
}
}
