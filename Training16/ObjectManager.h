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
	GLfloat*	m_pos;
	GLfloat*	m_col;
	GLint*		m_inex;
	int			m_num_vertex;
	int			m_size_pos;
	int			m_size_idx;
	int			m_num_idx;
	int			m_DRAW_TYPE;
	bool		m_isAnimRotating;
	bool		m_isActive;
	bool		m_isModeIDX;
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
	void CreateTetrahedron();
	void CreateSquarePyramid();
	void CreateCone();
	void CreateSqhere();

	void SetChangeActive(int mode);
	void ChangeWireSolidType();

	void MoveObject(int idx, float x, float y, float z);
	void RotateObject(int idx, float x, float y, float z);
	void ScaleObject(int idx, float x, float y, float z);

	void MoveAllObjects(float x, float y, float z);
	void RotateAllObjects(float x, float y, float z);
	void ScaleAllObjects(float x, float y, float z);

	void Reset();
};

