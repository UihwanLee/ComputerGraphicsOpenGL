#include "ObjectManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
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

	temp.m_pivot[0] = 0.f;
	temp.m_pivot[1] = 0.f;
	temp.m_pos = vertex;
	temp.m_inex = Indices;
	temp.m_col = colors;
	temp.m_isActive = true;

	m_ObjectList.emplace_back(temp);
}
