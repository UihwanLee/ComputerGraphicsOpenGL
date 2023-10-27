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

GLfloat Object::TriVertexs[9] = {
	0.0f, 0.3f, 0.0f,
	-0.3f, -0.3f, 0.0f,
	0.3f, -0.3f, 0.0f,
};

GLfloat Object::TriColors[9] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

GLint Object::TriIndexs[3] = {
	0, 1, 2
};

GLfloat Object::RectVertexs[12] = {
	0.3f, 0.3f, 0.0f, 
	0.3f, -0.3f, 0.0f, 
	-0.3f, -0.3f, 0.0f, 
	-0.3f, 0.3f, 0.0f 
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

GLfloat Object::PentaVertexs[15] = {
	0.0f, 0.3f, 0.0f,
	-0.3f, 0.1f, 0.0f,
	0.3f, 0.1f, 0.0f,
	-0.2f, -0.3f, 0.0f,
	0.2f, -0.3f, 0.0f
};

GLfloat Object::PentaColors[15] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
};

GLint Object::PentaIndexs[9] = {
	0, 1, 2,
	1, 2, 3,
	2, 3, 4
};
