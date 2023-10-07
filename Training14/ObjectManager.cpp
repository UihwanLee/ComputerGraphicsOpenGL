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
	// 좌표계
	GLfloat CoordinateVertex[] = {
		-1.0f, 0.f, 0.f,
		1.0f, 0.f, 0.f,
		0.0f, -1.0f, 0.f,
		0.0f, 1.0f, 0.f
	};

	GLfloat CoordinateColors[] = {
		0.0f, 1.f, 0.f,
		0.0f, 1.f, 0.f,
		1.0f, 0.f, 0.f,
		1.0f, 0.f, 0.f
	};

	temp.m_pos = new GLfloat[12];
	temp.m_col = new GLfloat[12];

	for (int i = 0; i < 12; i++)	temp.m_pos[i] = CoordinateVertex[i];
	for (int i = 0; i < 12; i++)	temp.m_col[i] = CoordinateColors[i];

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
	// 사각형
	GLfloat CubeVertexs[] = {
			-0.5f, 0.5f, -0.5f,  //0번점
			-0.5f, 0.5f, 0.5f,  //1번점
			0.5f, 0.5f, 0.5f,  //2번점
			0.5f, 0.5f, -0.5f,  //3번점

			-0.5f, -0.5f, -0.5f,  //4번점
			-0.5f, -0.5f, 0.5f,  //5번점
			0.5f, -0.5f, 0.5f,  //6번점
			0.5f, -0.5f, -0.5f,  //7번점
	};

	GLint CubeIndexs[] =
	{
		0,1,2,
		0,2,3,  //윗면

		1,5,6,
		1,6,2, //앞면

		2,6,7,
		2,7,3, //우측면

		0,4,5,
		0,5,1, //좌측면

		5,4,6,
		4,7,6,// 밑면

		0,7,4, //뒷면
		0,3,7
	};

	// RGB 135, 206, 235
	GLfloat CubeColors[] = {
			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //0번점
			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //1번점
			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //2번점
			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //3번점

			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //4번점
			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //5번점
			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //6번점
			135.f / 255.f,206.f / 255.f,235.f / 255.f,  //7번점
	};

	temp.m_pos = new GLfloat[24];
	temp.m_inex = new GLint[36];
	temp.m_col = new GLfloat[24];

	for (int i = 0; i < 24; i++)	temp.m_pos[i] = CubeVertexs[i];
	for (int i = 0; i < 36; i++)	temp.m_inex[i] = CubeIndexs[i];
	for (int i = 0; i < 24; i++)	temp.m_col[i] = CubeColors[i];

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
