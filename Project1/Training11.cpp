#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>

#define _CRT_SECURE_NO_WARNINGS

#define MAX_NUM_OBJECT 4

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

// �Է� ó�� �Լ�
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);

GLvoid Reset();
void ResetAllShape();
void DrawCoordinatePlane();
void DrawAllLine();
void DrawAllTriangle();
void DrawAllRectangle();
void DrawAllPentagon();

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

// ��ǥ���
GLfloat coordinatePlane[4][3];
GLfloat coordinatePlane_Color[4][3];

// ��
GLfloat pointShape[MAX_NUM_OBJECT][3];
GLfloat color_point[MAX_NUM_OBJECT][3];

// ��
GLfloat lineShape[MAX_NUM_OBJECT][2][3];
GLfloat colorLine[MAX_NUM_OBJECT][2][3];
GLvoid SetLinePos(int idx, bool isReset);

// �ﰢ��
GLfloat triShape[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL]; //--- �ﰢ�� ��ġ ��
GLfloat triShapeScale[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
GLfloat colorTri[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
GLvoid SetTrianglePos(int idx, bool isReset);

// �簢��
GLfloat rectShape[MAX_NUM_OBJECT][6][3];
GLfloat rectShapeScale[MAX_NUM_OBJECT][6][3];
GLfloat colorRect[MAX_NUM_OBJECT][6][3];
GLvoid SetRectanglePos(int idx, bool isReset);

// ������
GLfloat pentaShape[MAX_NUM_OBJECT][9][3];
GLfloat pentaShapeScale[MAX_NUM_OBJECT][9][3];
GLfloat colorPenta[MAX_NUM_OBJECT][9][3];
GLvoid SetPentagonPos(int idx, bool isReset);

GLuint vao, vbo[2], ebo[2];

// �Ǻ�
GLfloat pivot[4][2] = { {-0.5f, 0.5f},
						{0.5f, 0.5f},
						{-0.5f, -0.5f},
						{0.5f, -0.5f}, };

// �۷ι� ����
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
	Reset();
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
	DrawAllLine();
	DrawAllTriangle();
	DrawAllRectangle();
	DrawAllPentagon();

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

GLvoid Reset()
{
	CONDITION = 0;
	NUM_POINT = 0;
	NUM_LINE = 1;
	NUM_TRIANGLE = 1;
	NUM_RECTANGLE = 1;
	NUM_PENTAGON = 1;

	ResetAllShape();
	SetLinePos(NUM_LINE - 1, true);
	SetTrianglePos(NUM_TRIANGLE - 1, true);
	SetRectanglePos(NUM_RECTANGLE - 1, true);
	SetPentagonPos(NUM_PENTAGON - 1, true);
}

void ResetAllShape()
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

				triShape[i][j][0] = -0.1f;
				triShape[i][j][1] = -0.1f;

				triShapeScale[i][j][0] = -0.3f;
				triShapeScale[i][j][1] = -0.3f;

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
				colorLine[i][j][2] = 0.f;

				triShape[i][j][0] = 0.1f;
				triShape[i][j][1] = -0.1f;

				triShapeScale[i][j][0] = 0.3f;
				triShapeScale[i][j][1] = -0.3f;

				colorTri[i][j][0] = 1.0f;
				colorTri[i][j][1] = 1.0f;
				colorTri[i][j][2] = 0.0f;
			}
			else if (j == 2)
			{
				triShape[i][j][0] = 0.f;
				triShape[i][j][1] = 0.1f;

				triShapeScale[i][j][0] = 0.f;
				triShapeScale[i][j][1] = 0.3f;

				colorTri[i][j][0] = 1.0f;
				colorTri[i][j][1] = 1.0f;
				colorTri[i][j][2] = 0.0f;
			}

			triShape[i][j][2] = 0.f;
			triShapeScale[i][j][2] = 0.f;
		}

		// �簢��
		for (int j = 0; j < 6; j++)
		{
			if (j == 0)
			{
				rectShape[i][j][0] = 0.1f;
				rectShape[i][j][1] = 0.1f;

				rectShapeScale[i][j][0] = 0.3f;
				rectShapeScale[i][j][1] = 0.3f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}
			else if (j == 1 || j == 3)
			{
				rectShape[i][j][0] = 0.1f;
				rectShape[i][j][1] = -0.1f;

				rectShapeScale[i][j][0] = 0.3f;
				rectShapeScale[i][j][1] = -0.3f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}
			else if (j == 2 || j == 5)
			{
				rectShape[i][j][0] = -0.1f;
				rectShape[i][j][1] = 0.1f;

				rectShapeScale[i][j][0] = -0.3f;
				rectShapeScale[i][j][1] = 0.3f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}
			else if (j == 4)
			{
				rectShape[i][j][0] = -0.1f;
				rectShape[i][j][1] = -0.1f;

				rectShapeScale[i][j][0] = -0.3f;
				rectShapeScale[i][j][1] = -0.3f;

				colorRect[i][j][0] = 160.0f / 255.0f;
				colorRect[i][j][1] = 212.0f / 255.0f;
				colorRect[i][j][2] = 104.0f / 255.0f;
			}

			rectShape[i][j][2] = 0.f;
		}

		// ������
		for (int j = 0; j < 9; j++)
		{
			// ����
			colorPenta[i][j][0] = 1.0f;
			colorPenta[i][j][1] = 0.0f;
			colorPenta[i][j][2] = 0.0f;

			if (j == 0)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = 0.f;
				pentaShapeScale[i][j][1] = 0.3f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 1 || j == 4)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = -0.3f;
				pentaShapeScale[i][j][1] = 0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 2 || j == 3 || j == 7)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = 0.3f;
				pentaShapeScale[i][j][1] = 0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 5 || j == 6)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = -0.2f;
				pentaShapeScale[i][j][1] = -0.3f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 8)
			{
				pentaShape[i][j][0] = 0.f;
				pentaShape[i][j][1] = 0.f;
				pentaShape[i][j][2] = 0.f;

				pentaShapeScale[i][j][0] = 0.2f;
				pentaShapeScale[i][j][1] = -0.3f;
				pentaShapeScale[i][j][2] = 0.f;
			}
		}
	}
}

GLvoid SetLinePos(int idx, bool is_reset)
{
	if (is_reset)
	{
		lineShape[idx][0][0] = pivot[0][0] - 0.3f;
		lineShape[idx][0][1] = pivot[0][1] - 0.2f;
		lineShape[idx][1][0] = pivot[0][0] + 0.3f;
		lineShape[idx][1][1] = pivot[0][1] + 0.2f;
	}
}

GLvoid SetTrianglePos(int idx, bool isReset)
{
	if (isReset)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (j == 0) triShape[idx][i][j] = pivot[1][0] - triShapeScale[idx][i][j];
				if (j == 1) triShape[idx][i][j] = pivot[1][1] + triShapeScale[idx][i][j];
			}
		}
	}
}

GLvoid SetRectanglePos(int idx, bool isReset)
{
	if (isReset)
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (j == 0) rectShape[idx][i][j] = pivot[2][0] - rectShapeScale[idx][i][j];
				if (j == 1) rectShape[idx][i][j] = pivot[2][1] + rectShapeScale[idx][i][j];
			}
		}
	}
}

GLvoid SetPentagonPos(int idx, bool isReset)
{
	if (isReset)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (j == 0) pentaShape[idx][i][j] = pivot[3][0] - pentaShapeScale[idx][i][j];
				if (j == 1) pentaShape[idx][i][j] = pivot[3][1] + pentaShapeScale[idx][i][j];
			}
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

	DrawAllLine();
	DrawAllTriangle();
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

void ChangeFigure()
{

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
