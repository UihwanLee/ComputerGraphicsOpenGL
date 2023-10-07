//#include <iostream>
//#include <gl/glew.h> //--- 필요한 헤더파일 include
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <stdlib.h>
//#include <stdio.h>
//#define _CRT_SECURE_NO_WARNINGS
//
//#define MAX_NUM_OBJECT 10
//#define BYTE_SIZE_POINT 12
//#define BYTE_SIZE_LINE 24
//#define BYTE_SIZE_TRIANGLE 36
//#define BYTE_SIZE_RECTANGLE 72
//
//#define MAX_TRI_ROW 3
//#define MAX_TRI_COL 3
//
//using namespace std;
//
//void make_vertexShaders();
//void make_fragmentShaders();
//void make_shaderProgram();
//GLvoid drawScene();
//GLvoid Reshape(int w, int h);
//
//// 입력 처리 함수
//GLvoid MouseClick(int button, int state, int x, int y);
//GLvoid Keyboard(unsigned char key, int x, int y);
//
//void InitBuffer();
//void DrawAllPoint(int idx, float move_x, float move_y);
//void DrawAllLine(int idx, float move_x, float move_y);
//void DrawAllTriangle(int idx, float move_x, float move_y);
//void DrawAllRectangle(int idx, float move_x, float move_y);
//
//GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
//GLuint vertexShader, fragmentShader; //--- 세이더 객체
//GLuint shaderProgramID; //--- 셰이더 프로그램
//
//// 점
//GLfloat pointShape[MAX_NUM_OBJECT][3];
//GLfloat color_point[MAX_NUM_OBJECT][3];
//
//// 선
//GLfloat lineShape[MAX_NUM_OBJECT][2][3];
//GLfloat colorLine[MAX_NUM_OBJECT][2][3];
//
//// 삼각형
//GLfloat triShape[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL]; //--- 삼각형 위치 값
//GLfloat triShapeScale[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
//GLfloat colors[MAX_NUM_OBJECT][MAX_TRI_ROW][MAX_TRI_COL];
//
//// 사각형
//GLfloat rectShape[MAX_NUM_OBJECT][6][3];
//GLfloat rectShapeScale[MAX_NUM_OBJECT][6][3];
//GLfloat colorRect[MAX_NUM_OBJECT][6][3];
//unsigned int rectIndex[MAX_NUM_OBJECT][2][3];
//unsigned int index[] = 
//{
//	0, 1, 3, // 첫 번째 삼각형
//	1, 2, 3 // 두 번째 삼각형
//};
//
//GLuint vao, vbo[2], ebo[2];
//GLfloat curPos[3] = { 0.f, 0.f, 0.f };
//
//// 글로벌 변수
//
//bool g_left_button = false;
//int CONDITION = 0;
//int NUM_POINT = 0;
//int NUM_LINE = 0;
//int NUM_TRIANGLE = 0;
//int NUM_RECTANGLE = 0;
//
//
//int main(int argc, char** argv)
//{
//	//--- 윈도우 생성하기
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 600);
//	glutCreateWindow("Example1");
//	//--- GLEW 초기화하기
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
//	//--- 변경된 배경색 설정
//	//glClearColor(rColor, gColor, bColor, 1.0f);
//	glClearColor(1.0, 1.0, 1.0, 1.0f);
//
//	////glClearColor(1.0, 1.0, 1.0, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	////--- 렌더링 파이프라인에 세이더 불러오기
//	glUseProgram(shaderProgramID);
//	////--- 사용할 VAO 불러오기
//	glBindVertexArray(vao);
//
//	////--- 오브젝트 그리기
//	glPointSize(5.0);
//	DrawAllPoint(-1, 0.f, 0.f);
//	DrawAllLine(-1, 0.f, 0.f);
//	DrawAllTriangle(-1, 0.f, 0.f);
//	DrawAllRectangle(-1, 0.f, 0.f);
//
//	glutSwapBuffers(); //--- 화면에 출력하기
//}
//
//void InitBuffer()
//{
//	for (int i = 0; i < MAX_NUM_OBJECT; i++)
//	{
//		// { -0.1, -0.1, 0.0 }, { 0.1, -0.1, 0.0 }, { 0.0, 0.1, 0.0}
//		for (int j = 0; j < 3; j++)
//		{
//			pointShape[i][j] = 0.f;
//			color_point[i][j] = 0.f;
//			if (j == 0)
//			{
//				lineShape[i][j][0] = 0.f;
//				lineShape[i][j][1] = 0.f;
//				lineShape[i][j][2] = 0.f;
//
//				colorLine[i][j][0] = 0.f;
//				colorLine[i][j][1] = 0.f;
//				colorLine[i][j][2] = 0.f;
//
//				triShape[i][j][0] = -0.1f;
//				triShape[i][j][1] = -0.1f;
//
//				triShapeScale[i][j][0] = -0.1f;
//				triShapeScale[i][j][1] = -0.1f;
//
//				colors[i][j][0] = 1.0f;
//				colors[i][j][1] = 0.0f;
//				colors[i][j][2] = 0.0f;
//
//				rectIndex[i][j][0] = 0;
//				rectIndex[i][j][1] = 1;
//				rectIndex[i][j][2] = 3;
//			}
//			else if (j == 1)
//			{
//				lineShape[i][j][0] = 0.f;
//				lineShape[i][j][1] = 0.f;
//				lineShape[i][j][2] = 0.f;
//
//				colorLine[i][j][0] = 0.f;
//				colorLine[i][j][1] = 0.f;
//				colorLine[i][j][2] = 0.f;
//
//				triShape[i][j][0] = 0.1f;
//				triShape[i][j][1] = -0.1f;
//
//				triShapeScale[i][j][0] = 0.1f;
//				triShapeScale[i][j][1] = -0.1f;
//
//				colors[i][j][0] = 0.0f;
//				colors[i][j][1] = 1.0f;
//				colors[i][j][2] = 0.0f;
//
//				rectIndex[i][j][0] = 1;
//				rectIndex[i][j][1] = 2;
//				rectIndex[i][j][2] = 3;
//			}
//			else if (j == 2)
//			{
//				triShape[i][j][0] = 0.f;
//				triShape[i][j][1] = 0.1f;
//
//				triShapeScale[i][j][0] = 0.f;
//				triShapeScale[i][j][1] = 0.1f;
//
//				colors[i][j][0] = 0.0f;
//				colors[i][j][1] = 0.0f;
//				colors[i][j][2] = 1.0f;
//			}
//
//			triShape[i][j][2] = 0.f;
//			triShapeScale[i][j][2] = 0.f;
//		}
//
//		// 사각형
//		for (int j = 0; j < 6; j++)
//		{
//			//0.5f, 0.5f, 0.0f,  우측 상단
//			//0.5f, -0.5f, 0.0f,  우측 하단
//			//-0.5f, -0.5f, 0.0f,  좌측 하단
//			//-0.5f, 0.5f, 0.0f  좌측 상단
//
//			// 1 0 0
//			// 0 1 0
//			// 0 0 1
//			// 1 0 0
//
//			if (j == 0)
//			{
//				rectShape[i][j][0] = 0.1f;
//				rectShape[i][j][1] = 0.1f;
//
//				rectShapeScale[i][j][0] = 0.1f;
//				rectShapeScale[i][j][1] = 0.1f;
//
//				colorRect[i][j][0] = 1.0f;
//				colorRect[i][j][1] = 0.f;
//				colorRect[i][j][2] = 0.f;
//			}
//			else if (j == 1 || j == 3)
//			{
//				rectShape[i][j][0] = 0.1f;
//				rectShape[i][j][1] = -0.1f;
//
//				rectShapeScale[i][j][0] = 0.1f;
//				rectShapeScale[i][j][1] = -0.1f;
//
//				colorRect[i][j][0] = 0.f;
//				colorRect[i][j][1] = 1.f;
//				colorRect[i][j][2] = 0.f;
//			}
//			else if (j == 2 || j == 5)
//			{
//				rectShape[i][j][0] = -0.1f;
//				rectShape[i][j][1] = 0.1f;
//
//				rectShapeScale[i][j][0] = -0.1f;
//				rectShapeScale[i][j][1] = 0.1f;
//
//				colorRect[i][j][0] = 0.f;
//				colorRect[i][j][1] = 0.f;
//				colorRect[i][j][2] = 1.0f;
//			}
//			else if (j == 4)
//			{
//				rectShape[i][j][0] = -0.1f;
//				rectShape[i][j][1] = -0.1f;
//
//				rectShapeScale[i][j][0] = -0.1f;
//				rectShapeScale[i][j][1] = -0.1f;
//
//				colorRect[i][j][0] = 1.0f;
//				colorRect[i][j][1] = 0.f;
//				colorRect[i][j][2] = 0.f;
//			}
//
//			rectShape[i][j][2] = 0.f;
//		}
//	}
//}
//
//void MoveAllObject(float move_x, float move_y)
//{
//	DrawAllPoint(-1, move_x, move_y);
//	DrawAllLine(-1, move_x, move_y);
//	DrawAllTriangle(-1, move_x, move_y);
//	DrawAllRectangle(-1, move_x, move_y);
//}
//
//void TryDrawPoint()
//{
//	if (CONDITION != 1) return;
//
//	if (NUM_POINT + 1 > MAX_NUM_OBJECT || ((NUM_POINT + NUM_LINE + NUM_TRIANGLE + NUM_RECTANGLE + 1) > MAX_NUM_OBJECT))
//	{
//		cout << "NUM_OUT_ERROR" << endl;
//		return;
//	}
//
//	NUM_POINT += 1;
//
//	DrawAllPoint(NUM_POINT - 1, 0.f, 0.f);
//}
//
//void DrawAllPoint(int idx, float move_x, float move_y)
//{
//	if (idx != -1)
//	{
//		pointShape[idx][0] = curPos[0];
//		pointShape[idx][1] = curPos[1];
//	}
//
//	// 위치 이동 변화가 있으면 위치 이동
//	for (int i = 0; i < NUM_TRIANGLE; i++)
//	{
//		pointShape[i][0] += move_x;
//		pointShape[i][1] += move_y;
//	}
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	glGenBuffers(2, vbo);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_POINT * NUM_POINT, pointShape, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_POINT * NUM_POINT, color_point, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(1);
//
//	glPointSize(5.0);
//	glDrawArrays(GL_POINTS, 0, 1 * NUM_POINT);
//}
//
//void TryDrawLine()
//{
//	if (CONDITION != 2) return;
//
//	if (NUM_LINE + 1 > MAX_NUM_OBJECT || ((NUM_POINT + NUM_LINE + NUM_TRIANGLE + NUM_RECTANGLE + 1) > MAX_NUM_OBJECT))
//	{
//		cout << "NUM_OUT_ERROR" << endl;
//		return;
//	}
//
//	NUM_LINE += 1;
//
//	DrawAllLine(NUM_LINE - 1, 0.f, 0.f);
//}
//
//void DrawAllLine(int idx, float move_x, float move_y)
//{
//	// 클릭한 곳에 선 생성
//	if (idx != -1)
//	{
//		for (int i = 0; i < 2; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				if (j == 0) lineShape[idx][i][j] = curPos[0];
//				if (i == 0 && j == 1) lineShape[idx][i][j] = curPos[1] + 0.1f;
//				if (i == 1 && j == 1) lineShape[idx][i][j] = curPos[1] - 0.1f;
//			}
//		}
//	}
//
//	// 위치 이동 변화가 있으면 위치 이동
//	for (int i = 0; i < NUM_LINE; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			lineShape[i][j][0] += move_x;
//			lineShape[i][j][1] += move_y;
//		}
//	}
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	glGenBuffers(2, vbo);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * NUM_LINE, lineShape, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_LINE * NUM_LINE, colorLine, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(1);
//
//	glDrawArrays(GL_LINES, 0, 2 * NUM_LINE);
//}
//
//void TryDrawTriangle()
//{
//	if (CONDITION != 3) return;
//
//	if (NUM_TRIANGLE + 1 > MAX_NUM_OBJECT || ((NUM_POINT + NUM_LINE + NUM_TRIANGLE + NUM_RECTANGLE + 1) > MAX_NUM_OBJECT))
//	{
//		cout << "NUM_OUT_ERROR" << endl;
//		return;
//	}
//
//	NUM_TRIANGLE += 1;
//
//	DrawAllTriangle(NUM_TRIANGLE - 1, 0.f, 0.f);
//}
//
//void DrawAllTriangle(int idx, float move_x, float move_y)
//{
//	// 클릭한 곳에 삼각형 생성
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
//	// 위치 이동 변화가 있으면 위치 이동
//	for (int i = 0; i < NUM_TRIANGLE; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			triShape[i][j][0] += move_x;
//			triShape[i][j][1] += move_y;
//		}
//	}
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao); 
//	glGenBuffers(2, vbo); 
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_TRIANGLE, triShape, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_TRIANGLE * NUM_TRIANGLE, colors, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(1);
//
//	glDrawArrays(GL_TRIANGLES, 0, 3 * NUM_TRIANGLE);
//}
//
//void TryDrawRectangle()
//{
//	if (CONDITION != 4) return;
//
//	if (NUM_RECTANGLE + 1 > MAX_NUM_OBJECT || ((NUM_POINT + NUM_LINE + NUM_TRIANGLE + NUM_RECTANGLE + 1) > MAX_NUM_OBJECT))
//	{
//		cout << "NUM_OUT_ERROR" << endl;
//		return;
//	}
//
//	NUM_RECTANGLE += 1;
//
//	DrawAllRectangle(NUM_RECTANGLE - 1, 0.f, 0.f);
//}
//
//void DrawAllRectangle(int idx, float move_x, float move_y)
//{
//	// 클릭한 곳에 삼각형 생성
//	if (idx != -1)
//	{
//		for (int i = 0; i < 6; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				if (j == 0) rectShape[idx][i][j] = curPos[0] - rectShapeScale[idx][i][j];
//				if (j == 1) rectShape[idx][i][j] = curPos[1] + rectShapeScale[idx][i][j];
//			}
//		}
//	}
//
//	// 위치 이동 변화가 있으면 위치 이동
//	for (int i = 0; i < NUM_RECTANGLE; i++)
//	{
//		for (int j = 0; j < 6; j++)
//		{
//			rectShape[i][j][0] += move_x;
//			rectShape[i][j][1] += move_y;
//		}
//	}
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	glGenBuffers(2, vbo);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_RECTANGLE * NUM_RECTANGLE, rectShape, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, BYTE_SIZE_RECTANGLE * NUM_RECTANGLE, colorRect, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(1);
//
//	glDrawArrays(GL_TRIANGLES, 0, 6 * NUM_RECTANGLE);
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
//	make_vertexShaders(); //--- 버텍스 세이더 만들기
//	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
//	//-- shader Program
//	shaderProgramID = glCreateProgram();
//	glAttachShader(shaderProgramID, vertexShader);
//	glAttachShader(shaderProgramID, fragmentShader);
//	glLinkProgram(shaderProgramID);
//	//--- 세이더 삭제하기
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program 사용하기
//	glUseProgram(shaderProgramID);
//}
//
//
//void make_vertexShaders()
//{
//	vertexSource = filetobuf("vertex.glsl");
//	//--- 버텍스 세이더 객체 만들기
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//--- 세이더 코드를 세이더 객체에 넣기
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
//	//--- 버텍스 세이더 컴파일하기
//	glCompileShader(vertexShader);
//	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if(!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
//		return;
//	}
//}
//
//void make_fragmentShaders()
//{
//	fragmentSource = filetobuf("fragment.glsl");
//	//--- 프래그먼트 세이더 객체 만들기
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	//--- 세이더 코드를 세이더 객체에 넣기
//	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
//	//--- 프래그먼트 세이더 컴파일
//	glCompileShader(fragmentShader);
//	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
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
//		TryDrawPoint();
//		TryDrawLine();
//		TryDrawTriangle();
//		TryDrawRectangle();
// 	}
//
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//	{
//		g_left_button = false;
//	}
//
//	glutPostRedisplay();
//}
//
//GLvoid Reset()
//{
//	CONDITION = 0;
//	NUM_POINT = 0;
//	NUM_LINE = 0;
//	NUM_TRIANGLE = 0;
//	NUM_RECTANGLE = 0;
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'p':
//		CONDITION = 1;
//		break;
//	case 'l':
//		CONDITION = 2;
//		break;
//	case 't':
//		CONDITION = 3;
//		break;
//	case 'r':
//		CONDITION = 4;
//		break;
//	case 'w':
//		MoveAllObject(0.f, 0.1f);
//		break;
//	case 'a':
//		MoveAllObject(-0.1f, 0.f);
//		break;
//	case 's':
//		MoveAllObject(0.f, -0.1f);
//		break;
//	case 'd':
//		MoveAllObject(0.1f, 0.f);
//		break;
//	case 'c':
//		Reset();
//		break;
//	case 'q':
//		exit(EXIT_FAILURE);
//		break;
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}
