#include "ObjectManager.h"

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

	objInfo->m_isModeIDX = modeIDX;
	objInfo->m_isAnimRotating = isAinm;
	objInfo->m_isActive = isActive;

	objInfo->m_model[0] = 0.0f;
	objInfo->m_model[1] = 0.0f;
	objInfo->m_model[2] = 0.0f;
	objInfo->m_model[3] = 0.0f;
}

//void ObjectManager::CreateCube()
//{
//	temp.m_pos = new GLfloat[24];
//	temp.m_inex = new GLint[36];
//	temp.m_col = new GLfloat[24];
//
//	for (int i = 0; i < 24; i++)	temp.m_pos[i] = Object::CubeVertexs[i];
//	for (int i = 0; i < 36; i++)	temp.m_inex[i] = Object::CubeIndexs[i];
//	for (int i = 0; i < 24; i++)	temp.m_col[i] = Object::CubeColors[i];
//
//	InitObjectStruct(&temp, 36, 96, 144, 8, GL_LINE_LOOP, true, false, true);
//
//	m_ObjectList.emplace_back(temp);
//}

void ObjectManager::CreateLine()
{
}

void ObjectManager::CreateTri()
{
	temp.m_pos = new GLfloat[9];
	temp.m_col = new GLfloat[9];
	temp.m_inex = new GLint[3];

	for (int i = 0; i < 9; i++) temp.m_pos[i] = Object::TriVertexs[i];
	for (int i = 0; i < 9; i++) temp.m_col[i] = Object::TriColors[i];
	for (int i = 0; i < 3; i++) temp.m_inex[i] = Object::TriIndexs[i];

	InitObjectStruct(&temp, 3, 36, 12, 3, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateRect()
{
	temp.m_pos = new GLfloat[12];
	temp.m_col = new GLfloat[12];
	temp.m_inex = new GLint[6];

	for (int i = 0; i < 12; i++) temp.m_pos[i] = Object::RectVertexs[i];
	for (int i = 0; i < 12; i++) temp.m_col[i] = Object::RectColors[i];
	for (int i = 0; i < 6; i++) temp.m_inex[i] = Object::RectIndexs[i];

	InitObjectStruct(&temp, 6, 48, 24, 4, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreatePenta()
{
	temp.m_pos = new GLfloat[15];
	temp.m_col = new GLfloat[15];
	temp.m_inex = new GLint[9];

	for (int i = 0; i < 15; i++) temp.m_pos[i] = Object::PentaVertexs[i];
	for (int i = 0; i < 15; i++) temp.m_col[i] = Object::PentaColors[i];
	for (int i = 0; i < 9; i++) temp.m_inex[i] = Object::PentaIndexs[i];

	InitObjectStruct(&temp, 9, 60, 36, 5, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
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
		m_ObjectList[i].m_DRAW_TYPE = drawType;
	}
}

void ObjectManager::SetPosition(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty() || idx == 0) return;

	m_ObjectList[idx].m_pivot[0] = x;
	m_ObjectList[idx].m_pivot[1] = y;
	m_ObjectList[idx].m_pivot[2] = z;
}

void ObjectManager::SetRotate(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty() || idx == 0) return;

	m_ObjectList[idx].m_rotate[0] = x;
	m_ObjectList[idx].m_rotate[1] = y;
	m_ObjectList[idx].m_rotate[2] = z;
}

void ObjectManager::SetScale(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty() || idx == 0) return;

	m_ObjectList[idx].m_scale[0] = x;
	m_ObjectList[idx].m_scale[1] = y;
	m_ObjectList[idx].m_scale[2] = z;
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

void MoveAxisObject(GLfloat* posList, int SIZE, int startIDX, float moveDist)
{
	for (int i = 0; i < SIZE; i++)
	{
		posList[startIDX] += moveDist;
		startIDX += 3;
	}
}

void ObjectManager::Reset()
{
	m_ObjectList.clear();
}


