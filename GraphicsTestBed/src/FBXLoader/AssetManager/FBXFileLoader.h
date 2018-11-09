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
#pragma once
#include <fbxsdk.h>
#include <memory>
#include <string>

#include "Model/Model.h"

namespace ntr
{
namespace AssetLoader
{


class FBXFileLoader
{
public:
  /*!*****************************************************************************
  \brief
    FBXFileLoader constructor.
    Calls the necessary fbxsdk functions and sets up the file loader for
    loading files.
  *******************************************************************************/
  FBXFileLoader();

  /*!*****************************************************************************
  \brief
    FBXFileLoader destructor.
    Cleans up.
  *******************************************************************************/
  ~FBXFileLoader();

  /*!*****************************************************************************
  \brief
    Reads an ".fbx" file using fbxsdk file io. Writes to a scene pointer.

  \param infile
    Filepath of the desired ".fbx" file.

  \param loaded_scene
    Output variable. Returns a pointer to the loaded FBX scene.
  *******************************************************************************/
  bool LoadFile(const std::string &infile, FbxScene *&loaded_scene);

  /*!*****************************************************************************
    \brief
    Run the preprocessor on a given scene. Outputs an ".mdl" file which
    contains the vertex data for loading into the game engine.

    See "vertex_layout.txt" for the layout of the file.

    \param scene
    Loaded fbx scene. Must be an FBX scene pointer returned from the
    fbxsdk.

    \param outfile
    Name of the output file.
  *******************************************************************************/
  bool Preprocess(FbxScene *scene, const std::string &outfile);

  /*!*****************************************************************************
  \brief
    Debugging function. Prints out the fbx tree from a scene.

    \param scene
    Loaded fbx scene. Must be an FBX scene pointer returned from the
    fbxsdk.
  *******************************************************************************/
  void PrintTree(FbxScene *scene);

  void PrintModel(const Model &model);

private:
  void LoadModelData(FbxNode *p_node, Model &model);
  void LoadSkeletonData(FbxNode *p_node, Model &model, int joint_index, int parent_joint_index);
  void LoadAnimationData(FbxScene *scene, Model &model);

  void WriteModelToFile(Model &model, const std::string &filename);

  FbxManager *m_fbxManager;

  const uint32_t FILE_LAYOUT_VERSION = 0;

};

}
}