#include "ObjImporter.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <glm/glm.hpp>

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
	for (int i = 0, f = 0; i < mesh->mNumVertices; i++, f=i/3) {
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

		// Area factor:
		glm::vec3 uv1 = glm::vec3(mesh->mTextureCoords[0][f*3+0].x, mesh->mTextureCoords[0][f*3+0].y, 0.0);
		glm::vec3 uv2 = glm::vec3(mesh->mTextureCoords[0][f*3+1].x, mesh->mTextureCoords[0][f*3+1].y, 0.0);
		glm::vec3 uv3 = glm::vec3(mesh->mTextureCoords[0][f*3+2].x, mesh->mTextureCoords[0][f*3+2].y, 0.0);
		float uvA = 0.5f * glm::length(glm::cross(uv2-uv1, uv3-uv1));

		glm::vec3 v1 = glm::vec3(mesh->mVertices[f*3+0].x, mesh->mVertices[f*3+0].y, mesh->mVertices[f*3+0].z);
		glm::vec3 v2 = glm::vec3(mesh->mVertices[f*3+1].x, mesh->mVertices[f*3+1].y, mesh->mVertices[f*3+1].z);
		glm::vec3 v3 = glm::vec3(mesh->mVertices[f*3+2].x, mesh->mVertices[f*3+2].y, mesh->mVertices[f*3+2].z);
		float vA = 0.5f * glm::length(glm::cross(v2-v1, v3-v1));

		vertexData.push_back(uvA/vA);

	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
}
