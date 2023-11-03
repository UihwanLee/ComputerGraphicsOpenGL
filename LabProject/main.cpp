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
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, glm::mat4& model, int idx);

// 도형 IDX 변수
int line_idx = 0;
int box_idx = 1;
int tri_idx = 2;
int rect_idx = 3;


vector<Point> CrossPoint;

GLvoid CheckCollisionWithTri(int line, int tri);
GLvoid CheckCollisionWithLine();


// 애니메이션
GLvoid falling_Figure(int idx);
GLvoid moving_Box_Anim(int isAnim);

// 애니메이션 변수
bool isMovingBox = false;
float movingBoxSpeed = 0.05f;

int cur_idx = 0;

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
	ObjMgr.SetActive(0, false);

	ObjMgr.CreateRect();
	ObjMgr.SetScale(1, 0.6f, 0.1f, 1.0f);
	ObjMgr.SetPosition(1, 0.0f, -7.0f, 0.0f);

	ObjMgr.CreateTri();
	ObjMgr.SetPosition(2, 1.0f, 1.0f, 0.0f);

	ObjMgr.CreateRect();
	ObjMgr.SetPosition(3, -0.5f, 1.0f, 0.0f);


	//ObjMgr.CreatePenta();

	isMovingBox = true;
	if (isMovingBox) glutTimerFunc(30, moving_Box_Anim, isMovingBox);
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

	model = model * move * rot * scale;

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
	int NUM_VETEX, int SIZE_COL, int SIZE_IDX, glm::mat4& model, int idx)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, SIZE_COL, obj_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_IDX, obj_index, GL_STATIC_DRAW);

	// 모델변환
	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "modelTransform");

	ObjMgr.m_ObjectList[idx].m_model = ObjMgr.TransformModel(idx);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObjMgr.m_ObjectList[idx].m_model));

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

	isMovingBox = false;

	return;
}

GLvoid Start_Falling(int idx)
{
	ObjMgr.m_ObjectList[idx].m_isFalling = true;
	if (ObjMgr.m_ObjectList[idx].m_isFalling) glutTimerFunc(30, falling_Figure, idx);
}

GLvoid falling_Figure(int idx)
{
	float fall_Speed = ObjMgr.GetRandomFloatValue(0.02f, 0.04f);
	ObjMgr.Move(idx, 0.0f, -fall_Speed, 0.0f);

	// 땅에 떨어지거나 바구니에 담기면 종료
	if (ObjMgr.m_ObjectList[idx].m_model[3][1] < -1.5f)
	{
		ObjMgr.m_ObjectList[idx].m_isFalling = false;
	}

	glutPostRedisplay();

	if (ObjMgr.m_ObjectList[idx].m_isFalling) glutTimerFunc(30, falling_Figure, idx);
}

GLvoid moving_Box_Anim(int isAnim)
{
	float curBoxPos_x = ObjMgr.m_ObjectList[box_idx].m_model[3][0];
	float moving_offset = 0.8f;

	if (curBoxPos_x > moving_offset || curBoxPos_x < -moving_offset)
	{
		movingBoxSpeed = movingBoxSpeed * (-1.0f);
	}

	ObjMgr.Move(box_idx, movingBoxSpeed, 0.0f, 0.0f);

	glutPostRedisplay();

	if (isMovingBox) glutTimerFunc(30, moving_Box_Anim, isMovingBox);
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

	if (min_x <= P.x && max_x >= P.x)
	{
		inside = true;
	}

	cout <<  P.x <<  endl;

	// 세 삼각형의 넓이의 합이 삼각형 ABC의 넓이와 같으면 점 P는 삼각형 내부에 있음
	return inside;
}

// 뽑아낸 두 교차점을 기준으로 새로운 두 도형 생성
GLvoid CreatFigureByline(bool AB, bool BC, bool CA, Point A, Point B, Point C, float r, float g, float b)
{
	Point P1 = CrossPoint[0]; 
	Point P2 = CrossPoint[1];
	int idx = 3;

	// case 1 : AB / BC -> BP1P2 / AXYC
	if (AB == true && BC == true)
	{
		ObjMgr.CreateTriCustom(B, P1, P2, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, -0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//Start_Falling(idx);

		ObjMgr.CreateRectCustom(A, P1, P2, C, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//(idx);
	}

	// case 2 : AB / AC -> AXY / BXYC
	if (AB == true && CA == true)
	{
		ObjMgr.CreateTriCustom(A, P1, P2, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.0f, 0.01f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//Start_Falling(idx);

		ObjMgr.CreateRectCustom(B, P1, P2, C, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.0f, -0.01f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//Start_Falling(idx);
	}

	// case 3 : BC / AC -> CXY / AXYB
	if (BC == true && CA == true)
	{
		ObjMgr.CreateTriCustom(C, P1, P2, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;

		ObjMgr.CreateRectCustom(A, P2, P1, B, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, -0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
	}

	ObjMgr.SetActive(tri_idx, false);
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
bool doesIntersectTri(Point P1, Point P2, Point A, Point B) 
{
	float x1 = P1.x, y1 = P1.y;
	float x2 = P2.x, y2 = P2.y;
	float x3 = A.x, y3 = A.y;
	float x4 = B.x, y4 = B.y;

	float temp = A.y - B.y;

	float a1 = (y2 - y1) / (x2 - x1);
	float b1 = y1 - a1 * x1;

	float a2 = (y4 - y3) / (x4 - x3);

	Point intersection;

	// 기울기가 같으면(평행하면) 만나지 않음
	if (a1 == a2) {
		cout << "평행!" << endl;
		return false;
	}
	// 위 조건 중 하나라도 만족하면 삼각형과 직선이 만남
	// 두 교차점을 뽑아내고 두 교차점을 기준으로 양분화하기
	else
	{
		float intersectX = -10;
		float intersectY = -10;

		float m1 = (P2.y - P1.y) / (P2.x - P1.x);
		float c1 = P1.y - m1 * P1.x;

		float m2 = (B.y - A.y) / (B.x - A.x);
		float c2 = A.y - m2 * A.x;

		// 두 직선의 교차점을 계산
		intersectX = (c2 - c1) / (m1 - m2);
		intersectY = m1 * intersectX + c1;

		if (temp == 0) intersectY = A.y;

		if (intersectX >= std::min(x1, x2) && intersectX <= std::max(x1, x2) &&
			intersectX >= std::min(x3, x4) && intersectX <= std::max(x3, x4) &&
			intersectY >= std::min(y1, y2) && intersectY <= std::max(y1, y2) &&
			intersectY >= std::min(y3, y4) && intersectY <= std::max(y3, y4)) {
			intersection.x = intersectX;
			intersection.y = intersectY;
			CrossPoint.push_back(intersection);
			return true;
		}
	}
	return false;
}

bool doLinesIntersectRect(Point p1, Point p2, Point p3, Point p4) {
	float x1 = p1.x, y1 = p1.y;
	float x2 = p2.x, y2 = p2.y;
	float x3 = p3.x, y3 = p3.y;
	float x4 = p4.x, y4 = p4.y;

	float a1 = x1 - x2;
	float a2 = y1 - y2;
	float a3 = x3 - x4;
	float a4 = y3 - y4;

	float a = (y2 - y1) / (x2 - x1);
	float b = y1 - a * x1;

	bool isSameX = (a3 == 0) ? true : false;
	bool isSameY = (a4 == 0) ? true : false;
	Point intersection;

	if ((a1==0 && a3==0) || (a2==0 && a4==0)) {
		return false;
	}
	else {
		float intersectX = -10;
		float intersectY = -10;
		if (isSameX)
		{
			intersectX = x3;
			intersectY = a * x3 + b;
		}
		else if (isSameY)
		{
			intersectX = (y3-b)/a;
			intersectY = y3;
		}

		// Check if the intersection point is within the line segments
		if (intersectX >= std::min(x1, x2) && intersectX <= std::max(x1, x2) &&
			intersectX >= std::min(x3, x4) && intersectX <= std::max(x3, x4) &&
			intersectY >= std::min(y1, y2) && intersectY <= std::max(y1, y2) &&
			intersectY >= std::min(y3, y4) && intersectY <= std::max(y3, y4)) {
			intersection.x = intersectX;
			intersection.y = intersectY;
			CrossPoint.push_back(intersection);
			return true;
		}
		cout << endl;
	}

	return false;
}

// 선과 삼각형의 만나는 점 있는지 체크
GLvoid CheckCollisionWithTri(int line, int tri)
{
	CrossPoint.clear();
	Point P1, P2, A, B, C, R1, R2, R3;

	// 선 
	P1.x = ObjMgr.m_ObjectList[line].m_pos[0]; P2.x = ObjMgr.m_ObjectList[line].m_pos[3];
	P1.y = ObjMgr.m_ObjectList[line].m_pos[1]; P2.y = ObjMgr.m_ObjectList[line].m_pos[4];

	// 피봇을 기준으로 삼각형 세 점 뽑기
	float x = ObjMgr.m_ObjectList[tri].m_model[3][0];
	float y = ObjMgr.m_ObjectList[tri].m_model[3][1];

	// 삼각형
	// 0.3 / 2 => 0.15f
	A.x = x; A.y = y + 0.15f;
	B.x = x - 0.15f; B.y = y - 0.15f;
	C.x = x + 0.15f; C.y = y - 0.15f;

	// 색상
	float r = ObjMgr.m_ObjectList[tri].m_col[0];
	float g = ObjMgr.m_ObjectList[tri].m_col[1];
	float b = ObjMgr.m_ObjectList[tri].m_col[2];

	bool AB = false;
	bool BC = false;
	bool CA = false;

	if (doesIntersectTri(P1, P2, A, B))
	{
		AB = true;
	}
	if (doesIntersectTri(P1, P2, B, C))
	{
		BC = true;
	}
	if (doesIntersectTri(P1, P2, C, A))
	{
		CA = true;
	}

	if (CrossPoint.size() >= 2)
	{
		CreatFigureByline(AB, BC, CA, A, B, C, r, g, b);
		return;
	}
	else
	{
		cout << "삼각형과 만나지 않음!" << endl;
	}
}

// 뽑아낸 두 교차점을 기준으로 새로운 두 도형 생성
GLvoid CreatFigureByRect(bool AB, bool BC, bool CD, bool DA, Point A, Point B, Point C, Point D, float r, float g, float b)
{
	Point P1 = CrossPoint[0];
	Point P2 = CrossPoint[1];
	int idx = 3;

	// case 1 : AB / DA 
	if (AB == true && DA == true)
	{
		ObjMgr.CreateTriCustom(A, P2, P1, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, -0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//Start_Falling(idx);

		ObjMgr.CreatePentaCustom(P2, P1, D, B, C, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//(idx);
	}
	// case 2 : AB / BC 
	if (AB == true && BC == true)
	{
		ObjMgr.CreateTriCustom(B, P1, P2, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, -0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//Start_Falling(idx);

		ObjMgr.CreatePentaCustom(P1, P2, A, C, D, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
		//Start_Falling(idx);
	}

	// case 3 : BC / CD
	if (BC == true && CD == true)
	{
		ObjMgr.CreateTriCustom(C, P1, P2, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;

		ObjMgr.CreatePentaCustom(P1, P2, B, D, A, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, -0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
	}

	// case 4 : CD / DA
	if (CD == true && DA == true)
	{
		ObjMgr.CreateTriCustom(D, P1, P2, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;

		ObjMgr.CreatePentaCustom(P1, P2, C, A, B, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, -0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
	}

	// case 5 : DA / BC
	if (DA == true && BC == true)
	{
		ObjMgr.CreateRectCustom(P2, A, B, P1, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, -0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;

		ObjMgr.CreateRectCustom(D, P2, P1, C, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.01f, 0.0f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
	}

	// case 6 : AB / CD
	if (AB == true && CD == true)
	{
		ObjMgr.CreateRectCustom(A, P1, P2, D, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.0f, 0.01f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;

		ObjMgr.CreateRectCustom(P1, B, C, P2, r, g, b);
		idx = ObjMgr.m_ObjectList.size() - 1;
		ObjMgr.SetScale(idx, 1.0f, 1.0f, 1.0f);
		ObjMgr.SetPosition(idx, 0.0f, -0.01f, 0.0f);
		ObjMgr.m_ObjectList[idx].m_isCut = true;
	}
	

	ObjMgr.SetActive(3, false);
}


// 선과 사각형이 만나는 점 있는지 체크
GLvoid CheckCollisionWithRect(int line, int rect)
{
	CrossPoint.clear();
	Point P1, P2, A, B, C, D, R1, R2, R3;

	// 선 
	P1.x = ObjMgr.m_ObjectList[line].m_pos[0]; P2.x = ObjMgr.m_ObjectList[line].m_pos[3];
	P1.y = ObjMgr.m_ObjectList[line].m_pos[1]; P2.y = ObjMgr.m_ObjectList[line].m_pos[4];

	// 피봇을 기준으로 삼각형 세 점 뽑기
	float x = ObjMgr.m_ObjectList[rect].m_model[3][0];
	float y = ObjMgr.m_ObjectList[rect].m_model[3][1];

	// 사각형
	// 0.3 / 2 => 0.15f
	A.x = x - 0.15f; A.y = y + 0.15f;
	B.x = x - 0.15f; B.y = y - 0.15f;
	C.x = x + 0.15f; C.y = y - 0.15f;
	D.x = x + 0.15f; D.y = y + 0.15f;

	// 색상
	float r = ObjMgr.m_ObjectList[rect].m_col[0];
	float g = ObjMgr.m_ObjectList[rect].m_col[1];
	float b = ObjMgr.m_ObjectList[rect].m_col[2];

	bool AB = false;
	bool BC = false;
	bool CD = false;
	bool DA = false;

	if (doLinesIntersectRect(P1, P2, A, B))
	{
		AB = true;
	}
	if (doLinesIntersectRect(P1, P2, B, C))
	{
		BC = true;
	}
	if (doLinesIntersectRect(P1, P2, C, D))
	{
		CD = true;
	}
	if (doLinesIntersectRect(P1, P2, D, A))
	{
		DA = true;
	}

	if (CrossPoint.size() >= 2)
	{
		cout << "사각형과 만남!" << endl;
		cout << AB << ",  " << BC << ", " << CD << ", " << DA << endl;
		CreatFigureByRect(AB, BC, CD, DA, A, B, C, D, r, g, b);
		return;
	}
	else
	{
		cout << "사각형과 만나지 않음!" << endl;
	}
}

GLvoid CheckCollisionWithLine()
{
	for (int i = 2; i < ObjMgr.m_ObjectList.size(); i++)
	{
		if (ObjMgr.m_ObjectList[i].m_isActive)
		{
			// 삼각형
			if (ObjMgr.m_ObjectList[i].m_num_idx == 3)
			{
				if (ObjMgr.m_ObjectList[i].m_isCut == false)
				{
					CheckCollisionWithTri(line_idx, i);
				}
			}
			else if (ObjMgr.m_ObjectList[i].m_num_idx == 4)
			{
				if (ObjMgr.m_ObjectList[i].m_isCut == false)
				{
					CheckCollisionWithRect(line_idx, i);
				}
			}
		}
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