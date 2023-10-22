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

	// 좌표계 행렬
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

	void CreateCoordinate();
	void CreateCube(float x, float y, float z);
	void CreateCubeFace(int face);
	void CreateTetrahedron();
	void CreateSquarePyramid();
	void CreateSquarePyramidFace(int face);
	void CreateSquarePyramidBottom();
	void CreateCone();
	void CreateSqhere(float x, float y, float z);

	void CreateOrbit(float orbit_radius);
	void CreateOrbit2(float orbit_radius, float dir);

	void SetChangeActive(int mode);
	void ChangeWireSolidType();

	// 좌표계 변화
	glm::mat4 TransformScale(float x, float y, float z);
	glm::mat4 TransformRotate(float x, float y, float z);
	glm::mat4 TransformMove(float x, float y, float z);
	void TransformModel(int idx);

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

	void SetRotateRevolution(int idx);
	void SetRotate(int idx);

	// 자식 설정
	void SetChild(int idx, int idx_child);

	void Reset();
};





