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

	objInfo->m_vel[0] = 0.0f;
	objInfo->m_vel[1] = 0.0f;
	objInfo->m_vel[2] = 0.0f;

	objInfo->m_mass = 1.0f;

	objInfo->m_isModeIDX = modeIDX;
	objInfo->m_isAnimRotating = isAinm;
	objInfo->m_isActive = isActive;

	objInfo->m_model_pos[0] = 0.0f;
	objInfo->m_model_pos[1] = 0.0f;
	objInfo->m_model_pos[2] = 0.0f;
	objInfo->m_model_pos[3] = 0.0f;

	objInfo->m_modelInfo[0] = 0.0f;
	objInfo->m_modelInfo[1] = 0.0f;
	objInfo->m_modelInfo[2] = 0.0f;
	objInfo->m_modelInfo[3] = 0.0f;

	objInfo->m_model = glm::mat4(1.0f);
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

void ObjectManager::CreateCube(float x, float y, float z)
{
	temp.m_pos = new GLfloat[24];
	temp.m_inex = new GLint[36];
	temp.m_col = new GLfloat[24];

	for (int i = 0; i < 24; i++)	temp.m_pos[i] = Object::CubeVertexs[i];
	for (int i = 0; i < 36; i++)	temp.m_inex[i] = Object::CubeIndexs[i];

	// x = 0.0f, y = 0.0f, z = 0.0f이면 기본 색깔 지정
	if (x == 0.0f && y == 0.0f && z == 0.0f)
	{
		for (int i = 0; i < 24; i++)	temp.m_col[i] = Object::CubeNormal[i];
	}
	else
	{
		for (int i = 0; i < 24; i++)
		{
			if (i % 3 == 0) temp.m_col[i] = x;
			if (i % 3 == 1)	temp.m_col[i] = y;
			if (i % 3 == 2)	temp.m_col[i] = z;
		}
	}

	InitObjectStruct(&temp, 36, 96, 144, 8, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateCubeFace(int face, float x, float y, float z)
{
	// 육면체의 특정 면을 오브젝트로 생성
	temp.m_pos = new GLfloat[12];
	temp.m_inex = new GLint[6];
	temp.m_col = new GLfloat[12];

	// 생성하려는 면에 따라 4개의 점 나누기
	int point01, point02, point03, point04;
	point01 = point02 = point03 = point04 = 0;
	if (face == 0) { point01 = 0; point02 = 1; point03 = 2; point04 = 3; }
	else if (face == 1) { point01 = 1; point02 = 5; point03 = 6; point04 = 2; }
	else if (face == 2) { point01 = 2; point02 = 6; point03 = 7; point04 = 3; }
	else if (face == 3) { point01 = 3; point02 = 0; point03 = 4; point04 = 7; }
	else if (face == 4) { point01 = 0; point02 = 1; point03 = 5; point04 = 4; }
	else if (face == 5) { point01 = 4; point02 = 5; point03 = 6; point04 = 7; }

	bool isColor = (x == 0.0f && y == 0.0f && z == 0.0f) ? false : true;

	int temp_idx = 0;
	for (int i = point01 * 3; i < (point01 * 3) + 3; i++)
	{
		temp.m_pos[temp_idx] = Object::CubeVertexs[i];
		temp.m_col[temp_idx++] = Object::CubeNormal[i];
	}
	for (int i = point02 * 3; i < (point02 * 3) + 3; i++)
	{
		temp.m_pos[temp_idx] = Object::CubeVertexs[i];
		temp.m_col[temp_idx++] = Object::CubeNormal[i];
	}
	for (int i = point03 * 3; i < (point03 * 3) + 3; i++)
	{
		temp.m_pos[temp_idx] = Object::CubeVertexs[i];
		temp.m_col[temp_idx++] = Object::CubeNormal[i];
	}
	for (int i = point04 * 3; i < (point04 * 3) + 3; i++)
	{
		temp.m_pos[temp_idx] = Object::CubeVertexs[i];
		temp.m_col[temp_idx++] = Object::CubeNormal[i];
	}

	if (face == 0)
	{
		for (int i = 0; i < 12; i++)
		{
			if (i % 3 == 1) temp.m_pos[i] = 0.0f;
		}
	}

	if (isColor)
	{
		for (int i = 0; i < 12; i++)
		{
			if (i % 3 == 0) temp.m_col[i] = x;
			if (i % 3 == 1) temp.m_col[i] = y;
			if (i % 3 == 2) temp.m_col[i] = z;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		temp.m_inex[i] = Object::CubeIndexs[i];
	}

	InitObjectStruct(&temp, 6, 48, 24, 4, GL_TRIANGLES, true, false, true);

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

	InitObjectStruct(&temp, 18, 72, 72, 5, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateSquarePyramidFace(int face)
{
	temp.m_pos = new GLfloat[9];
	temp.m_inex = new GLint[3];
	temp.m_col = new GLfloat[9];

	// 생성하려는 면에 따라 3개의 점 나누기
	int point01, point02, point03;
	point01 = point02 = point03 = 0;
	if (face == 0) { point01 = 0; point02 = 1; point03 = 2; }
	else if (face == 1) { point01 = 0; point02 = 2; point03 = 3; }
	else if (face == 2) { point01 = 0; point02 = 4; point03 = 1; }
	else if (face == 3) { point01 = 0; point02 = 3; point03 = 4; }

	int temp_idx = 0;
	for (int i = point01 * 3; i < (point01 * 3) + 3; i++)
	{
		temp.m_pos[temp_idx] = Object::SquarePyramidVertexs[i];
		temp.m_col[temp_idx++] = Object::SquarePyramidColors[i];
	}
	for (int i = point02 * 3; i < (point02 * 3) + 3; i++)
	{
		temp.m_pos[temp_idx] = Object::SquarePyramidVertexs[i];
		temp.m_col[temp_idx++] = Object::SquarePyramidColors[i];
	}
	for (int i = point03 * 3; i < (point03 * 3) + 3; i++)
	{
		temp.m_pos[temp_idx] = Object::SquarePyramidVertexs[i];
		temp.m_col[temp_idx++] = Object::SquarePyramidColors[i];
	}

	for (int i = 0; i < 3; i++)
	{
		temp.m_inex[i] = Object::SquarePyramidIndexs[i];
	}

	InitObjectStruct(&temp, 3, 36, 12, 3, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateSquarePyramidBottom()
{
	temp.m_pos = new GLfloat[12];
	temp.m_inex = new GLint[6];
	temp.m_col = new GLfloat[12];

	for (int i = 3; i < 15; i++)
	{
		temp.m_pos[i] = Object::SquarePyramidVertexs[i];
		temp.m_col[i] = Object::SquarePyramidColors[i];
	}

	for (int i = 12; i < 18; i++)
	{
		temp.m_inex[i] = Object::SquarePyramidIndexs[i];
	}

	InitObjectStruct(&temp, 6, 48, 24, 4, GL_TRIANGLES, true, false, true);

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

	InitObjectStruct(&temp, SIZE_CONE_INDEX, 12 * (SIZE_CONE_VERTEX / 3), 12 * (SIZE_CONE_INDEX / 3), 0, GL_TRIANGLES, true, false, true);

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

void ObjectManager::CreateCylinder(float x, float y, float z)
{
	const float height = 1.0f;
	const float radius = 0.5f;
	const int sectors = 360;
	const int stacks = 1; // 한 번만 쌓음으로 원기둥으로 만듦

	vector<GLfloat> vertices;
	vector<GLuint> indices;

	for (int i = 0; i <= stacks; ++i)
	{
		float stackHeight = height * static_cast<float>(i) / stacks;
		for (int j = 0; j <= sectors; ++j)
		{
			float theta = glm::radians(static_cast<float>(j));
			float x = radius * cos(theta);
			float z = radius * sin(theta);

			// 좌표
			vertices.push_back(x);
			vertices.push_back(stackHeight);
			vertices.push_back(z);

			// 인덱스
			if (i < stacks && j < sectors)
			{
				int currentIdx = i * (sectors + 1) + j;
				int nextIdx = (i + 1) * (sectors + 1) + j;

				indices.push_back(currentIdx);
				indices.push_back(nextIdx);
				indices.push_back(currentIdx + 1);

				indices.push_back(nextIdx);
				indices.push_back(nextIdx + 1);
				indices.push_back(currentIdx + 1);
			}
		}
	}

	temp.m_pos = new GLfloat[SIZE_CYLINDER_VERTEX];
	temp.m_inex = new GLint[SIZE_CYLINDER_INDEX];
	temp.m_col = new GLfloat[SIZE_CYLINDER_VERTEX];

	cout << vertices.size() << endl;
	cout << indices.size() << endl;

	for (int i = 0; i < vertices.size(); i++)	temp.m_pos[i] = vertices[i];
	for (int i = 0; i < vertices.size(); i++)
	{
		if (i % 3 == 0) temp.m_col[i] = x;
		if (i % 3 == 1)temp.m_col[i] = y;
		if (i % 3 == 2)temp.m_col[i] = z;
	}
	for (int i = 0; i < indices.size(); i++)	temp.m_inex[i] = indices[i];

	InitObjectStruct(&temp, SIZE_CYLINDER_INDEX, 12 * (SIZE_CYLINDER_VERTEX / 3), 12 * (SIZE_CYLINDER_INDEX / 3), 0, GL_TRIANGLES, true, false, true);

	m_ObjectList.emplace_back(temp);
}


void ObjectManager::CreateOrbit(float orbit_radius)
{
	const int numPoints = 100;  // 궤도를 구성하는 점의 수
	const float radius = orbit_radius;  // 반지름 크기

	// 궤도를 그리기 위한 배열 초기화
	std::vector<GLfloat> vertices;  // 궤도의 점들을 저장할 배열
	std::vector<GLfloat> colors;    // 각 점의 색상을 저장할 배열
	std::vector<GLuint> indices;    // 궤도를 그리기 위한 인덱스 배열

	for (int i = 0; i < numPoints; ++i) {
		float angle = 2.0f * PI * i / numPoints;
		float x = radius * cos(angle);
		float y = radius * sin(angle);

		vertices.push_back(x);
		vertices.push_back(0.0f);
		vertices.push_back(y);

		colors.push_back(0.0f);
		colors.push_back(0.0f);
		colors.push_back(0.0f);

		indices.push_back(i);
	}

	// 궤도
	temp.m_pos = new GLfloat[vertices.size()];
	temp.m_inex = new GLint[indices.size()];
	temp.m_col = new GLfloat[vertices.size()];

	for (int i = 0; i < numPoints; i++) temp.m_inex[i] = indices[i];
	for (int i = 0; i < vertices.size(); i++) temp.m_pos[i] = vertices[i];
	for (int i = 0; i < vertices.size(); i++) temp.m_col[i] = colors[i];

	InitObjectStruct(&temp, indices.size(), 12 * (vertices.size() / 3), 12 * (indices.size() / 3), 0, GL_LINE_LOOP, true, false, true);

	m_ObjectList.emplace_back(temp);
}

void ObjectManager::CreateOrbit2(float orbit_radius, float dir)
{
	const int numPoints = 100;  // 궤도를 구성하는 점의 수
	const float radius = orbit_radius;  // 반지름 크기

	// 궤도를 그리기 위한 배열 초기화
	std::vector<GLfloat> vertices;  // 궤도의 점들을 저장할 배열
	std::vector<GLfloat> colors;    // 각 점의 색상을 저장할 배열
	std::vector<GLuint> indices;    // 궤도를 그리기 위한 인덱스 배열

	for (int i = 0; i < numPoints; ++i) {
		float angle = 2.0f * PI * i / numPoints;

		angle += glm::radians(45.0f);

		float x = radius * cos(angle);
		float y = radius * sin(angle);
		float z = radius * sin(angle);

		vertices.push_back(x);
		vertices.push_back(y * dir);
		vertices.push_back(z);

		colors.push_back(0.0f);
		colors.push_back(0.0f);
		colors.push_back(0.0f);

		indices.push_back(i);
	}

	// 궤도
	temp.m_pos = new GLfloat[vertices.size()];
	temp.m_inex = new GLint[indices.size()];
	temp.m_col = new GLfloat[vertices.size()];

	for (int i = 0; i < numPoints; i++) temp.m_inex[i] = indices[i];
	for (int i = 0; i < vertices.size(); i++) temp.m_pos[i] = vertices[i];
	for (int i = 0; i < vertices.size(); i++) temp.m_col[i] = colors[i];

	InitObjectStruct(&temp, indices.size(), 12 * (vertices.size() / 3), 12 * (indices.size() / 3), 0, GL_LINE_LOOP, true, false, true);

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
		// 2 5 8 not
		if (i != 2 && i != 5 && i != 8) m_ObjectList[i].m_DRAW_TYPE = drawType;
	}
}

void ObjectManager::SetWireSolidType(int idx, int type)
{
	m_ObjectList[idx].m_DRAW_TYPE = type;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  [ 변환 ] //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	m_ObjectList[idx].m_mass = (x * 100) * (y * 100) * (z * 100);
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

	float mass_x = m_ObjectList[idx].m_scale[0];
	float mass_y = m_ObjectList[idx].m_scale[1];
	float mass_z = m_ObjectList[idx].m_scale[2];

	m_ObjectList[idx].m_mass = (mass_x * 100) * (mass_y * 100) * (mass_z * 100);
}

void ObjectManager::AddForce(int idx, float x, float y, float z, float elapsedTime)
{
	float m_mass = m_ObjectList[idx].m_mass;

	float accX = x / 1.0f;
	float accY = y / 1.0f;
	float accZ = z / 1.0f;

	m_ObjectList[idx].m_vel[0] = m_ObjectList[idx].m_vel[0] + accX * elapsedTime;
	m_ObjectList[idx].m_vel[1] = m_ObjectList[idx].m_vel[1] + accY * elapsedTime;
	m_ObjectList[idx].m_vel[2] = m_ObjectList[idx].m_vel[2] + accZ * elapsedTime;
}

void ObjectManager::UpdatePos(int idx, float elapsedTime)
{
	float m_mass = 1.0f;

	float m_velX = m_ObjectList[idx].m_vel[0];
	float m_velY = m_ObjectList[idx].m_vel[1];
	float m_velZ = m_ObjectList[idx].m_vel[2];

	float normalForce = m_mass * GRAVITY;

	float frictionCoef = 50.f;

	float friction = frictionCoef * normalForce;

	float frictionDirX = -m_velX;
	float frictionDirZ = -m_velZ;

	float mag = sqrtf(frictionDirX * frictionDirX + frictionDirZ * frictionDirZ);


	if (mag > FLT_EPSILON)
	{
		frictionDirX = frictionDirX / mag;
		frictionDirZ = frictionDirZ / mag;

		float frictionForceX = frictionDirX * friction;
		float frictionForceZ = frictionDirZ * friction;

		float frictionAccX = frictionForceX / m_mass;
		float frictionAccZ = frictionForceZ / m_mass;

		float resultVelX = m_velX + frictionAccX * elapsedTime;
		float resultVelZ = m_velY + frictionAccZ * elapsedTime;
		float resultVelY = m_velY;

		//cout << resultVelX << ", " << resultVelZ << endl;

		if (resultVelX * m_velX < 0.f)
		{
			m_velX = 0.f;
		}
		else
		{
			m_velX = resultVelX;
		}

		if (resultVelY * m_velY < 0.f)
		{
			m_velY = 0.f;
		}
		else
		{
			m_velY = resultVelY;
		}

		if (resultVelZ * m_velZ < 0.f)
		{
			m_velZ = 0.f;
		}
		else
		{
			m_velZ = resultVelZ;

			cout << m_velZ << endl;
		}
	}

	m_ObjectList[idx].m_vel[0] = m_velX;
	m_ObjectList[idx].m_vel[1] = m_velY;
	m_ObjectList[idx].m_vel[2] = m_velZ;

	m_ObjectList[idx].m_pivot[0] = m_ObjectList[idx].m_pivot[0] + m_velX * elapsedTime;
	m_ObjectList[idx].m_pivot[1] = m_ObjectList[idx].m_pivot[1] + m_velY * elapsedTime;
	m_ObjectList[idx].m_pivot[2] = m_ObjectList[idx].m_pivot[2] + m_velZ * elapsedTime;

	//cout << m_velX << ", " << m_velZ << endl;

	if (m_ObjectList[idx].m_pivot[1] < -1.0f)
	{
		m_ObjectList[idx].m_pivot[1] = -1.0f;
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

void ObjectManager::SetChild(int idx, int idx_child)
{
	m_ObjectList[idx].m_child.emplace_back(idx_child);
}

void ObjectManager::Reset()
{
	m_ObjectList.clear();
}






