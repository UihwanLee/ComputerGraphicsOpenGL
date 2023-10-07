#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>
#include <random>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define _CRT_SECURE_NO_WARNINGS

#define MAX_NUM_OBJECT 1

#define BYTE_SIZE_LINE 24

#define WIDTH 800
#define HEIGHT 600

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,0.9,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

using namespace std;

float angle = 0.0f;

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
GLvoid ResetAllShape();
void DrawObjects(void* posList, void* colList, void* indexList);
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation);

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

// 좌표평면
GLfloat coordinatePlane[4][3];
GLfloat coordinatePlane_Color[4][3];

// 정육면체의 정점 데이터
GLint cube_indices[] = {
	0, 1, 2, 2, 3, 0, // 앞면
	4, 5, 6, 6, 7, 4, // 뒷면
	7, 3, 0, 0, 4, 7, // 왼쪽면
	1, 5, 6, 6, 2, 1, // 오른쪽면
	7, 6, 5, 5, 4, 7, // 윗면
	3, 2, 1, 1, 0, 3  // 아랫면
};

GLfloat cube_vertices[] = {
	-0.5f, -0.5f, -0.5f, // 0
	 0.5f, -0.5f, -0.5f, // 1
	 0.5f,  0.5f, -0.5f, // 2
	-0.5f,  0.5f, -0.5f, // 3
	-0.5f, -0.5f,  0.5f, // 4
	 0.5f, -0.5f,  0.5f, // 5
	 0.5f,  0.5f,  0.5f, // 6
	-0.5f,  0.5f,  0.5f  // 7
};

GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f
};

GLUquadricObj* qobj;

GLuint vao, vbo[2], ebo;
GLuint modelLocation;
glm::mat4 MVP, View, Projection;

// 글로벌 변수
int g_cur_area = 0;
int CONDITION = 1;
int NUM_OBJECT = 0;
int NUM_RECTANGLE = 0;

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

GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);
	glGenBuffers(1, &ebo);

	modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
}

GLvoid Reset()
{
	ResetAllShape();
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

	DrawObjects(cube_vertices, colors, cube_indices);

	//transformObject(1.0f, XY_AXIS, angle += 5.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glutSwapBuffers(); //--- 화면에 출력하기
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
}

void DrawObjects(void* posList, void* colList, void* indexList)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(posList), posList, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexList), indexList, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	/*glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), colList, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);*/

	glEnable(GL_DEPTH_TEST);
}

void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	MVP = Projection * View * Model;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &MVP[0][0]);
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


GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		g_left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid MouseDrag(int x, int y)
{
	// && g_cur_rect != -1
	if (g_left_button && cur_obj_idx != -1)
	{

	}

	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		break;
	case 'q':
		exit(EXIT_FAILURE);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}