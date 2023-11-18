#include "ObjectManager.h"

#define _CRT_SECURE_NO_WARNINGS

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		delete[] m_ObjectList[i].m_pos;
		delete[] m_ObjectList[i].m_col;
		delete[] m_ObjectList[i].m_inex;
	}
}

int ObjectManager::GetRandomIntValue(GLfloat min, GLfloat max)
{
	GLfloat value;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<GLfloat> dis(min, max);

	value = dis(gen);

	return int(value);
}

GLfloat ObjectManager::GetRandomFloatValue(GLfloat min, GLfloat max)
{
	GLfloat value;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<GLfloat> dis(min, max);

	value = dis(gen);

	return value;
}

GLvoid InitObjectStruct(ObjectInfo* objInfo, int num_ver, int sp, int si, int ni, int drawType, bool modeIDX, bool isAinm, bool isActive)
{
	objInfo->m_num_vertex = num_ver;
	objInfo->m_size_pos = sp;
	objInfo->m_size_idx = si;
	objInfo->m_num_idx = ni;
	objInfo->m_DRAW_TYPE = drawType;

	objInfo->m_pivot[0] = 0.0f;
	objInfo->m_pivot[1] = 0.0f;
	objInfo->m_pivot[2] = 0.0f;

	objInfo->m_rotate[0] = 0.0f;
	objInfo->m_rotate[1] = 0.0f;
	objInfo->m_rotate[2] = 0.0f;

	objInfo->m_scale[0] = 0.5f;
	objInfo->m_scale[1] = 0.5f;
	objInfo->m_scale[2] = 0.5f;

	objInfo->m_vel[0] = 0.0f;
	objInfo->m_vel[1] = 0.0f;
	objInfo->m_vel[2] = 0.0f;

	objInfo->m_mass = 1.0f;

	objInfo->m_isModeIDX = modeIDX;
	objInfo->m_isAnimRotating = isAinm;
	objInfo->m_isActive = isActive;

	objInfo->m_model_pos[0] = 0.0f;
	objInfo->m_model_pos[1] = 0.0f;
	objInfo->m_model_pos[2] = 0.0f;
	objInfo->m_model_pos[3] = 0.0f;

	objInfo->m_modelInfo[0] = 0.0f;
	objInfo->m_modelInfo[1] = 0.0f;
	objInfo->m_modelInfo[2] = 0.0f;
	objInfo->m_modelInfo[3] = 0.0f;

	objInfo->m_model = glm::mat4(1.0f);
}

GLvoid InitObject(ObjectInfo* objInfo, float r, float g, float b, int drawType, bool modeIDX, bool isActive)
{
	objInfo->m_DRAW_TYPE = drawType;

	objInfo->m_pivot[0] = 0.0f;
	objInfo->m_pivot[1] = 0.0f;
	objInfo->m_pivot[2] = 0.0f;

	objInfo->m_rotate[0] = 0.0f;
	objInfo->m_rotate[1] = 0.0f;
	objInfo->m_rotate[2] = 0.0f;

	objInfo->m_scale[0] = 0.5f;
	objInfo->m_scale[1] = 0.5f;
	objInfo->m_scale[2] = 0.5f;

	objInfo->m_color[0] = r;
	objInfo->m_color[1] = g;
	objInfo->m_color[2] = b;

	objInfo->m_isActive = isActive;

	objInfo->m_isModeIDX = modeIDX;

	objInfo->m_model = glm::mat4(1.0f);
}

void ObjectManager::CreateSmallCube(float r, float g, float b)
{
	ObjectInfo temp_obj;
	RoadObj("smallCube.txt", &temp_obj);

	// vertexs
	for (unsigned int i = 0; i < temp_obj.vertexIndices.size(); i++) {
		unsigned int vertexIndex = temp_obj.vertexIndices[i];
		glm::vec3 vertex = temp_obj.temp_vertices[vertexIndex - 1];
		temp_obj.vertices.push_back(vertex);
	}

	// uvs
	for (unsigned int i = 0; i < temp_obj.uvIndices.size(); i++) {
		unsigned int uvIndex = temp_obj.uvIndices[i];
		glm::vec2 uv = temp_obj.temp_uvs[uvIndex - 1];
		temp_obj.uvs.push_back(uv);
	}

	// normals
	for (unsigned int i = 0; i < temp_obj.normalIndices.size(); i++) {
		unsigned int normalIndex = temp_obj.normalIndices[i];
		glm::vec3 normal = temp_obj.temp_normals[normalIndex - 1];
		temp_obj.normals.push_back(normal);
	}

	InitObject(&temp_obj, r, g, b, GL_TRIANGLES, true, true);

	m_ObjectList.emplace_back(temp_obj);
}

void ObjectManager::CreateCube(float r, float g, float b)
{
	ObjectInfo temp_obj;
	RoadObj("cube.txt", &temp_obj);
	
	// vertexs
	for (unsigned int i = 0; i < temp_obj.vertexIndices.size(); i++) {
		unsigned int vertexIndex = temp_obj.vertexIndices[i];
		glm::vec3 vertex = temp_obj.temp_vertices[vertexIndex - 1];
		temp_obj.vertices.push_back(vertex);
	}

	// uvs
	for (unsigned int i = 0; i < temp_obj.uvIndices.size(); i++) {
		unsigned int uvIndex = temp_obj.uvIndices[i];
		glm::vec2 uv = temp_obj.temp_uvs[uvIndex - 1];
		temp_obj.uvs.push_back(uv);
	}

	// normals
	for (unsigned int i = 0; i < temp_obj.normalIndices.size(); i++) {
		unsigned int normalIndex = temp_obj.normalIndices[i];
		glm::vec3 normal = temp_obj.temp_normals[normalIndex - 1];
		temp_obj.normals.push_back(normal);
	}

	InitObject(&temp_obj, r, g, b, GL_TRIANGLES, true, true);

	m_ObjectList.emplace_back(temp_obj);
}

void ObjectManager::CreateSquarePyramid(float r, float g, float b)
{
	ObjectInfo temp_obj;
	RoadObj("pyramid.txt", &temp_obj);

	// vertexs
	for (unsigned int i = 0; i < temp_obj.vertexIndices.size(); i++) {
		unsigned int vertexIndex = temp_obj.vertexIndices[i];
		glm::vec3 vertex = temp_obj.temp_vertices[vertexIndex - 1];
		temp_obj.vertices.push_back(vertex);
	}

	// uvs
	for (unsigned int i = 0; i < temp_obj.uvIndices.size(); i++) {
		unsigned int uvIndex = temp_obj.uvIndices[i];
		glm::vec2 uv = temp_obj.temp_uvs[uvIndex - 1];
		temp_obj.uvs.push_back(uv);
	}

	// normals
	for (unsigned int i = 0; i < temp_obj.normalIndices.size(); i++) {
		unsigned int normalIndex = temp_obj.normalIndices[i];
		glm::vec3 normal = temp_obj.temp_normals[normalIndex - 1];
		temp_obj.normals.push_back(normal);
	}

	InitObject(&temp_obj, r, g, b, GL_TRIANGLES, true, true);

	m_ObjectList.emplace_back(temp_obj);
}

void ObjectManager::SetChangeActive(int mode)
{
	if (m_ObjectList.size() <= 2) return;

	// 정육면체를 보이게 하고 사각뿔 비활성화
	if (mode == 0)
	{
		m_ObjectList[1].m_isActive = true;
		m_ObjectList[2].m_isActive = false;
	}
	else if (mode == 1)
	{
		m_ObjectList[1].m_isActive = false;
		m_ObjectList[2].m_isActive = true;
	}

	return;
}

void ObjectManager::ChangeWireSolidType()
{
	int drawType;

	if (m_WireType == false)
	{
		drawType = GL_LINE_LOOP;
		m_WireType = true;
	}
	else
	{
		drawType = GL_TRIANGLES;
		m_WireType = false;
	}

	for (int i = 1; i < m_ObjectList.size(); i++)
	{
		// 2 5 8 not
		if (i != 2 && i != 5 && i != 8) m_ObjectList[i].m_DRAW_TYPE = drawType;
	}
}

void ObjectManager::SetWireSolidType(int idx, int type)
{
	m_ObjectList[idx].m_DRAW_TYPE = type;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  [ 변환 ] //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

glm::mat4 ObjectManager::TransformModel(int idx)
{
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	if (!m_ObjectList.empty())
	{
		float scale_x_parent; float scale_y_parent; float scale_z_parent;
		float move_x_parent; float move_y_parent; float move_z_parent;
		float rotate_y_parent;

		float move_x = m_ObjectList[idx].m_pivot[0];
		float move_y = m_ObjectList[idx].m_pivot[1];
		float move_z = m_ObjectList[idx].m_pivot[2];

		float rotate_x = m_ObjectList[idx].m_rotate[0];
		float rotate_y = m_ObjectList[idx].m_rotate[1];
		float rotate_z = m_ObjectList[idx].m_rotate[2];

		float scale_x = m_ObjectList[idx].m_scale[0];
		float scale_y = m_ObjectList[idx].m_scale[1];
		float scale_z = m_ObjectList[idx].m_scale[2];

		model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(scale_x, scale_y, scale_z));
		model = glm::rotate(model, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(move_x, move_y, move_z));

	}

	return model;
}

void ObjectManager::SetPosition(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	m_ObjectList[idx].m_pivot[0] = x;
	m_ObjectList[idx].m_pivot[1] = y;
	m_ObjectList[idx].m_pivot[2] = z;
}

void ObjectManager::SetRotate(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	m_ObjectList[idx].m_rotate[0] = x;
	m_ObjectList[idx].m_rotate[1] = y;
	m_ObjectList[idx].m_rotate[2] = z;
}

void ObjectManager::SetScale(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	m_ObjectList[idx].m_scale[0] = x;
	m_ObjectList[idx].m_scale[1] = y;
	m_ObjectList[idx].m_scale[2] = z;

	m_ObjectList[idx].m_mass = (x * 100) * (y * 100) * (z * 100);
}

void ObjectManager::SetActive(int idx, bool bActive)
{
	if (m_ObjectList.empty()) return;

	m_ObjectList[idx].m_isActive = bActive;
}

void ObjectManager::SetAllPositon(float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	for (int i = 1; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].m_pivot[0] += x;
		m_ObjectList[i].m_pivot[1] += y;
		m_ObjectList[i].m_pivot[2] += z;
	}
}

void ObjectManager::SetAllRotate(float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	for (int i = 1; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].m_rotate[0] += x;
		m_ObjectList[i].m_rotate[1] += y;
		m_ObjectList[i].m_rotate[2] += z;
	}
}

void ObjectManager::SetAllScale(float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	for (int i = 1; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].m_scale[0] = x;
		m_ObjectList[i].m_scale[1] = y;
		m_ObjectList[i].m_scale[2] = z;
	}
}

void ObjectManager::Move(int idx, float x, float y, float z)
{
	m_ObjectList[idx].m_pivot[0] += x;
	m_ObjectList[idx].m_pivot[1] += y;
	m_ObjectList[idx].m_pivot[2] += z;
}

void ObjectManager::Rotate(int idx, float x, float y, float z)
{
	if (x != 0.0f) m_ObjectList[idx].m_rotate[0] += x;
	else if (y != 0.0f) m_ObjectList[idx].m_rotate[1] += y;
	else if (z != 0.0f) m_ObjectList[idx].m_rotate[2] += z;
}

void ObjectManager::Scale(int idx, float x, float y, float z)
{
	m_ObjectList[idx].m_scale[0] += x;
	m_ObjectList[idx].m_scale[1] += y;
	m_ObjectList[idx].m_scale[2] += z;

	float mass_x = m_ObjectList[idx].m_scale[0];
	float mass_y = m_ObjectList[idx].m_scale[1];
	float mass_z = m_ObjectList[idx].m_scale[2];

	m_ObjectList[idx].m_mass = (mass_x * 100) * (mass_y * 100) * (mass_z * 100);
}

void ObjectManager::Reset()
{
	m_ObjectList.clear();
}

bool ObjectManager::RoadObj(const char* file_name, ObjectInfo* objInfo)
{
	char bind[128];

	FILE* file;
	fopen_s(&file, file_name, "r");

	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (!feof(file)) {

		// read the first word of the line
		fscanf_s(file, "%s", bind, sizeof(bind));

		if (bind[0] == 'v' && bind[1] == '\0') {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			objInfo->temp_vertices.push_back(vertex);
		}
		if ((bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0')) {
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			objInfo->temp_uvs.push_back(uv);
		}
		if ((bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0')) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			objInfo->temp_normals.push_back(normal);
		}
		if (bind[0] == 'f' && bind[1] == '\0') {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			objInfo->vertexIndices.push_back(vertexIndex[0]);
			objInfo->vertexIndices.push_back(vertexIndex[1]);
			objInfo->vertexIndices.push_back(vertexIndex[2]);
			objInfo->uvIndices.push_back(uvIndex[0]);
			objInfo->uvIndices.push_back(uvIndex[1]);
			objInfo->uvIndices.push_back(uvIndex[2]);
			objInfo->normalIndices.push_back(normalIndex[0]);
			objInfo->normalIndices.push_back(normalIndex[1]);
			objInfo->normalIndices.push_back(normalIndex[2]);
		}
	}

	return true;
}






