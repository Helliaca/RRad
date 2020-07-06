#include "ObjImporter.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

void ObjImporter::import_from_file(const std::string inputfile)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(inputfile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	//directory = path.substr(0, path.find_last_of('/'));
	aiMesh* mesh = scene->mMeshes[0];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertexData.push_back(mesh->mVertices[i].x);
		vertexData.push_back(mesh->mVertices[i].y);
		vertexData.push_back(mesh->mVertices[i].z);

		vertexData.push_back(mesh->mColors[0][i].r);
		vertexData.push_back(mesh->mColors[0][i].g);
		vertexData.push_back(mesh->mColors[0][i].b);

		vertexData.push_back(mesh->mNormals[i].x);
		vertexData.push_back(mesh->mNormals[i].y);
		vertexData.push_back(mesh->mNormals[i].z);

		vertexData.push_back(mesh->mTextureCoords[0][i].x);
		vertexData.push_back(mesh->mTextureCoords[0][i].y);
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
}
