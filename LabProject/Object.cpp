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
		0.0f, 0.f, 0.f,
		0.0f, 0.f, 0.f,
		0.0f, 0.f, 0.f,
		0.0f, 0.f, 0.f,
		0.0f, 0.f, 0.f,
		0.0f, 0.f, 0.f
};

GLfloat Object::RectVertexs[12] = {
	0.5f, 0.5f, 0.0f, 
	0.5f, -0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f, 
	-0.5f, 0.5f, 0.0f 
};

GLfloat Object::RectColors[12] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

GLint Object::RectIndexs[6] = {
	0, 1, 3,
	1, 2, 3
};
