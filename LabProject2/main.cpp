#include <iostream>
#include "OpenGL.h"
#include <random>
#include "InitShader.h"
#include "ObjectManager.h"
#include "Timer.h"
#include <cmath>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define WIDTH 800
#define HEIGHT 600

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

// Ŭ����
ObjectManager ObjMgr;
Timer timer;

// Ÿ�̸�
float startTime = 0.0f;
float prevTime = 0.0f;
GLvoid UpdateRender();

GLvoid Init();
GLvoid Message();
GLvoid Message2();
GLvoid Reset();
GLvoid drawScene(GLvoid);
GLvoid drawModel();
GLvoid drawProjection();
GLvoid drawView();
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[2], EBO;

GLvoid drawLight();
GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, glm::mat4& model, int idx);

GLvoid CreateCubePosBuffer();

// �ִϸ��̼� :: �ʱ�ȭ
GLvoid StopAllAnim();
GLvoid UpSideDown(int idx);

bool isRotatingFigure = false;
GLvoid RotatingFigure(int idx);
int figure_idx = 0;

// �ִϸ��̼� :: ī�޶�
GLvoid RotatingCamera(int isAnim);

float radius = 0.5f;

// �ִϸ��̼� :: ����
GLvoid CheckCollision();
bool CheckCollisionByBox(float x, float y, float z);

// ť�� ����
int CUBE_WIDTH = 5;
int CUBE_HEIGHT = 5;
vector<float> WIDTH_POS;
vector<float> HEIGHT_POS;
float CUBE_OFFSET = 1.1f;


GLfloat rotateSpeed = 4.0f;
GLfloat moveSpeed = 0.5f;
bool isLight = true;

// pre ��ġ
GLfloat prevPivot[3];
bool isCollisionUp = false;

// ���� ��ȯ
bool projectionMode = true;

// Camera
GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 LightPos = glm::vec3(0.0f, 5.0f, 0.0f);
glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);

float roatingLightDir = 1.0f;
bool isShowCube = true;
bool rotatingLight = false;
bool rotatingCarmera = false;
bool rotatingCamera_z = false;
float rotatingCameraRate = 0.0f;
float rotatingCameraRate_x = 0.0f;
float rotatingCameraRate_y = 0.0f;

void Keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);

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
	glutKeyboardUpFunc(keyboardUp);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid Init()
{
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	startTime = glutGet(GLUT_ELAPSED_TIME); // ���� �ð� ����
}

// �ð��� ����Ͽ� elapsedTime ���
GLvoid UpdateRender()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	float elapsedTime = 0.0f;

	if (prevTime == 0.0f)
	{
		elapsedTime = currentTime - currentTime;
	}
	else
	{
		elapsedTime = currentTime - prevTime;
	}
	prevTime = currentTime;

	timer.SetTimer(elapsedTime);

	if (ObjMgr.m_ObjectList.size() >= 14)
	{
		//ObjMgr.UpdatePos(7, elapsedTime);
		//ObjMgr.Move(7, 0.0f, -0.1f, 0.0f);

		//CheckCollision();
	}
}

GLvoid Message()
{
	/*while (true)
	{
		system("cls");
		cout << "������ ť����� ���� ���θ� �Է��ϼ���(5~25)(5~25): ";
		cin >> CUBE_WIDTH >> CUBE_HEIGHT;

		if (CUBE_WIDTH >= 5 && CUBE_WIDTH <= 25 && CUBE_HEIGHT >= 5 && CUBE_HEIGHT <= 25)
		{
			break;
		}
	}*/
	//cout << endl;

	Message2();
	Reset();
}

GLvoid Message2()
{
	system("cls");
	cout << "1: �ִϸ��̼� 1" << endl;
	cout << "2: �ִϸ��̼� 2" << endl;
	cout << "3: �ִϸ��̼� 3" << endl;
	cout << endl;
	cout << "t: ������ �Ҵ�/����" << endl;
	cout << "c: ���� ���� �ٲ۴�" << endl;
	cout << "y/Y: ī�޶� �ٴ��� y���� �������� ��/�� �������� ȸ���Ѵ�" << endl;
	cout << "+/-: ����ü�� �̵��ϴ� �ӵ� ����/����" << endl;
	cout << endl;
	cout << "r: ��� �� �ʱ�ȭ (���Ӱ� ���μ��� ���� �Է� �޾� �ִϸ��̼��� �����Ѵ�)" << endl;
	cout << "q: ���α׷� ����" << endl;
	cout << endl;
}

GLvoid CreateCubePosBuffer()
{
	WIDTH_POS.clear();
	HEIGHT_POS.clear();

	// �Է°��� ���� ��ġ ���� ����
	float start_offset_WIDTH = 0.0f;
	float start_offset_HEIGHT = 0.0f;
	if (CUBE_WIDTH % 2 == 0) start_offset_WIDTH = (CUBE_OFFSET / 2.0f) + CUBE_OFFSET * (CUBE_WIDTH / 2 - 1);
	else start_offset_WIDTH = (CUBE_OFFSET) * (CUBE_WIDTH / 2);

	for (int i = 0; i < CUBE_WIDTH; i++)
	{
		WIDTH_POS.emplace_back(start_offset_WIDTH);
		start_offset_WIDTH -= CUBE_OFFSET;
	}

	if (CUBE_HEIGHT % 2 == 0) start_offset_HEIGHT = (CUBE_OFFSET / 2.0f) + CUBE_OFFSET * (CUBE_HEIGHT / 2 - 1);
	else start_offset_HEIGHT = (CUBE_OFFSET) * (CUBE_HEIGHT / 2);

	for (int i = 0; i < CUBE_HEIGHT; i++)
	{
		HEIGHT_POS.emplace_back(start_offset_HEIGHT);
		start_offset_HEIGHT -= CUBE_OFFSET;
	}
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	CreateCubePosBuffer();

	// ����
	ObjMgr.CreateSqhere(1.0f, 1.0f, 0.0f);
	ObjMgr.SetPosition(0, LightPos.x, LightPos.y, LightPos.z);

	// ť�� ��
	int idx = 1;
	for (int i = 0; i < CUBE_WIDTH; i++)
	{
		for (int j = 0; j < CUBE_HEIGHT; j++)
		{
			float pos_X = WIDTH_POS[i];
			float pos_Z = HEIGHT_POS[j];

			ObjMgr.CreateCube(1.0f, 1.0f, 1.0f);
			ObjMgr.SetScale(idx, 0.05f, 0.1f, 0.05f);
			ObjMgr.SetPosition(idx, pos_X, 0.0f, pos_Z);
			idx += 1;
		}
	}
}

GLvoid drawScene()
{
	UpdateRender();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, WIDTH, HEIGHT);

	CameraPos = glm::vec3(1.5f, 2.0f, 0.0f);
	projectionMode = true;

	drawView();
	drawProjection();
	drawLight();
	drawModel();

	glViewport(WIDTH / 4, HEIGHT / 4, WIDTH, HEIGHT);

	CameraPos = glm::vec3(-0.1f, 10.0f, 0.0f);
	projectionMode = false;

	drawView();
	drawProjection();
	drawLight();
	drawModel();

	glutSwapBuffers();
}

GLvoid drawView()
{
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "viewTransform");

	glm::vec3 cameraPos = glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z); //--- ī�޶� ��ġ
	glm::vec3 cameraDirection = glm::vec3(AT.x, AT.y, AT.z); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// �亯ȯ
}

GLvoid drawProjection()
{
	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projectionTransform");

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
}

GLvoid drawLight()
{
	unsigned int lightPosLocation = glGetUniformLocation(ShaderProgram, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, LightPos.x, LightPos.y, LightPos.z);
	unsigned int lightColorLocation = glGetUniformLocation(ShaderProgram, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	if (isLight) glUniform3f(lightColorLocation, LightColor.r, LightColor.g, LightColor.b);
	else glUniform3f(lightColorLocation, 0.1, 0.1, 0.1);
	unsigned int viewPosLocation = glGetUniformLocation(ShaderProgram, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, 0.0f, 0.0f, 0.0f);
}

GLvoid drawModel()
{
	for (int i = 0; i < ObjMgr.m_ObjectList.size(); i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			DrawObjectByIDX(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_model, i);
		}
	}
}

GLvoid Reshape(int w, int h)
{
	// glViewport(0, 0, w, h);
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

GLvoid DrawObjectByIDX(int DRAW_TYPE, glm::mat4& model, int idx)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, ObjMgr.m_ObjectList[idx].vertices.size() * sizeof(glm::vec3), &ObjMgr.m_ObjectList[idx].vertices[0], GL_STATIC_DRAW);

	// �𵨺�ȯ
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");
	ObjMgr.m_ObjectList[idx].m_model = ObjMgr.TransformModel(idx);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObjMgr.m_ObjectList[idx].m_model));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, ObjMgr.m_ObjectList[idx].normals.size() * sizeof(glm::vec3), &ObjMgr.m_ObjectList[idx].normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	unsigned int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor"); //--- object Color�� ���� 
	glUniform3f(objColorLocation, ObjMgr.m_ObjectList[idx].m_color[0], ObjMgr.m_ObjectList[idx].m_color[1], ObjMgr.m_ObjectList[idx].m_color[2]);


	glDrawArrays(DRAW_TYPE, 0, ObjMgr.m_ObjectList[idx].vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid StopAllAnim()
{
	rotatingCarmera = false;
	rotatingLight = false;

	for (int i = 0; i < ObjMgr.m_ObjectList.size(); i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			for (int j = 0; j < ObjMgr.m_ObjectList[i].vertices.size(); j++)
			{
				if (ObjMgr.m_ObjectList[i].vertices[j].y > 0.0f)
				{
					ObjMgr.m_ObjectList[i].vertices[j].y = 0.5f;
				}
				ObjMgr.m_ObjectList[i].m_isUpSideDown = false;
				ObjMgr.m_ObjectList[i].m_isWaving = false;
			}
		}
	}
}

GLvoid UpSideDown(int idx)
{
	for (int i = 0; i < ObjMgr.m_ObjectList[idx].vertices.size(); i++)
	{
		if (ObjMgr.m_ObjectList[idx].vertices[i].y > 0.0f)
		{
			if (ObjMgr.m_ObjectList[idx].vertices[i].y >= 10.0f)
			{
				ObjMgr.m_ObjectList[idx].m_move_speed = -0.1f;
			}

			if (ObjMgr.m_ObjectList[idx].vertices[i].y <= 0.5f)
			{
				ObjMgr.m_ObjectList[idx].m_move_speed = 0.1f;
			}

			ObjMgr.m_ObjectList[idx].vertices[i].y += ObjMgr.m_ObjectList[idx].m_move_speed;
		}
	}

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isUpSideDown) glutTimerFunc(30, UpSideDown, idx);
}

GLvoid RotatingFigure(int idx)
{
	ObjMgr.Rotate(idx, 0.0f, 0.5f, 0.0f);

	glutPostRedisplay();

	if (isRotatingFigure) glutTimerFunc(30, RotatingFigure, idx);
}

float angle_camera = 0;
GLvoid RotatingCamera(int isAnim)
{
	float radius = 1.0f;
	CameraPos.x = sin(angle_camera) * radius;
	CameraPos.z = cos(angle_camera) * radius;

	angle_camera += 0.03f;

	glutPostRedisplay();

	if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
}

float angle_light = 0;
GLvoid RotatingLight(int isAnim)
{
	LightPos.y = sin(angle_light) * radius * roatingLightDir;
	LightPos.z = cos(angle_light) * radius;

	angle_light += 0.03f;

	ObjMgr.SetPosition(0, LightPos.x, LightPos.y, LightPos.z);

	glutPostRedisplay();


	if (rotatingLight) glutTimerFunc(30, RotatingLight, rotatingLight);
}

GLvoid ChangeLightRandomColor()
{
	float r = ObjMgr.GetRandomFloatValue(0.0f, 1.0f);
	float g = ObjMgr.GetRandomFloatValue(0.0f, 1.0f);
	float b = ObjMgr.GetRandomFloatValue(0.0f, 1.0f);
	LightColor.r = r;
	LightColor.g = g;
	LightColor.b = b;
}


void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		for (int i = 0; i < ObjMgr.m_ObjectList.size(); i++)
		{
			if (ObjMgr.m_ObjectList[i].m_isActive)
			{
				ObjMgr.m_ObjectList[i].m_isUpSideDown = true;
				if (ObjMgr.m_ObjectList[i].m_isUpSideDown) glutTimerFunc(30, UpSideDown, i);
			}
		}
		break;
	case '2':
		break;
	case '3':
		break;
	case 'Y':
	case 'y':
		break;
	case '+':
		break;
	case '-':
		break;
	// ���� �ѱ�/����
	case 'T':
	case 't':
		if (isLight) isLight = false;
		else isLight = true;
		break;
	case 'C':
	case 'c':
		ChangeLightRandomColor();
		break;
	case 'r':
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	default:
		break;
	}

	glutPostRedisplay();
}
