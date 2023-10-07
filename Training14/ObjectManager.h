#include <iostream>
#include "OpenGL.h"
#include <vector>

using namespace std;

typedef struct Object
{
	float		m_pivot[2];
	GLfloat*	m_pos;
	GLfloat*	m_col;
	GLint*		m_inex;
	int			m_num_vertex;
	int			m_size_pos;
	int			m_size_idx;
	int			m_DRAW_TYPE;
	bool		m_isActive;
	bool		m_isModeIDX;
};

class ObjectManager
{
public:
	Object			temp;
	vector<Object>	m_ObjectList;

public:
	ObjectManager();
	~ObjectManager();

	void CreateCoordinate();
	void CreateCube();
};