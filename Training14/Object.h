#include "OpenGL.h"

class Object
{
public:
	float	m_pivot[2];
	GLfloat*	m_pos;
	GLfloat*	m_col;
	GLint*	m_inex;
	bool	m_isActive;

public:
	Object();
	~Object();
};