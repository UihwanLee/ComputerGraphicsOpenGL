#include <iostream>
#include "OpenGL.h"
#include <random>
#include <vector>
#include "InitShader.h"
#include "ObjectManager.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define WIDTH 800
#define HEIGHT 600

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

// 클래스
ObjectManager ObjMgr;

GLvoid Init();
GLvoid Reset();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[2], EBO;
bool isDepthTest = false;

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL);
GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, float* modelInfo, int idx);

// 충돌처리
struct Point {
	float x, y;
};

vector<Point> CrossPoint;

GLvoid CheckCollisionWithTri(int line, int tri);
GLvoid CheckCollisionWithLine();


// 회전 애니메이션

glm::mat4 model = glm::mat4(1.0f);

void Keyboard(unsigned char key, int x, int y);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid MouseDrag(int x, int y);
bool g_left_button = false;


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Training");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	Init();

	InitProgram(ShaderProgram);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid Init()
{
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	Reset();
}

GLvoid Reset()
{
	ObjMgr.Reset();

	ObjMgr.CreateLine();
	ObjMgr.CreateTri();
	//ObjMgr.CreateRect();
	//ObjMgr.CreatePenta();
}

GLvoid drawScene()
{
	glClearColor(231.0f / 255.0f, 254.0f / 255.0f, 255.0f / 255.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	if (isDepthTest)	glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	for (int i = 0; i < ObjMgr.m_ObjectList.size(); i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			if (ObjMgr.m_ObjectList[i].m_isModeIDX)
			{
				DrawObjectByIDX(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_pos, ObjMgr.m_ObjectList[i].m_inex, ObjMgr.m_ObjectList[i].m_col,
					ObjMgr.m_ObjectList[i].m_pivot, ObjMgr.m_ObjectList[i].m_rotate, ObjMgr.m_ObjectList[i].m_scale,
					ObjMgr.m_ObjectList[i].m_num_vertex, ObjMgr.m_ObjectList[i].m_size_pos, ObjMgr.m_ObjectList[i].m_size_idx, ObjMgr.m_ObjectList[i].m_model,
					i);
			}
		}
	}

	for (int i = 0; i < 1; i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			if (ObjMgr.m_ObjectList[i].m_isModeIDX == false)
			{
				DrawObjectByArray(ObjMgr.m_ObjectList[i].m_DRAW_TYPE, ObjMgr.m_ObjectList[i].m_pos, ObjMgr.m_ObjectList[i].m_col,
					ObjMgr.m_ObjectList[i].m_num_vertex, ObjMgr.m_ObjectList[i].m_size_pos);
			}
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

GLvoid DrawObjectByArray(int DRAW_TYPE, void* posList, void* colList, int NUM_VETEX, int SIZE_COL)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, posList, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(1.0f, 1.0f, 1.0f));

	move = glm::translate(move, glm::vec3(0.0f, 0.0f, 0.0f));

	model = model * scale * move * rot;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, colList, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_LINES, 0, NUM_VETEX);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid DrawObjectByIDX(int DRAW_TYPE, void* obj_pos, void* obj_index, void* obj_color, float* pivot, float* rotateInfo, float* scaleInfo,
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, float* modelInfo, int idx)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_IDX, obj_index, GL_STATIC_DRAW);

	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 move = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(scaleInfo[0], scaleInfo[1], scaleInfo[2]));

	rot = glm::rotate(rot, glm::radians(rotateInfo[1]), glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, glm::radians(rotateInfo[0]), glm::vec3(0.0f, 1.0f, 0.0f));

	move = glm::translate(move, glm::vec3(pivot[0], pivot[1], pivot[2]));

	model = rot * move * scale;


	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(DRAW_TYPE, NUM_VETEX, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLvoid StopAllAnim()
{
	if (ObjMgr.m_ObjectList.size() < 2) return;

	for (int i = 1; i < ObjMgr.m_ObjectList.size(); i++)
	{
		ObjMgr.m_ObjectList[i].m_isAnimRotating = false;
	}

	Sleep(500);

	return;
}

// 세 점이 주어진 삼각형 내부에 있는지 확인하는 함수
bool isInsideTriangle(Point A, Point B, Point C, Point P) {
	double areaABC = 0.5 * abs(A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
	double areaPBC = 0.5 * abs(P.x * (B.y - C.y) + B.x * (C.y - P.y) + C.x * (P.y - B.y));
	double areaPCA = 0.5 * abs(P.x * (C.y - A.y) + C.x * (A.y - P.y) + A.x * (P.y - C.y));
	double areaPAB = 0.5 * abs(P.x * (A.y - B.y) + A.x * (B.y - P.y) + B.x * (P.y - A.y));

	// 세 삼각형의 넓이의 합이 삼각형 ABC의 넓이와 같으면 점 P는 삼각형 내부에 있음
	return (areaPBC + areaPCA + areaPAB == areaABC);
}

// 교차점이 선분 내에 있는지 확인하는 함수
bool isInsideLine(Point P1, Point P2, Point P) {
	bool inside = false;

	float max_x = (P1.x >= P2.x) ? P1.x : P2.x;
	float min_x = (P1.x < P2.x) ? P1.x : P2.x;
	float max_y = (P1.y >= P2.y) ? P1.y : P2.y;
	float min_y = (P1.y < P2.y) ? P1.y : P2.y;

	if (min_x <= P.x && max_x >= P.x && min_y <= P.y && max_y >= P.y)
	{
		inside = true;
	}

	// 세 삼각형의 넓이의 합이 삼각형 ABC의 넓이와 같으면 점 P는 삼각형 내부에 있음
	return inside;
}

// 두 직선의 교차점을 계산하는 함수
Point findIntersection(Point P1, Point P2, Point A, Point B) {
	float m1 = (P2.y - P1.y) / (P2.x - P1.x);
	float c1 = P1.y - m1 * P1.x;

	float m2 = (B.y - A.y) / (B.x - A.x);
	float c2 = A.y - m2 * A.x;

	// 두 직선의 교차점을 계산
	Point intersection;
	intersection.x = (c2 - c1) / (m1 - m2);
	intersection.y = m1 * intersection.x + c1;

	return intersection;
}

// 직선과의 교차 여부를 검사하는 함수
GLvoid doesIntersectTri(Point P1, Point P2, Point A, Point B, Point C) {
	// 직선의 방정식 계산
	float m = (P2.y - P1.y) / (P2.x - P1.x);
	float c = P1.y - m * P1.x;

	// 세 변에 대한 직선의 방정식 계산
	float lineAB = A.y - m * A.x - c;
	float lineBC = B.y - m * B.x - c;
	float lineCA = C.y - m * C.x - c;

	Point intersection;
	CrossPoint.clear();

	// 모든 세 변이 직선과 만나지 않으면 삼각형과 직선은 만나지 않음
	if ((lineAB > 0 && lineBC > 0 && lineCA > 0) || (lineAB < 0 && lineBC < 0 && lineCA < 0)) {
		return;
	}
	// 위 조건 중 하나라도 만족하면 삼각형과 직선이 만남
	else
	{
		if (lineAB != 0) {
			intersection = findIntersection(P1, P2, A, B);
			if (isInsideLine(A, B, intersection)) CrossPoint.push_back(intersection);
		}
		if (lineBC != 0) {
			intersection = findIntersection(P1, P2, B, C);
			if (isInsideLine(B, C, intersection)) CrossPoint.push_back(intersection);
		}
		if (lineCA != 0) {
			intersection = findIntersection(P1, P2, C, A);
			if (isInsideLine(C, A, intersection)) CrossPoint.push_back(intersection);
		}
	}
}

// 선과 삼각형의 만나는 점 있는지 체크
GLvoid CheckCollisionWithTri(int line, int tri)
{
	Point P1, P2, A, B, C, R1, R2, R3;

	// 선 
	P1.x = ObjMgr.m_ObjectList[line].m_pos[0]; P2.x = ObjMgr.m_ObjectList[line].m_pos[3];
	P1.y = ObjMgr.m_ObjectList[line].m_pos[1]; P2.y = ObjMgr.m_ObjectList[line].m_pos[4];

	// 삼각형
	A.x = ObjMgr.m_ObjectList[tri].m_pos[0]; B.x = ObjMgr.m_ObjectList[tri].m_pos[3]; C.x = ObjMgr.m_ObjectList[tri].m_pos[6];
	A.y = ObjMgr.m_ObjectList[tri].m_pos[1]; B.y = ObjMgr.m_ObjectList[tri].m_pos[4]; C.y = ObjMgr.m_ObjectList[tri].m_pos[7];


	doesIntersectTri(P1, P2, A, B, C);
}

GLvoid CheckCollisionWithLine()
{
	int line = 0;
	int tri = 1; int rect = 2;

	CheckCollisionWithTri(line, tri);

	if (CrossPoint.size() >= 2)
	{
		cout << "선과 삼각형이 만남!" << endl;
	}
	else
	{
		//cout << crossTri.size() << endl;
		cout << "선과 삼각형이 만나지 않음!" << endl;
	}
}

GLvoid StartLine(int idx, float x, float z)
{
	ObjMgr.m_ObjectList[idx].m_isActive = true;
	ObjMgr.m_ObjectList[idx].m_pos[0] = x;
	ObjMgr.m_ObjectList[idx].m_pos[1] = z;
}

GLvoid DragLine(int idx, float x, float z)
{
	ObjMgr.m_ObjectList[idx].m_pos[3] = x;
	ObjMgr.m_ObjectList[idx].m_pos[4] = z;
}

GLvoid EndLine(int idx)
{
	ObjMgr.m_ObjectList[idx].m_isActive = false;
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	int idx = 0;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// 마우스 클릭 ...

		float start_x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f; // x 끝점
		float start_z = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT); // y 끝점

		// 선 생성(선 활성화)
		StartLine(idx, start_x, start_z);

		g_left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// 마우스 클릭 땔 때 ....

		// 선 제거(선 비활성화)
		EndLine(idx);

		// 충돌 처리
		CheckCollisionWithLine();

		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid MouseDrag(int x, int y)
{
	if (g_left_button)
	{
		int idx = 0;

		// 마우스 드래그에 따른 선 길이 조정
		float drag_x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f; // x 끝점
		float drag_z = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT); // y 끝점

		DragLine(idx, drag_x, drag_z);
	}

	glutPostRedisplay();
}



void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		Reset();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

	glutPostRedisplay();
}