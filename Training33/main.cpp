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
GLvoid drawModel2();
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

// �ִϸ��̼� :: ��ֹ�
GLvoid BoxFalling(int idx);
bool isFalling = false;

// �ִϸ��̼� :: �κ�
GLvoid RotateRobotByDir(int isAnim);
GLvoid JumpRobobot(int isAnim);
GLvoid JumpChildRobot(int isAnim);

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
bool isRotatingDoor = false;
bool isRotatingRobot = false;
bool isRobotJumping = false;
bool isChildRobotJumping = false;

// �κ� ����
GLfloat playerSpeed = 0.2f;
GLfloat playerRotateArmSpeed = 5.0f;
GLfloat playerRotateLegSpeed = 3.0f;
GLfloat playerJumpSpeed = 0.5f;
GLfloat playerJumpRate = 0.0f;
GLfloat childJumpSpeed = 0.2f;
GLfloat childJumpRate = 0.0f;
float forceAmount = 10.0f;

GLfloat rotateSpeed = 4.0f;
GLfloat moveSpeed = 0.5f;
GLfloat doorRotateRate = 0.0f;
GLfloat armRotateRate = 0.0f;

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

bool isChildGenerate = false;

// ���� 
float boxHeight[3] = { -1.0f, -1.0f, -1.0f };
int currentBoxIDX = 0;

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
	cout << "h/H : ���� ����" << endl;
	cout << "p/P : ���� ����/ ���� ����" << endl;
	cout << endl;
	cout << "w/a/s/d: �κ��� ��/��/��/�� �������� �̵� ������ �ٲ㼭 �ȴ´�. �����ڸ��� �����ϸ� �κ��� �ڷ� ���� �ٽ� �ȴ´�." << endl;
	cout << "+/-: �ȴ� �ӵ��� �������ų�/��������. �ӵ��� �ٲ� �� �ȴ� �ٸ��� ������ �þ�ų�/�پ���." << endl;
	cout << "j: �κ��� ���ڸ����� �����Ѵ�. ��ֹ� ���� �����Ͽ� �ö󰡰� ������ �� �ִ�." << endl;
	cout << endl;
	cout << "t: ���� �κ� ����/����" << endl;
	cout << "f: ���� �κ��� �����Ѵ�." << endl;
	cout << endl;
	cout << "i: ��� ��ȯ�� �����ϰ� �ٽ� ����" << endl;
	cout << "Q: ���α׷� �����Ѵ�." << endl;
	cout << endl;
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	// ��� ����(ť�� , ��ֹ�)
	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(0, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(0, 0.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(1, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(1, 0.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(2, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(2, 1.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(3, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(3, -1.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(4, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(4, -2.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(5, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(5, 0.0f, -15.0f, 1.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(6, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(6, 0.0f, -15.0f, 2.0f);

	// �κ� ��ü(�ְ� �θ�)
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f, 1.0f, ObjectType::CUBE_02);
	ObjMgr.SetScale(7, 0.03f, 0.04f, 0.03f);
	ObjMgr.SetPosition(7, -1.5f, -2.0f, 0.0f);

	// �κ� �Ӹ�
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f, 1.0f, ObjectType::CUBE_03);
	ObjMgr.SetScale(8, 0.8f, 1.0f, 0.8f);
	ObjMgr.SetPosition(8, 0.0f, 0.5f, 0.0f);

	// �κ� ��
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(9, 0.45f, 0.25f, 0.3f);
	ObjMgr.SetPosition(9, 0.3f, 0.8f, 0.0f);

	// �κ� ������ ��
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(10, 0.4f, 1.0f, 0.3f);
	ObjMgr.SetPosition(10, 0.0f, -0.1f, 0.6f);

	// �κ� ���� ��
	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_02);
	ObjMgr.SetScale(11, 0.4f, 1.0f, 0.3f);
	ObjMgr.SetPosition(11, 0.0f, -0.1f, -0.6f);

	// �κ� ������ �ٸ�
	ObjMgr.CreateCube(138.0f / 255.0f, 43.0f / 255.0f, 226.0f / 255.0f, 1.0f, ObjectType::CUBE_02);
	ObjMgr.SetScale(12, 0.6f, 1.0f, 0.5f);
	ObjMgr.SetPosition(12, 0.0f, -1.0f, 0.2f);

	// �κ� ���� �ٸ�
	ObjMgr.CreateCube(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(13, 0.6f, 1.0f, 0.5f);
	ObjMgr.SetPosition(13, 0.0f, -1.0f, -0.2f);

	// �κ� CollisionBox
	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(14, 1.5f, 2.4f, 1.5f);
	ObjMgr.SetPosition(14, 0.0f, -0.2f, 0.0f);
	ObjMgr.SetWireSolidType(14, GL_LINE_LOOP);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(15, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(15, 0.0f, -15.0f, 3.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(16, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(16, 0.0f, -15.0f, -1.0f);

	int idx = 17;
	float dist = 3.0f;
	for (int i = 0; i < 7; i++)
	{
		if (i % 2 == 0)
		{
			ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
		}
		else
		{
			ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
		}
		ObjMgr.SetScale(idx, 0.1f, 0.01f, 0.1f);
		ObjMgr.SetPosition(idx, 1.0f, -15.0f, dist);
		idx += 1;
		dist -= 1.0f;
	}
	idx = 24;
	dist = 3.0f;

	for (int i = 0; i < 7; i++)
	{
		if (i % 2 == 0)
		{
			ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
		}
		else
		{
			ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
		}
		ObjMgr.SetScale(idx, 0.1f, 0.01f, 0.1f);
		ObjMgr.SetPosition(idx, -1.0f, -15.0f, dist);
		idx += 1;
		dist -= 1.0f;
	}

	idx = 31;
	dist = 3.0f;
	for (int i = 0; i < 7; i++)
	{
		if (i % 2 == 1)
		{
			ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
		}
		else
		{
			ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
		}
		ObjMgr.SetScale(idx, 0.1f, 0.01f, 0.1f);
		ObjMgr.SetPosition(idx, -2.0f, -15.0f, dist);
		idx += 1;
		dist -= 1.0f;
	}

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(38, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(38, 0.0f, -15.0f, -3.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f, 1.0f, ObjectType::CUBE_01);
	ObjMgr.SetScale(39, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(39, 0.0f, -15.0f, -2.0f);

	// �ڽ� �κ� 

	// �ڽ� �κ� ��ü(�ְ� �θ�)
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(40, 0.8f, 0.7f, 0.8f);
	ObjMgr.SetPosition(40, -2.5f, -0.5f, 0.0f);

	// �ڽ� �κ� �Ӹ�
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(41, 0.6f, 0.5f, 0.6f);
	ObjMgr.SetPosition(41, -0.0f, 0.7f, 0.0f);

	// �ڽ� �κ� ��
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(42, 0.3f, 0.2f, 0.3f);
	ObjMgr.SetPosition(42, 0.3f, 0.7f, 0.0f);

	// �ڽ� �κ� ������ ��
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(43, 1.0f, 0.4f, 0.3f);
	ObjMgr.SetPosition(43, -0.4f, 0.3f, 0.6f);

	// �ڽ� �κ� ���� ��
	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(44, 1.0f, 0.4f, 0.3f);
	ObjMgr.SetPosition(44, -0.4f, 0.3f, -0.6f);

	// �ڽ� �κ� ������ �ٸ�
	ObjMgr.CreateCube(138.0f / 255.0f, 43.0f / 255.0f, 226.0f / 255.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(45, 0.4f, 1.2f, 0.3f);
	ObjMgr.SetPosition(45, -0.0f, -1.0f, 0.3f);

	// �ڽ� �κ� ���� �ٸ�
	ObjMgr.CreateCube(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f, ObjectType::DEFAULT);
	ObjMgr.SetScale(46, 0.4f, 1.2f, 0.3f);
	ObjMgr.SetPosition(46, -0.0f, -1.0f, -0.3f);

	for (int i = 40; i < 47; i++)
	{
		ObjMgr.SetActive(i, false);
	}

	// ��ֹ�

	// ���� ��ֹ�
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f, 0.5f, ObjectType::DEFAULT);
	ObjMgr.SetScale(47, 0.07f, 0.04f, 0.1f);
	ObjMgr.SetPosition(47, -1.5f, -3.0f, 1.5f);

	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f, 0.5f, ObjectType::DEFAULT);
	ObjMgr.SetScale(48, 0.07f, 0.04f, 0.1f);
	ObjMgr.SetPosition(48, 1.0f, -3.0f, -1.5f);

	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f, 0.5f, ObjectType::DEFAULT);
	ObjMgr.SetScale(49, 0.07f, 0.04f, 0.1f);
	ObjMgr.SetPosition(49, 0.5f, -3.0f, 1.0f);

	// U�� ��ֹ�
	ObjMgr.CreateCube(0.0f, 1.0f, 1.0f, 0.5f, ObjectType::DEFAULT);
	ObjMgr.SetScale(50, 0.03f, 0.12f, 0.03f);
	ObjMgr.SetPosition(50, -3.0f, -0.8f, -9.0f);

	ObjMgr.CreateCube(0.0f, 1.0f, 1.0f, 0.5f, ObjectType::DEFAULT);
	ObjMgr.SetScale(51, 0.03f, 0.12f, 0.03f);
	ObjMgr.SetPosition(51, -3.0f, -0.8f, -6.0f);

	ObjMgr.CreateCube(0.0f, 1.0f, 1.0f, 0.5f, ObjectType::DEFAULT);
	ObjMgr.SetScale(52, 0.03f, 0.03f, 0.12f);
	ObjMgr.SetPosition(52, -3.0f, -0.8f, -1.87f);

	CameraPos = glm::vec3(0.6f, 0.1f, 0.0f);
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

	//--- ���� ��ü �׸���
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawModel2();

	//--- ���� ����
	glDisable(GL_BLEND);

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
	for (int i = 0; i < 47; i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			DrawObjectByIDX(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_model, i);
		}
	}
}

GLvoid drawModel2()
{
	for (int i = 47; i < ObjMgr.m_ObjectList.size(); i++)
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

	unsigned int objAlphaLocation = glGetUniformLocation(ShaderProgram, "alpha"); //--- object Alpha�� ����
	glUniform1f(objAlphaLocation, ObjMgr.m_ObjectList[idx].m_color[3]);

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

GLvoid StopAllAnim()
{
	rotatingCarmera = false;
	isRobotJumping = false;
	isRotatingRobot = false;

	rotateSpeed = 4.0f;
	doorRotateRate = 0.0f;
}

GLvoid BoxFalling(int idx)
{
	isFalling = true;

	int playerIDX = 7;
	int boxIDX = idx - 50;

	if (boxHeight[boxIDX] <= -2.0f)
	{
		isFalling = false;
		return;
	}
	else
	{
		float fallingSpeed = 0.05f;
		ObjMgr.Move(idx, 0.0f, -fallingSpeed, 0.0f);

		// �÷��̾ �Բ� Falling
		boxHeight[boxIDX] -= 0.02f;
	}

	glutPostRedisplay();

	if (isFalling) glutTimerFunc(30, BoxFalling, idx);
}

// �κ� ��, �ٸ� ȸ��
GLvoid RotateRobotByDir(int isAnim)
{
	int idx1 = 10;
	int idx2 = 11;
	int idx3 = 12;
	int idx4 = 13;

	int idx5 = 43;
	int idx6 = 44;
	int idx7 = 45;
	int idx8 = 46;

	ObjMgr.Rotate(idx1, 0.0f, 0.0f, playerRotateArmSpeed);
	ObjMgr.Rotate(idx2, 0.0f, 0.0f, -playerRotateArmSpeed);
	ObjMgr.Rotate(idx3, 0.0f, 0.0f, -playerRotateLegSpeed);
	ObjMgr.Rotate(idx4, 0.0f, 0.0f, playerRotateLegSpeed);

	ObjMgr.Rotate(idx5, 0.0f, 0.0f, playerRotateArmSpeed);
	ObjMgr.Rotate(idx6, 0.0f, 0.0f, -playerRotateArmSpeed);
	ObjMgr.Rotate(idx7, 0.0f, 0.0f, -playerRotateArmSpeed);
	ObjMgr.Rotate(idx8, 0.0f, 0.0f, playerRotateArmSpeed);


	if (armRotateRate > 10)
	{
		playerRotateArmSpeed = playerRotateArmSpeed * (-1.0f);
		playerRotateLegSpeed = playerRotateLegSpeed * (-1.0f);
		armRotateRate = 0.0f;
	}

	armRotateRate += 1;

	glutPostRedisplay();

	if (isRotatingRobot) glutTimerFunc(30, RotateRobotByDir, isRotatingRobot);
}

// �κ� ����
GLvoid JumpRobobot(int isAnim)
{
	int idx = 7;

	if (playerJumpRate > 10.0f)
	{
		playerJumpSpeed = -0.2f;
		if (playerJumpRate > 20.0f)
		{
			playerJumpRate = 0.0f;
			isRobotJumping = false;
		}
	}

	playerJumpRate += 1;

	ObjMgr.Move(idx, 0.0f, playerJumpSpeed, 0.0f);

	glutPostRedisplay();

	if (isRobotJumping) glutTimerFunc(30, JumpRobobot, isRobotJumping);
}

// �ڽ� �κ� ����
GLvoid JumpChildRobot(int isAnim)
{
	int idx = 40;

	if (childJumpRate > 10.0f)
	{
		childJumpSpeed = -0.09f;
		if (childJumpRate > 20.0f)
		{
			childJumpRate = 0.0f;
			isChildRobotJumping = false;
		}
	}

	childJumpRate += 1;

	ObjMgr.Move(idx, 0.0f, childJumpSpeed, 0.0f);

	glutPostRedisplay();

	if (isChildRobotJumping) glutTimerFunc(30, JumpChildRobot, isChildRobotJumping);
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

GLvoid CheckCollision()
{
	int idx = 7;
	float maxDist_x = 5.0f;
	float minDist_x = -8.0f;

	float maxDist_z = 10.0f;
	float minDist_z = -10.0f;

	// ���� �������� �ʰ� ����
	// ��ֹ� �Ǵܿ� �°� ����

	if (isCollisionUp)
	{
		ObjMgr.m_ObjectList[idx].m_pivot[1] = boxHeight[currentBoxIDX];
	}
	else
	{
		if (ObjMgr.m_ObjectList[idx].m_pivot[1] < -2.0f)
		{
			ObjMgr.m_ObjectList[idx].m_pivot[1] = -2.0f;
		}
	}


	// ���� ������ �ݴ�������� �̵�
	if (ObjMgr.m_ObjectList[idx].m_pivot[0] > maxDist_x)
	{
		ObjMgr.Move(7, -playerSpeed * 5, 0.0f, 0.0f);
		ObjMgr.SetRotate(7, 0.0f, 0.0f, 0.0f);
	}

	if (ObjMgr.m_ObjectList[idx].m_pivot[0] < minDist_x)
	{
		ObjMgr.Move(7, playerSpeed * 5, 0.0f, 0.0f);
		ObjMgr.SetRotate(7, 0.0f, 0.0f, 0.0f);
	}

	if (ObjMgr.m_ObjectList[idx].m_pivot[2] > maxDist_z)
	{
		ObjMgr.Move(7, 0.0f, 0.0f, -playerSpeed * 5);
		ObjMgr.SetRotate(7, 0.0f, 90.0f, 0.0f);
	}

	if (ObjMgr.m_ObjectList[idx].m_pivot[2] < minDist_z)
	{
		ObjMgr.Move(7, 0.0f, 0.0f, playerSpeed * 5);
		ObjMgr.SetRotate(7, 0.0f, -90.0f, 0.0f);
	}



	// ��ֹ� �浹ó��(CollsionBox)
	// ��ֹ� 1
	//float min_z = ObjMgr.m_ObjectList[16].m_model[3][2] - 0.057;
	//float max_z = ObjMgr.m_ObjectList[16].m_model[3][2] + 0.057;
	//float min_x = ObjMgr.m_ObjectList[16].m_model[3][0] - 0.057;
	//float max_x = ObjMgr.m_ObjectList[16].m_model[3][0] + 0.057;
	//
	//float player_x = ObjMgr.m_ObjectList[14].m_model[3][0];
	//float player_z = ObjMgr.m_ObjectList[14].m_model[3][2];

	//if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z)
	//{
	//	cout << max_z << ", " << min_z << endl;
	//	cout << "��ֹ� �浹!" << endl;

	//	// ���� ��ġ�� ����
	//	ObjMgr.m_ObjectList[7].m_pivot[0] = prevPivot[0];
	//	ObjMgr.m_ObjectList[7].m_pivot[1] = prevPivot[1];
	//	ObjMgr.m_ObjectList[7].m_pivot[2] = prevPivot[2];
	//}
	//else
	//{
	//	prevPivot[0] = ObjMgr.m_ObjectList[7].m_pivot[0];
	//	prevPivot[1] = ObjMgr.m_ObjectList[7].m_pivot[1];
	//	prevPivot[2] = ObjMgr.m_ObjectList[7].m_pivot[2];

	//}
}

bool CheckCollisionByBox(float x, float y, float z)
{
	bool isCollision = false;
	int playerIDX = 7;

	float offset = 0.057;

	isCollisionUp = false;

	// ��ֹ� �浹ó��(CollsionBox)

	// ��ֹ� 1
	int idx1 = 44;
	float min_z = -0.204f;
	float max_z = -0.144f;
	float min_x = -0.117f;
	float max_x = -0.033f;

	float max_y = ObjMgr.m_ObjectList[16].m_model[3][1] + 0.042;

	// Move �ϱ� �� �浹 üũ

	float player_x = ObjMgr.m_ObjectList[14].m_model[3][0] + x;
	float player_y = ObjMgr.m_ObjectList[14].m_model[3][1] + y;
	float player_z = ObjMgr.m_ObjectList[14].m_model[3][2] + z;

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z)
	{
		isCollision = true;
	}

	max_z = -0.234f;
	min_z = -0.3f;

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z)
	{
		isCollision = true;
	}

	// ��ֹ� 2
	int box1_idx = 50;
	int box2_idx = 52;
	int box3_idx = 51;
	min_x = -0.147f;
	max_x = -0.045f;
	min_z = 0.078f;
	max_z = 0.216f;

	max_y = -0.06f;

	// Move �ϱ� �� �浹 üũ

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y <= max_y)
	{
		if (boxHeight[0] > -2.0f) isCollision = true;
	}

	// �����ؼ� ������ ��ġ ����
	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y > max_y)
	{
		currentBoxIDX = 0;
		isCollisionUp = true;
		glutTimerFunc(1000, BoxFalling, box1_idx);
	}

	//// ��ֹ� 2
	min_x = -0.01f;
	max_x = 0.093f;
	min_z = 0.03f;
	max_z = 0.168f;

	max_y = -0.06f;

	// Move �ϱ� �� �浹 üũ

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y <= max_y)
	{
		if (boxHeight[2] > -2.0f) isCollision = true;
	}

	// �����ؼ� ������ ��ġ ����
	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y > max_y)
	{
		currentBoxIDX = 2;
		isCollisionUp = true;
		glutTimerFunc(1000, BoxFalling, box2_idx);
	}

	// ��ֹ�3
	min_x = 0.021f;
	max_x = 0.129f;
	min_z = -0.216f;
	max_z = -0.078f;

	max_y = -0.06f;

	// Move �ϱ� �� �浹 üũ

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y <= max_y)
	{
		if (boxHeight[1] > -2.0f) isCollision = true;
	}

	// �����ؼ� ������ ��ġ ����
	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y > max_y)
	{
		currentBoxIDX = 1;
		isCollisionUp = true;
		glutTimerFunc(1000, BoxFalling, box3_idx);
	}

	// ����� ��ֹ�

	min_x = -0.159f;
	min_z = 0.258f;

	// Move �ϱ� �� �浹 üũ

	if (player_x <= min_x && player_z >= min_z)
	{
		isCollision = true;
	}

	max_x = 0.081f;
	min_z = 0.264f;

	// Move �ϱ� �� �浹 üũ

	if (player_x >= max_x && player_z >= min_z)
	{
		isCollision = true;
	}

	max_x = 0.081f;
	max_z = -0.234f;

	if (player_x >= max_x && player_z <= max_z)
	{
		isCollision = true;
	}

	min_x = -0.147f;
	max_z = -0.234f;

	if (player_x <= min_x && player_z <= max_z)
	{
		isCollision = true;
	}

	return isCollision;
}

GLvoid SetActiveChildRobot()
{
	if (isChildGenerate == false)
	{
		isChildGenerate = true;
	}
	else
	{
		isChildGenerate = false;
	}

	for (int i = 40; i < 47; i++)
	{
		ObjMgr.SetActive(i, isChildGenerate);
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
		// �ִϸ��̼� :: �κ�
	case 'w':
		ObjMgr.SetRotate(7, 0.0f, 180.0f, 0.0f);
		isRotatingRobot = true;
		if (isRotatingRobot)glutTimerFunc(30, RotateRobotByDir, isRotatingRobot);
		break;
	case 's':
		ObjMgr.SetRotate(7, 0.0f, 0.0f, 0.0f);
		isRotatingRobot = true;
		if (isRotatingRobot)glutTimerFunc(30, RotateRobotByDir, isRotatingRobot);
		break;
	case 'a':
		ObjMgr.SetRotate(7, 0.0f, -90.0f, 0.0f);
		isRotatingRobot = true;
		if (isRotatingRobot)glutTimerFunc(30, RotateRobotByDir, isRotatingRobot);
		break;
	case 'd':
		ObjMgr.SetRotate(7, 0.0f, 90.0f, 0.0f);
		isRotatingRobot = true;
		if (isRotatingRobot)glutTimerFunc(30, RotateRobotByDir, isRotatingRobot);
		break;
	case 'J':
	case 'j':
		isRobotJumping = true;
		playerJumpSpeed = 0.3f;
		if (isRobotJumping)glutTimerFunc(30, JumpRobobot, isRobotJumping);
		break;
	case 'T':
	case 't':
		SetActiveChildRobot();
		break;
	case 'F':
	case 'f':
		isChildRobotJumping = true;
		childJumpSpeed = 0.1f;
		if (isChildRobotJumping)glutTimerFunc(30, JumpChildRobot, isChildRobotJumping);
		break;
	case '+':
		playerSpeed += 0.1f;
		playerRotateArmSpeed += 0.1f;
		playerRotateLegSpeed += 0.1f;
		break;
	case '-':
		playerSpeed -= 0.1f;
		playerRotateArmSpeed -= 0.1f;
		playerRotateLegSpeed -= 0.1f;
		break;
		// �ִϸ��̼� :: ī�޶�
	case 'X':
		CameraPos.x += 0.1f;
		break;
	case 'x':
		CameraPos.x -= 0.1f;
		break;
	case 'Z':
		CameraPos.z += 0.1f;
		break;
	case 'z':
		CameraPos.z -= 0.1f;
		break;
	case 'Y':
	case 'y':
		if (rotatingCarmera) rotatingCarmera = false;
		else if (rotatingCarmera == false) rotatingCarmera = true;
		if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

	float move_speed = 0.006f;
	if (key == 'w')
	{
		if (CheckCollisionByBox(-move_speed, 0.0f, 0.0f) == false)
		{
			ObjMgr.Move(7, -playerSpeed, 0.0f, 0.0f);
		}
	}
	if (key == 's')
	{
		if (CheckCollisionByBox(move_speed, 0.0f, 0.0f) == false)
		{
			ObjMgr.Move(7, playerSpeed, 0.0f, 0.0f);
		}
	}
	if (key == 'a')
	{
		if (CheckCollisionByBox(0.0f, 0.0f, move_speed) == false)
		{
			ObjMgr.Move(7, 0.0f, 0.0f, playerSpeed);
		}
	}
	if (key == 'd')
	{
		if (CheckCollisionByBox(0.0f, 0.0f, -move_speed) == false)
		{
			ObjMgr.Move(7, 0.0f, 0.0f, -playerSpeed);
		}
	}

	glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 's':
	case 'a':
	case 'd':
		isRotatingRobot = false;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}
