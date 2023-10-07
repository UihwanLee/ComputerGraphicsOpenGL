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
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[2], EBO;
bool isDepthTest = false;

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, int NUM_VETEX, int SIZE_COL, int SIZE_IDX);

// 회전 애니메이션
bool isRotating_X = false;
bool isRotating_Y = false;
GLfloat rotate_X = 30.0f;
GLfloat rotate_Y = 30.0f;
GLvoid RotatingAnimationX(int isAinm);
GLvoid RotatingAnimationY(int isAinm);

void InputKey(unsigned char key, int x, int y);

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
	glutKeyboardFunc(InputKey);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid Init()
{
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	ObjMgr.CreateCoordinate();
	ObjMgr.CreateCube();
	ObjMgr.CreateSquarePyramid();
	ObjMgr.SetChangeActive(0);
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	if(isDepthTest)	glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	for (int i = 0; i < ObjMgr.m_ObjectList.size(); i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			if (ObjMgr.m_ObjectList[i].m_isModeIDX)
			{
				DrawObjectByIDX(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_pos, ObjMgr.m_ObjectList[i].m_inex, ObjMgr.m_ObjectList[i].m_col,
					ObjMgr.m_ObjectList[i].m_num_vertex, ObjMgr.m_ObjectList[i].m_size_pos, ObjMgr.m_ObjectList[i].m_size_idx);
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

	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환


	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, colList, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_LINES, 0, NUM_VETEX);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, int NUM_VETEX, int SIZE_COL, int SIZE_IDX)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_IDX, obj_index, GL_STATIC_DRAW);

	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(0.5f, 0.6f, 0.5f));

	rot = glm::rotate(rot, glm::radians(rotate_Y), glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, glm::radians(rotate_X), glm::vec3(0.0f, 1.0f, 0.0f));

	move = glm::translate(move, glm::vec3(0.0f, 0.0f, 0.0f));

	model = model * scale * rot * move;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

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

GLvoid RotatingAnimationX(int dir)
{
	if (dir == 0) rotate_X += 5.0f;
	else rotate_X -= 5.0f;

	glutPostRedisplay();

	if (isRotating_X) glutTimerFunc(30, RotatingAnimationX, dir);
}

GLvoid RotatingAnimationY(int dir)
{
	if (dir == 0) rotate_Y += 5.0f;
	else rotate_Y -= 5.0f;

	glutPostRedisplay();

	if (isRotating_Y) glutTimerFunc(30, RotatingAnimationY, dir);
}

GLvoid StopAllAnim()
{
	isRotating_X = false;
	isRotating_Y = false;

	Sleep(1000);

	return;
}

GLvoid Reset()
{
	isRotating_X = false;
	isRotating_Y = false;
	rotate_X = 30.0f;
	rotate_Y = 30.0f;
	ObjMgr.Reset();
}

void InputKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'C':
	case 'c':
		ObjMgr.SetChangeActive(0);
		break;
	case 'P':
	case 'p':
		ObjMgr.SetChangeActive(1);
		break;
	case 'H':
	case 'h':
		if (isDepthTest) isDepthTest = false;
		else isDepthTest = true;
		break;
	case 'w':
	case 'W':
		ObjMgr.ChangeWireSolidType();
		break;
	case 'X':
		StopAllAnim();
		isRotating_X = true;
		if(isRotating_X) glutTimerFunc(30, RotatingAnimationX, 1);
		break;
	case 'x':
		StopAllAnim();
		isRotating_X = true;
		glutTimerFunc(30, RotatingAnimationX, 0);
		break;
	case 'Y':
		StopAllAnim();
		isRotating_Y = true;
		glutTimerFunc(30, RotatingAnimationY, 1);
		break;
	case 'y':
		StopAllAnim();
		isRotating_Y = true;
		glutTimerFunc(30, RotatingAnimationY, 0);
		break;
	case 'S':
	case 's':
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