#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <random>

#define WIDTH 600
#define HEIGHT 600

using namespace std;

GLvoid InitRect();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);


//사각형 이동하기
//화면 중앙에 사각형을 그린 후,
//마우스 버튼을 사각형 위에 클릭한 채로 드래그 하면
//사각형의 위치가 이동된다.
//마우스를 놓으면 더 이상 사각형이 이동하지 않는다.
//키보드를 눌러 사각형을 추가로 만든다.
//키보드 a : 화면의 랜덤한 위치에 다른 색상의 사각형을 만든다.최대 5개 만든다.새롭게 만든 사각형도 이동할 수 있다.
//사각형이 겹쳐져 있으면 나중에 만든 사각형이 위에 올라오고 그 사각형이 선택된다

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

	GLfloat RECT_SIZE = 0.2f;
};


// 글로벌 변수
Color bgColor;
vector<Rect> gRect;
vector<Rect> gParticleRect;
int g_rect_num = 0;
int g_cur_rect = 0;
bool g_left_button = false;

bool isAnim = false;

int condition = 0;


void main(int argc, char** argv)
{
	// 사각형 초기화
	InitRect();

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
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid InitRect()
{
	srand(static_cast<unsigned int>(std::time(nullptr)));

	int randomInt;

	randomInt = rand();
	GLfloat randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	GLfloat randX = -1.0f + randomFloat * 2.0f;

	randomInt = rand();
	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	GLfloat randY = -1.0f + randomFloat * 2.0f;

	Rect temp;
	float rect_size = 0.2f;
	temp.color = { 255.0f, 255.0f, 0.0f ,1.0f };
	temp.pivot = { 0.0f, 0.0f };
	temp.order = 0;
	temp.generate = 0;
	temp.RECT_SIZE = 0.2f;
	temp.isActive = true;

	gRect.emplace_back(temp);

	temp.color = { 255.0f, 0.0f, 0.0f ,1.0f };
	temp.pivot = { randX, randY };
	temp.order = 1;
	temp.generate = 1;
	temp.RECT_SIZE = 0.2f;
	temp.isActive = true;

	randomInt = rand();
	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	randX = -1.0f + randomFloat * 2.0f;

	randomInt = rand();
	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	randY = -1.0f + randomFloat * 2.0f;

	gRect.emplace_back(temp);

	temp.color = { 0.0f, 0.0f, 255.0f ,1.0f };
	temp.pivot = { randX, randY };
	temp.order = 2;
	temp.generate = 2;
	temp.RECT_SIZE = 0.2f;
	temp.isActive = true;

	gRect.emplace_back(temp);

	randomInt = rand();
	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	randX = -1.0f + randomFloat * 2.0f;

	randomInt = rand();
	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	randY = -1.0f + randomFloat * 2.0f;

	temp.color = { 0.0f, 255.0f, 0.0f ,1.0f };
	temp.pivot = { randX, randY };
	temp.order = 3;
	temp.generate = 3;
	temp.RECT_SIZE = 0.2f;
	temp.isActive = true;

	gRect.emplace_back(temp);

	randomInt = rand();
	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	randX = -1.0f + randomFloat * 2.0f;

	randomInt = rand();
	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
	randY = -1.0f + randomFloat * 2.0f;

	temp.color = { 0.0f, 255.0f, 0.0f ,1.0f };
	temp.pivot = { randX, randY };
	temp.order = 4;
	temp.generate = 4;
	temp.RECT_SIZE = 0.2f;
	temp.isActive = true;

	gRect.emplace_back(temp);
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

GLvoid drawScene()
{
	// 변경된 배경색 설정
	glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha);
	glClear(GL_COLOR_BUFFER_BIT);

	// 파티클 사각형 그리기
	for (int i = 0; i < gParticleRect.size(); i++)
	{
		if (gParticleRect[i].isActive)
		{
			glColor3f(gParticleRect[i].color.red, gParticleRect[i].color.green, gParticleRect[i].color.blue);
			glRectf(gParticleRect[i].pivot.x - gParticleRect[i].RECT_SIZE,
					gParticleRect[i].pivot.y - gParticleRect[i].RECT_SIZE,
					gParticleRect[i].pivot.x + gParticleRect[i].RECT_SIZE,
					gParticleRect[i].pivot.y + gParticleRect[i].RECT_SIZE);
		}
	}

	// 사각형 그리기
	for (int i = gRect.size() - 1; i >= 0; i--)
	{
		if (gRect[i].isActive)
		{
			glColor3f(gRect[i].color.red, gRect[i].color.green, gRect[i].color.blue);
			glRectf(gRect[i].pivot.x - gRect[i].RECT_SIZE,
				gRect[i].pivot.y - gRect[i].RECT_SIZE,
				gRect[i].pivot.x + gRect[i].RECT_SIZE,
				gRect[i].pivot.y + gRect[i].RECT_SIZE);
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

bool compare(Rect a, Rect b)
{
	return a.order <= b.order;
}

// order 순서대로 gRect 정렬
GLvoid SortRectByOrder()
{
	sort(gRect.begin(), gRect.end(), compare);
}

// 현재 클릭된 사각형의 order를 맨 앞에 두고 나머지 사각형 order 증가
GLvoid UpdateOrder()
{
	for (int i = 0; i < gRect.size(); i++)
	{
		if (i == g_cur_rect) gRect[i].order = 0;
		else gRect[i].order += 1;
	}
}

// 클릭한 마우스 포인터가 어느 사각형인지 체크
GLvoid CheckRect(int x, int y)
{
	float min_x, min_y, max_x, max_y;
	int min_generate = 99;

	g_cur_rect = -1;
	for (int i = 0; i < gRect.size(); i++)
	{
		if (gRect[i].isActive)
		{
			min_x = ((gRect[i].pivot.x - gRect[i].RECT_SIZE + 1.0f) / 2.0f) * WIDTH;
			max_x = ((gRect[i].pivot.x + gRect[i].RECT_SIZE + 1.0f) / 2.0f) * WIDTH;

			min_y = ((gRect[i].pivot.y - gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
			max_y = ((gRect[i].pivot.y + gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;

			if (gRect[i].pivot.y - gRect[i].RECT_SIZE >= 0 && gRect[i].pivot.y + gRect[i].RECT_SIZE > 0)
			{
				min_y = HEIGHT - (((gRect[i].pivot.y + gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT);
				max_y = ((gRect[i].pivot.y - gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
			}
			else if (gRect[i].pivot.y - gRect[i].RECT_SIZE < 0.0f && gRect[i].pivot.y + gRect[i].RECT_SIZE > 0.0f)
			{
				min_y = ((gRect[i].pivot.y - gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
				max_y = ((gRect[i].pivot.y + gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
			}
			else
			{
				min_y = ((gRect[i].pivot.y + gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
				max_y = HEIGHT - (((gRect[i].pivot.y - gRect[i].RECT_SIZE + 1.0f) / 2.0f) * HEIGHT);
			}

			if (min_x <= x && x <= max_x && min_y <= y && y <= max_y)
			{
				if (min_generate > gRect[i].generate)
				{
					min_generate = gRect[i].generate;
				}
			}
		}
	}

	// 겹칠경우 가장 먼저 생성된 사각형 클릭
	for (int i = 0; i < gRect.size(); i++)
	{
		if (gRect[i].generate == min_generate)
		{
			g_cur_rect = i;
			return;
		}
	}
}

// 사각형 사등분으로 분할 애니메이션
GLvoid GenerateParticleRect1(int idx)
{
	if (gRect[idx].isActive == false) return;

	gParticleRect.clear();

	GLfloat decreaseSIZE = 0.11f;

	Rect temp;

	// 1번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x - gRect[idx].RECT_SIZE / 2.0f;
	temp.pivot.y = gRect[idx].pivot.y - gRect[idx].RECT_SIZE / 2.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x + gRect[idx].RECT_SIZE / 2.0f;
	temp.pivot.y = gRect[idx].pivot.y - gRect[idx].RECT_SIZE / 2.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	temp.color = gRect[0].color;
	temp.pivot.x = gRect[idx].pivot.x - gRect[idx].RECT_SIZE / 2.0f;
	temp.pivot.y = gRect[idx].pivot.y + gRect[idx].RECT_SIZE / 2.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x + gRect[idx].RECT_SIZE / 2.0f;
	temp.pivot.y = gRect[idx].pivot.y + gRect[idx].RECT_SIZE / 2.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	//cout << temp.pivot.x << ", " << temp.pivot.y << ", " << temp.RECT_SIZE << endl;

	gParticleRect.emplace_back(temp);

	// 기존 사각형 삭제
	gRect[idx].isActive = false;

	isAnim = true;

	//if(is_anim)
}

// 8 방향
GLvoid GenerateParticleRect2(int idx)
{
	if (gRect[idx].isActive == false) return;

	gParticleRect.clear();

	GLfloat decreaseSIZE = 0.15f;

	Rect temp;
	// 1번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x - (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.pivot.y = gRect[idx].pivot.y - (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;


	gParticleRect.emplace_back(temp);

	// 2번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x;
	temp.pivot.y = gRect[idx].pivot.y - (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	// 3번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x + (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.pivot.y = gRect[idx].pivot.y - (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	// 4번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x - (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.pivot.y = gRect[idx].pivot.y;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	// 5번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x;
	temp.pivot.y = gRect[idx].pivot.y;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	// 6번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x + (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.pivot.y = gRect[idx].pivot.y;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	// 7번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x - (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.pivot.y = gRect[idx].pivot.y + (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;


	gParticleRect.emplace_back(temp);

	// 8번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x;
	temp.pivot.y = gRect[idx].pivot.y + (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	// 9번 사각형 생성
	temp.color = gRect[idx].color;
	temp.pivot.x = gRect[idx].pivot.x + (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.pivot.y = gRect[idx].pivot.y + (gRect[idx].RECT_SIZE * 2.0f) / 3.0f;
	temp.RECT_SIZE = gRect[idx].RECT_SIZE - decreaseSIZE;
	temp.isActive = true;

	gParticleRect.emplace_back(temp);

	
	// 기존 사각형 삭제
	gRect[idx].isActive = false;

	isAnim = true;

	//if(is_anim)
}

// 4 방향 대각선
GLvoid FourDir1()
{
	GLfloat move = 0.005f;
	GLfloat dirX = 1.0f;
	GLfloat dirY = 1.0f;
	for (int i = 0; i < gParticleRect.size(); i++)
	{
		// 크기 감소
		gParticleRect[i].RECT_SIZE = gParticleRect[i].RECT_SIZE - 0.001f;
		if (gParticleRect[i].RECT_SIZE <= 0.0f)
		{
			isAnim = false;
			return;
		}

		if (i == 0) { dirX = -1.0f, dirY = -1.0f; }
		else if (i == 1) { dirX = 1.0f, dirY = -1.0f; }
		else if (i == 2) { dirX = -1.0f, dirY = 1.0f; }
		else if (i == 3) { dirX = 1.0f, dirY = 1.0f; }

		gParticleRect[i].pivot.x += move * (dirX);
		gParticleRect[i].pivot.y += move * (dirY);

		if (gParticleRect[i].RECT_SIZE <= 0.0f)
		{
			gParticleRect[i].isActive = false;
			isAnim = false;
		}
	}
}

GLvoid FourDir2()
{
	GLfloat moveX = 0.005f;
	GLfloat moveY = 0.005f;
	GLfloat dirX = 1.0f;
	GLfloat dirY = 1.0f;
	for (int i = 0; i < gParticleRect.size(); i++)
	{
		// 크기 감소
		gParticleRect[i].RECT_SIZE = gParticleRect[i].RECT_SIZE - 0.001f;
		if (gParticleRect[i].RECT_SIZE <= 0.0f)
		{
			isAnim = false;
			return;
		}

		if (i == 0) { dirX = -1.0f, dirY = 1.0f; }
		else if (i == 1) { dirX = -1.0f, dirY = -1.0f; }
		else if (i == 2) { dirX = 1.0f, dirY = 1.0f; }
		else if (i == 3) { dirX = 1.0f, dirY = -1.0f; }

		gParticleRect[i].pivot.x += moveX * (dirX);
		gParticleRect[i].pivot.y += moveY * (dirY);

		if (gParticleRect[i].RECT_SIZE <= 0.0f)
		{
			gParticleRect[i].isActive = false;
			isAnim = false;
		}
	}
}

GLvoid EightDir1()
{
	GLfloat move = 0.005f;
	GLfloat dirX = 1.0f;
	GLfloat dirY = 1.0f;
	for (int i = 0; i < gParticleRect.size(); i++)
	{
		// 크기 감소
		gParticleRect[i].RECT_SIZE = gParticleRect[i].RECT_SIZE - 0.001f;
		if (gParticleRect[i].RECT_SIZE <= 0.0f)
		{
			isAnim = false;
			return;
		}

		if (i == 0) 
		{ dirX = -1.0f, dirY = -1.0f; 
		gParticleRect[i].pivot.x += move * (dirX);
		gParticleRect[i].pivot.y += move * (dirY);
		}
		else if (i == 1) 
		{ 
			dirX = 1.0f, dirY = -1.0f;
			gParticleRect[i].pivot.y += move * (dirY);
		}
		else if (i == 2) 
		{ 
			dirX = 1.0f, dirY = -1.0f;
			gParticleRect[i].pivot.x += move * (dirX);
			gParticleRect[i].pivot.y += move * (dirY);
		}
		else if (i == 3) 
		{ 
			dirX = -1.0f, dirY = 1.0f;
			gParticleRect[i].pivot.x += move * (dirX);
		}
		else if (i == 4) 
		{ 
			
		}
		else if (i == 5) 
		{ 
			dirX = 1.0f, dirY = -1.0f;
			gParticleRect[i].pivot.x += move * (dirX);
		}
		else if (i == 6) 
		{
			dirX = -1.0f, dirY = 1.0f;
			gParticleRect[i].pivot.x += move * (dirX);
			gParticleRect[i].pivot.y += move * (dirY);
		}
		else if (i == 7) 
		{ 
			dirX = 1.0f, dirY = 1.0f;
			gParticleRect[i].pivot.y += move * (dirY);
		}
		else if (i == 8) 
		{ 
			dirX = 1.0f, dirY = 1.0f;
			gParticleRect[i].pivot.x += move * (dirX);
			gParticleRect[i].pivot.y += move * (dirY);
		}

		if (gParticleRect[i].RECT_SIZE <= 0.0f)
		{
			gParticleRect[i].isActive = false;
			isAnim = false;
		}
	}
}

GLvoid ParticleAnim(int is_anim)
{
	if (isAnim == false || gParticleRect.empty()) return;

	// 0 -> x - , y -
	// 1 -> x + , y -
	// 2 -> x - , y +
	// 3 -> x + , y +


	if (condition == 0) FourDir1();
	else if (condition == 1) FourDir2();
	else if (condition == 2) EightDir1();

	glutPostRedisplay();

	if(isAnim) glutTimerFunc(30, ParticleAnim, isAnim);
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		CheckRect(x, y);
		UpdateOrder();
		SortRectByOrder();

		condition = GetRandomIntValue(0, 2.9);
		if (condition == 0) GenerateParticleRect1(0);
		else if (condition == 1) GenerateParticleRect1(0);
		else if (condition == 2) GenerateParticleRect2(0);
		glutTimerFunc(30, ParticleAnim, isAnim);
		g_left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_left_button = false;
	}

	glutPostRedisplay();
}

GLvoid RandomChange(int idx)
{
	if (g_rect_num >= 5) return;

	float r, g, b;
	r = rand() % 255 + 1;
	g = rand() % 255 + 1;
	b = rand() % 255 + 1;
	gRect[idx].color.red = r / 255.0f;
	gRect[idx].color.green = g / 255.0f;
	gRect[idx].color.blue = b / 255.0f;
}

GLvoid Reset()
{
	gRect.clear();
	gParticleRect.clear();

	isAnim = false;
	condition = -1;
	InitRect();
	for (int i = 0; i < gRect.size(); i++)
	{
		gRect[i].isActive = true;
		RandomChange(i);
	}
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
