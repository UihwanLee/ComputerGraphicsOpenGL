#include <iostream>
#include "OpenGL.h"
#include <random>
#include "InitShader.h"
#include "ObjectManager.h"
#include <cmath>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define WIDTH 800
#define HEIGHT 600

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

// Ŭ����
ObjectManager ObjMgr;

GLvoid Init();
GLvoid Message();
GLvoid Reset();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[2], EBO;
bool isDepthTest = false;

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
int NUM_VETEX, int SIZE_COL, int SIZE_IDX, glm::mat4& model, float* modelInfo, int idx);

// ��ȯ

// �ִϸ��̼�
GLvoid StopAllAnim();
GLvoid MovingThroughOrbit(int idx);
GLvoid MovingThroughOrbit2(int idx);
GLvoid MovingThroughOrbit3(int idx);
GLvoid MovingThroughOrbit4(int idx);
GLvoid MovingOrbitUp(int idx);

GLvoid RotatingCamera(int isAnim);
GLvoid RotatingCamera_Z(int isAnim);

void timer(int value);

bool isRotatingAnim = false;

GLfloat rotateSpeed = 5.0f;
GLfloat moveSpeed = 0.5f;

bool isFirst = true;

// ���� ��ȯ
bool projectionMode = true;

// Camera
GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(-1.0f, 1.0f, 2.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);

bool rotatingCarmera = false;
bool rotatingCamera_z = false;
float rotatingCameraRate = 0.0f;
float rotatingCameraRate_z = 0.0f;


void Keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Training");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	Init();
	Message();

	InitProgram(ShaderProgram);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutTimerFunc(0, timer, 0); // Ÿ�̸� �Լ� ���
	glutMainLoop();
}

GLvoid Init()
{
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	Reset();
}

GLvoid Message()
{
	cout << "h/H : ���� ����" << endl;
	cout << "p/P : ���� ����/ ���� ����" << endl;
	cout << "m/M : �ָ��� ��/���̾� ��" << endl;
	cout << endl;
	cout << "w/a/s/d : ��/��/��/�� �̵�" << endl;
	cout << "+/- : ��/�� �̵�" << endl;
	cout << "y/Y : ��ü ���� y������ ��/�� ȸ��" << endl;
	cout << "z/Z : �༺,��,�˵��� z�࿡ ���Ͽ� ��/�� �������� ȸ��" << endl;
	cout << endl;
}

const int numPoints = 100; // �˵��� �����ϴ� ���� ��
const float orbitRadius = 0.3f; // �˵� ������
const float rotationSpeed = 1.0f; // ���� �ӵ� (����/��)
const float revolutionSpeed = 0.0f; // ���� �ӵ� (����/��)

// ���� ��ü�� ���� ����
float revolutionAngle = 0.0f;

void timer(int value) {
	revolutionAngle += revolutionSpeed;
	if (revolutionAngle >= 2.0f * PI) {
		revolutionAngle -= 2.0f * PI;
	}

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0); // 16ms���� ������Ʈ (�� 60������/��)
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateCoordinate();

	// �¾�
	ObjMgr.CreateSqhere(0.0f, 0.0f, 255.0f);
	ObjMgr.SetScale(1, 0.3f, 0.4f, 0.3f);

	// �˵�1
	ObjMgr.CreateOrbit(2.0f);
	ObjMgr.SetScale(2, 0.3f, 0.4f, 0.3f);

	// ����1
	ObjMgr.CreateSqhere(0.0f, 255.0f, 0.0f);
	ObjMgr.SetScale(3, 0.2f, 0.3f, 0.2f);
	ObjMgr.SetPosition(3, 0.5f, 0.0f, 0.0f);

	// ��
	ObjMgr.CreateSqhere(255.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(4, 0.1f, 0.15f, 0.1f);
	ObjMgr.SetPosition(4, 0.75f, 0.0f, 0.0f);

	// �˵�2
	ObjMgr.CreateOrbit2(2.0f, 1.0f);
	ObjMgr.SetScale(5, 0.3f, 0.4f, 0.3f);

	// ����2
	ObjMgr.CreateSqhere(0.0f, 255.0f, 0.0f);
	ObjMgr.SetScale(6, 0.2f, 0.3f, 0.2f);
	ObjMgr.SetPosition(6, 0.5f, 0.7f, 0.4f);

	// ��2
	ObjMgr.CreateSqhere(255.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(7, 0.1f, 0.15f, 0.1f);
	ObjMgr.SetPosition(7, 0.87f, 0.7f, 0.35f);

	// �˵�3
	ObjMgr.CreateOrbit2(2.0f, -1.0f);
	ObjMgr.SetScale(8, 0.3f, 0.4f, 0.3f);

	// ����3
	ObjMgr.CreateSqhere(0.0f, 255.0f, 0.0f);
	ObjMgr.SetScale(9, 0.2f, 0.3f, 0.2f);
	ObjMgr.SetPosition(9, 0.45f, -0.5f, 0.45f);

	// ��3
	ObjMgr.CreateSqhere(255.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(10, 0.1f, 0.15f, 0.1f);
	ObjMgr.SetPosition(10, 0.7f, -0.45f, 0.4f);

	ObjMgr.SetAllModel();

	ObjMgr.m_ObjectList[0].m_isActive = false;

	// �ڽ� ����
	ObjMgr.SetChild(1, 2); // �¾� <- ����1
	ObjMgr.SetChild(1, 3); // �¾� <- �˵�1
	ObjMgr.SetChild(3, 4); // ����1 <- ��1
	ObjMgr.SetChild(1, 5); // �¾�1 <- �˵�2
	ObjMgr.SetChild(1, 6); // �¾�1 <- ����2
	ObjMgr.SetChild(6, 7); // ����2 <- ��2
	ObjMgr.SetChild(1, 8); // �¾�1 <- �˵�3
	ObjMgr.SetChild(1, 9); // �¾�1 <- ����3
	ObjMgr.SetChild(9, 10); // ����3 <- ��3

	//MovingThroughOrbit(3);
	//MovingThroughOrbit(4);
	//MovingThroughOrbit(6);
	//MovingThroughOrbit(7);

	//ObjMgr.SetChild(5, 6); // �˵�2 <- ����2


	// ������ ���� �ֱ⸦ ���� �ٲ�������
	// �¾� -> ����
	// ����1 -> ���� + ����
	MovingThroughOrbit(3);
	MovingThroughOrbit(4);
	MovingThroughOrbit3(6);
	MovingThroughOrbit(7);
	MovingThroughOrbit4(9);
	MovingThroughOrbit(10);
}

float angle = 0.0f; // ������ ���� ��ġ ����
int step[11] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // �� ���ܴ� ���� ������
float radius_list[11] = { 2.5f, 2.5f, 2.5f, 0.5f, 0.25f, 2.5f, 0.7f, 0.25f, 0.5f, 0.7f, 0.25f };
                          // 0     1     2     3     4     5     6     7     8     9
float angle_list[11] = { 2.5f, 2.5f, 2.5f, 0.5f, 0.25f, 2.5f, -45.0f, 0.25f, -45.0f, -45.0f, 2.5f };
                       // 0     1     2     3     4     5      6        7     8       9

float dir[10] = { 2.5f, 2.5f, 2.5f, 0.5f, 0.25f, 2.5f,  1.0f,   1.0f, -45.0f, -1.0f };
                 // 0     1     2     3     4     5      6        7     8       9


GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	if (isDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projectionTransform");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "viewTransform");

	glm::vec3 cameraPos = glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z); //--- ī�޶� ��ġ
	glm::vec3 cameraDirection = glm::vec3(AT.x, AT.y, AT.z); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	//view = glm::rotate(view, rotatingCameraRate, glm::vec3(0.0f, 1.0f, 0.0f));
	//view = glm::rotate(view, rotatingCameraRate_z, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// �亯ȯ

	glm::mat4 projection = glm::mat4(1.0f);

	// ��������
	if (projectionMode)
	{
		float left = -1.0f;
		float right = 1.0f;
		float bottom = -1.0f;
		float top = 1.0f;
		float zNear = -10.0f;
		float zFar = 10.0f;
		projection = glm::ortho(left, right, bottom, top, zNear, zFar);
	}
	else
	{
		// ���� ����
		float fov = 45.0f; // �þ߰� (Field of View)
		float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT); // ȭ���� ���� ���� ����
		float zNear = 0.1f; // ����� Ŭ���� ���
		float zFar = 50.0f; // �� Ŭ���� ���
		projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar); //--- ���� ���� ����: (���װ���, ��Ⱦ��, �����Ÿ�, �հŸ�)

	}

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// ������ȯ

	for (int i = 0; i < ObjMgr.m_ObjectList.size(); i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			if (ObjMgr.m_ObjectList[i].m_isModeIDX)
			{
				DrawObjectByIDX(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_pos, ObjMgr.m_ObjectList[i].m_inex, ObjMgr.m_ObjectList[i].m_col,
					ObjMgr.m_ObjectList[i].m_pivot, ObjMgr.m_ObjectList[i].m_rotate, ObjMgr.m_ObjectList[i].m_scale,
					ObjMgr.m_ObjectList[i].m_num_vertex, ObjMgr.m_ObjectList[i].m_size_pos, ObjMgr.m_ObjectList[i].m_size_idx, ObjMgr.m_ObjectList[i].m_model,
					ObjMgr.m_ObjectList[i].m_modelInfo, i);
			}
			else
			{
				DrawObjectByArray(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_pos, ObjMgr.m_ObjectList[i].m_col,
					ObjMgr.m_ObjectList[i].m_num_vertex, ObjMgr.m_ObjectList[i].m_size_pos);
			}
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, posList, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(1.0f, 1.0f, 1.0f));

	move = glm::translate(move, glm::vec3(0.0f, 0.0f, 0.0f));

	//rot = glm::rotate(rot, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//rot = glm::rotate(rot, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	model = model * move * rot * scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// �𵨺�ȯ

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, colList, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_LINES, 0, NUM_VETEX);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, glm::mat4& model, float* modelInfo, int idx)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_IDX, obj_index, GL_STATIC_DRAW);

	// �𵨺�ȯ
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);


	glDrawElements(DRAW_TYPE, NUM_VETEX, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid StopAllAnim()
{

}

// �˵� ����
GLvoid MovingThroughOrbit(int idx)
{
	float radius = radius_list[idx];

	angle = 2.0f * PI * step[idx] / 100;
	step[idx]++;

	if (step[idx] > 100)
	{
		step[idx] = 0;
	}

	float cur_move_x = radius * cos(angle);
	float cur_move_z = radius * sin(angle);

	float next_angle = 2.0f * PI * step[idx] / 100;

	float next_move_x = radius * cos(next_angle);
	float next_move_z = radius * sin(next_angle);

	float move_x = next_move_x - cur_move_x;
	float move_z = next_move_z - cur_move_z;

	ObjMgr.Move(idx, move_x, 0.0f, move_z);

	glutPostRedisplay();

	glutTimerFunc(30, MovingThroughOrbit, idx);
}

GLvoid MovingThroughOrbit2(int idx)
{
	float radius = radius_list[idx];
	float anlge = angle_list[idx];

	angle = 2.0f * PI * step[idx] / 100;
	step[idx]++;

	if (step[idx] > 100)
	{
		step[idx] = 0;
	}

	// �� ��� 45���� ����̷���, ������ 45���� ����
	angle += glm::radians(anlge);

	float cur_move_x = radius * cos(angle);
	float cur_move_y = radius * sin(angle);
	float cur_move_z = radius * sin(angle);

	float next_angle = 2.0f * PI * step[idx] / 100;

	// �� ��� 45���� ����̷���, ������ 45���� ����
	next_angle += glm::radians(anlge);

	float next_move_x = radius * cos(next_angle);
	float next_move_y = radius * sin(next_angle);
	float next_move_z = radius * sin(next_angle);

	float move_x = next_move_x - cur_move_x;
	float move_y = next_move_y - cur_move_y;
	float move_z = next_move_z - cur_move_z;

	ObjMgr.Move(idx, move_x, move_y * dir[idx], move_z);

	glutPostRedisplay();

	glutTimerFunc(30, MovingThroughOrbit2, idx);
}

int idx_orbit = 0;
float moveOffset = 0.3f;
float movingSpeed = 0.5f;
GLvoid MovingThroughOrbit3(int idx)
{
	if (idx_orbit + 5 > 300)
	{
		idx_orbit = 0;
	}
	//0.3f, 0.4f, 0.3f
	float scale_x = 0.3f;
	float scale_y = 0.4;
	float scale_z = 0.3f;

	float cur_move_x = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit] * scale_x;
	float cur_move_y = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit+1] * scale_y;
	float cur_move_z = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit+2] * scale_z;

	float next_move_x = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit + 3] * scale_x;
	float next_move_y = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit + 4] * scale_y;
	float next_move_z = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit + 5] * scale_z;

	float move_x = next_move_x - cur_move_x;
	float move_y = next_move_y - cur_move_y;
	float move_z = next_move_z - cur_move_z;

	ObjMgr.Move(idx, move_x, move_y * dir[idx], move_z);

	idx_orbit += 3;

	glutPostRedisplay();


	glutTimerFunc(30, MovingThroughOrbit3, idx);
}

int idx_orbit_2 = 0;
GLvoid MovingThroughOrbit4(int idx)
{
	if (idx_orbit_2 + 5 > 300)
	{
		idx_orbit_2 = 0;
	}
	//0.3f, 0.4f, 0.3f
	float scale_x = 0.3f;
	float scale_y = 0.4;
	float scale_z = 0.3f;

	float cur_move_x = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit_2] * scale_x;
	float cur_move_y = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit_2 + 1] * scale_y;
	float cur_move_z = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit_2 + 2] * scale_z;

	float next_move_x = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit_2 + 3] * scale_x;
	float next_move_y = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit_2 + 4] * scale_y;
	float next_move_z = ObjMgr.m_ObjectList[idx - 1].m_pos[idx_orbit_2 + 5] * scale_z;

	float move_x = next_move_x - cur_move_x;
	float move_y = next_move_y - cur_move_y;
	float move_z = next_move_z - cur_move_z;

	ObjMgr.Move(idx, move_x, move_y, move_z);

	idx_orbit_2 += 3;

	glutPostRedisplay();


	glutTimerFunc(30, MovingThroughOrbit4, idx);
}

float angle_camera = 0;
GLvoid RotatingCamera(int isAnim)
{
	float radius = 5.0f;
	CameraPos.x = sin(angle_camera) * radius;
	CameraPos.z = cos(angle_camera) * radius;

	angle_camera += 0.03f;
	
	glutPostRedisplay();

	if(rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
}

float angle_camera_z = 0;
GLvoid RotatingCamera_Z(int isAnim)
{
	float radius = 5.0f;
	CameraPos.x = sin(angle_camera_z) * radius;
	CameraPos.y = cos(angle_camera_z) * radius;

	angle_camera_z += 0.03f;

	glutPostRedisplay();

	if (rotatingCamera_z) glutTimerFunc(30, RotatingCamera_Z, rotatingCamera_z);
}

// y �����̵�
int count_Up = 0;
float move_y = 5.0f;
GLvoid MovingOrbitUp(int idx)
{
	ObjMgr.Rotate(idx, 0.0f, move_y, 0.0f);

	glutPostRedisplay();

	glutTimerFunc(30, MovingOrbitUp, idx);
}

GLvoid RotatingAnimationX(int idx)
{
	ObjMgr.SetRotate(idx, ObjMgr.m_ObjectList[idx].m_rotate[0] + rotateSpeed, ObjMgr.m_ObjectList[idx].m_rotate[1], ObjMgr.m_ObjectList[idx].m_rotate[2]);

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, idx);
}

GLvoid SetRotatingAnimation(bool isAnim)
{
	for (int i = 1; i < 7; i++)
	{
		ObjMgr.m_ObjectList[i].m_isAnimRotating = isAnim;
		if (ObjMgr.m_ObjectList[i].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, i);
	}
}

GLvoid MoveObject_X(int idx, float dir)
{
	float move = moveSpeed * dir;
	ObjMgr.Move(idx, move, 0, 0);
}

GLvoid MoveObject_Y(int idx, float dir)
{
	float move = moveSpeed * dir;
	ObjMgr.Move(idx, 0, move, 0);
}

GLvoid MoveObject_Z(int idx, float dir)
{
	float move = moveSpeed * dir;
	ObjMgr.Move(idx, 0, 0, move);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'H':
	case 'h':
		// ���� ����
		isDepthTest = !isDepthTest;
		break;
	case 'w':
		MoveObject_Y(1, 1.0f);
		break;
	case 'a':
		MoveObject_X(1, -1.0f);
		break;
	case 's':
		MoveObject_Y(1, -1.0f);
		break;
	case 'd':
		MoveObject_X(1, 1.0f);
		break;
	case '-':
		MoveObject_Z(1, -1.0f);
		break;
	case '+':
		MoveObject_Z(1, 1.0f);
		break;
	case 'M':
	case 'm':
		ObjMgr.ChangeWireSolidType();
		break;
	case 'Y':
	case 'y':
		rotatingCamera_z = false;
		if (rotatingCarmera) rotatingCarmera = false;
		else if(rotatingCarmera ==false) rotatingCarmera = true;
		if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
		break;
	case 'Z':
	case 'z':
		rotatingCarmera = false;
		if (rotatingCamera_z) rotatingCamera_z = false;
		else if (rotatingCamera_z == false) rotatingCamera_z = true;
		if (rotatingCamera_z) glutTimerFunc(30, RotatingCamera_Z, rotatingCamera_z);
		break;
	case 'P':
	case 'p':
		projectionMode = !projectionMode;
		break;
	case 'C':
	case 'c':
		Reset();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

	glutPostRedisplay();
}
