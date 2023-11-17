#pragma once
#pragma once
#include <iostream>
#include "OpenGL.h"
#include "Object.h"
#include <vector>
#include <random>

#define GRAVITY 9.8f

using namespace std;

typedef struct ObjectInfo
{
	float		m_pivot[3];
	float		m_rotate[3];
	float		m_scale[3];
	float		m_vel[3];
	float		m_mass;

	float		m_color[3];

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;

	GLfloat* m_pos;
	GLfloat* m_col;
	GLint* m_inex;
	int			m_num_vertex;
	int			m_size_pos;
	int			m_size_idx;
	int			m_num_idx;
	int			m_DRAW_TYPE;
	bool		m_isAnimRotating;
	bool		m_isActive;
	bool		m_isModeIDX;
	float		m_model_pos[4];

	// ÁÂÇ¥°è Çà·Ä
	float		m_modelInfo[4];
	glm::mat4	m_model;

	bool		m_Initmodel = false;
	bool		m_rotateRevol = false;

	vector<int> m_child;
};

class ObjectManager
{
public:
	ObjectInfo			temp;
	vector<ObjectInfo>	m_ObjectList;

	bool				m_WireType = false;

public:
	ObjectManager();
	~ObjectManager();

	int GetRandomIntValue(GLfloat min, GLfloat max);
	GLfloat GetRandomFloatValue(GLfloat min, GLfloat max);

	void CreateCube(float r, float g, float b);
	void CreateSquarePyramid(float r, float g, float b);

	void SetChangeActive(int mode);
	void ChangeWireSolidType();
	void SetWireSolidType(int idx, int type);

	// ÁÂÇ¥°è º¯È­
	glm::mat4 TransformModel(int idx);

	void SetPosition(int idx, float x, float y, float z);
	void SetRotate(int idx, float x, float y, float z);
	void SetScale(int idx, float x, float y, float z);
	void SetActive(int idx, bool bActive);

	void SetAllPositon(float x, float y, float z);
	void SetAllRotate(float x, float y, float z);
	void SetAllScale(float x, float y, float z);

	void Move(int idx, float x, float y, float z);
	void Rotate(int idx, float x, float y, float z);
	void Scale(int idx, float x, float y, float z);

	void Reset();

	bool RoadObj(const char* file, ObjectInfo* objInfo);
};





