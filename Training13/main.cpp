#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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

// 입력 처리 함수
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid MouseDrag(int x, int y);

GLvoid InitBuffer();
GLvoid Reset();
void ResetAllShape();
void DrawObjects(int DRAW_TYPE, int BYTE_SIZE, int vertex, int NUM, void* posList, void* colList);

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

// 좌표평면
GLfloat coordinatePlane[4][3];
GLfloat coordinatePlane_Color[4][3];

// 사각형
GLfloat rectShape[4][3];
GLfloat rectShapeScale[4][3];
GLfloat colorRect[4][3];

GLuint vao, vbo[2], ebo[2];

// 글로벌 변수
int g_cur_area = 0;
int CONDITION = 1;
int NUM_OBJECT = 0;
int NUM_RECTANGLE = 0;
bool is_click_rect;
float pivot[2] = { 0.f, 0.f };

// 마우스
bool g_left_button = false;
float mx, my;
int cur_obj_idx = -1;
int pre_obj_idx = -1;

int main(int argc, char** argv)
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
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
	//--- 변경된 배경색 설정
	//glClearColor(rColor, gColor, bColor, 1.0f);
	glClearColor(1.0, 1.0, 1.0, 1.0f);

	////glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	////--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	// 좌표평면 그리기
	DrawObjects(GL_LINES, BYTE_SIZE_LINE, 2, 2, coordinatePlane, coordinatePlane_Color);

	// 사각형 도형 그리기
	DrawObjects(GL_LINE_LOOP, 60, 4, 1, rectShape, colorRect);

	glutSwapBuffers(); //--- 화면에 출력하기
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
	// 좌표평면 갱신
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)			InitBufferByIdx(coordinatePlane, i, -1.0f, 0.f, 0.f);
		else if (i == 1)	InitBufferByIdx(coordinatePlane, i, 1.0f, 0.f, 0.f);
		else if (i == 2)	InitBufferByIdx(coordinatePlane, i, 0.f, -1.0f, 0.f);
		else if (i == 3)	InitBufferByIdx(coordinatePlane, i, 0.f, 1.0f, 0.f);

		InitBufferByIdx(coordinatePlane_Color, i, 0.f, 0.f, 0.f);
	}

	// 사각형
	InitBufferByIdx(rectShape, 0, -0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShape, 1, 0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShape, 2, 0.5f, 0.5f, 0.f);
	InitBufferByIdx(rectShape, 3, -0.5f, 0.5f, 0.f);

	InitBufferByIdx(rectShapeScale, 0, -0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShapeScale, 1, 0.5f, -0.5f, 0.f);
	InitBufferByIdx(rectShapeScale, 2, 0.5f, 0.5f, 0.f);
	InitBufferByIdx(rectShapeScale, 3, -0.5f, 0.5f, 0.f);

	// 색상
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
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}


void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

float GetClickPos(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
	return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}

// 두 점 사이의 거리를 계산하는 함수
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
		// 마우스 드래그에 따른 꼭짓점 이동
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
