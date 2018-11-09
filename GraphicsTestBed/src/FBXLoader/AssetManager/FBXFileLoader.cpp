/*!*****************************************************************************
\file   FBXFileLoader.h
\author Caylee Ayson
\par    Digipen Email: c.ayson\@digipen.edu
\par    DigiPen login: c.ayson
\date   09/28/2018
\brief
Processes ".fbx" files into a compact custom format containing the
necessary vertex data.

\par Copyright (C) DigiPen Institute of Technology 2017

*******************************************************************************/
#include "FBXFileLoader.h"
#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>


#include "FileWriter.h"
#include "../RoseEngine/Trace/Trace.h"

namespace ntr
{
namespace AssetLoader {
  ////////////////////////////////////////////////////////////////////////////////
  // Defines:

  #define NTR_RESERVED_BYTE 0

  #define NTR_SKELETON_FLAG 0

  #define NTR_NORMALS_FLAG  0
  #define NTR_UV_FLAG       1
  #define NTR_TANGENT_FLAG  2
  #define NTR_BINORMAL_FLAG 3
  #define NTR_DEFORMER_FLAG 4

  ////////////////////////////////////////////////////////////////////////////////

FBXFileLoader::FBXFileLoader()
{
  m_fbxManager = FbxManager::Create();
    // Create IO Settings
  FbxIOSettings *io_settings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
  m_fbxManager->SetIOSettings(io_settings);
//
//  // Determines what kinds of properties we load from FBX files
//#define NTR_LoadProperty(Propertys, Boolean) \
//          io_settings->SetBoolProp(Property, Boolean);
//
//#include "FBXIOSettings.inl"
//#undef NTR_LoadProperty



}

FBXFileLoader::~FBXFileLoader()
{
  m_fbxManager->Destroy();
}

static size_t tab_count = 0;
static void print_tabs()
{
  for (size_t i = 0; i < tab_count; ++i)
    std::cout << " -- ";
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void print_node(FbxNode *p_node)
{
  print_tabs();
  FbxDouble3 trans = p_node->LclTranslation.Get();
  FbxDouble3 rot = p_node->LclRotation.Get();
  FbxDouble3 scale = p_node->LclScaling.Get();

  std::cout << p_node->GetName();
  //std::cout << " - { Translation: ";
  //std::cout << "(" << trans[0] << ", " << trans[1] << ", " << trans[2] << ") - ";
  //std::cout << "Rotation: ";
  //std::cout << "(" << rot[0] << ", " << rot[1] << ", " << rot[2] << ") - ";
  //std::cout << "Scale: ";
  //std::cout << "(" << scale[0] << ", " << scale[1] << ", " << scale[2] << ")}";
  //std::cout << std::endl;

  //print_tabs();

    // List of all attribute types
  const char *attribute_type_names[] =
  { "Unknown", "Null", "Marker", "Skeleton", "Mesh", "Nurbs", "Patch",
    "Camera", "CameraStero", "CameraSwitcher", "Light", "OpticalReference",
    "OpticalMarker", "NurbsCurve", "TrimNurbsSurface", "Boundary",
    "NurbsSurface", "Shape", "LODGroup", "SubDiv", "CachedEffect", "Line" };

  for (int i = 0; i < p_node->GetNodeAttributeCount(); ++i)
  {
    FbxNodeAttribute *attribute = p_node->GetNodeAttributeByIndex(i);
    std::cout << " (" << attribute_type_names[attribute->GetAttributeType()] << ")";

      // print out vertices if mesh
    if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
    {
      //FbxMesh *mesh = p_node->GetMesh();
    }
  }

  std::cout << std::endl;

    // Print Children
  ++tab_count;

  for (int i = 0; i < p_node->GetChildCount(); ++i)
    print_node(p_node->GetChild(i));

  --tab_count;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool FBXFileLoader::LoadFile(const std::string & filepath, FbxScene *&loaded_scene)
{
    // Create the Importer
  FbxImporter *importer = FbxImporter::Create(m_fbxManager, "");
  bool status = importer->Initialize(filepath.c_str(), -1, m_fbxManager->GetIOSettings());

  if (!status)
  {
    return false;
  }
  
    // Load the scene
  loaded_scene = FbxScene::Create(m_fbxManager, "new_scene");
  importer->Import(loaded_scene);
  importer->Destroy();
  
  return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void FBXFileLoader::PrintTree(FbxScene *loaded_scene)
{
  FbxNode *root_node = loaded_scene->GetRootNode();

  for (int i = 0; i < root_node->GetChildCount(); ++i)
    print_node(root_node->GetChild(i));
}

void FBXFileLoader::PrintModel(const Model &)
{
    // Print Meshes
  //for (size_t i = 0; i < model.m_meshes.size(); ++i)
  //{
  //  traceI << "Mesh #" << i << std::endl;


  //}
}

void FBXFileLoader::LoadModelData(FbxNode * p_node, Model & model)
{
  for (int i = 0; i < p_node->GetNodeAttributeCount(); ++i)
  {
    FbxNodeAttribute *attribute = p_node->GetNodeAttributeByIndex(i);

    // Check if this node is a mesh
    if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
    {
      traceD << "Compressing Mesh #: " << model.m_meshes.size() << std::endl;

        // Set up mesh for loading
      FbxMesh *fbx_mesh = static_cast<FbxMesh*>(attribute);
      fbx_mesh->GenerateNormals(true, true, false);

      model.m_meshes.push_back(Mesh(fbx_mesh->GetControlPointsCount(), (fbx_mesh->GetPolygonCount() * 3)));
      Mesh &new_mesh = model.m_meshes.back();

        // Load positions
      FbxVector4 *positions = fbx_mesh->GetControlPoints();
      for (int j = 0; j < static_cast<int>(new_mesh.m_numVertices); ++j)
      {
        new_mesh.m_positions.push_back(static_cast<float>(positions[j][0]));
        new_mesh.m_positions.push_back(static_cast<float>(positions[j][1]));
        new_mesh.m_positions.push_back(static_cast<float>(positions[j][2]));
      }

        // Load indices
      int *indices = fbx_mesh->GetPolygonVertices();
      for (int j = 0; j < static_cast<int>(new_mesh.m_numIndices); ++j)
        new_mesh.m_indices.push_back(indices[j]);

        // There is usually only one layer per mesh. We can assume this.
      FbxLayer *element_layer = fbx_mesh->GetLayer(0);

        // Load normals
      FbxLayerElementNormal *normals = element_layer->GetNormals();
      if (normals)
      {
        model.m_containsNormals = true;
        new_mesh.LoadNormals(normals);
      }
  
        // Load UVs
      FbxLayerElementUV *uvs = element_layer->GetUVs();
      if (uvs)
      {
        model.m_containsUVs = true;
        new_mesh.LoadUVS(uvs, fbx_mesh);
      }

        // Loads tangents
      FbxLayerElementTangent *tangents = element_layer->GetTangents();
      if (tangents)
      {
        model.m_containsTangents = true;
        new_mesh.LoadTangents(tangents);
      }

        // Load binormals
      FbxLayerElementBinormal *binormals = element_layer->GetBinormals();
      if (binormals)
      {
        model.m_containsBinormals = true;
        new_mesh.LoadBinormals(binormals);
      }

        // Usually one deformer per mesh
      for (int j = 0; j < fbx_mesh->GetDeformerCount(); ++j)
      {
        // If we have a deformer, this is a rigged model
        new_mesh.m_containsDeformer = true;
        new_mesh.m_blendingWeights.resize(new_mesh.m_numVertices);
        FbxDeformer *deformer = fbx_mesh->GetDeformer(j);
        
        // Only doing skin deformers
        if (deformer->GetDeformerType() != FbxDeformer::EDeformerType::eSkin)
          continue;

        FbxSkin *fbx_skin = static_cast<FbxSkin*>(deformer);
        int cluster_count = fbx_skin->GetClusterCount();

        // A cluster is a set of vertices that a joint controls
        for (int i_cluster = 0; i_cluster < cluster_count; ++i_cluster)
        {
          // Find the joint associated with this cluster
          FbxCluster *fbx_cluster = fbx_skin->GetCluster(i_cluster);
          std::string joint_name = fbx_cluster->GetLink()->GetName();
          auto joint = std::find(model.m_skeleton.m_joints.begin(), model.m_skeleton.m_joints.end(), joint_name);

          FbxAMatrix transform_matrix;
          FbxAMatrix transform_link_matrix;
          FbxAMatrix global_bind_pose_matrix;

          // Calculate bind pose matrix, which is the starting transformation matrix
          fbx_cluster->GetTransformMatrix(transform_matrix);
          fbx_cluster->GetTransformLinkMatrix(transform_link_matrix);
          global_bind_pose_matrix = transform_link_matrix.Inverse() * transform_matrix;

          // Add the matrix to the joint
          joint->m_globalBindPoseMatrix = global_bind_pose_matrix;
          joint->m_jointNode = fbx_cluster->GetLink();

          for (int i_index = 0; i_index < fbx_cluster->GetControlPointIndicesCount(); ++i_index)
          {
            float weight = static_cast<float>(fbx_cluster->GetControlPointWeights()[i_index]);
            size_t j_index = joint - model.m_skeleton.m_joints.begin();

            new_mesh.m_blendingWeights[fbx_cluster->GetControlPointIndices()[i_index]].push_back({ static_cast<uint32_t>(j_index) , weight });
          }
        }
      }
    }
  }

  for (int i = 0; i < p_node->GetChildCount(); ++i)
    LoadModelData(p_node->GetChild(i), model);
}

void FBXFileLoader::LoadSkeletonData(FbxNode *p_node, Model &model, int joint_index, int parent_joint_index)
{
  for (int i = 0; i < p_node->GetNodeAttributeCount(); ++i)
  {
    if (p_node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eSkeleton)
    {
      model.m_skeleton.m_joints.push_back(Joint(p_node->GetName(), parent_joint_index, p_node));
      model.m_containsSkeleton = true;
    }
  }

  for (int i = 0; i < p_node->GetChildCount(); ++i)
    LoadSkeletonData(p_node->GetChild(i), model, static_cast<int>(model.m_skeleton.m_joints.size()), joint_index);
}

void FBXFileLoader::LoadAnimationData(FbxScene *scene, Model &model)
{
  // Animation stacks are like individual animations
  int anim_count = scene->GetSrcObjectCount<FbxAnimStack>();

  for (int i = 0; i < anim_count; ++i)
  {
    // Set the animation evaluator's current stack to this animation
    FbxAnimStack *anim_stack = scene->GetSrcObject<FbxAnimStack>(i);
    scene->SetCurrentAnimationStack(anim_stack);

    // Animation layers are used to blend anhimations together.
    // Bake layers together for easierness, with 1 second intervals
    FbxTimeSpan anim_span = anim_stack->GetLocalTimeSpan();
    //FbxTime time;
    //time.SetSecondDouble(1.0);

    //anim_stack->BakeLayers(scene->GetAnimationEvaluator(), anim_span.GetStart(), anim_span.GetStop(), time);

    FbxTime duration = anim_span.GetDuration();

    // For each joint, extract all keyframes for this animation
    for (auto &joint : model.m_skeleton.m_joints)
    {
      joint.m_animations.push_back(Animation(anim_count));
      Animation &anim = joint.m_animations.back();
      
       // Base keyframe is identity;
      anim.m_keyFrames.push_back(Keyframe());

      // Evaluate frames at 24 frames per second.
      for (FbxTime time = FbxTime(0); time < duration; time += FbxTime::GetOneFrameValue(FbxTime::eFrames24))
      {
        Keyframe key_frame;

        key_frame.m_time = static_cast<uint32_t>(time.GetMilliSeconds());
        key_frame.m_translation = joint.m_jointNode->EvaluateLocalTranslation(time);
        key_frame.m_rotation = joint.m_jointNode->EvaluateLocalRotation(time);
        key_frame.m_scale = joint.m_jointNode->EvaluateLocalScaling(time);

        
        if (anim.m_keyFrames.back() != key_frame)
          anim.m_keyFrames.push_back(key_frame);
      }
    }
  }
}

void FBXFileLoader::WriteModelToFile(Model &model, const std::string &filename)
{
    // Marks file as an mdl file in the header/footer
  static const char mdl_header[5] = ".mdl";
  static const char mdl_footer[17] = ".mdl END OF FILE";

  FileWriter file_writer;
  
    // File Header
  file_writer.Write(mdl_header, 4);
  file_writer.Write(FILE_LAYOUT_VERSION, sizeof(FILE_LAYOUT_VERSION));
  
  uint32_t num_meshes = static_cast<uint32_t>(model.m_meshes.size());
  file_writer.Write(num_meshes, sizeof(num_meshes));

  bool header_flags[4] = { NTR_RESERVED_BYTE };
  header_flags[NTR_SKELETON_FLAG] = model.m_containsSkeleton;
  file_writer.Write(header_flags, sizeof(bool) * 4);

    // Skeleton Mesh
  if (model.m_containsSkeleton) {
    uint32_t num_joints = static_cast<uint32_t>(model.m_skeleton.m_joints.size());
    file_writer.Write(num_joints, sizeof(uint32_t));
  }

  for (auto &joint : model.m_skeleton.m_joints) {
    file_writer.Write(joint.m_parentIndex, sizeof(uint32_t));
    FbxDouble4 *GBP_array = joint.m_globalBindPoseMatrix.Buffer();

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        float value = static_cast<float_t>(GBP_array[i][j]);

        if (-FBXSDK_FLOAT_EPSILON < value && value < FBXSDK_FLOAT_EPSILON)
          value = 0.0f;

        file_writer.Write<float>(value, sizeof(float_t));
      }
    }
  }

    // Meshes
  for (auto &mesh : model.m_meshes) {
    file_writer.Write(mesh.m_numVertices, sizeof(uint64_t));
    
      // Flags
    bool mesh_flags[8] = { NTR_RESERVED_BYTE };
    mesh_flags[NTR_NORMALS_FLAG] = !mesh.m_normals.empty();
    mesh_flags[NTR_UV_FLAG] = !mesh.m_uvs.empty();
    mesh_flags[NTR_TANGENT_FLAG] = !mesh.m_tangents.empty();
    mesh_flags[NTR_BINORMAL_FLAG] = !mesh.m_binormals.empty();
    mesh_flags[NTR_DEFORMER_FLAG] = mesh.m_containsDeformer;
  
    file_writer.Write(mesh_flags, sizeof(bool) * 8);
    
      // Vertices
    file_writer.Write(*mesh.m_positions.data(), sizeof(float_t) * 3 * mesh.m_numVertices);
    if (mesh_flags[NTR_NORMALS_FLAG])
      file_writer.Write(*mesh.m_normals.data(), sizeof(float_t) * 3 * mesh.m_numVertices);
    if (mesh_flags[NTR_UV_FLAG])
      file_writer.Write(*mesh.m_uvs.data(), sizeof(float_t) * 2 * mesh.m_numVertices);
    if (mesh_flags[NTR_TANGENT_FLAG])
      file_writer.Write(*mesh.m_tangents.data(), sizeof(float_t) * 3 * mesh.m_numVertices);
    if(mesh_flags[NTR_BINORMAL_FLAG])
      file_writer.Write(*mesh.m_binormals.data(), sizeof(float_t) * 3 * mesh.m_numVertices);

      // Indices
    file_writer.Write(mesh.m_numIndices, sizeof(uint64_t));
    file_writer.Write(*mesh.m_indices.data(), sizeof(uint64_t) * mesh.m_numIndices);

      // Clusters
    if (mesh.m_containsDeformer) 
    {
      for (auto &cluster : mesh.m_blendingWeights) {
        uint64_t num_blending_weights = cluster.size();
        file_writer.Write(num_blending_weights, sizeof(uint64_t));
     
        for (auto &weight : cluster) {
          file_writer.Write(weight.joint_index, sizeof(uint32_t));
          file_writer.Write(weight.blend_weight, sizeof(double_t));
        }
      }
    }

  }

  uint32_t num_animations = model.m_containsSkeleton ? static_cast<uint32_t>(model.m_skeleton.m_joints.front().m_animations.size()) : 0;
  file_writer.Write(num_animations, sizeof(uint32_t));

    // Animation Data
  for (uint32_t animation_id = 0; animation_id < num_animations; ++animation_id) 
  {
    for (auto &joint : model.m_skeleton.m_joints) 
    {
      uint32_t num_keyframes = static_cast<uint32_t>(joint.m_animations[animation_id].m_keyFrames.size());
      file_writer.Write(num_keyframes, sizeof(uint32_t));

      for (auto &keyframe : joint.m_animations[animation_id].m_keyFrames) {
          // milliseconds
        file_writer.Write(keyframe.m_time, sizeof(uint32_t));
        
          // translation
        for (size_t i = 0; i < 3; ++i) {
          float_t trans = static_cast<float_t>(keyframe.m_translation.mData[i]);
          file_writer.Write(trans, sizeof(float_t));
        }

          // scale
        for (size_t i = 0; i < 3; ++i) {
          float_t scale = static_cast<float_t>(keyframe.m_scale.mData[i]);
          file_writer.Write(scale, sizeof(float_t));
        }

          // rotation
        for (size_t i = 0; i < 4; ++i) {
          float_t scale = static_cast<float_t>(keyframe.m_rotation.mData[i]);
          file_writer.Write(scale, sizeof(float_t));
        }
      }
    }
  }

  file_writer.Write(mdl_footer, 16);
  file_writer.WriteToFile(filename);
}

bool FBXFileLoader::Preprocess(FbxScene *scene, const std::string &output)
{
  Model model;
  FbxNode *root_node = scene->GetRootNode();

    // Load Skeleton
  for (int i = 0; i < root_node->GetChildCount(); ++i)
    LoadSkeletonData(root_node->GetChild(i), model, 0, -1);

    // Load Model afterwards, because if there's a skeleton, then deformers in the model will rely on it
  for (int i = 0; i < root_node->GetChildCount(); ++i)
    LoadModelData(root_node->GetChild(i), model);

    // Load Animation Data
  if (model.m_containsSkeleton)
    LoadAnimationData(scene, model);

  WriteModelToFile(model, output);
  return true;
}
}
}