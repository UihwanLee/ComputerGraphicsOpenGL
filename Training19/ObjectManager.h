#pragma once
#pragma once
#include <iostream>
#include "OpenGL.h"
#include "Object.h"
#include <vector>
#include <random>

using namespace std;

typedef struct ObjectInfo
{
	float		m_pivot[3];
	float		m_rotate[3];
	float		m_scale[3];
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

	void CreateCoordinate();
	void CreateCube();
	void CreateCubeFace(int face);
	void CreateTetrahedron();
	void CreateSquarePyramid();
	void CreateSquarePyramidFace(int face);
	void CreateSquarePyramidBottom();
	void CreateCone();
	void CreateSqhere();

	void CreateOrbit(float orbit_radius);

	void SetChangeActive(int mode);
	void ChangeWireSolidType();

	// ÁÂÇ¥°è º¯È­
	glm::mat4 TransformScale(float x, float y, float z);
	glm::mat4 TransformRotate(float x, float y, float z);
	glm::mat4 TransformMove(float x, float y, float z);

	void SetPosition(int idx, float x, float y, float z);
	void SetRotate(int idx, float x, float y, float z);
	void SetScale(int idx, float x, float y, float z);
	void SetModel(int idx);

	void SetAllPositon(float x, float y, float z);
	void SetAllRotate(float x, float y, float z);
	void SetAllScale(float x, float y, float z);
	void SetAllModel();

	void Move(int idx, float x, float y, float z);
	void Rotate(int idx, float x, float y, float z);
	void Scale(int idx, float x, float y, float z);

	void Reset();
};



