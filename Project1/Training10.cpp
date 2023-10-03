#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

#define MAX_NUM_OBJECT 10
#define BYTE_SIZE_POINT 12
#define BYTE_SIZE_LINE 24
#define BYTE_SIZE_TRIANGLE 36
#define BYTE_SIZE_RECTANGLE 72

#define MAX_TRI_ROW 3
#define MAX_TRI_COL 3

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
void DrawAllPoint(int idx, float move_x, float move_y);
void DrawAllLine(int idx, float move_x, float move_y);

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

// ��
GLfloat pointShape[MAX_NUM_OBJECT][3];
GLfloat color_point[MAX_NUM_OBJECT][3];

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
int NUM_TRIANGLE = 0;
int NUM_RECTANGLE = 0;


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
	//glClearColor(rColor, gColor, bColor, 1.0f);
	glClearColor(1.0, 1.0, 1.0, 1.0f);

	////glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	////--- ����� VAO �ҷ�����
	glBindVertexArray(vao);

	////--- ������Ʈ �׸���
	glPointSize(5.0);
	DrawAllPoint(-1, 0.f, 0.f);
	DrawAllLine(-1, 0.f, 0.f);

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

void InitBuffer()
{
	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pointShape[i][j] = 0.f;
			color_point[i][j] = 0.f;

			if (j == 0)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;
				
				colorLine[i][j][0] = 0.f;
				colorLine[i][j][1] = 0.f;
				colorLine[i][j][2] = 0.f;
				
			}
			else if (j == 1)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;
				
				colorLine[i][j][0] = 0.f;
				colorLine[i][j][1] = 0.f;
				colorLine[i][j][2] = 0.f;
				
			}
		}
	}
}

void TryDrawPoint()
{
	if (CONDITION != 1) return;

	if (NUM_POINT + 1 > MAX_NUM_OBJECT || ((NUM_POINT + NUM_LINE + NUM_TRIANGLE + NUM_RECTANGLE + 1) > MAX_NUM_OBJECT))
	{
		cout << "NUM_OUT_ERROR" << endl;
		return;
	}

	NUM_POINT += 1;

	DrawAllPoint(NUM_POINT - 1, 0.f, 0.f);
}

void DrawAllPoint(int idx, float move_x, float move_y)
{
	if (idx != -1)
	{
		pointShape[idx][0] = curPos[0];
		pointShape[idx][1] = curPos[1];
	}

	// ��ġ �̵� ��ȭ�� ������ ��ġ �̵�
	for (int i = 0; i < NUM_TRIANGLE; i++)
	{
		pointShape[i][0] += move_x;
		pointShape[i][1] += move_y;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_POINT * NUM_POINT, pointShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_POINT * NUM_POINT, color_point, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, 1 * NUM_POINT);
}

void TryDrawLine()
{
	if (CONDITION != 2) return;

	if (NUM_LINE + 1 > MAX_NUM_OBJECT || ((NUM_POINT + NUM_LINE + NUM_TRIANGLE + NUM_RECTANGLE + 1) > MAX_NUM_OBJECT))
	{
		cout << "NUM_OUT_ERROR" << endl;
		return;
	}

	NUM_LINE += 1;

	DrawAllLine(NUM_LINE - 1, 0.f, 0.f);
}

void DrawAllLine(int idx, float move_x, float move_y)
{
	// Ŭ���� ���� �� ����
	if (idx != -1)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (j == 0) lineShape[idx][i][j] = curPos[0];
				if (i == 0 && j == 1) lineShape[idx][i][j] = curPos[1] + 0.1f;
				if (i == 1 && j == 1) lineShape[idx][i][j] = curPos[1] - 0.1f;
			}
		}
	}

	// ��ġ �̵� ��ȭ�� ������ ��ġ �̵�
	for (int i = 0; i < NUM_LINE; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			lineShape[i][j][0] += move_x;
			lineShape[i][j][1] += move_y;
		}
	}

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

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		curPos[0] = map(x, 0.0f, 800.0f, -1.0f, 1.0f);
		curPos[1] = map(y, 600.0f, 0.0f, -1.0f, 1.0f);
		g_left_button = true;
		TryDrawPoint();
		TryDrawLine();
 	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid Reset()
{
	CONDITION = 1;
	NUM_POINT = 0;
	NUM_LINE = 0;
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
