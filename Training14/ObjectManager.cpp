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
	// 사면체
	temp.m_pos = new GLfloat[12];
	temp.m_inex = new GLint[12];
	temp.m_col = new GLfloat[12];

	for (int i = 0; i < 12; i++)	temp.m_pos[i] = Object::TetrahedronVertexs[i];
	for (int i = 0; i < 12; i++)	temp.m_inex[i] = Object::TetrahedronIndexs[i];
	for (int i = 0; i < 12; i++)	temp.m_col[i] = Object::TetrahedronColors[i];

	temp.m_num_vertex = 12;
	temp.m_size_pos = 48;
	temp.m_size_idx = 48;
	temp.m_DRAW_TYPE = GL_TRIANGLES;

	temp.m_pivot[0] = 0.f;
	temp.m_pivot[1] = 0.f;
	temp.m_isModeIDX = true;
	temp.m_isActive = true;

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::SetChangeActive(int mode)
{
	if (m_ObjectList.size() <= 2) return;

	// 다시 리셋
	for (int i = 0; i < 36; i++)	m_ObjectList[1].m_inex[i] = Object::CubeIndexs[i];
	for (int i = 0; i < 12; i++)	m_ObjectList[2].m_inex[i] = Object::TetrahedronIndexs[i];

	m_ObjectList[1].m_size_idx = 144;
	m_ObjectList[2].m_size_idx = 48;

	// 정육면체를 보이게 하고 정사면체 비활성화
	if (mode == 0)
	{
		m_ObjectList[1].m_isActive = true;
		m_ObjectList[2].m_isActive = false;
	}
	else if(mode == 1)
	{
		m_ObjectList[1].m_isActive = false;
		m_ObjectList[2].m_isActive = true;
	}

	return;
}

void ObjectManager::ShowCubeShapeface(int idx, int face)
{
	if (m_ObjectList.size() <= idx) return;

	int start_idx = 0;

	if (face == 0)		start_idx = 0; // 윗면 
	else if (face == 1)	start_idx = 6; // 앞면
	else if (face == 2)	start_idx = 12; // 우측면
	else if (face == 3)	start_idx = 18; // 좌측면
	else if (face == 4)	start_idx = 24; // 밑면
	else if (face == 5)	start_idx = 30; // 밑면

	for (int i = 0; i < 6; i++)
	{
		m_ObjectList[idx].m_inex[i] = Object::CubeIndexs[start_idx];
		start_idx += 1;
	}

	m_ObjectList[idx].m_size_idx = 24;
}

void ObjectManager::ShowTetrahedronShapeface(int idx, int face)
{
	if (m_ObjectList.size() <= idx) return;

	int start_idx = 0;

	if (face == -1)		start_idx = 0; // 좌측면 
	else if (face == 6)	start_idx = 3; // 우측면
	else if (face == 7)	start_idx = 6; // 뒷면
	else if (face == 8)	start_idx = 9; // 아랫면

	for (int i = 0; i < 3; i++)
	{
		m_ObjectList[idx].m_inex[i] = Object::TetrahedronIndexs[start_idx];
		start_idx += 1;
	}

	m_ObjectList[idx].m_size_idx = 12;
}

void ObjectManager::ShowRandomCubeFace()
{
	// 겹치지 않는 2개의 숫자 뽑아내기
	int idx1, idx2;
	
	do
	{
		idx1 = rand() % 6;
		idx2 = rand() % 6;
	} while (idx1 == idx2);

	// 2개의 면의 인덱스를 뽑아내서 저장하기
	idx1 = idx1 * 6;
	idx2 = idx2 * 6;
	int idx = 0;

	for (int i = idx1; i < idx1 + 6; i++)
	{
		m_ObjectList[1].m_inex[idx] = Object::CubeIndexs[i];
		idx += 1;
	}

	for (int i = idx2; i < idx2 + 6; i++)
	{
		m_ObjectList[1].m_inex[idx] = Object::CubeIndexs[i];
		idx += 1;
	}

	m_ObjectList[1].m_size_idx = 48;
}

void ObjectManager::ShowRandomTetrahedronFace()
{
	// 겹치지 않는 2개의 숫자 뽑아내기
	int idx1, idx2;

	do
	{
		idx1 = rand() % 4;
		idx2 = rand() % 4;
	} while (idx1 == idx2);

	// 2개의 면의 인덱스를 뽑아내서 저장하기
	idx1 = idx1 * 3;
	idx2 = idx2 * 3;
	int idx = 0;

	for (int i = idx1; i < idx1 + 3; i++)
	{
		m_ObjectList[2].m_inex[idx] = Object::TetrahedronIndexs[i];
		idx += 1;
	}

	for (int i = idx2; i < idx2 + 3; i++)
	{
		m_ObjectList[2].m_inex[idx] = Object::TetrahedronIndexs[i];
		idx += 1;
	}

	m_ObjectList[2].m_size_idx = 24;
}
