#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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
GLvoid ResetObjects();
GLvoid UpdateObjects();
void StopAllAnim();
void ResetAllShape();
void DrawAllPoint();
void DrawAllLine();
void DrawAllTriangle();
void DrawAllRectangle();
void DrawAllPentagon();
void DrawAllHexagon();
void DrawObjects(int DRAW_TYPE, int BYTE_SIZE, int vertex, int NUM, void* posList, void* colList);

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

// ��
GLfloat pointShape[MAX_NUM_OBJECT][3];
GLfloat colorPoint[MAX_NUM_OBJECT][3];
GLvoid SetPointPos(int shape_idx, int pivot_idx);
bool isChangingToPoint = false;

// ��
GLfloat lineShape[MAX_NUM_OBJECT][2][3];
GLfloat colorLine[MAX_NUM_OBJECT][2][3];
GLvoid SetLinePos(int idx, int pivot_idx);
bool isChangingToLine = false;

// �ﰢ��
GLfloat triShape[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL]; //--- �ﰢ�� ��ġ ��
GLfloat triShapeScale[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
GLfloat colorTri[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
GLvoid SetTrianglePos(int shape_idx, int pivot_idx);
bool isChangingToTri = false;

// �簢��
GLfloat rectShape[MAX_NUM_OBJECT][6][3];
GLfloat rectShapeScale[MAX_NUM_OBJECT][6][3];
GLfloat colorRect[MAX_NUM_OBJECT][6][3];
GLvoid SetRectanglePos(int shape_idx, int pivot_idx);
bool isChangingToRect = false;

// ������
GLfloat pentaShape[MAX_NUM_OBJECT][9][3];
GLfloat pentaShapeScale[MAX_NUM_OBJECT][9][3];
GLfloat colorPenta[MAX_NUM_OBJECT][9][3];
GLvoid SetPentagonPos(int shape_idx, int pivot_idx);
bool isChangingToPenta = false;

// ������
GLfloat hexaShape[MAX_NUM_OBJECT][12][3];
GLfloat hexaShapeScale[MAX_NUM_OBJECT][12][3];
GLfloat colorHexa[MAX_NUM_OBJECT][12][3];
GLvoid SetHexagonPos(int shape_idx, int pivot_idx);

GLuint vao, vbo[2], ebo[2];

// ���� ������ ����ִ� ����ü
typedef struct Object
{
	vector<float> pivot;
	int type_f;				// ���� Ÿ�� : ��(0) / ��(1) / �ﰢ��(2) / �簢��(3) / ������(4)
	int shape_idx;
	bool isActive;
};

vector<Object> objectList;
Object obj;
vector<float> pivot;

// �۷ι� ����
int g_cur_area = 0;
int CONDITION = 1;
int NUM_OBJECT = 0;
int NUM_POINT = 0;
int NUM_LINE = 0;
int NUM_TRIANGLE = 0;
int NUM_RECTANGLE = 0;
int NUM_PENTAGON = 0;
int NUM_HEXAGON = 0;

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

	// �� ��и鿡 �ִ� ���� �׸���
	DrawObjects(GL_POINTS, BYTE_SIZE_POINT, 1, NUM_POINT, pointShape, colorPoint);
	DrawObjects(GL_LINES, BYTE_SIZE_LINE, 2, NUM_LINE, lineShape, colorLine);
	DrawObjects(GL_TRIANGLES, BYTE_SIZE_TRIANGLE, 3, NUM_TRIANGLE, triShape, colorTri);
	DrawObjects(GL_TRIANGLES, BYTE_SIZE_RECTANGLE, 6, NUM_RECTANGLE, rectShape, colorRect);
	DrawObjects(GL_TRIANGLES, BYTE_SIZE_PENTAGON, 9, NUM_PENTAGON, pentaShape, colorPenta);
	DrawObjects(GL_TRIANGLES, BYTE_SIZE_HEXAGON, 12, NUM_HEXAGON, hexaShape, colorHexa);

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
	StopAllAnim();
	ResetAllShape();

	// 15���� �Ǻ����� �����ϰ� ������ �� ��/��/�ﰢ��/�簢��/������ ���� �׸���
	ResetObjects();
	UpdateObjects();
}

GLvoid ResetObjects()
{
	objectList.clear();
	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		float x = GetRandomFloatValue(-1.0f, 1.0f);
		float y = GetRandomFloatValue(-1.0f, 1.0f);

		obj.pivot.clear();
		obj.pivot.emplace_back(x);
		obj.pivot.emplace_back(y);
		obj.type_f = (i % 5);
		obj.isActive = true;

		objectList.emplace_back(obj);
	}
}

void ResetObjectNum()
{
	NUM_OBJECT = 0;
	NUM_POINT = 0;
	NUM_LINE = 0;
	NUM_TRIANGLE = 0;
	NUM_RECTANGLE = 0;
	NUM_PENTAGON = 0;
	NUM_HEXAGON = 0;
}

GLvoid UpdateObjects()
{
	ResetObjectNum();

	NUM_OBJECT = objectList.size();
	// ���� �����ִ� ������Ʈ���� type_f ������ ���� ������ ������Ʈ�Ѵ�.
	for (int i = 0; i < NUM_OBJECT; i++)
	{
		// ��Ȱ��ȭ�� ������Ʈ�� �Ѿ
		if (objectList[i].isActive == false) continue;

		if (objectList[i].type_f == 0)
		{
			NUM_POINT += 1;
			objectList[i].shape_idx = NUM_POINT - 1;
			SetPointPos(NUM_POINT - 1, i);
		}
		else if (objectList[i].type_f == 1)
		{
			NUM_LINE += 1;
			objectList[i].shape_idx = NUM_LINE - 1;
			SetLinePos(NUM_LINE - 1, i);
		}
		else if (objectList[i].type_f == 2)
		{
			NUM_TRIANGLE += 1;
			objectList[i].shape_idx = NUM_TRIANGLE - 1;
			SetTrianglePos(NUM_TRIANGLE - 1, i);
		}
		else if (objectList[i].type_f == 3)
		{
			NUM_RECTANGLE += 1;
			objectList[i].shape_idx = NUM_RECTANGLE - 1;
			SetRectanglePos(NUM_RECTANGLE - 1, i);
		}
		else if (objectList[i].type_f == 4)
		{
			NUM_PENTAGON += 1;
			objectList[i].shape_idx = NUM_PENTAGON - 1;
			SetPentagonPos(NUM_PENTAGON - 1, i);
		}
		else if (objectList[i].type_f == 5)
		{
			NUM_HEXAGON += 1;
			objectList[i].shape_idx = NUM_HEXAGON - 1;
			SetHexagonPos(NUM_HEXAGON - 1, i);
		}
	}

}

void ResetAllShape()
{
	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		colorPoint[i][0] = 1.f;
		colorPoint[i][1] = 0.f;
		colorPoint[i][2] = 0.f;

		pointShape[i][0] = 0.f;
		pointShape[i][1] = 1.f;
		pointShape[i][2] = 0.f;

		for (int j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				// ��
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;

				// �ﰢ��
				triShape[i][j][0] = -0.1f;
				triShape[i][j][1] = -0.1f;
				triShape[i][j][2] = 0.f;

				triShapeScale[i][j][0] = -0.1f;
				triShapeScale[i][j][1] = -0.1f;
				triShapeScale[i][j][2] = 0.f;

				// �簢��
				rectShape[i][j][0] = 0.1f;
				rectShape[i][j][1] = 0.1f;

				rectShapeScale[i][j][0] = 0.1f;
				rectShapeScale[i][j][1] = 0.1f;
			}
			else if (j == 1)
			{
				// ��
				lineShape[i][j][0] = 0.f;
				lineShape[i][j][1] = 0.f;
				lineShape[i][j][2] = 0.f;

				// �ﰢ��
				triShape[i][j][0] = 0.1f;
				triShape[i][j][1] = -0.1f;
				triShape[i][j][2] = 0.f;

				triShapeScale[i][j][0] = 0.1f;
				triShapeScale[i][j][1] = -0.1f;
				triShapeScale[i][j][2] = 0.f;
			}
			else if (j == 2)
			{
				// �ﰢ��
				triShape[i][j][0] = 0.f;
				triShape[i][j][1] = 0.1f;
				triShape[i][j][2] = 0.f;

				triShapeScale[i][j][0] = 0.f;
				triShapeScale[i][j][1] = 0.1f;
				triShapeScale[i][j][2] = 0.f;
			}

			// ����
			if (j < 2)
			{
				colorLine[i][j][0] = 0.f;
				colorLine[i][j][1] = 0.f;
				colorLine[i][j][2] = 1.f;
			}
			if (j < 3)
			{
				colorTri[i][j][0] = 1.0f;
				colorTri[i][j][1] = 1.0f;
				colorTri[i][j][2] = 0.0f;
			}
		}

		// �簢��
		for (int j = 0; j < 6; j++)
		{
			if (j == 0)
			{
				rectShape[i][j][0] = 0.1f;
				rectShape[i][j][1] = 0.1f;

				rectShapeScale[i][j][0] = 0.1f;
				rectShapeScale[i][j][1] = 0.1f;
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
			colorRect[i][j][0] = 160.0f / 255.0f;
			colorRect[i][j][1] = 212.0f / 255.0f;
			colorRect[i][j][2] = 104.0f / 255.0f;
		}

		// ������
		for (int j = 0; j < 9; j++)
		{
			// ����
			colorPenta[i][j][0] = 1.0f;
			colorPenta[i][j][1] = 0.0f;
			colorPenta[i][j][2] = 0.0f;

			pentaShape[i][j][0] = 0.f;
			pentaShape[i][j][1] = 0.f;
			pentaShape[i][j][2] = 0.f;

			if (j == 0)
			{
				pentaShapeScale[i][j][0] = 0.f;
				pentaShapeScale[i][j][1] = 0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 1 || j == 4)
			{
				pentaShapeScale[i][j][0] = -0.1f;
				pentaShapeScale[i][j][1] = 0.03f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 2 || j == 3 || j == 7)
			{
				pentaShapeScale[i][j][0] = 0.1f;
				pentaShapeScale[i][j][1] = 0.03f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 5 || j == 6)
			{
				pentaShapeScale[i][j][0] = -0.06f;
				pentaShapeScale[i][j][1] = -0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
			else if (j == 8)
			{
				pentaShapeScale[i][j][0] = 0.06f;
				pentaShapeScale[i][j][1] = -0.1f;
				pentaShapeScale[i][j][2] = 0.f;
			}
		}

		// ������ 
		for (int j = 0; j < 12; j++)
		{
			// ����
			colorHexa[i][j][0] = 0.0f;
			colorHexa[i][j][1] = 1.0f;
			colorHexa[i][j][2] = 0.0f;

			hexaShape[i][j][0] = 0.f;
			hexaShape[i][j][1] = 0.f;

			hexaShape[i][j][2] = 0.f;
			hexaShapeScale[i][j][2] = 0.f;

			if (j == 0)
			{
				hexaShapeScale[i][j][0] = -0.1f;
				hexaShapeScale[i][j][1] = 0.f;
			}
			else if (j == 1 || j == 3 || j == 7)
			{
				hexaShapeScale[i][j][0] = -0.05f;
				hexaShapeScale[i][j][1] = 0.1f;
			}
			else if (j == 2 || j == 4)
			{
				hexaShapeScale[i][j][0] = -0.05f;
				hexaShapeScale[i][j][1] = -0.1f;
			}
			else if (j == 5 || j == 6 || j==10)
			{
				hexaShapeScale[i][j][0] = 0.05f;
				hexaShapeScale[i][j][1] = -0.1f;
			}
			else if (j == 8 || j == 9)
			{
				hexaShapeScale[i][j][0] = 0.05f;
				hexaShapeScale[i][j][1] = 0.1f;
			}
			else if (j == 11)
			{
				hexaShapeScale[i][j][0] = 0.1f;
				hexaShapeScale[i][j][1] = 0.f;
			}
		}
	}
}

GLvoid SetPointPos(int shape_idx, int pivot_idx)
{
	pointShape[shape_idx][0] = objectList[pivot_idx].pivot[0];
	pointShape[shape_idx][1] = objectList[pivot_idx].pivot[1];
}

GLvoid SetLinePos(int shape_idx, int pivot_idx)
{
	lineShape[shape_idx][0][0] = objectList[pivot_idx].pivot[0] - 0.1f;
	lineShape[shape_idx][0][1] = objectList[pivot_idx].pivot[1] - 0.06f;
	lineShape[shape_idx][1][0] = objectList[pivot_idx].pivot[0] + 0.1f;
	lineShape[shape_idx][1][1] = objectList[pivot_idx].pivot[1] + 0.06f;
}

GLvoid SetTrianglePos(int shape_idx, int pivot_idx)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0) triShape[shape_idx][i][j] = objectList[pivot_idx].pivot[0] - triShapeScale[shape_idx][i][j];
			if (j == 1) triShape[shape_idx][i][j] = objectList[pivot_idx].pivot[1] + triShapeScale[shape_idx][i][j];
		}
	}
}

GLvoid SetRectanglePos(int shape_idx, int pivot_idx)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0) rectShape[shape_idx][i][j] = objectList[pivot_idx].pivot[0] - rectShapeScale[shape_idx][i][j];
			if (j == 1) rectShape[shape_idx][i][j] = objectList[pivot_idx].pivot[1] + rectShapeScale[shape_idx][i][j];
		}
	}
}

GLvoid SetPentagonPos(int shape_idx, int pivot_idx)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0) pentaShape[shape_idx][i][j] = objectList[pivot_idx].pivot[0] - pentaShapeScale[shape_idx][i][j];
			if (j == 1) pentaShape[shape_idx][i][j] = objectList[pivot_idx].pivot[1] + pentaShapeScale[shape_idx][i][j];
		}
	}
}

GLvoid SetHexagonPos(int shape_idx, int pivot_idx)
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == 0) hexaShape[shape_idx][i][j] = objectList[pivot_idx].pivot[0] - hexaShapeScale[shape_idx][i][j];
			if (j == 1) hexaShape[shape_idx][i][j] = objectList[pivot_idx].pivot[1] + hexaShapeScale[shape_idx][i][j];
		}
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

	glPointSize(5.0);
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

	// ���콺 ��ư�� Ŭ���ϰų� ������ �� �� ������ ������Ʈ�� �ִ��� �Ǻ��ϱ�
	for (int i = 0; i < NUM_OBJECT; i++)
	{
		// ���� �巡���ϰ� �ִ� ������ �Ѿ��
		if (pre_obj_idx != -1 && pre_obj_idx == i) continue;

		// ��Ȱ��ȭ�� ������ �Ѿ��
		if (objectList[i].isActive == false) continue;

		if (objectList[i].type_f == 0)
		{
			if (CheckCollisinWithMouse(objectList[i].pivot[0] - 0.03f, objectList[i].pivot[1] - 0.03f, objectList[i].pivot[0] + 0.03f, objectList[i].pivot[1] + 0.03f))
			{
				cur_obj_idx = i;
				return;
			}
		}
		else if (objectList[i].type_f == 1)
		{
			if (CheckCollisinWithMouse(objectList[i].pivot[0] - 0.1f, objectList[i].pivot[1] - 0.06f, objectList[i].pivot[0] + 0.1f, objectList[i].pivot[1] + 0.06f))
			{
				cur_obj_idx = i;
				return;
			}
		}
		else if (objectList[i].type_f == 5)
		{
			if (CheckCollisinWithMouse(objectList[i].pivot[0] - 0.1f, objectList[i].pivot[1] - 0.1f, objectList[i].pivot[0] + 0.1f, objectList[i].pivot[1] + 0.1f))
			{
				cur_obj_idx = i;
				cout << "Hello" << cur_obj_idx << endl;
				return;
			}
		}
		else
		{
			if (CheckCollisinWithMouse(objectList[i].pivot[0] - 0.1f, objectList[i].pivot[1] - 0.1f, objectList[i].pivot[0] + 0.1f, objectList[i].pivot[1] + 0.1f))
			{
				cur_obj_idx = i;
				return;
			}
		}
	}
}

GLvoid GenerateNewObject(int vertex, float pivot_x, float pivot_y)
{
	obj.type_f = vertex;
	obj.pivot[0] = pivot_x;
	obj.pivot[1] = pivot_y;
	obj.isActive = true;
	objectList.push_back(obj);
}

GLvoid CombineObject(int comb1_idx, int comb2_idx)
{
	int vertex = (objectList[comb1_idx].type_f + 1) + (objectList[comb2_idx].type_f + 1);

	// 2���� ���� ��Ȱ��ȭ
	objectList[comb1_idx].isActive = false;
	objectList[comb2_idx].isActive = false;

	vertex = (vertex - 1) % 6;

	// �������� �������� ���ο� ���� ����
	GenerateNewObject(vertex, objectList[comb2_idx].pivot[0], objectList[comb2_idx].pivot[1]);

	UpdateObjects();

	pre_obj_idx = -1;
	cur_obj_idx = -1;
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
		pre_obj_idx = cur_obj_idx;
		CheckClickObject();

		if (cur_obj_idx != -1 && pre_obj_idx != -1)
		{
			CombineObject(pre_obj_idx, cur_obj_idx);
		}
	}

	glutPostRedisplay();
}

GLvoid MouseDrag(int x, int y)
{
	// && g_cur_rect != -1
	if (g_left_button && cur_obj_idx != -1)
	{
		if (objectList[cur_obj_idx].isActive)
		{
			// ���콺 �巡�׿� ���� ���� �̵�
			objectList[cur_obj_idx].pivot[0] = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
			objectList[cur_obj_idx].pivot[1] = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);

			if (objectList[cur_obj_idx].type_f == 0) SetPointPos(objectList[cur_obj_idx].shape_idx, cur_obj_idx);
			else if (objectList[cur_obj_idx].type_f == 1) SetLinePos(objectList[cur_obj_idx].shape_idx, cur_obj_idx);
			else if (objectList[cur_obj_idx].type_f == 2) SetTrianglePos(objectList[cur_obj_idx].shape_idx, cur_obj_idx);
			else if (objectList[cur_obj_idx].type_f == 3) SetRectanglePos(objectList[cur_obj_idx].shape_idx, cur_obj_idx);
			else if (objectList[cur_obj_idx].type_f == 4) SetPentagonPos(objectList[cur_obj_idx].shape_idx, cur_obj_idx);
			else if (objectList[cur_obj_idx].type_f == 5) SetHexagonPos(objectList[cur_obj_idx].shape_idx, cur_obj_idx);
		}
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
