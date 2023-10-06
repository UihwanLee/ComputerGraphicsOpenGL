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
GLvoid ResetObjects();
GLvoid UpdateObjects();
void ResetAllShape();
void DrawObjects(int DRAW_TYPE, int BYTE_SIZE, int vertex, int NUM, void* posList, void* colList);

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

// 좌표평면
GLfloat coordinatePlane[4][3];
GLfloat coordinatePlane_Color[4][3];

// 사각형
GLfloat rectShape[MAX_NUM_OBJECT][6][3];
GLfloat rectShapeScale[MAX_NUM_OBJECT][6][3];
GLfloat colorRect[MAX_NUM_OBJECT][6][3];
GLvoid SetRectanglePos(int shape_idx, int pivot_idx);
bool isChangingToRect = false;

GLuint vao, vbo[2], ebo[2];

// 도형 정보를 담고있는 구조체
typedef struct Object
{
	vector<float> pivot;
	int type_f;				// 도형 타입 : 점(0) / 선(1) / 삼각형(2) / 사각형(3) / 오각형(4)
	int shape_idx;
	float speed;
	float dir_x;
	float dir_y;
	bool isAnim;
	bool isActive;
};

vector<Object> objectList;
Object obj;
vector<float> pivot;

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
	DrawObjects(GL_TRIANGLES, BYTE_SIZE_RECTANGLE, 6, NUM_RECTANGLE, rectShape, colorRect);

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

	ResetObjects();
	UpdateObjects();
}

GLvoid ResetObjects()
{
	objectList.clear();
	obj.pivot.clear();
	obj.pivot.emplace_back(0.f);
	obj.pivot.emplace_back(0.f);
	obj.type_f = 3;
	obj.speed = 0.01f;
	obj.dir_x = 0.0f;
	obj.dir_y = 0.0f;
	obj.isAnim = false;
	obj.isActive = true;

	objectList.emplace_back(obj);
}

void ResetObjectNum()
{
	NUM_OBJECT = 0;
	NUM_RECTANGLE = 0;
}

GLvoid UpdateObjects()
{
	ResetObjectNum();

	NUM_OBJECT = objectList.size();
	// 현재 남아있는 오브젝트들을 type_f 변수에 따라 변수를 업데이트한다.
	for (int i = 0; i < NUM_OBJECT; i++)
	{
		// 비활성화된 오브젝트는 넘어감
		if (objectList[i].isActive == false) continue;

		NUM_RECTANGLE += 1;
		objectList[i].shape_idx = NUM_RECTANGLE - 1;
		SetRectanglePos(NUM_RECTANGLE - 1, i);
	}

}

// 행과 열이 다른 배열을 모두 받기 위해 템플릿 사용
template <typename T, size_t Depth, size_t Rows, size_t Cols>
GLvoid InitBufferByIdx(T(&buffer)[Depth][Rows][Cols], int i, int j, float x, float y, float z)
{
	buffer[i][j][0] = x;
	buffer[i][j][1] = y;
	buffer[i][j][2] = z;
}

GLvoid InitBufferByIdx2(float buffer[][3], int i, float x, float y, float z)
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
		if (i == 0)			InitBufferByIdx2(coordinatePlane, i, -1.0f, 0.f, 0.f);
		else if (i == 1)	InitBufferByIdx2(coordinatePlane, i, 1.0f, 0.f, 0.f);
		else if (i == 2)	InitBufferByIdx2(coordinatePlane, i, 0.f, -1.0f, 0.f);
		else if (i == 3)	InitBufferByIdx2(coordinatePlane, i, 0.f, 1.0f, 0.f);

		InitBufferByIdx2(coordinatePlane_Color, i, 0.f, 0.f, 0.f);
	}

	for (int i = 0; i < MAX_NUM_OBJECT; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			// 사각형
			if (j == 0)								InitBufferByIdx(rectShapeScale, i, j, 0.5f, 0.5f, 0.f);
			else if (j == 1 || j == 3)				InitBufferByIdx(rectShapeScale, i, j, 0.5f, -0.5f, 0.f);
			else if (j == 2 || j == 5)				InitBufferByIdx(rectShapeScale, i, j, -0.5f, 0.5f, 0.f);
			else if (j == 4)						InitBufferByIdx(rectShapeScale, i, j, -0.5f, -0.5f, 0.f);


			// 기본 버퍼 위치&색상 초기화
			if (j < 6)
			{
				InitBufferByIdx(rectShape, i, j, 0.f, 0.f, 0.f);
				InitBufferByIdx(colorRect, i, j, 160.0f / 255.0f, 212.0f / 255.0f, 104.0f / 255.0f);
			}
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

GLvoid SetObjectPosByIdx(int idx)
{
	
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

	// 마우스 버튼을 클릭하거나 놓았을 때 그 지점에 오브젝트가 있는지 판별하기
	for (int i = 0; i < NUM_OBJECT; i++)
	{
		// 현재 드래그하고 있는 도형은 넘어가기
		if (pre_obj_idx != -1 && pre_obj_idx == i) continue;

		// 비활성화된 도형은 넘어가기
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
			// 마우스 드래그에 따른 도형 이동
			objectList[cur_obj_idx].pivot[0] = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
			objectList[cur_obj_idx].pivot[1] = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);

			SetObjectPosByIdx(cur_obj_idx);
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
