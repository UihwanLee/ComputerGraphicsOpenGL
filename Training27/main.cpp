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

float radius = 0.5f;

// 애니메이션 :: 변수
GLvoid CheckCollision();
bool CheckCollisionByBox(float x, float y, float z);

// 애니메이션 :: 크레인
GLvoid MovingCrainBottomX(int isAnim);
GLvoid RotatingCrainMidY(int isAnim);
GLvoid RotatingCrainGun(int isAnim);
GLvoid MovingCrainGun(int isAnim);
GLvoid RotatingCrainArm(int isAnim);

bool isMovingCrainBottomX = false;
bool isMovingCrainBottomX_Front = true;
bool isRotatingCrainMidY = false;
bool isRotatingCrainMidY_Front = true;
bool isRotatingCrainGun = false;
bool isRotatingCrainGun_Front = true;
bool isMovingCrainGun = false;
bool isMovingCrainGun_Front = true;
bool isRotatingCrainArm = false;

GLfloat rotateSpeed = 4.0f;
GLfloat moveSpeed = 0.5f;
GLfloat curRotateRate = 0.0f;
GLfloat curRotateArmRate = 0.0f;

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

glm::vec3 LightPos = glm::vec3(0.5f, 0.5f, 0.0f);
glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);

float roatingLightDir = 1.0f;
bool isShowCube = true;
bool rotatingLight = false;
bool rotatingCarmera = false;
bool rotatingCamera_z = false;
float rotatingCameraRate = 0.0f;
float rotatingCameraRate_x = 0.0f;
float rotatingCameraRate_y = 0.0f;


bool rotateCamera_mode_Y = false;
bool isRotateCamera_Y = false;
float roatingCameraDir = 1.0f;

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
	cout << "h/H : 은면 제거" << endl;
	cout << "p/P : 직각 투영/ 원근 투영" << endl;
	cout << endl;
	cout << "m: 조명 켜기/끄기" << endl;
	cout << "c: 조명을 다른 색으로 바뀌도록 한다." << endl;
	cout << "y: 조명을 화면 중심의 y축 기준으로 양/음 방향으로 회전하기(공전)" << endl;
	cout << "s: 회전 멈추기" << endl;
	cout << endl;	
	cout << "z/Z: 카메라가 z축 방향으로 양/음으로 이동" << endl;
	cout << "x/X: 카메라가 x축 방향으로 양/음으로 이동" << endl;
	cout << "r/R: 화면의 중심의 y축에 대하여 카메라 양/음 방향으로 회전(중점에 대하여 공전)" << endl;
	cout << endl;
	cout << "q: 프로그램 종료" << endl;
	cout << endl;
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateSqhere(1.0f, 1.0f, 0.0f);
	ObjMgr.SetPosition(0, LightPos.x, LightPos.y, LightPos.z);

	// 평지
	ObjMgr.CreateCube(150.0f / 255.0f, 75.0f / 255.0f, 0.0f);
	ObjMgr.SetScale(1, 1.0f, 0.1f, 1.0f);
	ObjMgr.SetPosition(1, 0.0f, -0.2f, 0.0f);

	// 크레인 아래 몸체
	ObjMgr.CreateCube2(255.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(2, 0.2f, 0.2f, 0.2f);

	// 크레인 중앙 몸체
	ObjMgr.CreateCube(0.0f, 0.0f, 255.0f);
	ObjMgr.SetScale(3, 0.6f, 0.6f, 0.6f);
	ObjMgr.SetPosition(3, 0.0f, 0.4f, 0.0f);

	// 크레인 포신 왼쪽
	ObjMgr.CreateCube(255.0f, 255.0f, 0.0f);
	ObjMgr.SetScale(4, 1.0f, 0.2f, 0.15f);
	ObjMgr.SetPosition(4, 0.3f, 0.3f, 0.2f);

	//// 크레인 포신 오른쪽
	ObjMgr.CreateCube(255.0f, 255.0f, 0.0f);
	ObjMgr.SetScale(5, 1.0f, 0.2f, 0.15f);
	ObjMgr.SetPosition(5, 0.3f, 0.3f, -0.2f);

	//// 크레인 팔 왼쪽
	ObjMgr.CreateCube(255.0f, 0.0f, 255.0f);
	ObjMgr.SetScale(6, 0.15f, 0.5f, 0.15f);
	ObjMgr.SetPosition(6, 0.0f, 0.9f, 0.15f);
	ObjMgr.SetScale(6, 0.15f, 0.8f, 0.15f);
	ObjMgr.SetPosition(6, 0.0f, 0.9f, 0.7f);

	//// 크레인 팔 오른쪽
	ObjMgr.CreateCube(255.0f, 0.0f, 255.0f);
	ObjMgr.SetScale(7, 0.15f, 0.8f, 0.15f);
	ObjMgr.SetPosition(7, 0.0f, 0.9f, -0.7f);

	CameraPos = glm::vec3(1.0f, 0.3f, 0.0f);
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

	glm::vec3 cameraPos = glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(AT.x, AT.y, AT.z); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	if (rotateCamera_mode_Y)
	{
		view = glm::rotate(view, glm::radians(rotatingCameraRate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.15f, -0.6f));
		view = glm::rotate(view, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	}


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
	if (isLight) glUniform3f(lightColorLocation, LightColor.r, LightColor.g, LightColor.b);
	else glUniform3f(lightColorLocation, 0.5, 0.5, 0.5);
	unsigned int viewPosLocation = glGetUniformLocation(ShaderProgram, "viewPos"); //--- viewPos 값 전달: 카메라 위치
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

	unsigned int objColorLocation = glGetUniformLocation(ShaderProgram, "objectColor"); //--- object Color값 전달 
	glUniform3f(objColorLocation, ObjMgr.m_ObjectList[idx].m_color[0], ObjMgr.m_ObjectList[idx].m_color[1], ObjMgr.m_ObjectList[idx].m_color[2]);


	glDrawArrays(DRAW_TYPE, 0, ObjMgr.m_ObjectList[idx].vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid StopAllAnim()
{
	rotatingCarmera = false;
	rotatingLight = false;
}

GLvoid RotatingCamera_Y(int isAnim)
{
	rotatingCameraRate_y += 5.0f;

	glutPostRedisplay();

	if (isRotateCamera_Y) glutTimerFunc(30, RotatingCamera_Y, isRotateCamera_Y);
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

// 크레인 중앙 몸체 Y축 기준으로 회전 -> 팔 2개도 같이 회전해야함
GLvoid RotatingCrainMidY(int isAnim)
{
	int idx = 3;

	float rotateSpeed = 5.0f;
	rotateSpeed = (isRotatingCrainMidY_Front) ? 5.0f : -5.0f;

	ObjMgr.Rotate(idx, 0.0f, rotateSpeed, 0.0f);

	glutPostRedisplay();

	if (isRotatingCrainMidY) glutTimerFunc(30, RotatingCrainMidY, isRotatingCrainMidY);
}

GLvoid RotatingCrainGun(int isAnim)
{
	int idx1 = 4;
	int idx2 = 5;

	if (curRotateRate > 20.0f)
	{
		rotateSpeed = rotateSpeed * (-1.0f);
		curRotateRate = 0.0f;
	}

	ObjMgr.Rotate(idx1, 0.0f, -rotateSpeed, 0.0f);
	ObjMgr.Rotate(idx2, 0.0f, rotateSpeed, 0.0f);
	curRotateRate += 1;

	glutPostRedisplay();

	if (isRotatingCrainGun) glutTimerFunc(30, RotatingCrainGun, isRotatingCrainGun);
}

GLvoid MovingCrainGun(int isAnim)
{
	int idx1 = 4;
	int idx2 = 5;

	if (ObjMgr.m_ObjectList[idx1].m_rotate[1] != 0)
	{
		ObjMgr.Rotate(idx1, 0.0f, 4.0f, 0.0f);
		ObjMgr.Rotate(idx2, 0.0f, -4.0f, 0.0f);
	}
	else
	{
		if (isMovingCrainGun_Front)
		{
			if (ObjMgr.m_ObjectList[idx1].m_pivot[2] > 0.0f)
			{
				ObjMgr.Move(idx1, 0.0f, 0.0f, -0.01f);
				ObjMgr.Move(idx2, 0.0f, 0.0f, 0.01f);
			}
			else
			{
				ObjMgr.m_ObjectList[idx1].m_pivot[2] = 0.0f;
				ObjMgr.m_ObjectList[idx2].m_pivot[2] = 0.0f;
				isMovingCrainGun_Front = false;
				isMovingCrainGun = false;
			}
		}
		else
		{
			if (ObjMgr.m_ObjectList[idx1].m_pivot[2] < 0.2f)
			{
				ObjMgr.Move(idx1, 0.0f, 0.0f, 0.01f);
				ObjMgr.Move(idx2, 0.0f, 0.0f, -0.01f);
			}
			else
			{
				ObjMgr.m_ObjectList[idx1].m_pivot[2] = 0.2f;
				ObjMgr.m_ObjectList[idx2].m_pivot[2] = -0.2f;
				isMovingCrainGun_Front = true;
				isMovingCrainGun = false;
			}
		}
	}

	glutPostRedisplay();

	if (isMovingCrainGun) glutTimerFunc(30, MovingCrainGun, isMovingCrainGun);
}

GLvoid RotatingCrainArm(int isAnim)
{
	int idx1 = 6;
	int idx2 = 7;

	if (curRotateArmRate > 2.0f)
	{
		rotateSpeed = rotateSpeed * (-1.0f);
		curRotateArmRate = 0.0f;
	}

	ObjMgr.Rotate(idx1, 0.0f, 0.0f, -rotateSpeed);
	ObjMgr.Rotate(idx2, 0.0f, 0.0f, rotateSpeed);
	curRotateArmRate += 1;

	glutPostRedisplay();

	if (isRotatingCrainArm) glutTimerFunc(30, RotatingCrainArm, isRotatingCrainArm);
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
	CameraPos.x = sin(angle_camera) * radius * roatingCameraDir;
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
		// 애니메이션 :: 크레인 아래 몸체 이동
	case 'B':
		isMovingCrainBottomX_Front = false;
		if (isMovingCrainBottomX) isMovingCrainBottomX = false;
		else isMovingCrainBottomX = true;
		if (isMovingCrainBottomX) glutTimerFunc(30, MovingCrainBottomX, isMovingCrainBottomX);
		break;
	case 'b':
		isMovingCrainBottomX_Front = true;
		if (isMovingCrainBottomX) isMovingCrainBottomX = false;
		else isMovingCrainBottomX = true;
		if (isMovingCrainBottomX) glutTimerFunc(30, MovingCrainBottomX, isMovingCrainBottomX);
		break;
		// 애니메이션 :: 크레인 중앙 몸체 회전
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
	case 'F':
		break;
	case 'f':
		isRotatingCrainGun_Front = true;
		if (isRotatingCrainGun) isRotatingCrainGun = false;
		else isRotatingCrainGun = true;
		if (isRotatingCrainGun) glutTimerFunc(30, RotatingCrainGun, isRotatingCrainGun);
		break;
	case 'E':
	case 'e':
		rotateSpeed = 4.0f;
		curRotateRate = 0.0f;
		isMovingCrainGun = true;
		if (isMovingCrainGun)glutTimerFunc(30, MovingCrainGun, isMovingCrainGun);
		break;
	case 's':
		rotatingLight = false;
		rotatingCarmera = false;
		break;
	case 'Y':
	case 'y':
		if (rotatingLight) rotatingLight = false;
		else if (rotatingLight == false) { rotatingLight = true; roatingLightDir *= (-1.0f); }
		if (rotatingLight) glutTimerFunc(30, RotatingLight, rotatingLight);
		break;
	case 'R':
	case 'r':
		if (rotatingCarmera) rotatingCarmera = false;
		else { rotatingCarmera = true; roatingCameraDir *= (-1.0f); }
		if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
		break;
	case 'C':
	case 'c':
		ChangeLightRandomColor();
		break;
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
