#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS

#define MAX_NUM_OBJECT 15

#define BYTE_SIZE_POINT 12
#define BYTE_SIZE_LINE 24
#define BYTE_SIZE_TRIANGLE 36
#define BYTE_SIZE_RECTANGLE 72
#define BYTE_SIZE_PENTAGON 108

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

GLvoid Reset();
GLvoid ResetPivot();
void StopAllAnim();
void ResetAllShape();
void DrawAllPoint();
void DrawAllLine();
void DrawAllTriangle();
void DrawAllRectangle();
void DrawAllPentagon();

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

// 점
GLfloat pointShape[MAX_NUM_OBJECT][3];
GLfloat colorPoint[MAX_NUM_OBJECT][3];
GLvoid SetPointPos(int shape_idx, int pivot_idx);
bool isChangingToPoint = false;

// 선
GLfloat lineShape[MAX_NUM_OBJECT][2][3];
GLfloat colorLine[MAX_NUM_OBJECT][2][3];
GLvoid SetLinePos(int idx, int pivot_idx);
bool isChangingToLine = false;

// 삼각형
GLfloat triShape[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL]; //--- 삼각형 위치 값
GLfloat triShapeScale[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
GLfloat colorTri[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
GLvoid SetTrianglePos(int shape_idx, int pivot_idx);
bool isChangingToTri = false;

// 사각형
GLfloat rectShape[MAX_NUM_OBJECT][6][3];
GLfloat rectShapeScale[MAX_NUM_OBJECT][6][3];
GLfloat colorRect[MAX_NUM_OBJECT][6][3];
GLvoid SetRectanglePos(int shape_idx, int pivot_idx);
bool isChangingToRect = false;

// 오각형
GLfloat pentaShape[MAX_NUM_OBJECT][9][3];
GLfloat pentaShapeScale[MAX_NUM_OBJECT][9][3];
GLfloat colorPenta[MAX_NUM_OBJECT][9][3];
GLvoid SetPentagonPos(int shape_idx, int pivot_idx);
bool isChangingToPenta = false;

GLuint vao, vbo[2], ebo[2];

// 피봇
vector<vector<float>> pivot_list;
vector<float> pivot;

// 글로벌 변수
bool isActive[4] = { false, false, false, false };
int g_cur_area = 0;
bool g_left_button = false;
int CONDITION = 1;
int NUM_POINT = 0;
int NUM_LINE = 0;
int NUM_TRIANGLE = 0;
int NUM_RECTANGLE = 0;
int NUM_PENTAGON = 0;

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
	Reset();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseClick);
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

	// 각 사분면에 있는 도형 그리기
	DrawAllPoint();
	DrawAllLine();
	DrawAllTriangle();
	DrawAllRectangle();
	DrawAllPentagon();

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

GLvoid Reset()
{
	CONDITION = 0;
	NUM_POINT = 3;
	NUM_LINE = 3;
	NUM_TRIANGLE = 3;
	NUM_RECTANGLE = 3;
	NUM_PENTAGON = 3;

	// 15개의 피봇점을 랜덤하게 생성한 후 점/선/삼각형/사각형/오각형 각각 그리기
	ResetPivot();

	StopAllAnim();
	ResetAllShape();

	for (int idx = 0; idx < 3; idx++)
	{
		int pivot_idx = idx * 5;
		SetPointPos(idx, pivot_idx);
		SetLinePos(idx, pivot_idx+1);
		SetTrianglePos(idx, pivot_idx + 2);
		SetRectanglePos(idx, pivot_idx + 3);
		SetPentagonPos(idx, pivot_idx + 4);
	}
}

GLvoid ResetPivot()
{
	pivot_list.clear();

	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		float x = GetRandomFloatValue(-1.0f, 1.0f);
		float y = GetRandomFloatValue(-1.0f, 1.0f);

		pivot.clear();
		pivot.emplace_back(x);
		pivot.emplace_back(y);
		pivot_list.emplace_back(pivot);
	}
}

void ResetAllShape()
{
	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		colorPoint[i][0] = 1.f;
		colorPoint[i][1] = 0.f;
		colorPoint[i][2] = 0.f;
		for (int j = 0; j < 3; j++)
		{
			pointShape[i][j] = 0.f;
			if (j == 0)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;

				colorLine[i][j][0] = 0.f;
				colorLine[i][j][1] = 0.f;
				colorLine[i][j][2] = 1.f;

				triShape[i][j][0] = -0.1f;
				triShape[i][j][1] = -0.1f;

				triShapeScale[i][j][0] = -0.1f;
				triShapeScale[i][j][1] = -0.1f;

				colorTri[i][j][0] = 1.0f;
				colorTri[i][j][1] = 1.0f;
				colorTri[i][j][2] = 0.0f;
			}
			else if (j == 1)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;

				colorLine[i][j][0] = 0.f;
				colorLine[i][j][1] = 0.f;
				colorLine[i][j][2] = 1.f;

				triShape[i][j][0] = 0.1f;
				triShape[i][j][1] = -0.1f;

				triShapeScale[i][j][0] = 0.1f;
				triShapeScale[i][j][1] = -0.1f;

				colorTri[i][j][0] = 1.0f;
				colorTri[i][j][1] = 1.0f;
				colorTri[i][j][2] = 0.0f;
			}
			else if (j == 2)
			{
				triShape[i][j][0] = 0.f;
				triShape[i][j][1] = 0.1f;

				triShapeScale[i][j][0] = 0.f;
				triShapeScale[i][j][1] = 0.1f;

				colorTri[i][j][0] = 1.0f;
				colorTri[i][j][1] = 1.0f;
				colorTri[i][j][2] = 0.0f;
			}

			triShape[i][j][2] = 0.f;
			triShapeScale[i][j][2] = 0.f;
		}

		// 사각형
		for (int j = 0; j < 6; j++)
		{
			if (j == 0)
			{
				rectShape[i][j][0] = 0.1f;
				rectShape[i][j][1] = 0.1f;

				rectShapeScale[i][j][0] = 0.1f;
				rectShapeScale[i][j][1] = 0.1f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}
			else if (j == 1 || j == 3)
			{
				rectShape[i][j][0] = 0.1f;
				rectShape[i][j][1] = -0.1f;

				rectShapeScale[i][j][0] = 0.1f;
				rectShapeScale[i][j][1] = -0.1f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}
			else if (j == 2 || j == 5)
			{
				rectShape[i][j][0] = -0.1f;
				rectShape[i][j][1] = 0.1f;

				rectShapeScale[i][j][0] = -0.1f;
				rectShapeScale[i][j][1] = 0.1f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}
			else if (j == 4)
			{
				rectShape[i][j][0] = -0.1f;
				rectShape[i][j][1] = -0.1f;

				rectShapeScale[i][j][0] = -0.1f;
				rectShapeScale[i][j][1] = -0.1f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}

			rectShape[i][j][2] = 0.f;
		}

		// 오각형
		for (int j = 0; j < 9; j++)
		{
			// 색상
			colorPenta[i][j][0] = 1.0f;
			colorPenta[i][j][1] = 0.0f;
			colorPenta[i][j][2] = 0.0f;

			if (j == 0)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = 0.f;
				pentaShapeScale[i][j][1] = 0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 1 || j == 4)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = -0.1f;
				pentaShapeScale[i][j][1] = 0.03f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 2 || j == 3 || j == 7)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = 0.1f;
				pentaShapeScale[i][j][1] = 0.03f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 5 || j == 6)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = -0.06f;
				pentaShapeScale[i][j][1] = -0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 8)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = 0.06f;
				pentaShapeScale[i][j][1] = -0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
		}
	}
}

GLvoid SetPointPos(int shape_idx, int pivot_idx)
{
	pointShape[shape_idx][0] = 0.0f;
	pointShape[shape_idx][1] = 0.0f;
}

GLvoid SetLinePos(int shape_idx, int pivot_idx)
{
	lineShape[shape_idx][0][0] = pivot_list[pivot_idx][0] - 0.1f;
	lineShape[shape_idx][0][1] = pivot_list[pivot_idx][1] - 0.06f;
	lineShape[shape_idx][1][0] = pivot_list[pivot_idx][0] + 0.1f;
	lineShape[shape_idx][1][1] = pivot_list[pivot_idx][1] + 0.06f;
}

GLvoid SetTrianglePos(int shape_idx, int pivot_idx)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0) triShape[shape_idx][i][j] = pivot_list[pivot_idx][0] - triShapeScale[shape_idx][i][j];
			if (j == 1) triShape[shape_idx][i][j] = pivot_list[pivot_idx][1] + triShapeScale[shape_idx][i][j];
		}
	}
}

GLvoid SetRectanglePos(int shape_idx, int pivot_idx)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0) rectShape[shape_idx][i][j] = pivot_list[pivot_idx][0] - rectShapeScale[shape_idx][i][j];
			if (j == 1) rectShape[shape_idx][i][j] = pivot_list[pivot_idx][1] + rectShapeScale[shape_idx][i][j];
		}
	}
}

GLvoid SetPentagonPos(int shape_idx, int pivot_idx)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0) pentaShape[shape_idx][i][j] = pivot_list[pivot_idx][0] - pentaShapeScale[shape_idx][i][j];
			if (j == 1) pentaShape[shape_idx][i][j] = pivot_list[pivot_idx][1] + pentaShapeScale[shape_idx][i][j];
		}
	}
}

void ChangeTriRandom(int idx)
{
	// 색상 변경

	GLfloat color_r = GetRandomFloatValue(0.f, 1.0f);
	GLfloat color_g = GetRandomFloatValue(0.f, 1.0f);
	GLfloat color_b = GetRandomFloatValue(0.f, 1.0f);

	GLfloat rand_size = GetRandomFloatValue(0.2f, 3.0f);

	DrawAllLine();
	DrawAllTriangle();
}

void DrawAllPoint()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_POINT * NUM_POINT, pointShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_POINT * NUM_POINT, colorPoint, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, 1 * NUM_POINT);
}

void DrawAllLine()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * NUM_LINE, lineShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * NUM_LINE, colorLine, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINES, 0, 2 * NUM_LINE);
}

void DrawAllTriangle()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_TRIANGLE, triShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_TRIANGLE, colorTri, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3 * NUM_TRIANGLE);
}

void DrawAllRectangle()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_RECTANGLE * NUM_RECTANGLE, rectShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_RECTANGLE * NUM_RECTANGLE, colorRect, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 6 * NUM_RECTANGLE);
}

void DrawAllPentagon()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_PENTAGON * NUM_PENTAGON, pentaShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_PENTAGON * NUM_PENTAGON, colorPenta, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 9 * NUM_PENTAGON);
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

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
	return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		g_left_button = true;
		//TryDrawLine();
		//TryDrawTriangle();
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_left_button = false;
	}

	glutPostRedisplay();
}

void StopAllAnim()
{

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
