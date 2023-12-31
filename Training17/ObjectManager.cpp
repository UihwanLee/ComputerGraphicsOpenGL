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

GLvoid InitObjectStruct(ObjectInfo* objInfo, int num_ver, int sp, int si, int ni, int drawType, bool modeIDX, bool isAinm, bool isActive)
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

	objInfo->m_isModeIDX = modeIDX;
	objInfo->m_isAnimRotating = isAinm;
	objInfo->m_isActive = isActive;

	objInfo->m_model[0] = 0.0f;
	objInfo->m_model[1] = 0.0f;
	objInfo->m_model[2] = 0.0f;
	objInfo->m_model[3] = 0.0f;

	objInfo->m_model_pos[0] = 0.0f;
	objInfo->m_model_pos[1] = 0.0f;
	objInfo->m_model_pos[2] = 0.0f;
	objInfo->m_model_pos[3] = 0.0f;
}

void ObjectManager::CreateCoordinate()
{
	temp.m_pos = new GLfloat[18];
	temp.m_col = new GLfloat[18];

	for (int i = 0; i < 18; i++)	temp.m_pos[i] = Object::CoordinateVertexs[i];
	for (int i = 0; i < 18; i++)	temp.m_col[i] = Object::CoordinateColors[i];

	InitObjectStruct(&temp, 6, 72, 0, 0, GL_LINES, false, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateCube()
{
	temp.m_pos = new GLfloat[24];
	temp.m_inex = new GLint[36];
	temp.m_col = new GLfloat[24];

	for (int i = 0; i < 24; i++)	temp.m_pos[i] = Object::CubeVertexs[i];
	for (int i = 0; i < 36; i++)	temp.m_inex[i] = Object::CubeIndexs[i];
	for (int i = 0; i < 24; i++)	temp.m_col[i] = Object::CubeColors[i];

	InitObjectStruct(&temp, 36, 96, 144, 8, GL_LINE_LOOP, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateTetrahedron()
{
	// 사면체
	temp.m_pos = new GLfloat[12];
	temp.m_inex = new GLint[12];
	temp.m_col = new GLfloat[12];

	for (int i = 0; i < 12; i++)	temp.m_pos[i] = Object::TetrahedronVertexs[i];
	for (int i = 0; i < 12; i++)	temp.m_inex[i] = Object::TetrahedronIndexs[i];
	for (int i = 0; i < 12; i++)	temp.m_col[i] = Object::TetrahedronColors[i];

	InitObjectStruct(&temp, 12, 48, 48, 4, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateSquarePyramid()
{
	// 사각뿔
	temp.m_pos = new GLfloat[15];
	temp.m_inex = new GLint[18];
	temp.m_col = new GLfloat[15];

	for (int i = 0; i < 15; i++)	temp.m_pos[i] = Object::SquarePyramidVertexs[i];
	for (int i = 0; i < 18; i++)	temp.m_inex[i] = Object::SquarePyramidIndexs[i];
	for (int i = 0; i < 15; i++)	temp.m_col[i] = Object::SquarePyramidColors[i];

	InitObjectStruct(&temp, 18, 72, 72, 5, GL_LINE_LOOP, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateCone()
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// 원뿔의 정점과 인덱스 생성
	vertices.push_back(0.0f);  // 원뿔의 꼭대기
	vertices.push_back(CONE_HEIGHT);
	vertices.push_back(0.0f);

	for (int i = 0; i <= SLICES; ++i) {
		float theta = static_cast<float>(i) * 2.0f * PI / SLICES;
		float x = RADIUS * cos(theta);
		float z = RADIUS * sin(theta);
		vertices.push_back(x);
		vertices.push_back(0.0f);
		vertices.push_back(z);

		if (i < SLICES) {
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
	}

	// 원뿔
	temp.m_pos = new GLfloat[SIZE_CONE_VERTEX];
	temp.m_inex = new GLint[SIZE_CONE_INDEX];
	temp.m_col = new GLfloat[SIZE_CONE_VERTEX];

	for (int i = 0; i < vertices.size(); i++)	temp.m_pos[i] = vertices[i];
	for (int i = 0; i < vertices.size(); i++)	temp.m_col[i] = Object::ConeColors[i];
	for (int i = 0; i < indices.size(); i++)	temp.m_inex[i] = indices[i];

	InitObjectStruct(&temp, SIZE_CONE_INDEX, 12 * (SIZE_CONE_VERTEX / 3), 12 * (SIZE_CONE_INDEX / 3), 0, GL_LINE_LOOP, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateSqhere()
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
	for (int i = 0; i < vertices.size(); i++)	temp.m_col[i] = Object::SqhereColors[i];
	for (int i = 0; i < indices.size(); i++)	temp.m_inex[i] = indices[i];

	InitObjectStruct(&temp, SIZE_SQHERE_INDEX, 12 * (SIZE_SQHERE_VERTEX / 3), 12 * (SIZE_SQHERE_INDEX / 3), 0, GL_LINE_LOOP, true, false, true);

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

void ObjectManager::ChangeWireSolidType()
{
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

	for (int i = 1; i < m_ObjectList.size(); i++)
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

void ObjectManager::Reset()
{
	m_ObjectList.clear();
}


#include "ObjectManager.h"
