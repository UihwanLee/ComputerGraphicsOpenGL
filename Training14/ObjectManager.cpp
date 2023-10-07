#include "ObjectManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

GLvoid InitBufferByIdx(GLfloat* buffer, int i, float x, float y, float z)
{
	buffer[i] = x;
	buffer[i+1] = y;
	buffer[i+2] = z;
}

void ObjectManager::CreateCube()
{
	// �簢��
	GLfloat vertex[] = {
			-0.5f, 1.0f, -0.5f,  //0����
			-0.5f, 1.0f, 0.5f,  //1����
			0.5f, 1.0f, 0.5f,  //2����
			0.5f, 1.0f, -0.5f,  //3����

			-0.5f, 0.0f, -0.5f,  //4����
			-0.5f, 0.0f, 0.5f,  //5����
			0.5f, 0.0f, 0.5f,  //6����
			0.5f, 0.0f, -0.5f,  //7����
	};

	GLint Indices[] =
	{
		0,1,2,
		0,2,3,  //����

		1,5,6,
		1,6,2, //�ո�

		2,6,7,
		2,7,3, //������

		0,4,5,
		0,5,1, //������

		5,4,6,
		4,7,6,// �ظ�

		0,7,4, //�޸�
		0,3,7
	};

	GLfloat colors[] = {
			1.0f,0.0f,0.0f,  //0����
			0.0f,1.0f,0.0f,  //1����
			0.0f,0.0f,1.0f,  //2����
			1.0f,0.0f,0.0f,  //3����

			0.0f,1.0f,0.0f,  //4����
			0.0f,0.0f,1.0f,  //5����
			1.0f,0.0f,0.0f,  //6����
			0.0f,1.0f,0.0f,  //7����
	};

	temp.m_pos = new GLfloat[24];
	temp.m_inex = new GLint[36];
	temp.m_col = new GLfloat[24];

	for (int i = 0; i < 24; i++)	temp.m_pos[i] = vertex[i];
	for (int i = 0; i < 36; i++)	temp.m_inex[i] = Indices[i];
	for (int i = 0; i < 24; i++)	temp.m_col[i] = colors[i];

	temp.m_pivot[0] = 0.f;
	temp.m_pivot[1] = 0.f;
	temp.m_isActive = true;

	m_ObjectList.emplace_back(temp);
}
