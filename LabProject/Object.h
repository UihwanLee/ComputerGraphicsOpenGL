#include "OpenGL.h"
#include <iostream>

#define SIZE_CONE_VERTEX 66
#define SIZE_CONE_INDEX 60

#define SIZE_SQHERE_VERTEX 1323
#define SIZE_SQHERE_INDEX 2400

#define CONE_HEIGHT 1.0
#define RADIUS 0.7
#define STACKS 20
#define SLICES 20
#define PI 3.1415926535

class Object
{
public:
	// 좌표평면
	static GLfloat	CoordinateVertexs[18];
	static GLfloat	CoordinateColors[18];

	// 삼각형
	static GLfloat	TriVertexs[9];
	static GLfloat	TriColors[9];
	static GLint	TriIndexs[3];

	// 사각형
	static GLfloat	RectVertexs[12];
	static GLfloat	RectColors[12];
	static GLint	RectIndexs[6];

	// 오각형
	static GLfloat	PentaVertexs[15];
	static GLfloat	PentaColors[15];
	static GLint	PentaIndexs[9];


	// 육각형
};