#include <iostream>
#include "OpenGL.h"
#include "Object.h"
#include <vector>

using namespace std;

typedef struct ObjectInfo
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
	ObjectInfo			temp;
	vector<ObjectInfo>	m_ObjectList;

public:
	ObjectManager();
	~ObjectManager();

	void CreateCoordinate();
	void CreateCube();
	void CreateTetrahedron();

	void SetChangeActive(int mode);
	void ShowCubeShapeface(int idx, int face);
	void ShowTetrahedronShapeface(int idx, int face);
};