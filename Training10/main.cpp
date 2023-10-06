#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <random>
#include <vector>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define _CRT_SECURE_NO_WARNINGS

#define MAX_NUM_OBJECT 1000
#define BYTE_SIZE_POINT 12
#define BYTE_SIZE_LINE 24

#define MAX_TRI_ROW 3
#define MAX_TRI_COL 3

#define PI 3.14159265

using namespace std;

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);

// �Է� ó�� �Լ�
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);

void InitBuffer();
void DrawAllPoint();
void DrawAllLine();
void MakeSpiralAnim(int isAnim);
float map(float value, float fromLow, float fromHigh, float toLow, float toHigh);
void Reset();

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

// �����̷� ��ġ
GLfloat posSpiral_x[MAX_NUM_OBJECT][3];
GLfloat posSpiral_y[MAX_NUM_OBJECT][3];
GLfloat radius = 0.1f;
GLfloat spiralRadius = 0.1f;
int spiralCount = 0;
int spiralDeg = 0;

// Trace
vector < vector <float> > pivot_list;
vector<float> p;
int pivot_list_size = 0;

// �Ǻ�
GLfloat pivot[2] = { 0.0f, 0.0f };
int lineAmount = 500;
int count = 0;
GLfloat piMultiplier = 10.0f * PI;

// ��
GLfloat pointShape[MAX_NUM_OBJECT][3];
GLfloat colorPoint[MAX_NUM_OBJECT][3];

// ��
GLfloat lineShape[MAX_NUM_OBJECT][2][3];
GLfloat colorLine[MAX_NUM_OBJECT][2][3];

GLuint vao, vbo[2], ebo[2];
GLfloat curPos[3] = { 0.f, 0.f, 0.f };

// �۷ι� ����
bool g_left_button = false;
int CONDITION = 1;
int NUM_POINT = 0;
int NUM_LINE = 0;
bool isMakingSpiral = false;

int cycles = 3;
float res = 0.2;
float t = res;
bool isArrival = false;
int spiralMaxNum = 1;
int spiralNum = 0;

float rColor = 0.0f;
float gColor = 0.0f;
float bColor = 0.0f;


int main(int argc, char** argv)
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseClick);
	glutMainLoop();
}

GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	////glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	////--- ����� VAO �ҷ�����
	glBindVertexArray(vao);

	////--- ������Ʈ �׸���
	glPointSize(5.0);
	DrawAllPoint();
	DrawAllLine();

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

GLvoid ChangeRandomBG()
{
	rColor = GetRandomFloatValue(0.0f, 1.0f);
	gColor = GetRandomFloatValue(0.0f, 1.0f);
	bColor = GetRandomFloatValue(0.0f, 1.0f);
}

void InitBuffer()
{
	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pointShape[i][j] = 0.f;
			colorPoint[i][j] = 1.0f;

			posSpiral_x[i][j] = 0.f;
			posSpiral_y[i][j] = 0.f;

			if (j == 0)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;
				
				colorLine[i][j][0] = 1.0f;
				colorLine[i][j][1] = 1.0f;
				colorLine[i][j][2] = 1.0f;
				
			}
			else if (j == 1)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;
				
				colorLine[i][j][0] = 1.0f;
				colorLine[i][j][1] = 1.0f;
				colorLine[i][j][2] = 1.0f;
			}
		}
	}
}

void TryMakeSpiral()
{
	isMakingSpiral = true;
	if (isMakingSpiral) glutTimerFunc(30, MakeSpiralAnim, isMakingSpiral);
}

float archimedean(float t) 
{
	float r = t * 10;
	return r;
}

void MakeSpiral(float pivot_x, float pivot_y)
{
	if (isArrival == false)
	{
		if (t < 2 * PI * cycles)
		{
			if (CONDITION == 1) NUM_POINT += 1;
			else NUM_LINE += 1;

			spiralRadius = archimedean(t);
			float x = pivot_x + spiralRadius * (cos(t)) / 1000;
			float y = pivot_y + spiralRadius * (sin(t)) / 1000;

			if (CONDITION == 1)
			{
				pointShape[NUM_POINT - 1][0] = x;
				pointShape[NUM_POINT - 1][1] = y;
			}
			else
			{
				lineShape[NUM_LINE - 1][0][0] = x;
				lineShape[NUM_LINE - 1][0][1] = y + 0.01f;
				lineShape[NUM_LINE - 1][1][0] = x;
				lineShape[NUM_LINE - 1][1][1] = y - 0.01f;;
			}

			t += res;
		}
		else
		{
			t = (2 * PI * cycles) - res;
			isArrival = true;
		}
	}

	if (isArrival == true)
	{
		if (t > res)
		{
			if (CONDITION == 1) NUM_POINT += 1;
			else NUM_LINE += 1;

			spiralRadius = archimedean(t);
			float x = pivot_x + spiralRadius * ((cos(t)) * (-1.0f)) / 1000 + 0.385f;
			float y = pivot_y + spiralRadius * ((sin(t)) * (-1.0f)) / 1000;

			if (CONDITION == 1)
			{
				pointShape[NUM_POINT - 1][0] = x;
				pointShape[NUM_POINT - 1][1] = y;
			}
			else
			{
				lineShape[NUM_LINE - 1][0][0] = x;
				lineShape[NUM_LINE - 1][0][1] = y + 0.01f;
				lineShape[NUM_LINE - 1][1][0] = x;
				lineShape[NUM_LINE - 1][1][1] = y - 0.01f;;
			}

			t -= res;
		}
		else
		{
			cycles = 3;
			res = 0.2;
			t = res;
			isArrival = false;
			spiralNum += 1;
		}
	}
}

void MakeSpiralAnim(int isAnim)
{
	if (NUM_POINT > MAX_NUM_OBJECT) isMakingSpiral = false;


	//MakeSpiral(pivot[0], pivot[1]);
	MakeSpiral(pivot_list[spiralNum][0], pivot_list[spiralNum][1]);

	if (spiralNum >= spiralMaxNum)
	{
		isMakingSpiral = false;
	}

	DrawAllPoint();

	glutPostRedisplay();

	if(isMakingSpiral) glutTimerFunc(30, MakeSpiralAnim, isMakingSpiral);
}

void TryDrawPoint()
{
	if (CONDITION != 1) return;

	DrawAllPoint();
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

void TryDrawLine()
{
	if (CONDITION != 2) return;

	DrawAllLine();
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

	glDrawArrays(GL_LINE_LOOP, 0, 2 * NUM_LINE);
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
	if(!result)
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

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
	return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}

void MakePivotList(float initX, float initY)
{
	pivot_list.clear();
	p.clear();
	p.emplace_back(initX);
	p.emplace_back(initY);

	pivot_list.emplace_back(p);

	for (int i = 0; i < spiralMaxNum - 1; i++)
	{
		float x = GetRandomFloatValue(-1.0f, 1.0f);
		float y = GetRandomFloatValue(-1.0f, 1.0f);

		p.clear();
		p.emplace_back(x);
		p.emplace_back(y);

		pivot_list.emplace_back(p);
	}
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		curPos[0] = map(x, 0.0f, 800.0f, -1.0f, 1.0f);
		curPos[1] = map(y, 600.0f, 0.0f, -1.0f, 1.0f);
		g_left_button = true;

		if (isMakingSpiral == false)
		{
			Reset();
			pivot[0] = curPos[0];
			pivot[1] = curPos[1];
			ChangeRandomBG();
			MakePivotList(pivot[0], pivot[1]);
			TryMakeSpiral();
		}
 	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid Reset()
{
	NUM_POINT = 0;
	NUM_LINE = 0;
	cycles = 3;
	res = 0.2;
	t = res;
	isArrival = false;
	spiralNum = 0;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p':
		CONDITION = 1;
		break;
	case 'l':
		CONDITION = 2;
		break;
	case '1':
		Reset();
		spiralMaxNum = 1;
		break;
	case '2':
		Reset();
		spiralMaxNum = 2;
		break;
	case '3':
		Reset();
		spiralMaxNum = 3;
		break;
	case '4':
		Reset();
		spiralMaxNum = 4;
		break;
	case '5':
		Reset();
		spiralMaxNum = 5;
		break;
	case 'c':
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
