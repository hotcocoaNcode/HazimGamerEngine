#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
public:
	//Model(const char* file); //base constructor, if you just need a file view (usually not enough)

	Model(const char* file, float posit0, float posit1, float posit2, float scale0, float scale1, float scale2); //what will actually be used (un)commonly

	

	void Draw(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	//meshes n stuf
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;
	float pos[3];
	float sca[3];


	//no loady twicey
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	std::string text;

	void loadMesh(unsigned int indMesh);

	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndicies(json accessor);
	std::vector<Vertex> assembleVerticies(std::vector<glm::vec3> positions,std::vector<glm::vec3> normals,std::vector<glm::vec2> texUVs);
	std::vector<Texture> getTextures();

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif