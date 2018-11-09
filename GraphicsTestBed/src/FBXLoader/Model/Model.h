#pragma once
#include <vector>

#include "Skeleton/Skeleton.h"

#define NTR_BOOL_LOAD_NORMALS true
#define NTR_BOOL_LOAD_UVS true
#define NTR_BOOL_LOAD_TANGENTS true
#define NTR_BOOL_LOAD_BINORMALS true

namespace ntr
{
  namespace AssetLoader
  {

    class Mesh
    {
    public:
      Mesh(int num_verts, int num_indices);
      ~Mesh();

      void LoadNormals(FbxLayerElementNormal *normals);
      void LoadUVS(FbxLayerElementUV *uvs, FbxMesh *mesh);
      void LoadTangents(FbxLayerElementTangent *tangents);
      void LoadBinormals(FbxLayerElementBinormal *binormals);

      uint64_t m_numVertices;
      uint64_t m_numIndices;

      std::vector<float_t>  m_positions;
      std::vector<uint64_t> m_indices;
      std::vector<float_t>  m_normals;
      std::vector<float_t>  m_uvs;
      std::vector<float_t>  m_tangents;
      std::vector<float_t>  m_binormals;

      bool m_containsDeformer;
      std::vector<std::vector<BlendingWeight>> m_blendingWeights;
    };

    class Model
    {
    public:
      Model() : m_containsNormals(false), m_containsUVs(false), 
                m_containsTangents(false), m_containsBinormals(false), 
                m_containsSkeleton(false) {}
      ~Model();

      std::vector<Mesh> m_meshes;
      
      bool m_containsNormals;
      bool m_containsUVs;
      bool m_containsTangents;
      bool m_containsBinormals;
      bool m_containsSkeleton;
      
      Skeleton m_skeleton;
    };
  }
}