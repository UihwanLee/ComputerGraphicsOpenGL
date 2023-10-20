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

GLvoid CubeTopAnim(int isAnim);
GLvoid CubeFrontAnim(int isAnim);
GLvoid CubeSideAnim(int isAnim);
GLvoid CubeBackAnim(int isAnim);

bool isAnimCubeTop = false;
bool isAnimCubeFont = false;
bool isAnimCubeSide = false;
bool isAnimCubeSideUp = false;
bool isAnimCubeBack = false;

int SideAnimTime = 0;

bool isRotatingAnim = false;

GLfloat rotateSpeed = 5.0f;
GLfloat moveSpeed = -5.0f;

bool isFirst = true;

// ���� ��ȯ
bool projectionMode = true;

// Camera
GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(-1.0f, 1.0f, 2.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);

// model ����
bool isModelChange = false;


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
	cout << endl;
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

	ObjMgr.SetAllScale(0.3f, 0.4f, 0.3f);
	ObjMgr.SetAllModel();

	isAnimCubeSideUp = false;
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

	glm::mat4 model2 = glm::mat4(1.0f);
	glm::mat4 model1 = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(scaleInfo[0], scaleInfo[1], scaleInfo[2]));

	rot = glm::rotate(rot, glm::radians(rotateInfo[1]), glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, glm::radians(rotateInfo[0]), glm::vec3(0.0f, 1.0f, 0.0f));

	if(idx == 1) move = glm::translate(move, glm::vec3(pivot[0], pivot[1] + 0.2f, pivot[2]));
	else move = glm::translate(move, glm::vec3(pivot[0], pivot[1], pivot[2]));

	model1 = rot * move * scale;

	model2 = move * rot * scale;

	// model ����� ��� ���
	if (!ObjMgr.m_ObjectList[idx].m_Initmodel)
	{
		for (int j = 0; j < 4; j++) {
			modelInfo[j] = model1[3][j];
		}
		ObjMgr.m_ObjectList[idx].m_Initmodel = true;
	}

	for (int j = 0; j < 4; j++) {
		model2[3][j] = modelInfo[j];
	}

	if(isModelChange) glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model2));
	else glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model1));

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
}

GLvoid RotatingAnimationX(int idx)
{
	ObjMgr.SetRotate(idx, ObjMgr.m_ObjectList[idx].m_rotate[0] + rotateSpeed, ObjMgr.m_ObjectList[idx].m_rotate[1], ObjMgr.m_ObjectList[idx].m_rotate[2]);

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, idx);
}

GLvoid SetRotatingAnimation(bool isAnim)
{
	for (int i = 1; i < ObjMgr.m_ObjectList.size(); i++)
	{
		ObjMgr.m_ObjectList[i].m_isAnimRotating = isAnim;
		if (ObjMgr.m_ObjectList[i].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, i);
	}
}

GLvoid CubeTopAnim(int isAnim)
{
	int idx = 1;

	ObjMgr.SetRotate(idx, ObjMgr.m_ObjectList[idx].m_rotate[0], ObjMgr.m_ObjectList[idx].m_rotate[1] + rotateSpeed, ObjMgr.m_ObjectList[idx].m_rotate[2]);

	glutPostRedisplay();

	if (isAnimCubeTop) glutTimerFunc(30, CubeTopAnim, isAnimCubeTop);
}

GLvoid CubeFrontAnim(int isAnim)
{

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

	ObjMgr.SetPosition(idx1, ObjMgr.m_ObjectList[idx1].m_pivot[0], ObjMgr.m_ObjectList[idx1].m_pivot[1] + moveSpeed, ObjMgr.m_ObjectList[idx1].m_pivot[2]);
	ObjMgr.SetPosition(idx2, ObjMgr.m_ObjectList[idx2].m_pivot[0], ObjMgr.m_ObjectList[idx2].m_pivot[1] + moveSpeed, ObjMgr.m_ObjectList[idx2].m_pivot[2]);

	glutPostRedisplay();

	if (SideAnimTime > 10)
	{
		isAnimCubeSide = false;
		SideAnimTime = 0;
	}

	if (isAnimCubeSide) glutTimerFunc(30, CubeSideAnim, isAnimCubeSide);
}

GLvoid CubeBackAnim(int isAnim)
{

}

void Keyboard(unsigned char key, int x, int y)
{
	isModelChange = false;
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
		SetRotatingAnimation(isRotatingAnim);
		break;
	// ����ü �ִϸ��̼�
	case 't':
		//����ü�� ���� �ִϸ��̼� ����/���� (����)
		isModelChange = true;
		isAnimCubeTop = !isAnimCubeTop;
		for (int i = 1; i < ObjMgr.m_ObjectList.size(); i++)
		{
			ObjMgr.m_ObjectList[i].m_Initmodel = !ObjMgr.m_ObjectList[i].m_Initmodel;
		}
		if (isAnimCubeTop) glutTimerFunc(30, CubeTopAnim, isAnimCubeTop);
		break;
	case 'f':
		//����ü�� �ո��� ����/ �ո��� �ݴ´� (ȸ��)
		break;
	case 's':
		// ����ü�� ������ ����/ �ݴ´� (�����̵� �̵� ��ȯ)
		isAnimCubeSideUp = !isAnimCubeSideUp;
		isAnimCubeSide = true;
		if (isAnimCubeSide) glutTimerFunc(30, CubeSideAnim, isAnimCubeSideUp);
		break;
	case 'b':
		// ����ü�� �޸��� ����/ �ݴ´� (ũ�� Ŀ��/�۾���)
		break;
	// �簢�� �ִϸ��̼�
	case 'o':
		break;
	case 'r':
		break;
	case 'P':
	case 'p':
		projectionMode = !projectionMode;
		break;
	case 'C':
	case 'c':
		// ���� �ٲٱ�
		ObjMgr.m_ObjectList[1].m_isActive = !ObjMgr.m_ObjectList[1].m_isActive;
		ObjMgr.m_ObjectList[2].m_isActive = !ObjMgr.m_ObjectList[2].m_isActive;
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
