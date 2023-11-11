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

// 애니메이션 :: 큐브

// 애니메이션 :: 로봇
GLvoid RotateRobotByDir(int isAnim);
GLvoid JumpRobobot(int isAnim);
GLvoid JumpChildRobot(int isAnim);

// 애니메이션 :: 카메라
GLvoid RotatingCamera(int isAnim);

// 애니메이션 :: 변수
bool isOpenDoor = false;
bool isRotatingDoor = false;
bool isRotatingRobot = false;
bool isRobotJumping = false;
bool isChildRobotJumping = false;

// 로봇 변수
GLfloat playerSpeed = 0.2f;
GLfloat playerRotateArmSpeed = 5.0f;
GLfloat playerRotateLegSpeed = 3.0f;
GLfloat playerJumpSpeed = 0.5f;
GLfloat playerJumpRate = 0.0f;
GLfloat childJumpSpeed = 0.2f;
GLfloat childJumpRate = 0.0f;
float forceAmount = 10.0f;

GLvoid CheckCollision();
bool CheckCollisionByBox(float x, float y, float z);

GLfloat rotateSpeed = 4.0f;
GLfloat moveSpeed = 0.5f;
GLfloat doorRotateRate = 0.0f;
GLfloat armRotateRate = 0.0f;

// pre 위치
GLfloat prevPivot[3];
bool isCollisionUp = false;

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

bool isChildGenerate = false;

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
		ObjMgr.Move(7, 0.0f, -0.1f, 0.0f);

		CheckCollision();
	}
}

GLvoid Message()
{
	cout << "h/H : 은면 제거" << endl;
	cout << "p/P : 직각 투영/ 원근 투영" << endl;
	cout << endl;
	cout << "w/a/s/d: 로봇이 앞/뒤/좌/우 방향으로 이동 방향을 바꿔서 걷는다. 가장자리에 도달하면 로봇은 뒤로 돌아 다시 걷는다." << endl;
	cout << "+/-: 걷는 속도가 빨라지거나/느려진다. 속도가 바뀔 때 걷는 다리의 각도가 늘어나거나/줄어든다." << endl;
	cout << "j: 로봇이 제자리에서 점프한다. 장애물 위로 점프하여 올라가고 내려갈 수 있다." << endl;
	cout << endl;
	cout << "t: 작은 로봇 생성/삭제" << endl;
	cout << "f: 작은 로봇이 점프한다." << endl;
	cout << endl;
	cout << "i: 모든 변환을 리셋하고 다시 시작" << endl;
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
	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(1, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(1, 0.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(2, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(2, 1.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(3, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(3, -1.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(4, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(4, -2.0f, -15.0f, 0.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(5, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(5, 0.0f, -15.0f, 1.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(6, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(6, 0.0f, -15.0f, 2.0f);

	// 로봇 몸체(최고 부모)
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(7, 0.03f, 0.04f, 0.03f);
	ObjMgr.SetPosition(7, -1.5f, -4.0f, 0.0f);

	// 로봇 머리
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(8, 0.8f, 1.0f, 0.8f);
	ObjMgr.SetPosition(8, 0.0f, 0.5f, 0.0f);

	// 로봇 코
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(9, 0.45f, 0.25f, 0.3f);
	ObjMgr.SetPosition(9, 0.3f, 0.8f, 0.0f);

	// 로봇 오른쪽 팔
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(10, 0.4f, 1.0f, 0.3f);
	ObjMgr.SetPosition(10, 0.0f, -0.1f, 0.6f);

	// 로봇 왼쪽 팔
	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(11, 0.4f, 1.0f, 0.3f);
	ObjMgr.SetPosition(11, 0.0f, -0.1f, -0.6f);

	// 로봇 오른쪽 다리
	ObjMgr.CreateCube(138.0f / 255.0f, 43.0f / 255.0f, 226.0f / 255.0f);
	ObjMgr.SetScale(12, 0.6f, 1.0f, 0.5f);
	ObjMgr.SetPosition(12, 0.0f, -1.0f, 0.2f);

	// 로봇 왼쪽 다리
	ObjMgr.CreateCube(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(13, 0.6f, 1.0f, 0.5f);
	ObjMgr.SetPosition(13, 0.0f, -1.0f, -0.2f);

	// 로봇 CollisionBox
	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(14, 1.5f, 2.4f, 1.5f);
	ObjMgr.SetPosition(14, 0.0f, -0.2f, 0.0f);
	ObjMgr.SetWireSolidType(14, GL_LINE_LOOP);
	//ObjMgr.SetActive(14, false);


	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(15, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(15, 0.0f, -15.0f, 3.0f);

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(16, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(16, 0.0f, -15.0f, -1.0f);

	int idx = 17;
	float dist = 3.0f;
	for (int i = 0; i < 7; i++)
	{
		if (i % 2 == 0)
		{
			ObjMgr.CreateCube(1.0f, 1.0f, 0.0f);
		}
		else
		{
			ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
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
			ObjMgr.CreateCube(1.0f, 1.0f, 0.0f);
		}
		else
		{
			ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
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
			ObjMgr.CreateCube(1.0f, 1.0f, 0.0f);
		}
		else
		{
			ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
		}
		ObjMgr.SetScale(idx, 0.1f, 0.01f, 0.1f);
		ObjMgr.SetPosition(idx, -2.0f, -15.0f, dist);
		idx += 1;
		dist -= 1.0f;
	}

	ObjMgr.CreateCube(0.0f / 255.0f, 204.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(38, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(38, 0.0f, -15.0f, -3.0f);

	ObjMgr.CreateCube(1.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(39, 0.1f, 0.01f, 0.1f);
	ObjMgr.SetPosition(39, 0.0f, -15.0f, -2.0f);

	// 자식 로봇 

	// 자식 로봇 몸체(최고 부모)
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(40, 0.8f, 0.7f, 0.8f);
	ObjMgr.SetPosition(40, -2.5f, -0.5f, 0.0f);

	// 자식 로봇 머리
	ObjMgr.CreateCube(0.0f, 0.0f, 1.0f);
	ObjMgr.SetScale(41, 0.6f, 0.5f, 0.6f);
	ObjMgr.SetPosition(41, -0.0f, 0.7f, 0.0f);

	// 자식 로봇 코
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(42, 0.3f, 0.2f, 0.3f);
	ObjMgr.SetPosition(42, 0.3f, 0.7f, 0.0f);

	// 자식 로봇 오른쪽 팔
	ObjMgr.CreateCube(1.0f, 0.0f, 0.0f);
	ObjMgr.SetScale(43, 1.0f, 0.4f, 0.3f);
	ObjMgr.SetPosition(43, -0.4f, 0.3f, 0.6f);

	// 자식 로봇 왼쪽 팔
	ObjMgr.CreateCube(0.0f, 1.0f, 0.0f);
	ObjMgr.SetScale(44, 1.0f, 0.4f, 0.3f);
	ObjMgr.SetPosition(44, -0.4f, 0.3f, -0.6f);

	// 자식 로봇 오른쪽 다리
	ObjMgr.CreateCube(138.0f / 255.0f, 43.0f / 255.0f, 226.0f / 255.0f);
	ObjMgr.SetScale(45, 0.4f, 1.2f, 0.3f);
	ObjMgr.SetPosition(45, -0.0f, -1.0f, 0.3f);

	// 자식 로봇 왼쪽 다리
	ObjMgr.CreateCube(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	ObjMgr.SetScale(46, 0.4f, 1.2f, 0.3f);
	ObjMgr.SetPosition(46, -0.0f, -1.0f, -0.3f);

	for (int i = 40; i < 47; i++)
	{
		ObjMgr.SetActive(i, false);
	}



	prevPivot[0] = ObjMgr.m_ObjectList[7].m_pivot[0];
	prevPivot[1] = ObjMgr.m_ObjectList[7].m_pivot[1];
	prevPivot[2] = ObjMgr.m_ObjectList[7].m_pivot[2];
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
	isRobotJumping = false;
	isRotatingRobot = false;

	rotateSpeed = 4.0f;
	doorRotateRate = 0.0f;
}

// 로봇 팔, 다리 회전
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

// 로봇 점프
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

// 자식 로봇 점프
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

	float maxDist_z = 8.0f;
	float minDist_z = -8.0f;

	// 땅에 떨어지지 않게 고정
	// 장애물 판단에 맞게 조정

	if (isCollisionUp)
	{
		if (ObjMgr.m_ObjectList[idx].m_pivot[1] < -1.5f)
		{
			ObjMgr.m_ObjectList[idx].m_pivot[1] = -1.5f;
		}
	}
	else
	{
		/*if (ObjMgr.m_ObjectList[idx].m_pivot[1] >= -1.5f)
		{
			cout << "Hello" << endl;
			ObjMgr.m_ObjectList[idx].m_pivot[1] = -1.5f;
		}*/

		if (ObjMgr.m_ObjectList[idx].m_pivot[1] < -2.0f)
		{
			ObjMgr.m_ObjectList[idx].m_pivot[1] = -2.0f;
		}
	}

	// 벽에 닿으면 반대방향으로 이동
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



	// 장애물 충돌처리(CollsionBox)
	// 장애물 1
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
	//	cout << "장애물 충돌!" << endl;

	//	// 이전 위치로 갱신
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

	float offset = 0.057;

	isCollisionUp = false;

	// 장애물 충돌처리(CollsionBox)

	// 장애물 1
	float min_z = ObjMgr.m_ObjectList[16].m_model[3][2] - offset;
	float max_z = ObjMgr.m_ObjectList[16].m_model[3][2] + offset;
	float min_x = ObjMgr.m_ObjectList[16].m_model[3][0] - offset;
	float max_x = ObjMgr.m_ObjectList[16].m_model[3][0] + offset;

	float max_y = ObjMgr.m_ObjectList[16].m_model[3][1] + 0.042;

	// Move 하기 전 충돌 체크

	float player_x = ObjMgr.m_ObjectList[14].m_model[3][0] + x;
	float player_y = ObjMgr.m_ObjectList[14].m_model[3][1] + y;
	float player_z = ObjMgr.m_ObjectList[14].m_model[3][2] + z;

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y <= max_y)
	{
		isCollision = true;
	}

	// 점프해서 들어오면 위치 조정
	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y > max_y)
	{
		isCollisionUp = true;
	}

	// 장애물 2
	min_z = ObjMgr.m_ObjectList[18].m_model[3][2] - offset;
	max_z = ObjMgr.m_ObjectList[18].m_model[3][2] + offset;
	min_x = ObjMgr.m_ObjectList[18].m_model[3][0] - offset;
	max_x = ObjMgr.m_ObjectList[18].m_model[3][0] + offset;

	max_y = ObjMgr.m_ObjectList[18].m_model[3][1] + 0.042;

	// Move 하기 전 충돌 체크

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y <= max_y)
	{
		isCollision = true;
	}

	// 점프해서 들어오면 위치 조정
	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y > max_y)
	{
		isCollisionUp = true;
	}

	// 장애물 2
	min_z = ObjMgr.m_ObjectList[20].m_model[3][2] - offset;
	max_z = ObjMgr.m_ObjectList[20].m_model[3][2] + offset;
	min_x = ObjMgr.m_ObjectList[20].m_model[3][0] - offset;
	max_x = ObjMgr.m_ObjectList[20].m_model[3][0] + offset;

	max_y = ObjMgr.m_ObjectList[20].m_model[3][1] + 0.042;

	// Move 하기 전 충돌 체크

	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y <= max_y)
	{
		isCollision = true;
	}

	// 점프해서 들어오면 위치 조정
	if (player_x >= min_x && player_x <= max_x && player_z >= min_z && player_z <= max_z && player_y > max_y)
	{
		isCollisionUp = true;
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
		// 은면 제거
		if (isDepthTest) isDepthTest = false;
		else isDepthTest = true;
		break;
		// 애니메이션 :: 로봇
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
		// 애니메이션 :: 카메라
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
	case 'P':
	case 'p':
		projectionMode = !projectionMode;
		break;
	case 'I':
	case 'i':
		Reset();
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
		//if (CheckCollisionByBox(-move_speed, 0.0f, 0.0f) == false)
		ObjMgr.Move(7, -playerSpeed, 0.0f, 0.0f);
	}
	if (key == 's')
	{
		//if (CheckCollisionByBox(move_speed, 0.0f, 0.0f) == false)
		ObjMgr.Move(7, playerSpeed, 0.0f, 0.0f);
	}
	if (key == 'a')
	{
		//if (CheckCollisionByBox(0.0f, 0.0f, move_speed) == false)
		ObjMgr.Move(7, 0.0f, 0.0f, playerSpeed);
	}
	if (key == 'd')
	{
		//if (CheckCollisionByBox(0.0f, 0.0f, -move_speed) == false)
		ObjMgr.Move(7, 0.0f, 0.0f, -playerSpeed);
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
