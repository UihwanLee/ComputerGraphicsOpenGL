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

	// ��ǥ�� ���
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
	void CreateCubeFace(int face, float x, float y, float z);
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
	void SetWireSolidType(int idx, int type);

	// ��ǥ�� ��ȭ
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

	void AddForce(int idx, float x, float y, float z, float elapsedTime);
	void UpdatePos(int idx, float elapsedTime);

	// �ڽ� ����
	void SetChild(int idx, int idx_child);

	void Reset();
};





