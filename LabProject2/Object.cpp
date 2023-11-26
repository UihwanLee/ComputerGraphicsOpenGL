#include "Object.h"

GLfloat Object::CoordinateVertexs[18] = {
	1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, 1.0f
};

GLfloat Object::CoordinateColors[18] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
};

GLfloat Object::CubeVertexs[108] = {


	-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f - 0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f
};

GLfloat Object::CubeNormal[108] = {

	0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

GLint Object::CubeIndexs[36] =
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

GLfloat Object::TetrahedronVertexs[12] =
{
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, -0.5f, 0.5f,
};

GLfloat Object::TetrahedronColors[12] =
{
	135.f / 255.f,206.f / 255.f,235.f / 255.f,
	135.f / 255.f,206.f / 255.f,235.f / 255.f,
	135.f / 255.f,206.f / 255.f,235.f / 255.f,
	135.f / 255.f,206.f / 255.f,235.f / 255.f,
};

GLint Object::TetrahedronIndexs[12] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 3,
	1, 2, 3
};

GLfloat Object::SquarePyramidVertexs[15] =
{
	0.0f, 0.5f, 0.0f,		// ������
	-0.5f, -0.5f, -0.5f,	// �ظ�
	-0.5f, -0.5f, 0.5f,		// �ظ�
	0.5f, -0.5f, 0.5f,		// �ظ�
	0.5f, -0.5f, -0.5f,		// �ظ�
};

GLfloat Object::SquarePyramidNormal[15] =
{
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,  //4����
	0.0f, 0.0f, -1.0f,  //5����
	0.0f, 0.0f, -1.0f,  //6����
	0.0f, 0.0f, -1.0f,  //7����
};

GLint Object::SquarePyramidIndexs[18] =
{
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 1,
	1, 2, 3,
	3, 4, 1
};

GLfloat Object::ConeVertexs[SIZE_CONE_VERTEX] = { 0.0f };
GLfloat Object::ConeColors[SIZE_CONE_VERTEX] = { 0.0f };
GLint Object::ConeIndexs[SIZE_CONE_INDEX] = { 0.0f };

GLfloat Object::SqhereVertexs[SIZE_SQHERE_VERTEX] = { 0.0f };
GLfloat Object::SqhereColors[SIZE_SQHERE_VERTEX] = { 0.0f };
GLint Object::SqhereIndexs[SIZE_SQHERE_INDEX] = { 0 };




