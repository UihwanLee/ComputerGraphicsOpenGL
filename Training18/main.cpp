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
GLvoid SetRotatingAnimation(bool isAnim);
GLvoid RotatingAnimationX(int idx);
GLvoid RotatingCube(int isAnim);

GLvoid CubeTopAnim(int isAnim);
GLvoid CubeFrontAnim(int isAnim);
GLvoid CubeSideAnim(int isAnim);
GLvoid CubeBackAnim(int isAnim);

bool isAnimCubeTop = false;
bool isAnimCubeFont = false;
bool isAnimCubeSide = false;
bool isAnimCubeBack = false;

int FrontAnimTime = 0;
int SideAnimTime = 0;
int BackAnimTime = 0;
bool isAnimCubeFrontDown = false;
bool isAnimCubeSideUp = false;
bool isAnimCubeBackScale = false;

bool isDown = false;
bool isDown2 = false;

GLvoid PyramidAnim_01(int idx);
GLvoid PyramidAnim_01_End(int idx);
bool isRotatingAnim = false;

bool isAnimPyramid_01 = false;
bool isAnimPyramid_02 = false;

int PyramidFullTime = 0;
int PyramidTime = 0;

bool isCube = true;

GLfloat rotateSpeed = 5.0f;
GLfloat moveSpeed = -5.0f;
GLfloat downSpeed = -5.0f;

bool isFirst = true;

// ���� ��ȯ
bool projectionMode = true;

// Camera
GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(-1.0f, 1.0f, 2.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);


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
	cout << "h/H : �������� ����/����" << endl;
	cout << "y/Y : y�� �����Ѵ�/�����" << endl;
	cout << endl;
	cout << "t : ����ü ���� �ִϸ��̼�" << endl;
	cout << "f : ����ü �ո� �ִϸ��̼�" << endl;
	cout << "s : ����ü ���� �ִϸ��̼�" << endl;
	cout << "b : ����ü �޸� �ִϸ��̼�" << endl;
	cout << endl;
	cout << "o : �簢�� ��� �� ������/�ݴ´�" << endl;
	cout << "r : �簢�� �� �� ������/�ݴ´�" << endl;
	cout << endl;
	cout << "p : ���� ����/ ���� ����" << endl;
	cout << "c : ���� �ٲٱ� ����ü/�簢��" << endl;
	cout << endl;
}

GLvoid SetActive(bool isCube)
{
	if (isCube)
	{
		for (int i = 1; i < 7; i++) ObjMgr.m_ObjectList[i].m_isActive = true;
		for (int i = 7; i < 13; i++) ObjMgr.m_ObjectList[i].m_isActive = false;
	}
	else
	{
		for (int i = 1; i < 7; i++) ObjMgr.m_ObjectList[i].m_isActive = false;
		for (int i = 7; i < 13; i++) ObjMgr.m_ObjectList[i].m_isActive = true;
	}
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateCoordinate();
	for (int i = 0; i < 6; i++)
	{
		ObjMgr.CreateCubeFace(i);
	}
	ObjMgr.CreateSquarePyramidFace(0);
	ObjMgr.CreateSquarePyramidFace(1);
	ObjMgr.CreateSquarePyramidFace(2);
	ObjMgr.CreateSquarePyramidFace(3);
	ObjMgr.CreateSquarePyramidFace(4);
	ObjMgr.CreateSquarePyramidFace(5);

	ObjMgr.SetAllScale(0.3f, 0.4f, 0.3f);
	ObjMgr.SetAllModel();

	isAnimCubeSideUp = false;
	SetActive(true);
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
	isAnimCubeTop = false;
	isAnimCubeSide = false;
	isAnimCubeFont = false;
	isAnimCubeBack = false;

	FrontAnimTime = 0;
	SideAnimTime = 0;
	BackAnimTime = 0;

	isAnimCubeSideUp = false;
	isAnimCubeFrontDown = false;
	isAnimCubeBackScale = false;

	isAnimPyramid_01 = false;
	isAnimPyramid_02 = false;

	PyramidFullTime = 0;
	PyramidTime = 0;

	isDown = false;
	isDown2 = false;
}

GLvoid RotatingAnimationX(int idx)
{
	ObjMgr.Rotate(idx, 0.0f, rotateSpeed, 0.0f);

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, idx);
}

GLvoid RotatingCube(int isAnim)
{
	for (int i = 1; i < 7; i++)
	{
		ObjMgr.m_ObjectList[i].m_isAnimRotating = isAnim;
		if (ObjMgr.m_ObjectList[i].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, i);
	}
}

GLvoid CubeTopAnim(int isAnim)
{
	int idx = 1;

	ObjMgr.Rotate(idx, rotateSpeed, 0.0f, 0.0f);

	glutPostRedisplay();

	if (isAnimCubeTop) glutTimerFunc(30, CubeTopAnim, isAnimCubeTop);
	else
	{
		ObjMgr.Move(idx, 0.0f, -0.5f, 0.0f);
	}
}

GLvoid CubeFrontAnim(int isAnim)
{
	int idx = 2;

	if (isAnimCubeFrontDown)
	{
		downSpeed = 5.0f;
	}
	else
	{
		downSpeed = -5.0f;
	}

	FrontAnimTime += 1;

	ObjMgr.Rotate(idx, downSpeed, 0.0f, 0.0f);

	glutPostRedisplay();

	if (FrontAnimTime > 17)
	{
		isAnimCubeFont = false;
		FrontAnimTime = 0;
	}

	if (isAnimCubeFont) glutTimerFunc(30, CubeFrontAnim, isAnimCubeFont);
}

GLvoid CubeSideAnim(int isAnim)
{
	int idx1 = 3;
	int idx2 = 5;

	if (isAnimCubeSideUp)
	{
		moveSpeed = 0.05f;
	}
	else
	{
		moveSpeed = -0.05f;
	}

	SideAnimTime += 1;

	ObjMgr.Move(idx1, 0.0f, moveSpeed, 0.0f);
	ObjMgr.Move(idx2, 0.0f, moveSpeed, 0.0f);

	glutPostRedisplay();

	if (SideAnimTime > 15)
	{
		isAnimCubeSide = false;
		SideAnimTime = 0;
	}

	if (isAnimCubeSide) glutTimerFunc(30, CubeSideAnim, isAnimCubeSide);
}

GLvoid CubeBackAnim(int isAnim)
{
	int idx1 = 4;
	int idx2 = 6;

	float scaleSpeed;
	if (isAnimCubeBackScale)
	{
		scaleSpeed = -0.005f;
	}
	else
	{
		scaleSpeed = 0.005f;
	}

	BackAnimTime += 1;

	ObjMgr.Scale(idx1, scaleSpeed, scaleSpeed, scaleSpeed);
	ObjMgr.Scale(idx2, scaleSpeed, scaleSpeed, scaleSpeed);

	glutPostRedisplay();

	if (BackAnimTime > 20)
	{
		isAnimCubeBack = false;
		BackAnimTime = 0;
	}

	if (isAnimCubeBack) glutTimerFunc(30, CubeBackAnim, isAnimCubeBack);
}

float moveOffset = 0.03f;
float movingSpeed = 0.05f;

float target_X[4] = { -1.5f, 0.0f, 0.0f, 1.5f };
float target_Y[4] = { -0.5f, -0.5f, -0.5f, -0.5f };
float target_Z[4] = { 0.0f, 1.5f, -1.5f, 0.0f };

float target_X2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
float target_Y2[4] = { -1.5f, -1.5f, -1.5f, -1.5f };
float target_Y2_T[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
float target_Z2[4] = { 0.0f, -0.0f, 0.0f, 0.0f };


GLvoid PyramidAnim_01(int idx)
{
	if (idx >= 11)
	{
		isAnimPyramid_02 = false;
		return;
	}

	int idx_target = idx - 7;

	bool isAnim = true;

	float targetX, targetY, targetZ;

	targetX = target_X[idx_target];
	targetY = target_Y[idx_target];
	targetZ = target_Z[idx_target];

	float x = ObjMgr.m_ObjectList[idx].m_pos[0] - targetX;
	float y = ObjMgr.m_ObjectList[idx].m_pos[1] - targetY;
	float z = ObjMgr.m_ObjectList[idx].m_pos[2] - targetZ;

	// ���� ��ġ���� ���� ��ġ�� �̵�
	GLfloat distance = sqrt(x * x + y * y + z * z);
	if (distance > moveOffset) {
		ObjMgr.m_ObjectList[idx].m_pos[0] -= (x / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pos[1] -= (y / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pos[2] -= (z / distance) * movingSpeed;
	}
	else
	{
		isAnim = false;

		ObjMgr.m_ObjectList[idx].m_pos[0] = target_X[idx_target];
		ObjMgr.m_ObjectList[idx].m_pos[1] = target_Y[idx_target];
		ObjMgr.m_ObjectList[idx].m_pos[2] = target_Z[idx_target];

		if (isAnimPyramid_01)
		{
			glutTimerFunc(30, PyramidAnim_01_End, idx);
		}
		else if (isAnimPyramid_02)
		{
			int idx_new = idx++;
			glutTimerFunc(30, PyramidAnim_01, idx);
		}
	}

	glutPostRedisplay();

	if (isAnim) glutTimerFunc(30, PyramidAnim_01, idx);
}

GLvoid PyramidAnim_01_End(int idx)
{
	if (idx <= 6)
	{
		isAnimPyramid_02 = false;
		return;
	}

	int idx_target = idx - 7;

	bool isAnim = true;

	float targetX = target_X2[idx_target];
	float targetY = target_Y2[idx_target];
	float targetZ = target_Z2[idx_target];

	if (isAnimPyramid_01 && isDown == false)
	{
		targetY = target_Y2_T[idx_target];
	}

	if (isAnimPyramid_02 && isDown2 == false)
	{
		targetY = target_Y2_T[idx_target];
	}

	float x = ObjMgr.m_ObjectList[idx].m_pos[0] - targetX;
	float y = ObjMgr.m_ObjectList[idx].m_pos[1] - targetY;
	float z = ObjMgr.m_ObjectList[idx].m_pos[2] - targetZ;

	// ���� ��ġ���� ���� ��ġ�� �̵�
	GLfloat distance = sqrt(x * x + y * y + z * z);
	if (distance > moveOffset) {
		ObjMgr.m_ObjectList[idx].m_pos[0] -= (x / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pos[1] -= (y / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pos[2] -= (z / distance) * movingSpeed;
	}
	else
	{
		isAnim = false;

		ObjMgr.m_ObjectList[idx].m_pos[0] = target_X2[idx_target];
		ObjMgr.m_ObjectList[idx].m_pos[1] = target_Y2[idx_target];
		ObjMgr.m_ObjectList[idx].m_pos[2] = target_Z2[idx_target];

		if (isAnimPyramid_01 && isDown == false)
		{
			ObjMgr.m_ObjectList[idx].m_pos[0] = target_X2[idx_target];
			ObjMgr.m_ObjectList[idx].m_pos[1] = target_Y2_T[idx_target];
			ObjMgr.m_ObjectList[idx].m_pos[2] = target_Z2[idx_target];
		}

		if (isAnimPyramid_02)
		{
			ObjMgr.m_ObjectList[idx].m_pos[0] = target_X2[idx_target];
			ObjMgr.m_ObjectList[idx].m_pos[1] = target_Y2_T[idx_target];
			ObjMgr.m_ObjectList[idx].m_pos[2] = target_Z2[idx_target];

			int idx_new = idx - 1;
			glutTimerFunc(30, PyramidAnim_01_End, idx_new);
		}

		if (isAnimPyramid_01 && idx==10) isAnimPyramid_01 = false;
	}

	glutPostRedisplay();

	if (isAnim) glutTimerFunc(30, PyramidAnim_01_End, idx);
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
	case 'Y':
	case 'y':
		// y�࿡ ���� �����Ѵ�/�����.
		isRotatingAnim = (key == 'Y') ? true : false;
		RotatingCube(isRotatingAnim);
		break;
	// ����ü �ִϸ��̼�
	case 't':
		//����ü�� ���� �ִϸ��̼� ����/���� (����)
		Reset();
		isAnimCubeTop = !isAnimCubeTop;
		ObjMgr.Move(1, 0.0f, 0.5f, 0.0f);
		if (isAnimCubeTop) glutTimerFunc(30, CubeTopAnim, isAnimCubeTop);
		break;
	case 'f':
		//����ü�� �ո��� ����/ �ո��� �ݴ´� (ȸ��)
		isAnimCubeFrontDown = !isAnimCubeFrontDown;
		isAnimCubeFont = true;
		if (isAnimCubeFont) glutTimerFunc(30, CubeFrontAnim, isAnimCubeFont);
		break;
	case 's':
		// ����ü�� ������ ����/ �ݴ´� (�����̵� �̵� ��ȯ)
		isAnimCubeSideUp = !isAnimCubeSideUp;
		isAnimCubeSide = true;
		if (isAnimCubeSide) glutTimerFunc(30, CubeSideAnim, isAnimCubeSideUp);
		break;
	case 'b':
		// ����ü�� �޸��� ����/ �ݴ´� (ũ�� Ŀ��/�۾���)
		isAnimCubeBackScale = !isAnimCubeBackScale;
		isAnimCubeBack = true;
		if (isAnimCubeBack) glutTimerFunc(30, CubeBackAnim, isAnimCubeBackScale);
		break;
	// �簢�� �ִϸ��̼�
	case 'o':
		isAnimPyramid_01 = true;
		if (isDown) isDown = false;
		else isDown = true;
		if (isAnimPyramid_01) glutTimerFunc(30, PyramidAnim_01, 7);
		if (isAnimPyramid_01) glutTimerFunc(30, PyramidAnim_01, 8);
		if (isAnimPyramid_01) glutTimerFunc(30, PyramidAnim_01, 9);
		if (isAnimPyramid_01) glutTimerFunc(30, PyramidAnim_01, 10);
		break;
	case 'r':
		isAnimPyramid_02 = true;
		if (isDown2) isDown2 = false;
		else isDown2 = true;
		if (isDown2) glutTimerFunc(30, PyramidAnim_01, 7);
		else  glutTimerFunc(30, PyramidAnim_01_End, 10);
		break;
	case 'P':
	case 'p':
		projectionMode = !projectionMode;
		break;
	case 'C':
	case 'c':
		// ���� �ٲٱ�
		isCube = !isCube;
		SetActive(isCube);
		break;
	case 'Z':
	case 'z':
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
