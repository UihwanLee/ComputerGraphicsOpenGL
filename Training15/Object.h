#include "OpenGL.h"

class Object
{
public:
	// ��ǥ���
	static GLfloat	CoordinateVertexs[12];
	static GLfloat	CoordinateColors[12];

	// ������ü
	static GLfloat	CubeVertexs[24];
	static GLfloat	CubeColors[24];
	static GLint	CubeIndexs[36];

	// �����ü
	static GLfloat	TetrahedronVertexs[12];
	static GLfloat	TetrahedronColors[12];
	static GLint	TetrahedronIndexs[12];

	// ���簢��
	static GLfloat	SquarePyramidVertexs[15];
	static GLfloat	SquarePyramidColors[15];
	static GLint	SquarePyramidIndexs[18];
};