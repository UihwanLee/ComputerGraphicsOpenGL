#pragma once
#include <iostream>
#include "OpenGL.h"
#include "Object.h"
#include <vector>
#include <random>

using namespace std;

// 충돌처리
struct Point {
	float x = 0.0f;
	float y = 0.0f;
};

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
	bool		m_isCut;
	bool		m_isFalling;
	bool		m_isDown;
	bool		m_isActive;
	bool		m_isModeIDX;

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

	void CreateLine();		// 절취선 생성

	void CreateTri();
	void CreateRect();
	void CreatePenta();

	void CreateTriCustom(Point p1, Point p2, Point p3, float r, float g, float b);
	void CreateRectCustom(Point p1, Point p2, Point p3, Point p4, float r, float g, float b);
	void CreatePentaCustom(Point p1, Point p2, Point p3, Point p4, Point p5, float r, float g, float b);

	void SetChangeActive(int mode);
	void ChangeWireSolidType(int start_idx);

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

	// 좌표계 변화
	glm::mat4 TransformModel(int idx);

	void Reset();
};


