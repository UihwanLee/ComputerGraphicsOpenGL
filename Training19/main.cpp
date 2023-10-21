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

// 클래스
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

// 변환

// 애니메이션
GLvoid StopAllAnim();
GLvoid MovingThroughOrbit(int idx);
GLvoid MovingOrbitUp(int idx);

void timer(int value);

bool isRotatingAnim = false;

GLfloat rotateSpeed = 5.0f;
GLfloat moveSpeed = 0.5f;

bool isFirst = true;

// 투영 변환
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
	glutTimerFunc(0, timer, 0); // 타이머 함수 등록
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
	cout << "p/P : 직각 투영/ 원근 투영" << endl;
	cout << "m/M : 솔리드 모델/와이어 모델" << endl;
	cout << endl;
	cout << "w/a/s/d : 좌/우/상/하 이동" << endl;
	cout << "+/- : 앞/뒤 이동" << endl;
	cout << "y/Y : 전체 도형 y축으로 양/음 회전" << endl;
	cout << "z/Z : 행성,달,궤도가 z축에 대하여 양/음 방향으로 회전" << endl;
	cout << endl;
}

const int numPoints = 100; // 궤도를 구성하는 점의 수
const float orbitRadius = 0.3f; // 궤도 반지름
const float rotationSpeed = 1.0f; // 자전 속도 (라디안/초)
const float revolutionSpeed = 0.1f; // 공전 속도 (라디안/초)

// 현재 객체의 공전 각도
float revolutionAngle = 0.0f;

void timer(int value) {
	revolutionAngle += revolutionSpeed;
	if (revolutionAngle >= 2.0f * PI) {
		revolutionAngle -= 2.0f * PI;
	}

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0); // 16ms마다 업데이트 (약 60프레임/초)
}

GLvoid Reset()
{
	StopAllAnim();
	ObjMgr.Reset();

	ObjMgr.CreateCoordinate();

	// 태양
	ObjMgr.CreateSqhere();
	ObjMgr.SetScale(1, 0.3f, 0.4f, 0.3f);

	// 궤도1
	ObjMgr.CreateOrbit(2.0f);
	ObjMgr.SetScale(2, 0.3f, 0.4f, 0.3f);

	// 지구1
	ObjMgr.CreateSqhere();
	ObjMgr.SetScale(3, 0.2f, 0.3f, 0.2f);
	ObjMgr.SetPosition(3, 0.5f, 0.0f, 0.0f);

	// 달
	ObjMgr.CreateSqhere();
	ObjMgr.SetScale(4, 0.1f, 0.15f, 0.1f);
	ObjMgr.SetPosition(4, 0.75f, 0.0f, 0.0f);

	// 궤도2
	ObjMgr.CreateOrbit(2.0f);
	ObjMgr.SetScale(5, 0.3f, 0.4f, 0.3f);
	ObjMgr.SetRotate(5, 45.0f, 0.0f, 0.0f);

	// 지구2
	ObjMgr.CreateSqhere();
	ObjMgr.SetScale(6, 0.2f, 0.3f, 0.2f);
	//ObjMgr.SetRotate(6, 45.0f, 0.0f, 0.0f);
	ObjMgr.SetPosition(6, 0.5f, 0.0f, 0.0f);


	// 달2
	ObjMgr.CreateSqhere();
	ObjMgr.SetScale(7, 0.1f, 0.15f, 0.1f);
	ObjMgr.SetPosition(7, 0.75f, 0.0f, 0.0f);

	ObjMgr.SetAllModel();

	ObjMgr.m_ObjectList[0].m_isActive = false;

	// 자식 설정
	//ObjMgr.SetChild(1, 3); // 태양 <- 지구1
	//ObjMgr.SetChild(1, 2); // 태양 <- 궤도1
	//ObjMgr.SetChild(3, 4); // 지구1 <- 달1
	//ObjMgr.SetChild(1, 5); // 태양1 <- 궤도2
	//ObjMgr.SetChild(1, 6); // 태양1 <- 지구2
	//ObjMgr.SetChild(6, 7); // 지구2 <- 달2
	////ObjMgr.SetChild()

	//MovingThroughOrbit(3);
	//MovingThroughOrbit(4);
	//MovingThroughOrbit(6);
	//MovingThroughOrbit(7);

	ObjMgr.SetChild(1, 2); // 태양 <- 궤도1
	ObjMgr.SetChild(2, 3); // 궤도1 <- 지구1
	ObjMgr.SetChild(1, 5); // 태양1 <- 궤도2
	//ObjMgr.SetChild(5, 6); // 궤도2 <- 지구2


	// 공전과 자전 주기를 자주 바꿔져야함
	// 태양 -> 자전
	// 지구1 -> 공전 + 자전
	MovingOrbitUp(1);
}

float angle = 0.0f; // 도형의 현재 위치 각도
int step[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // 한 스텝당 각도 증가량
float radius_list[9] = { 2.5f, 2.5f, 2.5f, 2.5f, 5.0f, 2.5f, 2.5f, 5.0f, 2.5f};
                       // 0     1     2     3     4     5     6     7     8

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	if (isDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projectionTransform");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "viewTransform");

	glm::vec3 cameraPos = glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(AT.x, AT.y, AT.z); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// 뷰변환

	glm::mat4 projection = glm::mat4(1.0f);

	// 직교투영
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
		// 원근 투영
		float fov = 45.0f; // 시야각 (Field of View)
		float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT); // 화면의 가로 세로 비율
		float zNear = 0.1f; // 가까운 클리핑 평면
		float zFar = 50.0f; // 먼 클리핑 평면
		projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar); //--- 투영 공간 설정: (뷰잉각도, 종횡비, 가까운거리, 먼거리)

	}

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// 투영변환

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

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
	model = glm::rotate(model, revolutionAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(orbitRadius, 0.0f, 0.0f));

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

}

// 궤도 돌기
GLvoid MovingThroughOrbit(int idx)
{
	float radius = radius_list[idx];
	angle = 2.0f * PI * step[idx] / 100;
	/*if (angle >= 2.0f * PI) {
		angle -= 2.0f * PI;
	}*/
	step[idx]++;

	if (step[idx] > 100)
	{
		step[idx] = 0;
	}

	float cur_move_x = radius * cos(angle);
	float cur_move_z = radius * sin(angle);

	float next_angle = 2.0f * PI * step[idx] / 100;

	float next_move_x = radius * cos(next_angle);
	float next_move_z = radius * sin(next_angle);

	float move_x = next_move_x - cur_move_x;
	float move_z = next_move_z - cur_move_z;

	ObjMgr.Move(idx, move_x, 0.0f, move_z);

	glutPostRedisplay();

	glutTimerFunc(30, MovingThroughOrbit, idx);
}

// y 상하이동
int count_Up = 0;
float move_y = 5.0f;
GLvoid MovingOrbitUp(int idx)
{
	ObjMgr.Rotate(idx, 0.0f, move_y, 0.0f);

	glutPostRedisplay();

	glutTimerFunc(30, MovingOrbitUp, idx);
}

GLvoid RotatingAnimationX(int idx)
{
	ObjMgr.SetRotate(idx, ObjMgr.m_ObjectList[idx].m_rotate[0] + rotateSpeed, ObjMgr.m_ObjectList[idx].m_rotate[1], ObjMgr.m_ObjectList[idx].m_rotate[2]);

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, idx);
}

GLvoid SetRotatingAnimation(bool isAnim)
{
	for (int i = 1; i < 7; i++)
	{
		ObjMgr.m_ObjectList[i].m_isAnimRotating = isAnim;
		if (ObjMgr.m_ObjectList[i].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, i);
	}
}

GLvoid MoveObject_X(int idx, float dir)
{
	float move = moveSpeed * dir;
	ObjMgr.Move(idx, move, 0, 0);
}

GLvoid MoveObject_Y(int idx, float dir)
{
	float move = moveSpeed * dir;
	ObjMgr.Move(idx, 0, move, 0);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		MoveObject_Y(1, 1.0f);
		break;
	case 'a':
		MoveObject_X(1, -1.0f);
		break;
	case 's':
		MoveObject_Y(1, -1.0f);
		break;
	case 'd':
		MoveObject_X(1, 1.0f);
		break;
	case 'M':
	case 'm':
		ObjMgr.ChangeWireSolidType();
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
