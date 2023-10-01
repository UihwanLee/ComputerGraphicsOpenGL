#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>

#define _CRT_SECURE_NO_WARNINGS

#define MAX_NUM_OBJECT 4
#define BYTE_SIZE_LINE 24
#define BYTE_SIZE_TRIANGLE 36

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

void InitBuffer();
void DrawCoordinatePlane();
void DrawAllLine(int idx, float r, float g, float b, float size);
void DrawAllTriangle(int idx, float r, float g, float b, float size);

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

// ��ǥ���
GLfloat coordinatePlane[4][3];
GLfloat coordinatePlane_Color[4][3];

// ��
GLfloat lineShape[MAX_NUM_OBJECT][2][3];
GLfloat colorLine[MAX_NUM_OBJECT][2][3];

// �ﰢ��
GLfloat triShape[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL]; //--- �ﰢ�� ��ġ ��
GLfloat triShapeScale[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
GLfloat colors[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];

GLuint vao, vbo[2], ebo[2];
GLfloat curPos[4][3] = { { 0.f, 0.f, 0.f },
						 { 0.f, 0.f, 0.f }, 
						 { 0.f, 0.f, 0.f },
						 { 0.f, 0.f, 0.f } };

// �۷ι� ����
bool isActive[4] = { false, false, false, false };
int g_cur_area = 0;
bool g_left_button = false;
int CONDITION = 1;
int NUM_OBJECT = 0;


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

	// ��ǥ��� �׸���
	DrawCoordinatePlane();

	// �� ��и鿡 �ִ� �ﰢ�� �׸���
	DrawAllLine(-1, -1.f, -1.f, -1.f, 1.f);
	DrawAllTriangle(-1, -1.f, -1.f, -1.f, 1.f);

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

void InitBuffer()
{
	// ��ǥ��� ����
	for (int i = 0; i < 4; i++)
	{
		if (i == 0) 
		{
			coordinatePlane[i][0] = -1.f;
			coordinatePlane[i][1] = 0.f;
		}
		else if (i == 1)
		{
			coordinatePlane[i][0] = 1.f;
			coordinatePlane[i][1] = 0.f;
		}
		else if (i == 2)
		{
			coordinatePlane[i][0] = 0.f;
			coordinatePlane[i][1] = -1.f;
		}
		else if (i == 3)
		{
			coordinatePlane[i][0] = 0.f;
			coordinatePlane[i][1] = 1.f;
		}

		coordinatePlane[i][2] = 0.f;

		coordinatePlane_Color[i][0] = 0.f;
		coordinatePlane_Color[i][1] = 0.f;
		coordinatePlane_Color[i][2] = 0.f;
	}


	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;

				colorLine[i][j][0] = 0.f;
				colorLine[i][j][1] = 0.f;
				colorLine[i][j][2] = 0.f;

				triShape[i][j][0] = -0.1f;
				triShape[i][j][1] = -0.1f;

				triShapeScale[i][j][0] = -0.1f;
				triShapeScale[i][j][1] = -0.1f;

				colors[i][j][0] = 1.0f;
				colors[i][j][1] = 0.0f;
				colors[i][j][2] = 0.0f;
			}
			else if (j == 1)
			{
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;

				colorLine[i][j][0] = 0.f;
				colorLine[i][j][1] = 0.f;
				colorLine[i][j][2] = 0.f;

				triShape[i][j][0] = 0.1f;
				triShape[i][j][1] = -0.1f;

				triShapeScale[i][j][0] = 0.1f;
				triShapeScale[i][j][1] = -0.1f;

				colors[i][j][0] = 0.0f;
				colors[i][j][1] = 1.0f;
				colors[i][j][2] = 0.0f;
			}
			else if (j == 2)
			{
				triShape[i][j][0] = 0.f;
				triShape[i][j][1] = 0.1f;

				triShapeScale[i][j][0] = 0.f;
				triShapeScale[i][j][1] = 0.1f;

				colors[i][j][0] = 0.0f;
				colors[i][j][1] = 0.0f;
				colors[i][j][2] = 1.0f;
			}

			triShape[i][j][2] = 0.f;
			triShapeScale[i][j][2] = 0.f;
		}
	}
}

void DrawCoordinatePlane()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * 2, coordinatePlane, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * 2, coordinatePlane_Color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINES, 0, 2 * 2);
}

void ChangeTriRandom(int idx)
{
	// ���� ����

	GLfloat color_r = GetRandomFloatValue(0.f, 1.0f);
	GLfloat color_g = GetRandomFloatValue(0.f, 1.0f);
	GLfloat color_b = GetRandomFloatValue(0.f, 1.0f);

	GLfloat rand_size = GetRandomFloatValue(0.2f, 3.0f);

	DrawAllLine(idx, color_r, color_g, color_b, rand_size / 10.0f);
	DrawAllTriangle(idx, color_r, color_g, color_b, rand_size);
}

void TryDrawLine()
{
	if (CONDITION != 2) return;

	if (NUM_OBJECT + 1 > MAX_NUM_OBJECT)
	{
		return;
	}

	NUM_OBJECT += 1;

	DrawAllLine(NUM_OBJECT - 1, -1.f, -1.f, -1.f, 0.1f);
}

void DrawAllLine(int idx, float r, float g, float b, float size)
{
	if (CONDITION != 2) return;

	// Ŭ���� ���� �� ����
	if (idx != -1)
	{
		if (isActive[idx] == false)
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (j == 0) lineShape[idx][i][j] = curPos[idx][0];
					if (i == 0 && j == 1) lineShape[idx][i][j] = curPos[idx][1] + 0.1f;
					if (i == 1 && j == 1) lineShape[idx][i][j] = curPos[idx][1] - 0.1f;
				}
			}
			isActive[idx] = true;
		}

		// ���� && ������ ����
		if (isActive[idx] && r != -1)
		{
			for (int i = 0; i < 2; i++)
			{
				colorLine[idx][i][0] = r;
				colorLine[idx][i][1] = g;
				colorLine[idx][i][2] = b;

				for (int j = 0; j < 3; j++)
				{
					if (j == 0) lineShape[idx][i][j] = curPos[idx][0];
					if (i == 0 && j == 1) lineShape[idx][i][j] = curPos[idx][1] + size;
					if (i == 1 && j == 1) lineShape[idx][i][j] = curPos[idx][1] - size;
				}
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * NUM_OBJECT, lineShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * NUM_OBJECT, colorLine, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINES, 0, 2 * NUM_OBJECT);
}

void TryDrawTriangle()
{
	if (CONDITION != 1) return;

	if (NUM_OBJECT + 1 > MAX_NUM_OBJECT)
	{
		return;
	}

	NUM_OBJECT += 1;

	DrawAllTriangle(NUM_OBJECT - 1, -1.f, -1.f, -1.f, 0.f);
}

void DrawAllTriangle(int idx, float r, float g, float b, float size)
{
	if (CONDITION != 1) return;

	// Ŭ���� ���� �ﰢ�� ����
	if (idx != -1)
	{
		if (isActive[idx] == false)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (j == 0) triShape[idx][i][j] = curPos[idx][0] - triShapeScale[idx][i][j];
					if (j == 1) triShape[idx][i][j] = curPos[idx][1] + triShapeScale[idx][i][j];
				}
			}
			isActive[idx] = true;
		}

		// ���� && ������ ����
		if (isActive[idx] && r != -1)
		{
			for (int i = 0; i < 3; i++)
			{
				colors[idx][i][0] = r;
				colors[idx][i][1] = g;
				colors[idx][i][2] = b;

				if (i == 0)
				{
					triShape[idx][i][0] = curPos[idx][0] - (triShapeScale[idx][i][0] * size);
					triShape[idx][i][1] = curPos[idx][1] + (triShapeScale[idx][i][1] * size);
				}
				if (i == 1)
				{
					triShape[idx][i][0] = curPos[idx][0] - (triShapeScale[idx][i][0] * size);
					triShape[idx][i][1] = curPos[idx][1] + (triShapeScale[idx][i][1] * size);
				}
				if (i == 2)
				{
					triShape[idx][i][0] = curPos[idx][0] - (triShapeScale[idx][i][0]);
					triShape[idx][i][1] = curPos[idx][1] + (triShapeScale[idx][i][1] * size);
				}
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_OBJECT, triShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_OBJECT, colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3 * NUM_OBJECT);
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

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
	return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}

// Ŭ���� ���콺 �����Ͱ� ��� �������� üũ
GLvoid CheckArea(int x, int y)
{
	// �� 1 ����
	if (0 <= x && x < (WIDTH / 2) && 0 <= y && y < (HEIGHT / 2))
	{
		g_cur_area = 1;
	}

	// �� 2 ����
	if ((WIDTH / 2) <= x && x <= (WIDTH) && 0 <= y && y < (HEIGHT / 2))
	{
		g_cur_area = 2;
	}

	// �� 3 ����
	if (0 <= x && x < (WIDTH / 2) && (HEIGHT / 2) <= y && y <= (HEIGHT))
	{
		g_cur_area = 3;
	}

	// �� 4 ����
	if ((WIDTH / 2) <= x && x <= (WIDTH) && (HEIGHT / 2) <= y && y <= (HEIGHT))
	{
		g_cur_area = 4;
	}
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		CheckArea(x, y);
		curPos[g_cur_area-1][0] = map(x, 0.0f, 800.0f, -1.0f, 1.0f);
		curPos[g_cur_area-1][1] = map(y, 600.0f, 0.0f, -1.0f, 1.0f);
		g_left_button = true;
		ChangeTriRandom(g_cur_area-1);
		TryDrawLine();
		TryDrawTriangle();
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid Reset()
{
	CONDITION = 0;
	NUM_OBJECT = 0;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		CONDITION = 1;
		break;
	case 'b':
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
