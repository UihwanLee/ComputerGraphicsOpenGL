#include <iostream>
#include "OpenGL.h"
#include "Object.h"
#include <vector>

using namespace std;

class ObjectManager
{
public:
	Object			temp;
	vector<Object>	m_ObjectList;

public:
	ObjectManager();
	~ObjectManager();

	void CreateCube();
};