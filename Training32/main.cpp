#include <iostream>
#include "OpenGL.h"
#include <random>
#include "InitShader.h"
#include "ObjectManager.h"
#include "Timer.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
GLvoid Reset();
GLvoid drawScene(GLvoid);
GLvoid drawModel();
GLvoid drawProjection();
GLvoid drawView();
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[3], EBO;
GLuint texture;
bool isDepthTest = true;

GLvoid drawLight();
GLvoid DrawObjectByIDX(int DRAW_TYPE, glm::mat4& model, int idx);

// �ִϸ��̼� :: �ʱ�ȭ
GLvoid StopAllAnim();

bool isRotatingFigure = false;
GLvoid RotatingFigure(int idx);
int figure_idx = 0;

// �ִϸ��̼� :: ī�޶�
GLvoid RotatingCamera(int isAnim);
GLvoid RotatingCamera_X(int isAnim);

float radius = 0.5f;

// �ִϸ��̼� :: ����
GLvoid CheckCollision();
bool CheckCollisionByBox(float x, float y, float z);

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

glm::vec3 LightPos = glm::vec3(0.5f, 0.0f, 0.0f);
glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 LightVolume = glm::vec3(0.5f, 0.5f, 0.5f);

float roatingLightDir = 1.0f;
bool isShowCube = true;
bool rotatingLight = false;
bool rotatingCarmera = false;
bool rotatingCarmera_X = false;
bool rotatingCamera_z = false;
float rotatingCameraRate = 0.0f;
float rotatingCameraRate_x = 0.0f;
float rotatingCameraRate_y = 0.0f;

void Keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);

GLvoid SetCube(bool active);
GLvoid SetPyramid(bool active);

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
	glGenBuffers(3, VBO);
	glGenBuffers(1, &EBO);
	glGenTextures(2, &texture);

	Reset();

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
	cout << endl;
	cout << "c: ����ü" << endl;
	cout << "p: �簢��" << endl;
	cout << "x/X: x���� �߽����� ȸ��" << endl;
	cout << "y/Y: y���� �߽����� ȸ��" << endl;
	cout << endl;
	cout << "q: ���α׷� ����" << endl;
	cout << endl;
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(0, 0.1f, 0.1f, 0.1f);
	ObjMgr.SetPosition(0, 0.01f, 0.0f, 0.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::CUBE_02);
	ObjMgr.SetScale(1, 0.1f, 0.1f, 0.1f);
	ObjMgr.SetPosition(1, 0.0f, 0.0f, 0.01f);

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::CUBE_03);
	ObjMgr.SetScale(2, 0.1f, 0.1f, 0.1f);
	ObjMgr.SetPosition(2, 0.0f, 0.0f, -0.01f);

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::CUBE_04);
	ObjMgr.SetScale(3, 0.1f, 0.1f, 0.1f);
	ObjMgr.SetPosition(3, -0.01f, 0.0f, 0.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::CUBE_04);
	ObjMgr.SetScale(4, 0.1f, 0.1f, 0.09f);
	ObjMgr.SetPosition(4, 0.0f, 0.01f, 0.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::CUBE_04);
	ObjMgr.SetScale(5, 0.1f, 0.1f, 0.1f);
	ObjMgr.SetPosition(5, 0.0f, -0.01f, 0.0f);

	ObjMgr.CreateSquarePyramid(1.0f, 1.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(6, 0.03f, 0.03f, 0.03f);
	ObjMgr.SetPosition(6, 0.01f, 0.0f, 0.0f);

	ObjMgr.CreateSquarePyramid(1.0f, 1.0f, 1.0f, ObjectType::CUBE_02);
	ObjMgr.SetScale(7, 0.03f, 0.03f, 0.03f);
	ObjMgr.SetPosition(7, 0.0f, 0.0f, 0.01f);

	ObjMgr.CreateSquarePyramid(1.0f, 1.0f, 1.0f, ObjectType::CUBE_03);
	ObjMgr.SetScale(8, 0.03f, 0.03f, 0.03f);
	ObjMgr.SetPosition(8, 0.0f, 0.0f, -0.01f);

	ObjMgr.CreateSquarePyramid(1.0f, 1.0f, 1.0f, ObjectType::CUBE_04);
	ObjMgr.SetScale(9, 0.03f, 0.03f, 0.03f);
	ObjMgr.SetPosition(9, -0.01f, 0.0f, 0.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::CUBE_04);
	ObjMgr.SetScale(10, 0.16f, 0.001f, 0.16f);
	ObjMgr.SetPosition(10, 0.0f, -65.0f, 0.0f);

	SetCube(false);

	ObjMgr.CreateCube(1.0f, 1.0f, 1.0f, ObjectType::SKY);
	ObjMgr.SetScale(11, 0.01f, 5.0f, 5.0f);
	ObjMgr.SetPosition(11, -10.0f, 0.0f, 0.0f);

	CameraPos = glm::vec3(0.5f, 0.2f, 0.2f);
}

GLvoid drawScene()
{
	UpdateRender();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	if (isDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

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
	glUniform3f(lightColorLocation, LightColor.r, LightColor.g, LightColor.b);
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
	glViewport(0, 0, w, h);
}

GLvoid DrawObjectByIDX(int DRAW_TYPE, glm::mat4& model, int idx)
{
	// vPos
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, ObjMgr.m_ObjectList[idx].vertices.size() * sizeof(glm::vec3), &ObjMgr.m_ObjectList[idx].vertices[0], GL_STATIC_DRAW);

	// �𵨺�ȯ
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");
	ObjMgr.m_ObjectList[idx].m_model = ObjMgr.TransformModel(idx);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObjMgr.m_ObjectList[idx].m_model));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// vNormal
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, ObjMgr.m_ObjectList[idx].normals.size() * sizeof(glm::vec3), &ObjMgr.m_ObjectList[idx].normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// color
	unsigned int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, ObjMgr.m_ObjectList[idx].m_color[0], ObjMgr.m_ObjectList[idx].m_color[1], ObjMgr.m_ObjectList[idx].m_color[2]);

	// vTexture
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, ObjMgr.m_ObjectList[idx].uvs.size() * sizeof(glm::vec2), &ObjMgr.m_ObjectList[idx].uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	// Texture
	int widthImage, heightImage, numberOfChannel;
	// stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	unsigned char* data = stbi_load("white.png", &widthImage, &heightImage, &numberOfChannel, 0);

	if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::DEFAULT)
	{
		data = stbi_load("white.png", &widthImage, &heightImage, &numberOfChannel, 0);
	}
	else if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::CUBE_01 || ObjMgr.m_ObjectList[idx].m_type == ObjectType::PYRAMID_01)
	{
		data = stbi_load("Texture01.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	}
	else if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::CUBE_02 || ObjMgr.m_ObjectList[idx].m_type == ObjectType::PYRAMID_02)
	{
		data = stbi_load("Texture02.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	}
	else if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::CUBE_03 || ObjMgr.m_ObjectList[idx].m_type == ObjectType::PYRAMID_03)
	{
		data = stbi_load("Texture03.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	}
	else if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::CUBE_04 || ObjMgr.m_ObjectList[idx].m_type == ObjectType::PYRAMID_04)
	{
		data = stbi_load("Texture04.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	}
	else if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::CUBE_05 || ObjMgr.m_ObjectList[idx].m_type == ObjectType::PYRAMID_05)
	{
		data = stbi_load("Texture05.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	}
	else if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::CUBE_06)
	{
		data = stbi_load("Texture06.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	}
	else if (ObjMgr.m_ObjectList[idx].m_type == ObjectType::SKY)
	{
		data = stbi_load("sky.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	}

	unsigned int objTextureLocation = glGetUniformLocation(ShaderProgram, "outTexture");
	glUniform1i(objTextureLocation, 0);

	glActiveTexture(GL_TEXTURE0); //--- ���� 0�� Ȱ��ȭ
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glDrawArrays(DRAW_TYPE, 0, ObjMgr.m_ObjectList[idx].vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	stbi_image_free(data);
}

float angle_camera_x = 0;
float angle_camera = 0;
GLvoid StopAllAnim()
{
	angle_camera_x = 0;
	angle_camera = 0;
	rotatingCarmera = false;
	rotatingCarmera_X = false;
}

GLvoid RotatingFigure(int idx)
{
	ObjMgr.Rotate(idx, 0.0f, 0.5f, 0.0f);

	glutPostRedisplay();

	if (isRotatingFigure) glutTimerFunc(30, RotatingFigure, idx);
}

GLvoid RotatingCamera(int isAnim)
{
	float radius = 1.0f;
	CameraPos.x = sin(angle_camera) * radius;
	CameraPos.z = cos(angle_camera) * radius;

	angle_camera += 0.03f;

	glutPostRedisplay();

	if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
}

GLvoid RotatingCamera_X(int isAnim)
{
	float radius = 1.0f;
	CameraPos.y = sin(angle_camera_x) * radius;
	CameraPos.z = cos(angle_camera_x) * radius;

	angle_camera_x += 0.03f;

	glutPostRedisplay();

	if (rotatingCarmera_X) glutTimerFunc(30, RotatingCamera_X, rotatingCarmera_X);
}

float angle_light = 0;
GLvoid RotatingLight(int isAnim)
{
	LightPos.x = sin(angle_light) * radius * roatingLightDir;
	LightPos.z = cos(angle_light) * radius;

	angle_light += 0.03f;

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

GLvoid SetCube(bool active)
{
	for (int i = 0; i < 6; i++)
	{
		ObjMgr.SetActive(i, active);
	}
}

GLvoid SetPyramid(bool active)
{
	for (int i = 6; i < 10; i++)
	{
		ObjMgr.SetActive(i, active);
	}
}


void Keyboard(unsigned char key, int x, int y)
{
	float force_x, force_y, force_z;
	force_x = force_y = force_z = 0.f;
	bool input_w = false;
	switch (key)
	{
	case 'H':
	case 'h':
		if (isDepthTest) isDepthTest = false;
		else isDepthTest = true;
		break;
	case 'C':
	case 'c':
		SetCube(true);
		SetPyramid(false);
		break;
	case 'P':
	case 'p':
		SetCube(false);
		SetPyramid(true);
		break;
	case 'X':
	case 'x':
		StopAllAnim();
		if (rotatingCarmera_X) rotatingCarmera_X = false;
		else if (rotatingCarmera_X == false) { rotatingCarmera_X = true; }
		if (rotatingCarmera_X) glutTimerFunc(30, RotatingCamera_X, rotatingCarmera_X);
		break;
	case 'Y':
	case 'y':
		StopAllAnim();
		if (rotatingCarmera) rotatingCarmera = false;
		else if (rotatingCarmera == false) { rotatingCarmera = true; }
		if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
		break;
	case 'S':
	case 's':
		StopAllAnim();
		CameraPos = glm::vec3(0.5f, 0.2f, 0.2f);
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
