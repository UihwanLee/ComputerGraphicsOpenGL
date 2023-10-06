#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define _CRT_SECURE_NO_WARNINGS

#define MAX_NUM_OBJECT 1

#define BYTE_SIZE_POINT 12
#define BYTE_SIZE_LINE 24
#define BYTE_SIZE_TRIANGLE 36
#define BYTE_SIZE_RECTANGLE 72
#define BYTE_SIZE_PENTAGON 108
#define BYTE_SIZE_HEXAGON 144

#define MAX_TRI_ROW 3
#define MAX_TRI_COL 3

#define WIDTH 800
#define HEIGHT 600

using namespace std;

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);

// �Է� ó�� �Լ�
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid MouseDrag(int x, int y);

GLvoid InitBuffer();
GLvoid Reset();
void ResetAllShape();
void DrawObjects(int DRAW_TYPE, int BYTE_SIZE, int vertex, int NUM, void* posList, void* colList);

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

// ��ǥ���
GLfloat coordinatePlane[4][3];
GLfloat coordinatePlane_Color[4][3];

// �簢��
GLfloat rectShape[4][3];
GLfloat rectShapeScale[4][3];
GLfloat colorRect[4][3];

GLuint vao, vbo[2], ebo[2];

// �۷ι� ����
int g_cur_area = 0;
int CONDITION = 1;
int NUM_OBJECT = 0;
int NUM_RECTANGLE = 0;
bool is_click_rect;
float pivot[2] = { 0.f, 0.f };

// ���콺
bool g_left_button = false;
float mx, my;
int cur_obj_idx = -1;
int pre_obj_idx = -1;

int main(int argc, char** argv)
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	Reset();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutMainLoop();
}

GLvoid drawScene()
{
	//--- ����� ���� ����
	//glClearColor(rColor, gColor, bColor, 1.0f);
	glClearColor(1.0, 1.0, 1.0, 1.0f);

	////glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	////--- ����� VAO �ҷ�����
	glBindVertexArray(vao);

	// ��ǥ��� �׸���
	DrawObjects(GL_LINES, BYTE_SIZE_LINE, 2, 2, coordinatePlane, coordinatePlane_Color);

	// �簢�� ���� �׸���
	DrawObjects(GL_LINE_LOOP, 60, 4, 1, rectShape, colorRect);

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

int GetRandomIntValue(GLfloat min, GLfloat max)
{
	GLfloat value;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<GLfloat> dis(min, max);

	value = dis(gen);

	return int(value);
}

GLfloat GetRandomFloatValue(GLfloat min, GLfloat max)
{
	GLfloat value;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<GLfloat> dis(min, max);

	value = dis(gen);

	return value;
}

GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);
}

GLvoid Reset()
{
	ResetAllShape();
}


GLvoid InitBufferByIdx(float buffer[][3], int i, float x, float y, float z)
{
	buffer[i][0] = x;
	buffer[i][1] = y;
	buffer[i][2] = z;
}

void ResetAllShape()
{
	// ��ǥ��� ����
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)			InitBufferByIdx(coordinatePlane, i, -1.0f, 0.f, 0.f);
		else if (i == 1)	InitBufferByIdx(coordinatePlane, i, 1.0f, 0.f, 0.f);
		else if (i == 2)	InitBufferByIdx(coordinatePlane, i, 0.f, -1.0f, 0.f);
		else if (i == 3)	InitBufferByIdx(coordinatePlane, i, 0.f, 1.0f, 0.f);

		InitBufferByIdx(coordinatePlane_Color, i, 0.f, 0.f, 0.f);
	}

	// �簢��
	InitBufferByIdx(rectShape, 0, -0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShape, 1, 0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShape, 2, 0.5f, 0.5f, 0.f);
	InitBufferByIdx(rectShape, 3, -0.5f, 0.5f, 0.f);

	InitBufferByIdx(rectShapeScale, 0, -0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShapeScale, 1, 0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShapeScale, 2, 0.5f, 0.5f, 0.f);
	InitBufferByIdx(rectShapeScale, 3, -0.5f, 0.5f, 0.f);

	// ����
	InitBufferByIdx(colorRect, 0, 0.f, 1.0f, 0.f);
	InitBufferByIdx(colorRect, 1, 0.f, 0.f, 1.0f);
	InitBufferByIdx(colorRect, 2, 1.f, 1.f, 0.f);
	InitBufferByIdx(colorRect, 3, 1.f, 0.f, 0.f);
}

GLvoid UpdateVertex(int idx, float x, float y)
{
	rectShapeScale[idx][0] = x;
	rectShapeScale[idx][1] = y;
}

GLvoid UpdateRect()
{
	for (int i = 0; i < 4; i++)
	{
		rectShape[i][0] = pivot[0] + rectShapeScale[i][0];
		rectShape[i][1] = pivot[1] + rectShapeScale[i][1];
	}

}


void DrawObjects(int DRAW_TYPE, int BYTE_SIZE, int vertex, int NUM, void* posList, void* colList)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE * NUM, posList, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE * NUM, colList, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(DRAW_TYPE, 0, vertex * NUM);
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fopen_s(&fptr, file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}


void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

float GetClickPos(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
	return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}

// �� �� ������ �Ÿ��� ����ϴ� �Լ�
float distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

bool CheckPointInRect()
{
	float d1 = distance(mx, my, rectShape[0][0], rectShape[0][1]);
	float d2 = distance(mx, my, rectShape[1][0], rectShape[1][1]);
	float d3 = distance(mx, my, rectShape[2][0], rectShape[2][1]);
	float d4 = distance(mx, my, rectShape[3][0], rectShape[3][1]);

	float comp_dist = distance(rectShape[0][0], rectShape[0][1], rectShape[1][0], rectShape[1][1]) + distance(rectShape[1][0], rectShape[1][1], rectShape[2][0], rectShape[2][1]) +
		distance(rectShape[2][0], rectShape[2][1], rectShape[3][0], rectShape[3][1]) + distance(rectShape[3][0], rectShape[3][1], rectShape[0][0], rectShape[0][1]);

	return (d1 + d2 + d3 + d4) <= comp_dist;
}

bool CheckCollisinWithMouse(float min_x, float min_y, float max_x, float max_y)
{
	bool isCollision = false;

	if (min_x <= mx && mx <= max_x && min_y <= my && my <= max_y)
	{
		isCollision = true;
	}

	return isCollision;
}

GLvoid CheckClickObject()
{
	cur_obj_idx = -1;
	is_click_rect = false;
	
	for (int i = 0; i < 4; i++)
	{
		if (CheckCollisinWithMouse(rectShape[i][0] - 0.01f, rectShape[i][1] - 0.01f, rectShape[i][0] + 0.01f, rectShape[i][1] + 0.01f))
		{
			cur_obj_idx = i;
			return;
		}
	}

	if (CheckPointInRect())
	{
		is_click_rect = true;
		return;
	}
}


GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		g_left_button = true;
		mx = GetClickPos(x, 0.0f, 800.0f, -1.0f, 1.0f);
		my = GetClickPos(y, 0.0f, 600.0f, 1.0f, -1.0f);
		CheckClickObject();
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_left_button = false;
		mx = GetClickPos(x, 0.0f, 800.0f, -1.0f, 1.0f);
		my = GetClickPos(y, 0.0f, 600.0f, 1.0f, -1.0f);
		CheckClickObject();
		if (cur_obj_idx != -1) UpdateVertex(cur_obj_idx, mx, my);
	}

	glutPostRedisplay();
}

GLvoid MouseDrag(int x, int y)
{
	// && g_cur_rect != -1
	if (g_left_button)
	{
		// ���콺 �巡�׿� ���� ������ �̵�
		if (cur_obj_idx != -1)
		{
			rectShape[cur_obj_idx][0] = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
			rectShape[cur_obj_idx][1] = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		}

		if (cur_obj_idx == -1 && is_click_rect)
		{
			for (int i = 0; i < 4; i++)
			{
				pivot[0] = ((2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f);
				pivot[1] = (1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT));
			}
			UpdateRect();
		}
	}

	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		Reset();
		break;
	case 'q':
		exit(EXIT_FAILURE);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
