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

// 클래스
ObjectManager ObjMgr;
Timer timer;

// 타이머
float startTime = 0.0f;
float prevTime = 0.0f;
GLvoid UpdateRender();

GLvoid Init();
GLvoid Message();
GLvoid Reset();
GLvoid drawScene(GLvoid);
GLvoid drawModel();
GLvoid drawModelAlpha();
GLvoid drawProjection();
GLvoid drawView();
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[2], EBO;
bool isDepthTest = true;

GLvoid drawLight();
GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, glm::mat4& model, int idx);

// 애니메이션 :: 초기화
GLvoid StopAllAnim();

bool isRotatingFigure = false;
GLvoid RotatingFigure(int idx);
int figure_idx = 0;

// 애니메이션 :: 카메라
GLvoid RotatingCamera(int isAnim);

// 애니메이션 :: 행성
GLvoid RotatingStar(int idx);

// 애니메이션 :: 눈 내리기
GLvoid SnowAnim(int idx);
bool snow_on = true;

float radius = 0.5f;

// 애니메이션 :: 변수
GLvoid CheckCollision();
bool CheckCollisionByBox(float x, float y, float z);

bool isLight = true;

// pre 위치
GLfloat prevPivot[3];
bool isCollisionUp = false;

// 투영 변환
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

	Reset();

	startTime = glutGet(GLUT_ELAPSED_TIME); // 시작 시간 설정
}

// 시간을 계산하여 elapsedTime 계산
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
	cout << "s: 하늘에서 눈이 내린다/멈춘다" << endl;
	cout << "0~5: 시어핀스키 프렉탈 삼각형의 단계" << endl;
	cout << endl;
	cout << "r: 조명이 화면 중앙의 y축에 대하여 공전한다 / 멈춘다" << endl;
	cout << "n/f: 조명이 가까워지기 / 멀어지기" << endl;
	cout << "+ / -: 조명의 세기 높아지기 / 낮춰지기" << endl;
	cout << endl;
	cout << "q: 프로그램 종료" << endl;
	cout << endl;
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(0, 0.5f, 0.03f, 0.5f);

	ObjMgr.CreateSquarePyramid(1.0f, 0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(1, 0.03f, 0.03f, 0.03f);

	ObjMgr.CreateSqhere(1.0f, 0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(2, 0.01f, 0.01f, 0.01f);
	ObjMgr.SetPosition(2, 0.0f, 15.0f, 0.0f);

	ObjMgr.CreateSqhere(0.0f, 1.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(3, 0.01f, 0.01f, 0.01f);
	ObjMgr.SetPosition(3, 0.0f, 15.0f, 0.0f);

	ObjMgr.CreateSqhere(0.0f, 0.0f, 1.0f, 1.0f);
	ObjMgr.SetScale(4, 0.01f, 0.01f, 0.01f);
	ObjMgr.SetPosition(4, 0.0f, 15.0f, 0.0f);

	// 눈
	int idx = 5;
	for (int i = 0; i < 40; i++)
	{
		ObjMgr.CreateSqhere(1.0f, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetScale(idx, 0.001f, 0.001f, 0.001f);

		float rand_x = ObjMgr.GetRandomFloatValue(-200.0f, 200.0f);
		float rand_z = ObjMgr.GetRandomFloatValue(-200.0f, 200.0f);

		ObjMgr.SetPosition(idx, rand_x, 200.0f, rand_z);

		ObjMgr.m_ObjectList[idx].m_snow_speed = ObjMgr.GetRandomFloatValue(1.0f, 5.0f);

		glutTimerFunc(30, SnowAnim, idx);

		idx++;
	}

	ObjMgr.CreateCube(139.0f/255.0f, 0.0f, 1.0f, 0.5f);
	ObjMgr.SetScale(45, 0.05f, 0.15f, 0.05f);
	ObjMgr.SetPosition(45, 0.0f, 0.0f, 2.0f);

	ObjMgr.CreateCube(139.0f / 255.0f, 0.0f, 1.0f, 0.5f);
	ObjMgr.SetScale(46, 0.05f, 0.15f, 0.05f);
	ObjMgr.SetPosition(46, 0.0f, 0.0f, -3.0f);

	ObjMgr.CreateCube(139.0f / 255.0f, 0.0f, 1.0f, 0.5f);
	ObjMgr.SetScale(47, 0.05f, 0.15f, 0.05f);
	ObjMgr.SetPosition(47, -4.0f, 0.0f, -1.0f);

	ObjMgr.CreateCube(139.0f / 255.0f, 0.0f, 1.0f, 0.5f);
	ObjMgr.SetScale(48, 0.05f, 0.15f, 0.05f);
	ObjMgr.SetPosition(48, 3.0f, 0.0f, 0.0f);

	CameraPos = glm::vec3(0.5f, 0.4f, 0.0f);

	glutTimerFunc(30, RotatingStar, 1);
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

	//--- 투명 객체 그리기
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawModelAlpha();

	//--- 블렌딩 해제
	glDisable(GL_BLEND);

	glutSwapBuffers();
}

GLvoid drawView()
{
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "viewTransform");

	glm::vec3 cameraPos = glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(AT.x, AT.y, AT.z); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// 뷰변환
}

GLvoid drawProjection()
{
	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projectionTransform");

	glm::mat4 projection = glm::mat4(1.0f);

	// 직교투영
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
		// 원근 투영
		float fov = 45.0f; // 시야각 (Field of View)
		float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT); // 화면의 가로 세로 비율
		float zNear = 0.1f; // 가까운 클리핑 평면
		float zFar = 50.0f; // 먼 클리핑 평면
		projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar); //--- 투영 공간 설정: (뷰잉각도, 종횡비, 가까운거리, 먼거리)

	}

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// 투영변환
}

GLvoid drawLight()
{
	unsigned int lightPosLocation = glGetUniformLocation(ShaderProgram, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, LightPos.x, LightPos.y, LightPos.z);
	unsigned int lightColorLocation = glGetUniformLocation(ShaderProgram, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, LightColor.r, LightColor.g, LightColor.b);
	unsigned int viewPosLocation = glGetUniformLocation(ShaderProgram, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, 0.0f, 0.0f, 0.0f);
}

GLvoid drawModel()
{
	for (int i = 0; i < 45; i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			DrawObjectByIDX(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_model, i);
		}
	}
}

GLvoid drawModelAlpha()
{
	for (int i = 45; i < ObjMgr.m_ObjectList.size(); i++)
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

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

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

	// 모델변환
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");
	ObjMgr.m_ObjectList[idx].m_model = ObjMgr.TransformModel(idx);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObjMgr.m_ObjectList[idx].m_model));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, ObjMgr.m_ObjectList[idx].normals.size() * sizeof(glm::vec3), &ObjMgr.m_ObjectList[idx].normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	unsigned int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, ObjMgr.m_ObjectList[idx].m_color[0], ObjMgr.m_ObjectList[idx].m_color[1], ObjMgr.m_ObjectList[idx].m_color[2]);

	unsigned int objAlphaLocation = glGetUniformLocation(ShaderProgram, "alpha"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform1f(objAlphaLocation, ObjMgr.m_ObjectList[idx].m_color[3]);

	glDrawArrays(DRAW_TYPE, 0, ObjMgr.m_ObjectList[idx].vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid StopAllAnim()
{
	rotatingCarmera = false;
	rotatingLight = false;
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
	LightPos.x = sin(angle_light) * radius * roatingLightDir;
	LightPos.z = cos(angle_light) * radius;

	angle_light += 0.03f;

	glutPostRedisplay();


	if (rotatingLight) glutTimerFunc(30, RotatingLight, rotatingLight);
}

float angle_star = 0;
GLvoid RotatingStar(int isAnim)
{
	ObjMgr.SetPosition(2, 0.0f, cos(angle_star) * 30.0f, sin(angle_star) * 20.0f);
	ObjMgr.SetPosition(3, 0.0f, cos(angle_star) * 20.0f, sin(angle_star) * 15.0f);
	ObjMgr.SetPosition(4, 0.0f, cos(angle_star) * 15.0f, sin(angle_star) * 10.0f);

	angle_star += 0.03f;

	glutPostRedisplay();

	glutTimerFunc(30, RotatingStar, isAnim);
}

GLvoid SnowAnim(int idx)
{
	float speed = ObjMgr.m_ObjectList[idx].m_snow_speed;
	ObjMgr.Move(idx, 0.0f, -speed, 0.0f);

	if (ObjMgr.m_ObjectList[idx].m_pivot[1] < 0.0f)
	{
		ObjMgr.m_ObjectList[idx].m_pivot[1] = 200.0f;
	}

	glutPostRedisplay();

	glutTimerFunc(30, SnowAnim, idx);
}

GLvoid SetSnowAnim(bool active)
{
	for (int i = 5; i < 45; i++)
	{
		ObjMgr.m_ObjectList[i].m_isActive = active;
	}
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
	float force_x, force_y, force_z;
	force_x = force_y = force_z = 0.f;
	bool input_w = false;
	switch (key)
	{
	case 'H':
	case 'h':
		// 은면 제거
		if (isDepthTest) isDepthTest = false;
		else isDepthTest = true;
		break;
	case 'S':
	case 's':
		if (snow_on) snow_on = false;
		else snow_on = true;
		SetSnowAnim(snow_on);
		break;
	case 'N':
	case 'n':
		LightPos.x -= 0.1f;
		break;
	case 'F':
	case 'f':
		LightPos.x += 0.1f;
		break;
	case 'R':
	case 'r':
		if (rotatingLight) rotatingLight = false;
		else if (rotatingLight == false) { rotatingLight = true; roatingLightDir *= (-1.0f); }
		if (rotatingLight) glutTimerFunc(30, RotatingLight, rotatingLight);
		break;
	case '+':
		LightColor.x += 0.05f;
		LightColor.y += 0.05f;
		LightColor.z += 0.05f;
		break;
	case '-':
		LightColor.x -= 0.05f;
		LightColor.y -= 0.05f;
		LightColor.z -= 0.05f;
		break;
	case 'P':
	case 'p':
		projectionMode = !projectionMode;
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
