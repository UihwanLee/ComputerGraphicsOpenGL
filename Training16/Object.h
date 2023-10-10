#include "OpenGL.h"

class Object
{
public:
	// ÁÂÇ¥Æò¸é
	static GLfloat	CoordinateVertexs[12];
	static GLfloat	CoordinateColors[12];

	// Á¤À°¸éÃ¼
	static GLfloat	CubeVertexs[24];
	static GLfloat	CubeColors[24];
	static GLint	CubeIndexs[36];

	// Á¤»ç¸éÃ¼
	static GLfloat	TetrahedronVertexs[12];
	static GLfloat	TetrahedronColors[12];
	static GLint	TetrahedronIndexs[12];

	// Á¤»ç°¢»Ô
	static GLfloat	SquarePyramidVertexs[15];
	static GLfloat	SquarePyramidColors[15];
	static GLint	SquarePyramidIndexs[18];
};