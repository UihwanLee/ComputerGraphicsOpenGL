#include "ObjectManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		delete[] m_ObjectList[i].m_pos;
		delete[] m_ObjectList[i].m_col;
		delete[] m_ObjectList[i].m_inex;
	}
}

int ObjectManager::GetRandomIntValue(GLfloat min, GLfloat max)
{
	GLfloat value;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<GLfloat> dis(min, max);

	value = dis(gen);

	return int(value);
}

GLfloat ObjectManager::GetRandomFloatValue(GLfloat min, GLfloat max)
{
	GLfloat value;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<GLfloat> dis(min, max);

	value = dis(gen);

	return value;
}

GLvoid InitObjectStruct(ObjectInfo* objInfo, int num_ver, int sp, int si, int ni, int drawType, bool modeIDX, bool isFalling, bool isActive)
{
	objInfo->m_num_vertex = num_ver;
	objInfo->m_size_pos = sp;
	objInfo->m_size_idx = si;
	objInfo->m_num_idx = ni;
	objInfo->m_DRAW_TYPE = drawType;

	objInfo->m_pivot[0] = 0.0f;
	objInfo->m_pivot[1] = 0.0f;
	objInfo->m_pivot[2] = 0.0f;

	objInfo->m_rotate[0] = 0.0f;
	objInfo->m_rotate[1] = 0.0f;
	objInfo->m_rotate[2] = 0.0f;

	objInfo->m_scale[0] = 0.5f;
	objInfo->m_scale[1] = 0.5f;
	objInfo->m_scale[2] = 0.5f;

	objInfo->m_traceIDX = 0;
	for(int i=0; i<6; i++) objInfo->m_isTrace[i] = false;

	objInfo->m_isModeIDX = modeIDX;
	objInfo->m_isCut = false;
	objInfo->m_isDown = false;
	objInfo->m_isFalling = isFalling;
	objInfo->m_isActive = isActive;
}

//void ObjectManager::CreateCube()
//{
//	temp.m_pos = new GLfloat[24];
//	temp.m_inex = new GLint[36];
//	temp.m_col = new GLfloat[24];
//
//	for (int i = 0; i < 24; i++)	temp.m_pos[i] = Object::CubeVertexs[i];
//	for (int i = 0; i < 36; i++)	temp.m_inex[i] = Object::CubeIndexs[i];
//	for (int i = 0; i < 24; i++)	temp.m_col[i] = Object::CubeColors[i];
//
//	InitObjectStruct(&temp, 36, 96, 144, 8, GL_LINE_LOOP, true, false, true);
//
//	m_ObjectList.emplace_back(temp);
//}

void ObjectManager::CreateLine()
{
	temp.m_pos = new GLfloat[6];
	temp.m_col = new GLfloat[6];

	for (int i = 0; i < 6; i++) temp.m_pos[i] = Object::LineVertexs[i];
	for (int i = 0; i < 6; i++) temp.m_col[i] = Object::LineColors[i];

	InitObjectStruct(&temp, 2, 24, 0, 0, GL_LINES, false, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateTri()
{
	temp.m_pos = new GLfloat[9];
	temp.m_col = new GLfloat[9];
	temp.m_inex = new GLint[3];

	for (int i = 0; i < 9; i++) temp.m_pos[i] = Object::TriVertexs[i];
	for (int i = 0; i < 9; i++) temp.m_col[i] = Object::TriColors[i];
	for (int i = 0; i < 3; i++) temp.m_inex[i] = Object::TriIndexs[i];

	InitObjectStruct(&temp, 3, 36, 12, 3, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateRect()
{
	temp.m_pos = new GLfloat[12];
	temp.m_col = new GLfloat[12];
	temp.m_inex = new GLint[6];

	for (int i = 0; i < 12; i++) temp.m_pos[i] = Object::RectVertexs[i];
	for (int i = 0; i < 12; i++) temp.m_col[i] = Object::RectColors[i];
	for (int i = 0; i < 6; i++) temp.m_inex[i] = Object::RectIndexs[i];

	InitObjectStruct(&temp, 6, 48, 24, 4, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreatePenta()
{
	temp.m_pos = new GLfloat[15];
	temp.m_col = new GLfloat[15];
	temp.m_inex = new GLint[9];

	for (int i = 0; i < 15; i++) temp.m_pos[i] = Object::PentaVertexs[i];
	for (int i = 0; i < 15; i++) temp.m_col[i] = Object::PentaColors[i];
	for (int i = 0; i < 9; i++) temp.m_inex[i] = Object::PentaIndexs[i];

	InitObjectStruct(&temp, 9, 60, 36, 5, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateTriCustom(Point p1, Point p2, Point p3, float r, float g, float b)
{
	temp.m_pos = new GLfloat[9];
	temp.m_col = new GLfloat[9];
	temp.m_inex = new GLint[3];

	temp.m_pos[0] = p1.x; temp.m_pos[1] = p1.y; temp.m_pos[2] = 0.0f;
	temp.m_pos[3] = p2.x; temp.m_pos[4] = p2.y; temp.m_pos[5] = 0.0f;
	temp.m_pos[6] = p3.x; temp.m_pos[7] = p3.y; temp.m_pos[8] = 0.0f;

	for (int i = 0; i < 9; i++)
	{
		if (i % 3 == 0) temp.m_col[i] = r;
		if (i % 3 == 1) temp.m_col[i] = g;
		if (i % 3 == 2) temp.m_col[i] = b;
	}

	for (int i = 0; i < 3; i++) temp.m_inex[i] = Object::TriIndexs[i];

	InitObjectStruct(&temp, 3, 36, 12, 3, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateRectCustom(Point p1, Point p2, Point p3, Point p4, float r, float g, float b)
{
	temp.m_pos = new GLfloat[12];
	temp.m_col = new GLfloat[12];
	temp.m_inex = new GLint[6];

	temp.m_pos[0] = p1.x; temp.m_pos[1] = p1.y; temp.m_pos[2] = 0.0f;
	temp.m_pos[3] = p2.x; temp.m_pos[4] = p2.y; temp.m_pos[5] = 0.0f;
	temp.m_pos[6] = p3.x; temp.m_pos[7] = p3.y; temp.m_pos[8] = 0.0f;
	temp.m_pos[9] = p4.x; temp.m_pos[10] = p4.y; temp.m_pos[11] = 0.0f;

	for (int i = 0; i < 12; i++)
	{
		if (i % 3 == 0) temp.m_col[i] = r;
		if (i % 3 == 1) temp.m_col[i] = g;
		if (i % 3 == 2) temp.m_col[i] = b;
	}

	for (int i = 0; i < 6; i++) temp.m_inex[i] = Object::RectIndexs[i];

	InitObjectStruct(&temp, 6, 48, 24, 4, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreatePentaCustom(Point p1, Point p2, Point p3, Point p4, Point p5, float r, float g, float b)
{
	temp.m_pos = new GLfloat[15];
	temp.m_col = new GLfloat[15];
	temp.m_inex = new GLint[9];

	temp.m_pos[0] = p1.x; temp.m_pos[1] = p1.y; temp.m_pos[2] = 0.0f;
	temp.m_pos[3] = p2.x; temp.m_pos[4] = p2.y; temp.m_pos[5] = 0.0f;
	temp.m_pos[6] = p3.x; temp.m_pos[7] = p3.y; temp.m_pos[8] = 0.0f;
	temp.m_pos[9] = p4.x; temp.m_pos[10] = p4.y; temp.m_pos[11] = 0.0f;
	temp.m_pos[12] = p5.x; temp.m_pos[13] = p5.y; temp.m_pos[14] = 0.0f;

	for (int i = 0; i < 15; i++)
	{
		if (i % 3 == 0) temp.m_col[i] = r;
		if (i % 3 == 1) temp.m_col[i] = g;
		if (i % 3 == 2) temp.m_col[i] = b;
	}

	for (int i = 0; i < 9; i++) temp.m_inex[i] = Object::PentaIndexs[i];

	InitObjectStruct(&temp, 9, 60, 36, 5, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateSqhere(float x, float y, float z)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// 구의 정점과 인덱스 생성
	for (int i = 0; i <= STACKS; ++i) {
		float phi = static_cast<float>(i) * PI / STACKS;
		for (int j = 0; j <= SLICES; ++j) {
			float theta = static_cast<float>(j) * 2.0f * PI / SLICES;
			float x = RADIUS * sin(phi) * cos(theta);
			float y = RADIUS * cos(phi);
			float z = RADIUS * sin(phi) * sin(theta);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	for (int i = 0; i < STACKS; ++i) {
		for (int j = 0; j < STACKS; ++j) {
			int first = i * (SLICES + 1) + j;
			int second = first + SLICES + 1;
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	// 원
	temp.m_pos = new GLfloat[SIZE_SQHERE_VERTEX];
	temp.m_inex = new GLint[SIZE_SQHERE_INDEX];
	temp.m_col = new GLfloat[SIZE_SQHERE_VERTEX];

	for (int i = 0; i < vertices.size(); i++)	temp.m_pos[i] = vertices[i];
	for (int i = 0; i < vertices.size(); i++)
	{
		if (i % 3 == 0) temp.m_col[i] = x;
		if (i % 3 == 1)temp.m_col[i] = y;
		if (i % 3 == 2)temp.m_col[i] = z;
	}
	for (int i = 0; i < indices.size(); i++)	temp.m_inex[i] = indices[i];

	InitObjectStruct(&temp, SIZE_SQHERE_INDEX, 12 * (SIZE_SQHERE_VERTEX / 3), 12 * (SIZE_SQHERE_INDEX / 3), 0, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}


void ObjectManager::SetChangeActive(int mode)
{
	if (m_ObjectList.size() <= 2) return;

	// 정육면체를 보이게 하고 사각뿔 비활성화
	if (mode == 0)
	{
		m_ObjectList[1].m_isActive = true;
		m_ObjectList[2].m_isActive = false;
	}
	else if (mode == 1)
	{
		m_ObjectList[1].m_isActive = false;
		m_ObjectList[2].m_isActive = true;
	}

	return;
}

void ObjectManager::ChangeWireSolidType(int start_idx)
{
	if (m_ObjectList.size() < start_idx + 1) return;

	int drawType;

	if (m_WireType == false)
	{
		drawType = GL_LINE_LOOP;
		m_WireType = true;
	}
	else
	{
		drawType = GL_TRIANGLES;
		m_WireType = false;
	}

	for (int i = start_idx; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].m_DRAW_TYPE = drawType;
	}
}

void ObjectManager::SetPosition(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty() || idx == 0) return;

	m_ObjectList[idx].m_pivot[0] = x;
	m_ObjectList[idx].m_pivot[1] = y;
	m_ObjectList[idx].m_pivot[2] = z;
}

void ObjectManager::SetRotate(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty() || idx == 0) return;

	m_ObjectList[idx].m_rotate[0] = x;
	m_ObjectList[idx].m_rotate[1] = y;
	m_ObjectList[idx].m_rotate[2] = z;
}

void ObjectManager::SetScale(int idx, float x, float y, float z)
{
	if (m_ObjectList.empty() || idx == 0) return;

	m_ObjectList[idx].m_scale[0] = x;
	m_ObjectList[idx].m_scale[1] = y;
	m_ObjectList[idx].m_scale[2] = z;
}

void ObjectManager::SetActive(int idx, bool bActive)
{
	if (m_ObjectList.empty()) return;

	m_ObjectList[idx].m_isActive = bActive;
}

void ObjectManager::SetAllPositon(float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	for (int i = 1; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].m_pivot[0] += x;
		m_ObjectList[i].m_pivot[1] += y;
		m_ObjectList[i].m_pivot[2] += z;
	}
}

void ObjectManager::SetAllRotate(float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	for (int i = 1; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].m_rotate[0] += x;
		m_ObjectList[i].m_rotate[1] += y;
		m_ObjectList[i].m_rotate[2] += z;
	}
}

void ObjectManager::SetAllScale(float x, float y, float z)
{
	if (m_ObjectList.empty()) return;

	for (int i = 1; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].m_scale[0] = x;
		m_ObjectList[i].m_scale[1] = y;
		m_ObjectList[i].m_scale[2] = z;
	}
}

void MoveAxisObject(GLfloat* posList, int SIZE, int startIDX, float moveDist)
{
	for (int i = 0; i < SIZE; i++)
	{
		posList[startIDX] += moveDist;
		startIDX += 3;
	}
}

void ObjectManager::Move(int idx, float x, float y, float z)
{
	m_ObjectList[idx].m_pivot[0] += x;
	m_ObjectList[idx].m_pivot[1] += y;
	m_ObjectList[idx].m_pivot[2] += z;
}

void ObjectManager::Rotate(int idx, float x, float y, float z)
{
	if (x != 0.0f) m_ObjectList[idx].m_rotate[0] += x;
	else if (y != 0.0f) m_ObjectList[idx].m_rotate[1] += y;
	else if (z != 0.0f) m_ObjectList[idx].m_rotate[2] += z;
}

void ObjectManager::Scale(int idx, float x, float y, float z)
{
	m_ObjectList[idx].m_scale[0] += x;
	m_ObjectList[idx].m_scale[1] += y;
	m_ObjectList[idx].m_scale[2] += z;
}

glm::mat4 ObjectManager::TransformModel(int idx)
{
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	if (!m_ObjectList.empty())
	{
		float scale_x_parent; float scale_y_parent; float scale_z_parent;
		float move_x_parent; float move_y_parent; float move_z_parent;
		float rotate_y_parent;

		float move_x = m_ObjectList[idx].m_pivot[0];
		float move_y = m_ObjectList[idx].m_pivot[1];
		float move_z = m_ObjectList[idx].m_pivot[2];

		float rotate_x = m_ObjectList[idx].m_rotate[0];
		float rotate_y = m_ObjectList[idx].m_rotate[1];
		float rotate_z = m_ObjectList[idx].m_rotate[2];

		float scale_x = m_ObjectList[idx].m_scale[0];
		float scale_y = m_ObjectList[idx].m_scale[1];
		float scale_z = m_ObjectList[idx].m_scale[2];

		model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(scale_x, scale_y, scale_z));
		model = glm::rotate(model, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(move_x, move_y, move_z));
	}

	return model;
}

void ObjectManager::Reset()
{
	m_ObjectList.clear();
}


