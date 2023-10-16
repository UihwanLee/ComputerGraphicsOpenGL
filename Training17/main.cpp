#include <iostream>
#include "OpenGL.h"
#include <random>
#include "InitShader.h"
#include "ObjectManager.h"

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

// 회전 애니메이션
bool isRotating_X = false;
bool isRotating_Y = false;

bool isRotating = true;
bool isFirst = true;

// 움직임
int move_option = 0;

glm::mat4 model = glm::mat4(1.0f);

GLfloat rotate_X = -30.0f;
GLfloat rotate_Y = -30.0f;
GLvoid RotatingAnimationX(int isAinm);
GLvoid RotatingAnimationY(int isAinm);
GLfloat moveDir = 0.5f;

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

GLvoid RotatingAnimationX(int idx)
{
	ObjMgr.SetRotate(idx, ObjMgr.m_ObjectList[idx].m_rotate[0] + moveDir, ObjMgr.m_ObjectList[idx].m_rotate[1], ObjMgr.m_ObjectList[idx].m_rotate[2]);

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, idx);
}

GLvoid RotatingAnimationY(int idx)
{
	ObjMgr.SetRotate(idx, ObjMgr.m_ObjectList[idx].m_rotate[0], ObjMgr.m_ObjectList[idx].m_rotate[1], ObjMgr.m_ObjectList[idx].m_rotate[2] + moveDir);

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationY, idx);
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

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'A':
	case 'a':
		move_option = 0;
		break;
	case 'S':
	case 's':
		move_option = 1;
		break;
	case 'D':
	case 'd':
		move_option = 2;
		break;
	case 'R':
	case 'r':
		moveDir = (key == 'R') ? 0.5f : -0.5f;
		isRotating = false;
		StopAllAnim();
		ObjMgr.m_ObjectList[1].m_isAnimRotating = true;
		ObjMgr.m_ObjectList[2].m_isAnimRotating = true;
		if (ObjMgr.m_ObjectList[1].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, 1);
		if (ObjMgr.m_ObjectList[2].m_isAnimRotating) glutTimerFunc(30, RotatingAnimationX, 2);
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

GLvoid MoveObjects(bool isX, float dir)
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