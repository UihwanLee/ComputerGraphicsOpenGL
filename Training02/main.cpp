#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define WIDTH 800
#define HEIGHT 600

using namespace std;

GLvoid InitRect();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);

struct Color
{
	float red = 1.0f;
	float green = 1.0f;
	float blue = 1.0f;
	float alpha = 1.0f;
};

struct Figure
{
	GLfloat x1;
	GLfloat y1;
	GLfloat x2;
	GLfloat y2;
};

struct Rect
{
	Color color;
	Figure figure;
};

// �۷ι� ����
Color bgColor;
Rect gRect[4];
int g_cur_area = 0;
int g_cur_rect = 0;

void main(int argc, char** argv)
{
	// �簢�� �ʱ�ȭ
	InitRect();

	// ������ ����
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example2");

	// GLEW �ʱ�ȭ
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
	glutMouseFunc(Mouse);
	//glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid InitRect()
{
	gRect[0].color = { 255.0f, 255.0f, 0.0f ,1.0f };
	gRect[0].figure = { -0.99f, 0.01f, -0.01f, 0.99f };

	gRect[1].color = { 255.0f, 0.0f, 0.0f ,1.0f };
	gRect[1].figure = { 0.01f, 0.01f, 0.99f, 0.99f };

	gRect[2].color = { 0.0f, 0.0f, 255.0f ,1.0f };
	gRect[2].figure = { -0.99f, -0.99f, -0.01f, -0.01f };

	gRect[3].color = { 0.0f, 255.0f, 0.0f ,1.0f };
	gRect[3].figure = { 0.01f, -0.99f, 0.99f, -0.01f };
}

GLvoid drawScene()
{
	// ����� ���� ����
	glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha);
	glClear(GL_COLOR_BUFFER_BIT);

	// �簢�� �׸���
	for (int i = 0; i < 4; i++)
	{
		glColor3f(gRect[i].color.red, gRect[i].color.green, gRect[i].color.blue);
		glRectf(gRect[i].figure.x1, gRect[i].figure.y1, gRect[i].figure.x2, gRect[i].figure.y2);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

// Ŭ���� ���콺 �����Ͱ� ��� �������� üũ
GLvoid CheckArea(int x, int y)
{
	int rect_x1, rect_y1, rect_x2, rect_y2;
	// �� 1�簢�� : x(400 * 0.01 ~ 400 * 0.99) y(300 * 0.01 ~ 300 * 0.99)
	rect_x1 = (WIDTH/2) * (1+ gRect[0].figure.x1);
	rect_x2 = (WIDTH / 2) * (1 + gRect[0].figure.x2);
	rect_y1 = (HEIGHT / 2) * gRect[0].figure.y1;
	rect_y2 = (HEIGHT / 2) * gRect[0].figure.y2;

	g_cur_rect = 0;

	// �� 1 ����
	if (0 <= x && x < (WIDTH / 2) && 0 <= y && y < (HEIGHT / 2))
	{
		g_cur_area = 1;
		if (rect_x1 <= x && x <= rect_x2 && rect_y1 <= y && y <= rect_y2) g_cur_rect = 1;
	}

	// �� 2�簢�� : x(400 * 0.01 ~ 400 * 0.99) y(300 * 0.01 ~ 300 * 0.99)
	rect_x1 = (WIDTH/2) * gRect[1].figure.x1 + (WIDTH/2);
	rect_x2 = (WIDTH/2) * gRect[1].figure.x2 + (WIDTH/2);
	rect_y1 = (HEIGHT/2) * gRect[1].figure.y1;
	rect_y2 = (HEIGHT/2) * gRect[1].figure.y2;

	// �� 2 ����
	if ((WIDTH / 2) <= x && x <= (WIDTH) && 0 <= y && y < (HEIGHT / 2))
	{
		g_cur_area = 2;
		if (rect_x1 <= x && x <= rect_x2 && rect_y1 <= y && y <= rect_y2) g_cur_rect = 2;
	}

	// �� 3�簢�� : x(400 * 0.01 ~ 400 * 0.99) y(300 * 0.01 ~ 300 * 0.99)
	rect_x1 = (WIDTH / 2) * (1 + gRect[2].figure.x1);
	rect_x2 = (WIDTH / 2) * (1 + gRect[2].figure.x2);
	rect_y1 = (HEIGHT / 2) * (1 + gRect[2].figure.y1) + (HEIGHT / 2);
	rect_y2 = (HEIGHT / 2) * (1 + gRect[2].figure.y2) + (HEIGHT / 2);

	// �� 3 ����
	if (0 <= x && x < (WIDTH / 2) && (HEIGHT / 2) <= y && y <= (HEIGHT))
	{
		g_cur_area = 3;
		if (rect_x1 <= x && x <= rect_x2 && rect_y1 <= y && y <= rect_y2) g_cur_rect = 3;
	}

	/*cout << rect_x1 << ", " << rect_x2 << endl;
	cout << rect_y1 << ", " << rect_y2 << endl;
	cout << x << ", " << y << endl;*/

	// �� 4�簢�� : x(400 * 0.01 ~ 400 * 0.99) y(300 * 0.01 ~ 300 * 0.99)
	rect_x1 = (WIDTH / 2) * gRect[3].figure.x1 + (WIDTH / 2);
	rect_x2 = (WIDTH / 2) * gRect[3].figure.x2 + (WIDTH / 2);
	rect_y1 = (HEIGHT / 2) * (1 + gRect[3].figure.y1) + (HEIGHT / 2);
	rect_y2 = (HEIGHT / 2) * (1 + gRect[3].figure.y2) + (HEIGHT / 2);

	// �� 4 ����
	if ((WIDTH / 2) <= x && x <= (WIDTH) && (HEIGHT / 2) <= y && y <= (HEIGHT))
	{
		g_cur_area = 4;
		if (rect_x1 <= x && x <= rect_x2 && rect_y1 <= y && y <= rect_y2) g_cur_rect = 4;
	}
}

GLvoid ChangeRectColor(int idx, float _red, float _green, float _blue)
{
	gRect[idx].color.red = _red/255.0;
	gRect[idx].color.green = _green/255.0;
	gRect[idx].color.blue = _blue/255.0;
}

void ChangeBGColor(float _red, float _green, float _blue)
{
	bgColor.red = _red / 255.0;
	bgColor.blue = _green / 255.0;
	bgColor.green = _blue / 255.0;
}

GLvoid RandomChange(int idx)
{
	srand(time(NULL));
	int r, g, b;
	r = rand() % 255 + 1;
	g = rand() % 255 + 1;
	b = rand() % 255 + 1;
	if (idx == -1) ChangeBGColor(r, g, b);
	else ChangeRectColor(idx, r, g, b);
}

GLvoid ChangColor()
{
	// ���� �������� �ٲٱ�
	srand(time(NULL));
	if (g_cur_rect == 0)
	{
		RandomChange(-1);
	}
	else if (g_cur_rect != 0)
	{
		// ���� �簢�� ���� �������� �ٲٱ�
		RandomChange(g_cur_rect-1);
	}
}

GLvoid ChangeSize()
{
	float SIZE = 0.1f;
	if (g_cur_rect != 0)
	{
		// -0.99f, 0.01f, -0.01f, 0.99f
		// 0.01f, 0.01f, 0.99f, 0.99f + + - -
		// -0.99f, -0.99f, -0.01f, -0.01f + + - -
		// 0.01f, -0.99f, 0.99f, -0.01f 
		if ((g_cur_rect == 1 || g_cur_rect == 3) && gRect[g_cur_rect - 1].figure.x1 + SIZE > -0.5f) return;
		if ((g_cur_rect == 2 || g_cur_rect == 4) && gRect[g_cur_rect - 1].figure.x1 + SIZE > 0.5f) return;

		gRect[g_cur_rect - 1].figure.x1 += SIZE;
		gRect[g_cur_rect - 1].figure.y1 += SIZE;
		gRect[g_cur_rect - 1].figure.x2 -= SIZE;
		gRect[g_cur_rect - 1].figure.y2 -= SIZE;
	}
	else
	{
		if ((g_cur_area == 1 || g_cur_area == 3)&& gRect[g_cur_area - 1].figure.x1 - SIZE < -0.99f) return;
		if ((g_cur_area == 2 || g_cur_area == 4) && gRect[g_cur_area - 1].figure.x1 - SIZE < 0.01f) return;

		gRect[g_cur_area - 1].figure.x1 -= SIZE;
		gRect[g_cur_area - 1].figure.y1 -= SIZE;
		gRect[g_cur_area - 1].figure.x2 += SIZE;
		gRect[g_cur_area - 1].figure.y2 += SIZE;
	}
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		CheckArea(x, y);
		ChangColor();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		CheckArea(x, y);
		ChangeSize();
	}

	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	default:
		break;
	}
	glutPostRedisplay();
}