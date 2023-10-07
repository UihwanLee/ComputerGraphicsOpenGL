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

GLvoid InitBufferByIdx(GLfloat* buffer, int i, float x, float y, float z)
{
	buffer[i] = x;
	buffer[i+1] = y;
	buffer[i+2] = z;
}

void ObjectManager::CreateCoordinate()
{
	temp.m_pos = new GLfloat[12];
	temp.m_col = new GLfloat[12];

	for (int i = 0; i < 12; i++)	temp.m_pos[i] = Object::CoordinateVertexs[i];
	for (int i = 0; i < 12; i++)	temp.m_col[i] = Object::CoordinateColors[i];

	temp.m_num_vertex = 4;
	temp.m_size_pos = 48;
	temp.m_size_idx = 0;
	temp.m_DRAW_TYPE = GL_LINES;

	temp.m_pivot[0] = 0.f;
	temp.m_pivot[1] = 0.f;

	temp.m_isModeIDX = false;
	temp.m_isActive = true;

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateCube()
{
	temp.m_pos = new GLfloat[24];
	temp.m_inex = new GLint[36];
	temp.m_col = new GLfloat[24];

	for (int i = 0; i < 24; i++)	temp.m_pos[i] = Object::CubeVertexs[i];
	for (int i = 0; i < 36; i++)	temp.m_inex[i] = Object::CubeIndexs[i];
	for (int i = 0; i < 24; i++)	temp.m_col[i] = Object::CubeColors[i];

	temp.m_num_vertex = 36;
	temp.m_size_pos = 96;
	temp.m_size_idx = 144;
	temp.m_DRAW_TYPE = GL_TRIANGLES;

	temp.m_pivot[0] = 0.f;
	temp.m_pivot[1] = 0.f;
	temp.m_isModeIDX = true;
	temp.m_isActive = true;

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateTetrahedron()
{
	// ���ü
}

void ObjectManager::ShowCubeShapeface(int idx, int face)
{
	if (m_ObjectList.size() <= idx) return;

	int start_idx = 0;

	if (face == 0)		start_idx = 0; // ���� 
	else if (face == 1)	start_idx = 6; // �ո�
	else if (face == 2)	start_idx = 12; // ������
	else if (face == 3)	start_idx = 18; // ������
	else if (face == 4)	start_idx = 24; // �ظ�
	else if (face == 5)	start_idx = 30; // �ظ�

	for (int i = 0; i < 6; i++)
	{
		m_ObjectList[idx].m_inex[i] = Object::CubeIndexs[start_idx];
		start_idx += 1;
	}

	m_ObjectList[idx].m_size_idx = 24;
}