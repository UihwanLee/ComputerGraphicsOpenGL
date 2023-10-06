#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>

#define WIDTH 600
#define HEIGHT 600

using namespace std;

GLvoid Init();
GLvoid GenerateRect();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid MouseDrag(int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);

struct Color
{
	float red = 1.0f;
	float green = 1.0f;
	float blue = 1.0f;
	float alpha = 1.0f;
};

struct Pivot
{
	GLfloat x;
	GLfloat y;
};

struct Rect
{
	Color color;
	Pivot pivot;
	bool isActive = false;
	int generate;
	int order;
	int condition_y;
	GLfloat size;
};


// 글로벌 변수
Color bgColor;
vector<Rect> gRect;
Rect gPlayer;
bool g_left_button = false;


void main(int argc, char** argv)
{
	// 사각형 초기화 및 생성
	Init();

	// 윈도우 생성
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example2");

	// GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLfloat GetRandomFloatValue(GLfloat min, GLfloat max)
{
	GLfloat value;

	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<float> dis(min, max);

	value = dis(gen);

	return value;
}

GLvoid GenerateRect()
{

	GLfloat randX, randY;

	// 랜덤 값 생성
	randX = GetRandomFloatValue(-1.0f, 1.0f);
	randY = GetRandomFloatValue(-1.0f, 1.0f);

	// 랜덤 색상
	GLfloat r, g, b;
	r = GetRandomFloatValue(0.0f, 255.0f);
	g = GetRandomFloatValue(0.0f, 255.0f);
	b = GetRandomFloatValue(0.0f, 255.0f);


	Rect temp;
	temp.color = { r / 255.0f, g / 255.0f, b / 255.0f,1.0f };
	temp.pivot = { randX, randY };
	temp.order = 0;
	temp.generate = 0;
	temp.size = 0.02f;
	temp.isActive = true;

	gRect.emplace_back(temp);
}

GLvoid Init()
{
	// 플레이어 초기화
	gPlayer.color = { 0.0f, 0.0f, 0.0f, 1.0f };
	gPlayer.pivot = { 0.0f, 0.0f };
	gPlayer.size = 0.04f;
	gPlayer.isActive = false;

	int MAX_SIZE = GetRandomFloatValue(20, 40);
	for (int i = 0; i < MAX_SIZE; i++)
	{
		GenerateRect();
	}
}

GLvoid ResetRect()
{
	gRect.clear();

	//// 플레이어 리셋
	//gPlayer.color = { 0.0f, 0.0f, 0.0f, 1.0f };
	//gPlayer.pivot = { 0.0f, 0.0f };
	//gPlayer.size = 0.04f;

	// 사각형 다시 생성
	int MAX_SIZE = GetRandomFloatValue(20, 40);
	for (int i = 0; i < MAX_SIZE; i++)
	{
		GenerateRect();
	}
}

GLvoid drawScene()
{
	// 변경된 배경색 설정
	glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < gRect.size(); i++)
	{
		// 사각형 그리기
		if (gRect[i].isActive)
		{
			glColor3f(gRect[i].color.red, gRect[i].color.green, gRect[i].color.blue);
			glRectf(gRect[i].pivot.x - gRect[i].size, gRect[i].pivot.y - gRect[i].size, gRect[i].pivot.x + gRect[i].size, gRect[i].pivot.y + gRect[i].size);
		}

		// 플레이어 생성
		if (gPlayer.isActive)
		{
			glColor3f(gPlayer.color.red, gPlayer.color.green, gPlayer.color.blue);
			glRectf(gPlayer.pivot.x - gPlayer.size, gPlayer.pivot.y - gPlayer.size, gPlayer.pivot.x + gPlayer.size, gPlayer.pivot.y + gPlayer.size);
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

// 플레이어와 사각형 충돌체크
bool CheckCollisionPlayer(Pivot player, GLfloat playerSize, Pivot rect, GLfloat rectSize)
{
	// 사각형
	GLfloat x1 = rect.x - rectSize;
	GLfloat x2 = rect.x + rectSize;
	GLfloat y1 = rect.y - rectSize;
	GLfloat y2 = rect.y + rectSize;

	// 플레이어
	GLfloat w1 = player.x - playerSize;
	GLfloat w2 = player.x + playerSize;
	GLfloat h1 = player.y - playerSize;
	GLfloat h2 = player.y + playerSize;

	if (x2 >= w1 && h1 <= y2 && x1 <= w2 && y1 <= h2)
	{
		return true;
	}

	return false;
}

GLvoid CheckAllCollision()
{
	for (int i = 0; i < gRect.size(); i++)
	{
		if (gRect[i].isActive)
		{
			if (CheckCollisionPlayer(gPlayer.pivot, gPlayer.size, gRect[i].pivot, gRect[i].size))
			{
				// 사각형 비활성화
				gRect[i].isActive = false;

				// 플레이어 색깔 먹은 사각형으로 변환
				gPlayer.color = gRect[i].color;

				// 플레이어 크기 증가
				gPlayer.size += 0.01f;
			}
		}
	}
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gPlayer.isActive = true;
		g_left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// 플레이어 리셋
		gPlayer.isActive = false;
		gPlayer.color = { 0.0f, 0.0f, 0.0f, 1.0f };
		gPlayer.pivot = { 0.0f, 0.0f };
		gPlayer.size = 0.04f;

		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid MouseDrag(int x, int y)
{
	if (g_left_button && gPlayer.isActive)
	{
		// 마우스 드래그에 따른 도형 이동
		gPlayer.pivot.x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		gPlayer.pivot.y = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);

		// 충돌체크
		CheckAllCollision();
	}

	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		ResetRect();
		break;
	case 'q':
		exit(EXIT_FAILURE);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
