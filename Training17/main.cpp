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
GLvoid Reset();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[2], EBO;
bool isDepthTest = false;

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, float* modelInfo, int idx);

// 애니메이션
float originPivot_f1[3];
float originPivot_f2[3];
float swapPivot_f1[3];
float swapPivot_f2[3];
bool animRotatingSwap = false;
bool animMovingDirectSwap = false;
GLvoid StopAllAnim();
GLvoid RotatinSwapAnim(int idx);
GLvoid MovingDirectOriginAnim(int idx);
GLvoid MovingDirectSwapAnim(int idx);
GLvoid MovingUpDownSwapAnim(int idx);
float moveOffset = 0.03f;

bool isFirst = true;

// 움직임
bool move_up = false;
int move_option = 0;
int movingSwapOption = 0;
float movingSpeed = 0.05f;

glm::mat4 model = glm::mat4(1.0f);

GLfloat rotate_X = -30.0f;
GLfloat rotate_Y = -30.0f;

GLfloat moveDir = 1.0f;

void Keyboard(unsigned char key, int x, int y);
void KeyboardSpecial(int key, int x, int y);

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

	InitProgram(ShaderProgram);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid Init()
{
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	Reset();
}

GLvoid Reset()
{
	ObjMgr.Reset();

	ObjMgr.CreateCoordinate();
	ObjMgr.CreateCube();
	ObjMgr.CreateSqhere();

	ObjMgr.SetPosition(1, 0.0f, 0.0f, 0.5f);
	ObjMgr.SetPosition(2, 0.0f, 0.0f, -0.5f);
	ObjMgr.SetAllRotate(-30.0f, -30.0f, 0.0f);
	ObjMgr.SetAllScale(0.3f, 0.4f, 0.3f);
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	if (isDepthTest)	glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	for (int i = 0; i < ObjMgr.m_ObjectList.size(); i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			if (ObjMgr.m_ObjectList[i].m_isModeIDX)
			{
				DrawObjectByIDX(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_pos, ObjMgr.m_ObjectList[i].m_inex, ObjMgr.m_ObjectList[i].m_col,
					ObjMgr.m_ObjectList[i].m_pivot, ObjMgr.m_ObjectList[i].m_rotate, ObjMgr.m_ObjectList[i].m_scale,
					ObjMgr.m_ObjectList[i].m_num_vertex, ObjMgr.m_ObjectList[i].m_size_pos, ObjMgr.m_ObjectList[i].m_size_idx, ObjMgr.m_ObjectList[i].m_model,
					i);
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

	rot = glm::rotate(rot, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	model = model * scale * move * rot;

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
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, float* modelInfo, int idx)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_IDX, obj_index, GL_STATIC_DRAW);

	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");

	glm::mat4 model2 = glm::mat4(1.0f);
	glm::mat4 model1 = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(scaleInfo[0], scaleInfo[1], scaleInfo[2]));

	rot = glm::rotate(rot, glm::radians(rotateInfo[1]), glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, glm::radians(rotateInfo[0]), glm::vec3(0.0f, 1.0f, 0.0f));

	move = glm::translate(move, glm::vec3(pivot[0], pivot[1], pivot[2]));

	model1 = rot * move * scale;
	model2 = move * rot * scale;

	// model 행렬의 요소 출력
	if (!ObjMgr.m_ObjectList[idx].m_Initmodel)
	{
		for (int j = 0; j < 4; j++) {
			ObjMgr.m_ObjectList[idx].m_model_pos[j] = model1[3][j];
			modelInfo[j] = model1[3][j];
		}
		ObjMgr.m_ObjectList[idx].m_Initmodel = true;
	}

	for (int j = 0; j < 4; j++) {
		model2[3][j] = modelInfo[j];
	}

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model1));		// 모델변환

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(DRAW_TYPE, NUM_VETEX, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid SettingSwap()
{
	for (int i = 0; i < 3; i++)
	{
		swapPivot_f1[i] = ObjMgr.m_ObjectList[1].m_pivot[i];
		swapPivot_f2[i] = ObjMgr.m_ObjectList[2].m_pivot[i];
	}
}

GLvoid StopAllAnim()
{
	if (ObjMgr.m_ObjectList.size() < 2) return;

	for (int i = 1; i < ObjMgr.m_ObjectList.size(); i++)
	{
		ObjMgr.m_ObjectList[i].m_isAnimRotating = false;
	}

	Sleep(500);

	return;
}

GLvoid MovingOriginAnim(int idx)
{
	bool isDone = false;

	float x = ObjMgr.m_ObjectList[idx].m_pivot[0];
	float y = ObjMgr.m_ObjectList[idx].m_pivot[1];
	float z = ObjMgr.m_ObjectList[idx].m_pivot[2];

	// 현재 위치에서 원점으로 이동
	GLfloat distance = sqrt(x * x + y * y + z * z);
	if (distance > moveOffset) {
		ObjMgr.m_ObjectList[idx].m_pivot[0] -= (x / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[1] -= (y / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[2] -= (z / distance) * movingSpeed;
	}
	else
	{
		isDone = true;
	}

	glutPostRedisplay();

	if (isDone == false) glutTimerFunc(30, MovingOriginAnim, idx);
	if (isDone)
	{
		if(movingSwapOption == 0) glutTimerFunc(30, MovingDirectOriginAnim, idx);
		else if(movingSwapOption == 1) glutTimerFunc(30, MovingDirectSwapAnim, idx);
	}
}

GLvoid MovingDirectOriginAnim(int idx)
{
	bool isSwap = false;

	float targetX = (idx == 1) ? swapPivot_f1[0] : swapPivot_f2[0];
	float targetY = (idx == 1) ? swapPivot_f1[1] : swapPivot_f2[1];
	float targetZ = (idx == 1) ? swapPivot_f1[2] : swapPivot_f2[2];

	float x = ObjMgr.m_ObjectList[idx].m_pivot[0] - targetX;
	float y = ObjMgr.m_ObjectList[idx].m_pivot[1] - targetY;
	float z = ObjMgr.m_ObjectList[idx].m_pivot[2] - targetZ;

	// 현재 위치에서 원래 위치로 이동
	GLfloat distance = sqrt(x * x + y * y + z * z);
	if (distance > moveOffset) {
		ObjMgr.m_ObjectList[idx].m_pivot[0] -= (x / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[1] -= (y / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[2] -= (z / distance) * movingSpeed;
	}
	else
	{
		isSwap = true;
	}

	glutPostRedisplay();

	if (isSwap == false) glutTimerFunc(30, MovingDirectOriginAnim, idx);
}

GLvoid MovingDirectSwapAnim(int idx)
{
	bool isSwap = false;

	float targetX = (idx == 1) ? swapPivot_f2[0] : swapPivot_f1[0];
	float targetY = (idx == 1) ? swapPivot_f2[1] : swapPivot_f1[1];
	float targetZ = (idx == 1) ? swapPivot_f2[2] : swapPivot_f1[2];

	float x = ObjMgr.m_ObjectList[idx].m_pivot[0] - targetX;
	float y = ObjMgr.m_ObjectList[idx].m_pivot[1] - targetY;
	float z = ObjMgr.m_ObjectList[idx].m_pivot[2] - targetZ;

	// 현재 위치에서 상대 위치로 이동
	GLfloat distance = sqrt(x * x + y * y + z * z);
	if (distance > moveOffset) {
		ObjMgr.m_ObjectList[idx].m_pivot[0] -= (x / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[1] -= (y / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[2] -= (z / distance) * movingSpeed;
	}
	else
	{
		isSwap = true;
	}

	glutPostRedisplay();

	if (isSwap == false) glutTimerFunc(30, MovingDirectSwapAnim, idx);
}

GLvoid MovingUpDownSwapAnim(int idx)
{
	bool isSwap = false;

	float targetUp = (idx == 1) ? 0.5f : -0.5f;

	float x = ObjMgr.m_ObjectList[idx].m_pivot[0];
	float y = ObjMgr.m_ObjectList[idx].m_pivot[1] + targetUp;
	float z = ObjMgr.m_ObjectList[idx].m_pivot[2];

	// 현재 위치에서 상대 위치로 이동
	GLfloat distance = sqrt(x * x + y * y + z * z);
	if (distance > moveOffset) {
		ObjMgr.m_ObjectList[idx].m_pivot[0] -= (x / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[1] -= (y / distance) * movingSpeed;
		ObjMgr.m_ObjectList[idx].m_pivot[2] -= (z / distance) * movingSpeed;
	}
	else
	{
		isSwap = true;
	}

	glutPostRedisplay();

	if (isSwap == false) glutTimerFunc(30, MovingUpDownSwapAnim, idx);
	if (isSwap)
	{
		glutTimerFunc(30, MovingDirectSwapAnim, idx);
	}
}

GLvoid MoveObjects(bool isX, float dir)
{
	if (move_up)
	{
		if (isX == false)
		{
			if (move_option == 0)
			{
				// 모두 이동
				ObjMgr.SetPosition(1, ObjMgr.m_ObjectList[1].m_pivot[0], ObjMgr.m_ObjectList[1].m_pivot[1] + dir, ObjMgr.m_ObjectList[1].m_pivot[2]);
				ObjMgr.SetPosition(2, ObjMgr.m_ObjectList[2].m_pivot[0], ObjMgr.m_ObjectList[2].m_pivot[1] + dir, ObjMgr.m_ObjectList[2].m_pivot[2]);
			}
			else if (move_option == 1)
			{
				// 큐브만 이동
				ObjMgr.SetPosition(1, ObjMgr.m_ObjectList[1].m_pivot[0], ObjMgr.m_ObjectList[1].m_pivot[1] + dir, ObjMgr.m_ObjectList[1].m_pivot[2]);
			}
			else if (move_option == 2)
			{
				// 구만 이동
				ObjMgr.SetPosition(2, ObjMgr.m_ObjectList[2].m_pivot[0], ObjMgr.m_ObjectList[2].m_pivot[1] + dir, ObjMgr.m_ObjectList[2].m_pivot[2]);
			}
		}
	}
	else
	{
		if (move_option == 0)
		{
			// 모두 이동
			if (isX)
			{
				ObjMgr.SetPosition(1, ObjMgr.m_ObjectList[1].m_pivot[0] + dir, ObjMgr.m_ObjectList[1].m_pivot[1], ObjMgr.m_ObjectList[1].m_pivot[2]);
				ObjMgr.SetPosition(2, ObjMgr.m_ObjectList[2].m_pivot[0] + dir, ObjMgr.m_ObjectList[2].m_pivot[1], ObjMgr.m_ObjectList[2].m_pivot[2]);
			}
			else
			{
				ObjMgr.SetPosition(1, ObjMgr.m_ObjectList[1].m_pivot[0], ObjMgr.m_ObjectList[1].m_pivot[1], ObjMgr.m_ObjectList[1].m_pivot[2] + dir);
				ObjMgr.SetPosition(2, ObjMgr.m_ObjectList[2].m_pivot[0], ObjMgr.m_ObjectList[2].m_pivot[1], ObjMgr.m_ObjectList[2].m_pivot[2] + dir);
			}
		}
		else if (move_option == 1)
		{
			// 큐브만 이동
			if (isX) ObjMgr.SetPosition(1, ObjMgr.m_ObjectList[1].m_pivot[0] + dir, ObjMgr.m_ObjectList[1].m_pivot[1], ObjMgr.m_ObjectList[1].m_pivot[2]);
			else ObjMgr.SetPosition(1, ObjMgr.m_ObjectList[1].m_pivot[0], ObjMgr.m_ObjectList[1].m_pivot[1], ObjMgr.m_ObjectList[1].m_pivot[2] + dir);
		}
		else if (move_option == 2)
		{
			// 구만 이동
			if (isX) ObjMgr.SetPosition(2, ObjMgr.m_ObjectList[2].m_pivot[0] + dir, ObjMgr.m_ObjectList[2].m_pivot[1], ObjMgr.m_ObjectList[2].m_pivot[2]);
			else ObjMgr.SetPosition(2, ObjMgr.m_ObjectList[2].m_pivot[0], ObjMgr.m_ObjectList[2].m_pivot[1], ObjMgr.m_ObjectList[2].m_pivot[2] + dir);
		}
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'A':
	case 'a':
		move_up = false;
		move_option = 0;
		break;
	case 'S':
	case 's':
		move_up = false;
		move_option = 1;
		break;
	case 'D':
	case 'd':
		move_up = false;
		move_option = 2;
		break;
	case 'W':
	case 'w':
		move_up = true;
		break;
	case 'R':
	case 'r':
		break;
	case 'T':
	case 't':
		SettingSwap();
		movingSwapOption = 0;
		glutTimerFunc(30, MovingOriginAnim, 1);
		glutTimerFunc(30, MovingOriginAnim, 2);
		break;
	case '1':
		SettingSwap();
		movingSwapOption = 1;
		glutTimerFunc(30, MovingOriginAnim, 1);
		glutTimerFunc(30, MovingOriginAnim, 2);
		break;
	case '2':
		SettingSwap();
		StopAllAnim();
		ObjMgr.m_ObjectList[1].m_isAnimRotating = true;
		ObjMgr.m_ObjectList[2].m_isAnimRotating = true;
		break;
	case '3':
		SettingSwap();
		glutTimerFunc(30, MovingUpDownSwapAnim, 1);
		glutTimerFunc(30, MovingUpDownSwapAnim, 2);
		break;
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

void KeyboardSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		MoveObjects(false, 0.1f);
		break;
	case GLUT_KEY_DOWN:
		MoveObjects(false, -0.1f);
		break;
	case GLUT_KEY_LEFT:
		MoveObjects(true, -0.1f);
		break;
	case GLUT_KEY_RIGHT:
		MoveObjects(true, 0.1f);
		break;
	}

	glutPostRedisplay();
}