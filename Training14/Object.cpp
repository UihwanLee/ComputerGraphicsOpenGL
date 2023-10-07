#include "Object.h"

Object::Object()
{

}

Object::~Object()
{
	delete[] m_pos;
	delete[] m_inex;
	delete[] m_col;
}
