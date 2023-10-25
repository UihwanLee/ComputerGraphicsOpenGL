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
bool isDepthTest = true;

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, glm::mat4& model, float* modelInfo, int idx);

// �ִϸ��̼� :: �ʱ�ȭ
GLvoid StopAllAnim();

// �ִϸ��̼� :: ũ����
GLvoid MovingCrainBottomX(int isAnim);
GLvoid RotatingCrainMidY(int isAnim);

// �ִϸ��̼� :: ī�޶�
GLvoid RotatingCamera(int isAnim);
GLvoid RotatingCamera_Z(int isAnim);

// �ִϸ��̼� :: ����
bool isMovingCrainBottomX = false;
bool isMovingCrainBottomX_Front = true;
bool isRotatingCrainMidY = false;
bool isRotatingCrainMidY_Front = true;

GLfloat rotateSpeed = 5.0f;
GLfloat moveSpeed = 0.5f;

// ���� ��ȯ
bool projectionMode = true;

// Camera
GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(0.5f, 0.5f, 0.0f);
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
	cout << "b/B: ũ������ �Ʒ� ��ü�� x�� �������� ��/�� �������� �̵��Ѵ�" << endl;
	cout << "f/F: ������ y�࿡ ���Ͽ� ��/�� �������� ȸ���ϴµ�, �� ������ ���� �ݴ�������� ȸ���Ѵ�." << endl;
	cout << "e/E: 2�� ������ ���ݾ� �̵��ؼ� �� ���� �ȴ�/�ٽ� ���ڸ��� �̵��ؼ� 2���� �ȴ�." << endl;
	cout << "t/T: ũ������ �� �� 2���� ���� z�࿡ ���Ͽ� ��/�� �������� ���� �ݴ�������� ȸ���Ѵ�. " << endl;
	cout << endl;
	cout << "z/Z: ī�޶� z�� ��/�� �������� �̵�" << endl;
	cout << "x/X: ī�޶� x�� ��/�� �������� �̵�" << endl;
	cout << "y/Y: ī�޶� ���� y�࿡ ���Ͽ� ȸ��" << endl;
	cout << "r/R: ȭ���� �߽��� y�࿡ ���Ͽ� ī�޶� ȸ�� (������ ���Ͽ� ����)" << endl;
	cout << endl;
	cout << "a/A: r ���ɾ�� ���� ȭ���� �߽��� �࿡ ���Ͽ� ī�޶� ȸ���ϴ� �ִϸ��̼��� �����Ѵ�/�����." << endl;
	cout << "s/S: ��� ������ ���߱�" << endl;
	cout << "c/C: ��� �������� �ʱ�ȭ�ȴ�." << endl;
	cout << "Q: ���α׷� �����Ѵ�." << endl;
	cout << endl;
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateCoordinate();

	int idx = 0;

	// ����
	ObjMgr.CreateCube(0.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(1, 1.0f, 0.1f, 1.0f);
	ObjMgr.SetPosition(1, 0.0f, -0.2f, 0.0f);

	// ũ���� �Ʒ� ��ü
	ObjMgr.CreateCube(255.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(2, 0.2f, 0.2f, 0.2f);

	// ũ���� �߾� ��ü
	ObjMgr.CreateCube(0.0f, 0.0f, 255.0f);
	ObjMgr.SetScale(3, 0.6f, 0.6f, 0.6f);
	ObjMgr.SetPosition(3, 0.0f, 0.4f, 0.0f);

	// ũ���� ���� ����
	ObjMgr.CreateCube(255.0f, 255.0f, 0.0f);
	ObjMgr.SetScale(4, 0.3f, 0.03f, 0.03f);
	ObjMgr.SetPosition(4, 0.0f, 1.5f, 1.5f);

	//// ũ���� ���� ������
	ObjMgr.CreateCube(255.0f, 255.0f, 0.0f);
	ObjMgr.SetScale(5, 0.3f, 0.03f, 0.03f);
	ObjMgr.SetPosition(5, 0.0f, 1.5f, -1.5f);

	//// ũ���� �� ����
	ObjMgr.CreateCube(255.0f, 0.0f, 255.0f);
	ObjMgr.SetScale(6, 0.03f, 0.25f, 0.03);
	ObjMgr.SetPosition(6, 0.0f, 0.4f, 1.0f);

	//// ũ���� �� ������
	ObjMgr.CreateCube(255.0f, 0.0f, 255.0f);
	ObjMgr.SetScale(7, 0.03f, 0.25f, 0.03);
	ObjMgr.SetPosition(7, 0.0f, 0.4f, -1.0f);

	// �ڽ� ����
	//ObjMgr.SetChild(2, 3); // ũ���� �Ʒ� ��ü <- ũ���� �߾� ��ü
	//ObjMgr.SetChild(2, 4); // ũ���� �Ʒ� ��ü <- ũ���� ���� ����
	//ObjMgr.SetChild(2, 5); // ũ���� �Ʒ� ��ü <- ũ���� ���� ������

	//ObjMgr.SetChild(3, 6); // ũ���� �߾� ��ü <- ũ���� �� ����
	//ObjMgr.SetChild(3, 7); // ũ���� �߾� ��ü <- ũ���� �� ������
}

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

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// �亯ȯ

	glm::mat4 projection = glm::mat4(1.0f);

	// ��������
	if (projectionMode == false)
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

	ObjMgr.m_ObjectList[idx].m_model = ObjMgr.TransformModel(idx);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObjMgr.m_ObjectList[idx].m_model));

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
	isMovingCrainBottomX = false;
}

GLvoid MovingCrainBottomX(int isAnim)
{
	int idx = 2;

	float speed = 0.0f;
	speed = (isMovingCrainBottomX_Front) ? 0.05f : -0.05f;

	ObjMgr.Move(2, speed, 0.0f, 0.0f);

	glutPostRedisplay();

	if (isMovingCrainBottomX) glutTimerFunc(30, MovingCrainBottomX, isMovingCrainBottomX);
}

// ũ���� �߾� ��ü Y�� �������� ȸ�� -> �� 2���� ���� ȸ���ؾ���
GLvoid RotatingCrainMidY(int isAnim)
{
	int idx = 3;

	float rotateSpeed = 5.0f;
	rotateSpeed = (isRotatingCrainMidY_Front) ? 5.0f : -5.0f;

	ObjMgr.Rotate(idx, 0.0f, rotateSpeed, 0.0f);

	glutPostRedisplay();

	if (isRotatingCrainMidY) glutTimerFunc(30, RotatingCrainMidY, isRotatingCrainMidY);
}

float angle_camera = 0;
GLvoid RotatingCamera(int isAnim)
{
	float radius = 5.0f;
	CameraPos.x = sin(angle_camera) * radius;
	CameraPos.z = cos(angle_camera) * radius;

	angle_camera += 0.03f;

	glutPostRedisplay();

	if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
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
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'H':
	case 'h':
		// ���� ����
		if (isDepthTest) isDepthTest = false;
		else isDepthTest = true;
		break;
	// �ִϸ��̼� :: ũ���� �Ʒ� ��ü �̵�
	case 'B':
		isMovingCrainBottomX_Front = false;
		if (isMovingCrainBottomX) isMovingCrainBottomX = false;
		else isMovingCrainBottomX = true;
		if(isMovingCrainBottomX) glutTimerFunc(30, MovingCrainBottomX, isMovingCrainBottomX);
		break;
	case 'b':
		isMovingCrainBottomX_Front = true;
		if (isMovingCrainBottomX) isMovingCrainBottomX = false;
		else isMovingCrainBottomX = true;
		if (isMovingCrainBottomX) glutTimerFunc(30, MovingCrainBottomX, isMovingCrainBottomX);
		break;
	// �ִϸ��̼� :: ũ���� �߾� ��ü ȸ��
	case 'M':
		isRotatingCrainMidY_Front = false;
		if (isRotatingCrainMidY) isRotatingCrainMidY = false;
		else isRotatingCrainMidY = true;
		if (isRotatingCrainMidY) glutTimerFunc(30, RotatingCrainMidY, isRotatingCrainMidY);
		break;
	case 'm':
		isRotatingCrainMidY_Front = true;
		if (isRotatingCrainMidY) isRotatingCrainMidY = false;
		else isRotatingCrainMidY = true;
		if (isRotatingCrainMidY) glutTimerFunc(30, RotatingCrainMidY, isRotatingCrainMidY);
		break;
	// �ִϸ��̼� :: ī�޶�
	case 'Y':
	case 'y':
		rotatingCamera_z = false;
		if (rotatingCarmera) rotatingCarmera = false;
		else if (rotatingCarmera == false) rotatingCarmera = true;
		if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
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