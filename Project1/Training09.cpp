//#include <iostream>
//#include <gl/glew.h> //--- �ʿ��� ������� include
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <random>
//#include <cmath>
//
//#define _CRT_SECURE_NO_WARNINGS
//
//#define MAX_NUM_OBJECT 4
//#define BYTE_SIZE_LINE 24
//#define BYTE_SIZE_TRIANGLE 36
//
//#define MAX_TRI_ROW 3
//#define MAX_TRI_COL 3
//
//#define WIDTH 800
//#define HEIGHT 600
//
//#define PI 3.14159265
//
//using namespace std;
//
//void make_vertexShaders();
//void make_fragmentShaders();
//void make_shaderProgram();
//GLvoid drawScene();
//GLvoid Reshape(int w, int h);
//
//// �Է� ó�� �Լ�
//GLvoid MouseClick(int button, int state, int x, int y);
//GLvoid Keyboard(unsigned char key, int x, int y);
//
//void InitBuffer();
//void DrawAllTriangle(int idx);
//void SetSpiralPivot();
//
//GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
//GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
//GLuint shaderProgramID; //--- ���̴� ���α׷�
//
//
//// �ﰢ��
//GLfloat triShape[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL]; //--- �ﰢ�� ��ġ ��
//GLfloat triShapeScale[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
//GLfloat triShapeChangeScale[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
//GLfloat colors[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
//
//GLuint vao, vbo[2], ebo[2];
//GLfloat curPos[2] = { 0.f, 0.f };
//
//// �۷ι� ����
//bool isActive[4] = { false, false, false, false };
//int g_cur_area = 0;
//bool g_left_button = false;
//int CONDITION = 1;
//int NUM_OBJECT = 0;
//
//float speed = 0.01f;
//float dir_x[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//float dir_y[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//float zigzagDist_y[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
//bool zigzag_yUp[4] = { false, false, false, false };
//
//float pivot[4][2] = { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} };
//float spiralPivot[4][2] = { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} };
//
//// �簢�� �����̷�
//float rectSpiralDir[4][2] = { {-1.0f, 0.0f}, {-1.0f, 0.0f}, {-1.0f, 0.0f}, {-1.0f, 0.0f} };
//float rectSpiralMaxDist[4] = {0.1f, 0.1f, 0.1f, 0.1f};
//float rectSpiralDist[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
//int rectSpiralDistCount[4] = { 0, 0, 0, 0 };
//int rectSpiralMaxCount[4] = {0, 0, 0, 0};
//
//// �� �����̷�
//float circleSpiralRadius[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
//int circleSpiralDeg[4] = {0, 0, 0, 0};
//
//bool isMovingDiagonal = false;
//bool isMovingZigZag = false;
//bool isMovingRectSpiral = false;
//bool isMovingCircleSpiral = false;
//
//
//int main(int argc, char** argv)
//{
//	//--- ������ �����ϱ�
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(WIDTH, HEIGHT);
//	glutCreateWindow("Example1");
//	//--- GLEW �ʱ�ȭ�ϱ�
//	glewExperimental = GL_TRUE;
//	glewInit();
//	make_shaderProgram();
//	InitBuffer();
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(Reshape);
//	glutKeyboardFunc(Keyboard);
//	glutMouseFunc(MouseClick);
//	glutMainLoop();
//}
//
//GLvoid drawScene()
//{
//	//--- ����� ���� ����
//	//glClearColor(rColor, gColor, bColor, 1.0f);
//	glClearColor(1.0, 1.0, 1.0, 1.0f);
//
//	////glClearColor(1.0, 1.0, 1.0, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	////--- ������ ���������ο� ���̴� �ҷ�����
//	glUseProgram(shaderProgramID);
//	////--- ����� VAO �ҷ�����
//	glBindVertexArray(vao);
//
//	DrawAllTriangle(-1);
//
//	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
//}
//
//int GetRandomIntValue(GLfloat min, GLfloat max)
//{
//	GLfloat value;
//
//	random_device rd;
//	mt19937 gen(rd());
//
//	uniform_real_distribution<GLfloat> dis(min, max);
//
//	value = dis(gen);
//
//	return int(value);
//}
//
//GLfloat GetRandomFloatValue(GLfloat min, GLfloat max)
//{
//	GLfloat value;
//
//	random_device rd;
//	mt19937 gen(rd());
//
//	uniform_real_distribution<GLfloat> dis(min, max);
//
//	value = dis(gen);
//
//	return value;
//}
//
//void InitBuffer()
//{
//	for (int i = 0; i < MAX_NUM_OBJECT; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			if (j == 0)
//			{
//				triShape[i][j][0] = -0.1f;
//				triShape[i][j][1] = -0.1f;
//
//				triShapeScale[i][j][0] = -0.1f;
//				triShapeScale[i][j][1] = -0.1f;
//
//				triShapeChangeScale[i][j][0] = -0.1f;
//				triShapeChangeScale[i][j][1] = -0.1f;
//
//				colors[i][j][0] = 1.0f;
//				colors[i][j][1] = 0.0f;
//				colors[i][j][2] = 0.0f;
//			}
//			else if (j == 1)
//			{
//				triShape[i][j][0] = 0.1f;
//				triShape[i][j][1] = -0.1f;
//
//				triShapeScale[i][j][0] = 0.1f;
//				triShapeScale[i][j][1] = -0.1f;
//
//				triShapeChangeScale[i][j][0] = 0.1f;
//				triShapeChangeScale[i][j][1] = -0.1f;
//
//				colors[i][j][0] = 0.0f;
//				colors[i][j][1] = 1.0f;
//				colors[i][j][2] = 0.0f;
//			}
//			else if (j == 2)
//			{
//				triShape[i][j][0] = 0.f;
//				triShape[i][j][1] = 0.2f;
//
//				triShapeScale[i][j][0] = 0.f;
//				triShapeScale[i][j][1] = 0.2f;
//
//				triShapeChangeScale[i][j][0] = 0.f;
//				triShapeChangeScale[i][j][1] = 0.2f;
//
//				colors[i][j][0] = 0.0f;
//				colors[i][j][1] = 0.0f;
//				colors[i][j][2] = 1.0f;
//			}
//
//			triShape[i][j][2] = 0.f;
//			triShapeScale[i][j][2] = 0.f;
//			triShapeChangeScale[i][j][2] = 0.f;
//		}
//	}
//}
//
//void TryDrawTriangle()
//{
//	if (NUM_OBJECT + 1 > MAX_NUM_OBJECT)
//	{
//		return;
//	}
//
//	NUM_OBJECT += 1;
//
//	DrawAllTriangle(NUM_OBJECT - 1);
//}
//
//void DrawAllTriangle(int idx)
//{
//	// Ŭ���� ���� �ﰢ�� ����
//	if (idx != -1)
//	{
//		for (int i = 0; i < 3; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				if (j == 0) triShape[idx][i][j] = curPos[0] - triShapeScale[idx][i][j];
//				if (j == 1) triShape[idx][i][j] = curPos[1] + triShapeScale[idx][i][j];
//			}
//		}
//	}
//
//	// ��ġ �̵� ��ȭ�� ������ ��ġ �̵�
//	if (isMovingDiagonal || isMovingZigZag)
//	{
//		for (int i = 0; i < NUM_OBJECT; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				triShape[i][j][0] += speed * dir_x[i];
//				triShape[i][j][1] += speed * dir_y[i];
//			}
//
//			pivot[i][0] += speed * dir_x[i];
//			pivot[i][1] += speed * dir_y[i];
//		}
//	}
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	glGenBuffers(2, vbo);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_OBJECT, triShape, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_OBJECT, colors, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(1);
//
//	glDrawArrays(GL_TRIANGLES, 0, 3 * NUM_OBJECT);
//}
//
//GLvoid Reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//}
//
//char* filetobuf(const char* file)
//{
//	FILE* fptr;
//	long length;
//	char* buf;
//	fopen_s(&fptr, file, "rb"); // Open file for reading 
//	if (!fptr) // Return NULL on failure 
//		return NULL;
//	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
//	length = ftell(fptr); // Find out how many bytes into the file we are 
//	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
//	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
//	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
//	fclose(fptr); // Close the file 
//	buf[length] = 0; // Null terminator 
//	return buf; // Return the buffer 
//}
//
//void make_shaderProgram()
//{
//	make_vertexShaders(); //--- ���ؽ� ���̴� �����
//	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
//	//-- shader Program
//	shaderProgramID = glCreateProgram();
//	glAttachShader(shaderProgramID, vertexShader);
//	glAttachShader(shaderProgramID, fragmentShader);
//	glLinkProgram(shaderProgramID);
//	//--- ���̴� �����ϱ�
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program ����ϱ�
//	glUseProgram(shaderProgramID);
//}
//
//
//void make_vertexShaders()
//{
//	vertexSource = filetobuf("vertex.glsl");
//	//--- ���ؽ� ���̴� ��ü �����
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
//	//--- ���ؽ� ���̴� �������ϱ�
//	glCompileShader(vertexShader);
//	//--- �������� ����� ���� ���� ���: ���� üũ
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
//		return;
//	}
//}
//
//void make_fragmentShaders()
//{
//	fragmentSource = filetobuf("fragment.glsl");
//	//--- �����׸�Ʈ ���̴� ��ü �����
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
//	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
//	//--- �����׸�Ʈ ���̴� ������
//	glCompileShader(fragmentShader);
//	//--- �������� ����� ���� ���� ���: ������ ���� üũ
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
//		return;
//	}
//}
//
//float map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
//	return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
//}
//
//GLvoid MouseClick(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		curPos[0] = map(x, 0.0f, 800.0f, -1.0f, 1.0f);
//		curPos[1] = map(y, 600.0f, 0.0f, -1.0f, 1.0f);
//		g_left_button = true;
//		TryDrawTriangle();
//		if (NUM_OBJECT + 1 <= MAX_NUM_OBJECT) pivot[NUM_OBJECT - 1][0] = curPos[0];
//		if (NUM_OBJECT + 1 <= MAX_NUM_OBJECT) pivot[NUM_OBJECT - 1][1] = curPos[1];
//		if(isMovingRectSpiral || isMovingCircleSpiral) SetSpiralPivot();
//	}
//
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//	{
//		g_left_button = false;
//	}
//
//	glutPostRedisplay();
//}
//
//GLvoid ChangeTriangleShape(int idx, int dir)
//{
//	// �����¿� ����
//	if (dir == 1)
//	{
//		triShapeChangeScale[idx][0][0] = -0.1f;
//		triShapeChangeScale[idx][0][1] = 0.1f;
//
//		triShapeChangeScale[idx][1][0] = 0.1f;
//		triShapeChangeScale[idx][1][1] = 0.1f;
//
//		triShapeChangeScale[idx][2][0] = 0.0f;
//		triShapeChangeScale[idx][2][1] = -0.2f;
//	}
//	else if (dir == 2)
//	{
//		triShapeChangeScale[idx][0][0] = -0.1f;
//		triShapeChangeScale[idx][0][1] = -0.1f;
//
//		triShapeChangeScale[idx][1][0] = 0.1f;
//		triShapeChangeScale[idx][1][1] = -0.1f;
//
//		triShapeChangeScale[idx][2][0] = 0.0f;
//		triShapeChangeScale[idx][2][1] = 0.2f;
//	}
//	else if (dir == 3)
//	{
//		triShapeChangeScale[idx][0][0] = -0.1f;
//		triShapeChangeScale[idx][0][1] = 0.1f;
//
//		triShapeChangeScale[idx][1][0] = -0.1f;
//		triShapeChangeScale[idx][1][1] = -0.1f;
//
//		triShapeChangeScale[idx][2][0] = 0.1f;
//		triShapeChangeScale[idx][2][1] = 0.0f;
//	}
//	else if (dir == 4)
//	{
//		triShapeChangeScale[idx][0][0] = 0.1f;
//		triShapeChangeScale[idx][0][1] = 0.1f;
//
//		triShapeChangeScale[idx][1][0] = 0.1f;
//		triShapeChangeScale[idx][1][1] = -0.1f;
//
//		triShapeChangeScale[idx][2][0] = -0.1f;
//		triShapeChangeScale[idx][2][1] = 0.0f;
//	}
//
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			if (j == 0) triShape[idx][i][j] = pivot[idx][0] - triShapeChangeScale[idx][i][j];
//			if (j == 1) triShape[idx][i][j] = pivot[idx][1] + triShapeChangeScale[idx][i][j];
//		}
//	}
//}
//
//int CheckCollision(int idx)
//{
//	float TRI_TOP_y = triShape[idx][2][1];
//	float TRI_LEFT_BOTTOM_x = triShape[idx][0][0];
//	float TRI_LEFT_BOTTOM_y = triShape[idx][0][1];
//	float TRI_RIGHT_BOTTOM_x = triShape[idx][1][0];
//	float TRI_RIGHT_BOTTOM_y = triShape[idx][1][1];
//
//	// ���� ���� ���� ���
//	if (TRI_TOP_y >= 1.0f || TRI_LEFT_BOTTOM_y >= 1.0f || TRI_RIGHT_BOTTOM_y >= 1.0f)
//	{
//		ChangeTriangleShape(idx, 1);
//		return 1;
//	}
//
//	// �Ʒ��� ���� ���� ���
//	if (TRI_LEFT_BOTTOM_y <= -1.0f || TRI_TOP_y <= -1.0f || TRI_LEFT_BOTTOM_y <= -1.0f || TRI_RIGHT_BOTTOM_y <= -1.0f)
//	{
//		ChangeTriangleShape(idx, 2);
//		return 2;
//	}
//
//	// ������ ���� ���� ���
//	if (TRI_RIGHT_BOTTOM_x >= 1.0f || TRI_LEFT_BOTTOM_x >= 1.0f || TRI_RIGHT_BOTTOM_y >= 1.0f)
//	{
//		ChangeTriangleShape(idx, 3);
//		return 3;
//	}
//
//	// ���� ���� ���� ���
//	if (TRI_LEFT_BOTTOM_x <= -1.0f || TRI_RIGHT_BOTTOM_x <= -1.0f || TRI_RIGHT_BOTTOM_y <= -1.0f)
//	{
//		ChangeTriangleShape(idx, 4);
//		return 4;
//	}
//
//	return -1;
//}
//
//// �̵��ϴ� �簢�� ���� �浹 üũ �� ���� �ٲٱ�
//GLvoid CheckCollision1(int idx)
//{
//	float TRI_TOP_y = triShape[idx][2][1];
//	float TRI_LEFT_BOTTOM_x = triShape[idx][0][0];
//	float TRI_LEFT_BOTTOM_y = triShape[idx][0][1];
//	float TRI_RIGHT_BOTTOM_x = triShape[idx][1][0];
//	float TRI_RIGHT_BOTTOM_y = triShape[idx][1][1];
//
//	// ���� ���� ���� ���
//	if (CheckCollision(idx) == 1)
//	{
//		dir_y[idx] = dir_y[idx] * (-1.0f);
//	}
//
//	// �Ʒ��� ���� ���� ���
//	if (CheckCollision(idx) == 2)
//	{
//		dir_y[idx] = dir_y[idx] * (-1.0f);
//	}
//
//	// ������ ���� ���� ���
//	if (CheckCollision(idx) == 3)
//	{
//		dir_x[idx] = dir_x[idx] * (-1.0f);
//	}
//
//	// ���� ���� ���� ���
//	if (CheckCollision(idx) == 4)
//	{
//		dir_x[idx] = dir_x[idx] * (-1.0f);
//	}
//}
//
//GLvoid MovingDiagonal(int isAnim)
//{
//	for (int i = 0; i < NUM_OBJECT; i++)
//	{
//		CheckCollision1(i);
//	}
//
//	DrawAllTriangle(-1.0f);
//
//	glutPostRedisplay();
//
//	if(isMovingDiagonal) glutTimerFunc(30, MovingDiagonal, isMovingDiagonal);
//}
//
//// ������� � �浹ó��
//GLvoid CheckCollision2(int idx)
//{
//	float TRI_TOP_y = triShape[idx][2][1];
//	float TRI_TOP_x = triShape[idx][2][0];
//	float TRI_LEFT_BOTTOM_x = triShape[idx][0][0];
//	float TRI_LEFT_BOTTOM_y = triShape[idx][0][1];
//	float TRI_RIGHT_BOTTOM_x = triShape[idx][1][0];
//	float TRI_RIGHT_BOTTOM_y = triShape[idx][1][1];
//
//	// ���� ���� ���� ���
//	if (TRI_TOP_y >= 1.0f || TRI_LEFT_BOTTOM_y >= 1.0f || TRI_RIGHT_BOTTOM_y >= 1.0f)
//	{
//		ChangeTriangleShape(idx, 1);
//		zigzag_yUp[idx] = false;
//	}
//
//	// �Ʒ��� ���� ���� ���
//	if (TRI_LEFT_BOTTOM_y <= -1.0f || TRI_TOP_y <= -1.0f || TRI_LEFT_BOTTOM_y <= -1.0f || TRI_RIGHT_BOTTOM_y <= -1.0f)
//	{
//		ChangeTriangleShape(idx, 2);
//		zigzag_yUp[idx] = true;
//	}
//
//	// ������ ���� ���� ���
//	if (TRI_RIGHT_BOTTOM_x >= 1.0f || TRI_LEFT_BOTTOM_x >= 1.0f || TRI_RIGHT_BOTTOM_y >= 1.0f || TRI_TOP_x >= 1.0f)
//	{
//		ChangeTriangleShape(idx, 3);
//		dir_x[idx] = 0.0f;
//		dir_y[idx] = (zigzag_yUp[idx]) ? 1.0f : - 1.0f;
//		zigzagDist_y[idx] += 0.05f;
//	}
//
//	// ���� ���� ���� ���
//	if (TRI_LEFT_BOTTOM_x <= -1.0f || TRI_RIGHT_BOTTOM_x <= -1.0f || TRI_RIGHT_BOTTOM_y <= -1.0f || TRI_TOP_x <= -1.0f)
//	{
//		ChangeTriangleShape(idx, 4);
//		dir_x[idx] = 0.0f;
//		dir_y[idx] = (zigzag_yUp[idx]) ? 1.0f : -1.0f;
//		zigzagDist_y[idx] += 0.05f;
//	}
//
//	// �Ʒ� �̵��� Ư�� ������ ���� ��� : �����̸� ������ �̵� / �������̸� ���� �̵�
//	if (zigzagDist_y[idx] >= 1.0f)
//	{
//		dir_x[idx] = (TRI_LEFT_BOTTOM_x <= -1.0f || TRI_RIGHT_BOTTOM_x <= -1.0f) ? 1.0f : -1.0f;
//		dir_y[idx] = 0.0f;
//		zigzagDist_y[idx] = 0.0f;
//	}
//}
//
//GLvoid GoZigZag()
//{
//	for (int i = 0; i < NUM_OBJECT; i++)
//	{
//		float TRI_LEFT_x = triShape[i][0][0];
//		
//		dir_x[i] = (TRI_LEFT_x < 0.0f) ? 1.0f : -1.0f;
//		dir_y[i] = 0.0f;
//	}
//}
//
//GLvoid MovingZigZag(int isAnim)
//{
//	for (int i = 0; i < NUM_OBJECT; i++)
//	{
//		CheckCollision2(i);
//	}
//
//	DrawAllTriangle(-1.0f);
//
//	glutPostRedisplay();
//
//	if (isMovingZigZag) glutTimerFunc(30, MovingZigZag, isMovingZigZag);
//}
//
//GLvoid SetSpiralPivot()
//{
//	for (int i = 0; i < NUM_OBJECT; i++)
//	{
//		//spiralPivot[i][0] = triShape[i][0][0] + triShapeScale[i][0][0];
//		//spiralPivot[i][1] = triShape[i][2][1] - triShapeScale[i][2][1];
//
//		spiralPivot[i][0] = pivot[i][0];
//		spiralPivot[i][1] = pivot[i][1];
//	}
//}
//
//GLvoid MovingRectSpiral(int isAnim)
//{
//	for (int idx = 0; idx < NUM_OBJECT; idx++)
//	{
//		float x = spiralPivot[idx][0] + speed * rectSpiralDir[idx][0];
//		float y = spiralPivot[idx][1] + speed * rectSpiralDir[idx][1];
//		for (int i = 0; i < 3; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				if (j == 0) triShape[idx][i][j] += speed * rectSpiralDir[idx][0];
//				if (j == 1) triShape[idx][i][j] += speed * rectSpiralDir[idx][1];
//			}
//		}
//		rectSpiralDist[idx] += 0.01f;
//
//		if (rectSpiralDist[idx] >= rectSpiralMaxDist[idx])
//		{
//			rectSpiralDist[idx] = 0.0f;
//			rectSpiralMaxCount[idx] += 1;
//			rectSpiralDistCount[idx] += 1;
//
//			// ������ �����Ҷ� ���� ���� ����
//			if (rectSpiralDistCount[idx] % 4 == 1)
//			{
//				rectSpiralDir[idx][0] = 0.0f;
//				rectSpiralDir[idx][1] = -1.0f;
//			}
//			else if (rectSpiralDistCount[idx] % 4 == 2)
//			{
//				rectSpiralDir[idx][0] = 1.0f;
//				rectSpiralDir[idx][1] = 0.0f;
//			}
//			else if (rectSpiralDistCount[idx] % 4 == 3)
//			{
//				rectSpiralDir[idx][0] = 0.0f;
//				rectSpiralDir[idx][1] = 1.0f;
//			}
//			else
//			{
//				rectSpiralDir[idx][0] = -1.0f;
//				rectSpiralDir[idx][1] = 0.0f;
//			}
//		}
//
//		if (rectSpiralMaxCount[idx] >= 2)
//		{
//			// �Ÿ� ����
//			rectSpiralMaxDist[idx] = rectSpiralMaxDist[idx] + 0.1f;
//			rectSpiralMaxCount[idx] = 0;
//		}
//	}
//
//	DrawAllTriangle(-1.0f);
//
//	glutPostRedisplay();
//
//	if (isMovingRectSpiral) glutTimerFunc(30, MovingRectSpiral, isMovingRectSpiral);
//}
//
//GLvoid MovingCircleSpiral(int isAnim)
//{
//	for (int idx = 0; idx < NUM_OBJECT; idx++)
//	{
//		// pivot ��� radian = degree * (pi/180)
//		float x = spiralPivot[idx][0] + circleSpiralRadius[idx] * (cos(circleSpiralDeg[idx] * (PI / 180.f)));
//		float y = spiralPivot[idx][1] + circleSpiralRadius[idx] * (sin(circleSpiralDeg[idx] * (PI / 180.f)));
//		for (int i = 0; i < 3; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				if (j == 0) triShape[idx][i][j] = x - triShapeChangeScale[idx][i][j];
//				if (j == 1) triShape[idx][i][j] = y + triShapeChangeScale[idx][i][j];
//			}
//		}
//
//		circleSpiralDeg[idx] = ((circleSpiralDeg[idx] + 5) % 360);
//		if (circleSpiralDeg[idx] % 180 == 0) circleSpiralRadius[idx] = circleSpiralRadius[idx] * 1.2f;
//		if (circleSpiralRadius[idx] > 0.8) circleSpiralRadius[idx] = 0.2f;
//	}
//
//	DrawAllTriangle(-1.0f);
//
//	glutPostRedisplay();
//
//	if (isMovingCircleSpiral) glutTimerFunc(30, MovingCircleSpiral, isMovingCircleSpiral);
//}
//
//void StopAllAnim()
//{
//	// �̵� ������ �ʱ�ȭ
//	for (int i = 0; i < NUM_OBJECT; i++)
//	{
//		dir_x[i] = 1.0f;
//		dir_y[i] = 1.0f;
//		zigzagDist_y[i] = 0.0f;
//		zigzag_yUp[i] = false;
//
//		circleSpiralDeg[i] = 0;
//		circleSpiralRadius[i] = 0.2f;
//
//		// �����̷� ����� ��� �ٽ� Pivot���� ��������
//		if ((isMovingRectSpiral || isMovingCircleSpiral))
//		{
//			for (int p = 0; p < 3; p++)
//			{
//				for (int q = 0; q < 3; q++)
//				{
//					if (q == 0) triShape[i][p][q] = spiralPivot[i][0] - triShapeScale[i][p][q];
//					if (q == 1) triShape[i][p][q] = spiralPivot[i][1] + triShapeScale[i][p][q];
//				}
//			}
//		}
//
//		rectSpiralDir[i][0] = -1.0f;
//		rectSpiralDir[i][1] = 0.0f;
//		rectSpiralDist[i] = 0.0f;
//		rectSpiralMaxDist[i] = 0.1f;
//		rectSpiralDistCount[i] = 0;
//		rectSpiralMaxCount[i] = 0;
//	}
//
//	isMovingDiagonal = false;
//	isMovingZigZag = false;
//	isMovingRectSpiral = false;
//	isMovingCircleSpiral = false;
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case '1':
//		StopAllAnim();
//		isMovingDiagonal = true;
//		glutTimerFunc(30, MovingDiagonal, isMovingDiagonal);
//		break;
//	case '2':
//		StopAllAnim();
//		GoZigZag();
//		isMovingZigZag = true;
//		glutTimerFunc(30, MovingZigZag, isMovingZigZag);
//		break;
//	case '3':
//		StopAllAnim();
//		isMovingRectSpiral = true;
//		SetSpiralPivot();
//		if (isMovingRectSpiral) glutTimerFunc(30, MovingRectSpiral, isMovingRectSpiral);
//		break;
//	case '4':
//		StopAllAnim();
//		isMovingCircleSpiral = true;
//		SetSpiralPivot();
//		glutTimerFunc(30, MovingCircleSpiral, isMovingCircleSpiral);
//		break;
//	case 'q':
//		exit(EXIT_FAILURE);
//		break;
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}
