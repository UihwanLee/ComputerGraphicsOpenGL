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

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, glm::mat4& model, float* modelInfo, int idx);

// 애니메이션 :: 초기화
GLvoid StopAllAnim();
GLvoid MoveObject(int isAnim);
GLvoid MovingSqhere(int idx);

// 애니메이션 :: 카메라
GLvoid RotatingCamera(int isAnim);

// 애니메이션 :: 변수
bool isMovingObject = false;

GLvoid CheckCollision();
bool CheckCollisionByBox(float x, float y, float z);

GLfloat rotateSpeed = 4.0f;


// 투영 변환
bool projectionMode = true;

// Camera
GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(0.5f, 0.5f, 0.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);

bool rotatingCarmera = false;
bool rotatingCamera_z = false;
float rotatingCameraRate = 0.0f;
float rotatingCameraRate_x = 0.0f;
float rotatingCameraRate_y = 0.0f;

void Keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid MouseDrag(int x, int y);

bool g_left_button = false;
float curAngle = 0.0f;
float preAngle = 0.0f;
float dirAngle = 0.0f;

struct Dir {
	float x = -1.0f;
	float y = 1.0f;
	float z = 1.0f;
};

vector<Dir> dirList;
int curSqhereIDX = 9;
GLvoid CreateSqhere();

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
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
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
}

GLvoid Message()
{
	cout << "h/H : 은면 제거" << endl;
	cout << endl;
	cout << "z/Z: z축으로 양/음 방향으로 이동" << endl;
	cout << "y/Y: 화면의 Y축에 대하여 시계/반시계 방향으로 회전" << endl;
	cout << "B: 볼이 새로 생겨서 튕기기 시작한다 (최대 5개)" << endl;
	cout << endl;
	cout << "마우스를 왼쪽으로 옮기면 육면체가 z축에 대하여 왼쪽으로 회전" << endl;
	cout << "마우스를 오른쪽으로 옮기면 육면체가 z축에 대하여 오른쪽으로 회전" << endl;
	cout << endl;
	cout << "c/C: 모든 변환을 리셋하고 다시 시작" << endl;
	cout << "Q: 프로그램 종료한다." << endl;
	cout << endl;
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateCoordinate();
	ObjMgr.SetActive(0, false);

	CameraPos = glm::vec3(0.6f, 0.1f, 0.0f);

	// 배경 생성(큐브 , 장애물)
	ObjMgr.CreateCubeFace(5, 1.0f, 1.0f, 1.0f);
	ObjMgr.SetScale(1, 0.8f, 0.8f, 0.8f);
	ObjMgr.SetPosition(1, -1.0f, 0.0f, 0.0f);

	ObjMgr.CreateCubeFace(2, 128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	ObjMgr.SetScale(2, 0.8f, 0.8f, 0.8f);
	ObjMgr.SetPosition(2, -1.5f, 0.0f, 0.0f);

	ObjMgr.CreateCubeFace(3, 150.0f / 255.0f, 75.0f / 255.0f, 0.0f / 255.0f);
	ObjMgr.SetScale(3, 0.8f, 0.8f, 0.8f);
	ObjMgr.SetPosition(3, -1.0f, 0.0f, 0.0f);

	ObjMgr.CreateCubeFace(1, 255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(4, 0.8f, 0.8f, 0.8f);
	ObjMgr.SetPosition(4, -1.0f, 0.0f, 0.0f);

	ObjMgr.CreateCubeFace(0, 0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(5, 0.8f, 0.8f, 0.8f);
	ObjMgr.SetPosition(5, -1.0f, 0.4f, 0.0f);

	// 다보탑
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(6, 0.2f, 0.1f, 0.1f);
	ObjMgr.SetPosition(6, -5.0f, -3.5f, 0.0f);

	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(7, 0.8f, 0.8f, 0.8f);
	ObjMgr.SetPosition(7, 0.4f, -0.0f, 0.1f);

	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(8, 0.5f, 0.5f, 0.5f);
	ObjMgr.SetPosition(8, 0.7f, -0.1f, 0.2f);

	// 공
	ObjMgr.CreateSqhere(0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(9, 0.05f, 0.05f, 0.05f);
	ObjMgr.SetPosition(9, -4.0f, 0.0f, 0.0f);

	Dir dir;
	dirList.emplace_back(dir);
	glutTimerFunc(30, MovingSqhere, 9);


	isMovingObject = true;
	if (isMovingObject) glutTimerFunc(30, MoveObject, isMovingObject);
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


	/*if (rotateCamera_mode_X)
	{
		view = glm::rotate(view, glm::radians(rotatingCameraRate_x), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.15f, -0.6f));
		view = glm::rotate(view, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (rotateCamera_mode_Y)
	{
		view = glm::rotate(view, glm::radians(rotatingCameraRate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.15f, -0.6f));
		view = glm::rotate(view, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}*/
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

GLvoid drawModel()
{
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

GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, glm::mat4& model, float* modelInfo, int idx)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_IDX, obj_index, GL_STATIC_DRAW);

	// 모델변환
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
	rotatingCarmera = false;

	rotateSpeed = 4.0f;
}

float angle_camera = 0;
float angle_rotate = 0.03f;
GLvoid RotatingCamera(int isAnim)
{
	float radius = 2.0f;

	CameraPos.x = sin(angle_camera) * radius;
	CameraPos.z = cos(angle_camera) * radius;

	angle_camera += angle_rotate;

	glutPostRedisplay();

	if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
}

GLvoid CreateSqhere()
{
	if (dirList.size() >= 5) return;

	curSqhereIDX += 1;

	Dir dir;
	dirList.emplace_back(dir);

	ObjMgr.CreateSqhere(0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(curSqhereIDX, 0.05f, 0.05f, 0.05f);
	ObjMgr.SetPosition(curSqhereIDX, -4.0f, 0.0f, 0.0f);
	glutTimerFunc(30, MovingSqhere, curSqhereIDX);
}

GLvoid CheckCollision(int idx)
{
	float SqhereX = ObjMgr.m_ObjectList[idx].m_pivot[0];
	float SqhereY = ObjMgr.m_ObjectList[idx].m_pivot[1];
	float SqhereZ = ObjMgr.m_ObjectList[idx].m_pivot[2];


	// X : -5.0 ~ -1.0
	// Y : -3.5 ~ 3.5
	// Z : -3.5 ~ 3.5
	float MAX_X = -3.0f; float MIN_X = -10.0f;
	float MAX_Y = 3.5f; float MIN_Y = -3.5f;
	float MAX_Z = 3.5f; float MIN_Z = -3.5f;

	int dirIDX = idx - 9;

	// 천장/바닥에 닿은 경우
	if (SqhereX < MIN_X || SqhereX > MAX_X)
	{
		dirList[dirIDX].x = dirList[dirIDX].x * (-1.0f);
	}

	// 상하 벽에 닿은 경우
	if (SqhereY < MIN_Y || SqhereY > MAX_Y)
	{
		dirList[dirIDX].y = dirList[dirIDX].y * (-1.0f);
	}

	// 양쪽 벽에 닿은 경우
	if (SqhereZ < MIN_Z || SqhereZ > MAX_Z)
	{
		dirList[dirIDX].z = dirList[dirIDX].z * (-1.0f);
	}
}

GLvoid MovingSqhere(int idx)
{
	int dirIDX = idx - 9;

	CheckCollision(idx);

	float move_x = 0.08f * dirList[dirIDX].x;
	float move_y = 0.05f * dirList[dirIDX].y;
	float move_z = 0.03f * dirList[dirIDX].z;

	ObjMgr.Move(idx, move_x, move_y, move_z);

	glutPostRedisplay();

	//cout << "Hello" << endl;

	glutTimerFunc(30, MovingSqhere, idx);
}

GLvoid CheckCollision()
{

}

GLvoid RotateCube()
{
	ObjMgr.Rotate(2, 30.0f, 0.0f, 0.0f);
}

GLvoid MoveObject(int isAnim)
{
	int idx = 6;

	// 마우스 회전에 따른 이동

	float angle = int(curAngle) % 360;
	float moveSpeed = 0.1f;
	float dir = 1.0f;


	if (angle > 0 && angle < 90)
	{
		moveSpeed = 0.1f;
		ObjMgr.Move(idx, 0.0f, 0.0f, moveSpeed);
	}
	if (angle > 90 && angle < 180)
	{
		moveSpeed = 0.1f;
		ObjMgr.Move(idx, 0.0f, moveSpeed, 0.0f);
	}
	if (angle > 180 && angle < 270)
	{
		moveSpeed = -0.1f;
		ObjMgr.Move(idx, 0.0f, 0.0f, moveSpeed);
	}
	if (angle > 270 && angle < 360)
	{
		moveSpeed = -0.1f;
		ObjMgr.Move(idx, 0.0f, moveSpeed, 0.0f);
	}


	if (angle > -90 && angle < 0)
	{
		moveSpeed = -0.1f;
		ObjMgr.Move(idx, 0.0f, 0.0f, moveSpeed);
	}
	if (angle > -180 && angle < -90)
	{
		moveSpeed = 0.1f;
		ObjMgr.Move(idx, 0.0f, moveSpeed, 0.0f);
	}
	if (angle > -270 && angle < -180)
	{
		moveSpeed = 0.1f;
		ObjMgr.Move(idx, 0.0f, 0.0f, moveSpeed);
	}
	if (angle > -360 && angle < 0)
	{
		moveSpeed = -0.1f;
		ObjMgr.Move(idx, 0.0f, moveSpeed, 0.0f);
	}


	// 최대 이동 거리 제한
	if (ObjMgr.m_ObjectList[idx].m_pivot[2] < -3.5f)
	{
		ObjMgr.m_ObjectList[idx].m_pivot[2] = -3.5f;
	}
	if (ObjMgr.m_ObjectList[idx].m_pivot[2] > 3.5f)
	{
		ObjMgr.m_ObjectList[idx].m_pivot[2] = 3.5f;
	}
	if (ObjMgr.m_ObjectList[idx].m_pivot[1] < -3.5f)
	{
		ObjMgr.m_ObjectList[idx].m_pivot[1] = -3.5f;
	}
	if (ObjMgr.m_ObjectList[idx].m_pivot[1] > 3.5f)
	{
		ObjMgr.m_ObjectList[idx].m_pivot[1] = 3.5f;
	}

	glutPostRedisplay();

	if (isMovingObject) glutTimerFunc(10, MoveObject, isMovingObject);
}

bool CheckCollisionByBox(float x, float y, float z)
{
	bool isCollision = false;


	return isCollision;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'H':
	case 'h':
		// 은면 제거
		if (isDepthTest) isDepthTest = false;
		else isDepthTest = true;
		break;
	// 공 생성
	case 'B':
	case 'b':
		CreateSqhere();
		break;
	case 'F':
	case 'f':
		RotateCube();
		break;
	// 카메라
	case 'Z':
		CameraPos.x += 0.1f;
		break;
	case 'z':
		CameraPos.x -= 0.1f;
		break;
	case 'Y':
		angle_rotate = -0.03f;
		if (rotatingCarmera) rotatingCarmera = false;
		else if (rotatingCarmera == false) rotatingCarmera = true;
		if (rotatingCarmera) glutTimerFunc(30, RotatingCamera, rotatingCarmera);
		break;
	case 'y':
		angle_rotate = 0.03f;
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

float GetClickPos(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
	return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	int idx = 0;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// 마우스 클릭 ...

		float start_x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f; // x 끝점
		float start_y = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT); // y 끝점

		curAngle = GetClickPos(x, 0.0f, 800.0f, -60.0f, 60.0f);

		g_left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// 마우스 클릭 땔 때 ....
		preAngle = curAngle;

		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid MouseDrag(int x, int y)
{
	if (g_left_button)
	{
		int idx = 0;

		// 마우스 드래그에 따른 선 길이 조정
		float drag_x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f; // x 끝점
		float drag_z = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT); // y 끝점

		curAngle = GetClickPos(x, 0.0f, 800.0f, -60.0f, 60.0f);

		//dirAngle = curAngle - preAngle;

		//float change_x = GetClickPos(x, 0.0f, 800.0f, -2.0f, 2.0f);
		//float change_y = GetClickPos(y, 0.0f, 600.0f, 2.0f, -2.0f);

		// 마우스 드래그에 따른 Cube Rotate
		ObjMgr.SetRotate(2, curAngle, 0.0f, 0.0f);
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
