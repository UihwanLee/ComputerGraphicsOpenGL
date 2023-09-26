//#include <iostream>
//#include <gl/glew.h> //--- 필요한 헤더파일 include
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <vector>
//#include <algorithm>
//#include <time.h>
//
//#define WIDTH 600
//#define HEIGHT 600
//
//using namespace std;
//
//GLvoid InitRect();
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLvoid MouseClick(int button, int state, int x, int y);
//GLvoid MouseDrag(int x, int y);
//GLvoid Keyboard(unsigned char key, int x, int y);
//
//
////사각형 이동하기
////화면 중앙에 사각형을 그린 후,
////마우스 버튼을 사각형 위에 클릭한 채로 드래그 하면
////사각형의 위치가 이동된다.
////마우스를 놓으면 더 이상 사각형이 이동하지 않는다.
////키보드를 눌러 사각형을 추가로 만든다.
////키보드 a : 화면의 랜덤한 위치에 다른 색상의 사각형을 만든다.최대 5개 만든다.새롭게 만든 사각형도 이동할 수 있다.
////사각형이 겹쳐져 있으면 나중에 만든 사각형이 위에 올라오고 그 사각형이 선택된다
//
//struct Color
//{
//	float red = 1.0f;
//	float green = 1.0f;
//	float blue = 1.0f;
//	float alpha = 1.0f;
//};
//
//struct Pivot
//{
//	GLfloat x;
//	GLfloat y;
//};
//
//struct Rect
//{
//	Color color;
//	Pivot pivot;
//	bool isActive = false;
//	int generate;
//	int order;
//	int condition_y;
//};
//
//
//// 글로벌 변수
//Color bgColor;
//vector<Rect> gRect;
//int g_rect_num = 0;
//int g_cur_rect = 0;
//bool g_left_button = false;
//
//float g_dist_x1;
//float g_dist_x2;
//float g_dist_y1;
//float g_dist_y2;
//
//GLfloat RECT_SIZE = 0.2f;
//
//
//void main(int argc, char** argv)
//{
//	// 사각형 초기화
//	InitRect();
//
//	// 윈도우 생성
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(0, 0);
//	glutInitWindowSize(WIDTH, HEIGHT);
//	glutCreateWindow("Example2");
//
//	// GLEW 초기화
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		std::cerr << "Unable to initialize GLEW" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	else
//		std::cout << "GLEW Initialized\n";
//
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(Reshape);
//	glutMouseFunc(MouseClick);
//	glutMotionFunc(MouseDrag);
//	glutKeyboardFunc(Keyboard);
//	glutMainLoop();
//}
//
//GLvoid InitRect()
//{
//	srand(static_cast<unsigned int>(std::time(nullptr)));
//
//	int randomInt;
//
//	randomInt = rand();
//	GLfloat randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	GLfloat randX = -1.0f + randomFloat * 2.0f;
//
//	randomInt = rand();
//	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	GLfloat randY = -1.0f + randomFloat * 2.0f;
//
//	Rect temp;
//	float rect_size = 0.2f;
//	temp.color = { 255.0f, 255.0f, 0.0f ,1.0f };
//	temp.pivot = { 0.0f, 0.0f };
//	temp.order = 0;
//	temp.generate = 0;
//	temp.isActive = true;
//
//	gRect.emplace_back(temp);
//
//	temp.color = { 255.0f, 0.0f, 0.0f ,1.0f };
//	temp.pivot = { randX, randY };
//	temp.order = 1;
//	temp.generate = 1;
//	temp.isActive = false;
//
//	randomInt = rand();
//	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	randX = -1.0f + randomFloat * 2.0f;
//
//	randomInt = rand();
//	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	randY = -1.0f + randomFloat * 2.0f;
//
//	gRect.emplace_back(temp);
//
//	temp.color = { 0.0f, 0.0f, 255.0f ,1.0f };
//	temp.pivot = { randX, randY };
//	temp.order = 2;
//	temp.generate = 2;
//	temp.isActive = false;
//
//	gRect.emplace_back(temp);
//
//	randomInt = rand();
//	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	randX = -1.0f + randomFloat * 2.0f;
//
//	randomInt = rand();
//	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	randY = -1.0f + randomFloat * 2.0f;
//
//	temp.color = { 0.0f, 255.0f, 0.0f ,1.0f };
//	temp.pivot = { randX, randY };
//	temp.order = 3;
//	temp.generate = 3;
//	temp.isActive = false;
//
//	gRect.emplace_back(temp);
//
//	randomInt = rand();
//	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	randX = -1.0f + randomFloat * 2.0f;
//
//	randomInt = rand();
//	randomFloat = static_cast<float>(randomInt) / RAND_MAX;
//	randY = -1.0f + randomFloat * 2.0f;
//
//	temp.color = { 0.0f, 255.0f, 0.0f ,1.0f };
//	temp.pivot = { randX, randY };
//	temp.order = 4;
//	temp.generate = 4;
//	temp.isActive = false;
//
//	gRect.emplace_back(temp);
//}
//
//GLvoid drawScene()
//{
//	// 변경된 배경색 설정
//	glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// 사각형 그리기
//	for (int i = gRect.size() - 1; i >= 0; i--)
//	{
//		if (gRect[i].isActive)
//		{
//			glColor3f(gRect[i].color.red, gRect[i].color.green, gRect[i].color.blue);
//			glRectf(gRect[i].pivot.x - RECT_SIZE, gRect[i].pivot.y - RECT_SIZE, gRect[i].pivot.x + RECT_SIZE, gRect[i].pivot.y + RECT_SIZE);
//		}
//	}
//
//	glutSwapBuffers();
//}
//
//GLvoid Reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//}
//
//bool compare(Rect a, Rect b)
//{
//	return a.order <= b.order;
//}
//
//// order 순서대로 gRect 정렬
//GLvoid SortRectByOrder()
//{
//	sort(gRect.begin(), gRect.end(), compare);
//}
//
//// 현재 클릭된 사각형의 order를 맨 앞에 두고 나머지 사각형 order 증가
//GLvoid UpdateOrder()
//{
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		if (i == g_cur_rect) gRect[i].order = 0;
//		else gRect[i].order += 1;
//	}
//}
//
//// 클릭한 마우스 포인터가 어느 사각형인지 체크
//GLvoid CheckRect(int x, int y)
//{
//	float min_x, min_y, max_x, max_y;
//	int min_generate = 99;
//
//	g_cur_rect = -1;
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		if (gRect[i].isActive)
//		{
//			min_x = ((gRect[i].pivot.x - RECT_SIZE + 1.0f) / 2.0f) * WIDTH;
//			max_x = ((gRect[i].pivot.x + RECT_SIZE + 1.0f) / 2.0f) * WIDTH;
//
//			min_y = ((gRect[i].pivot.y - RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
//			max_y = ((gRect[i].pivot.y + RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
//
//			if (gRect[i].pivot.y - RECT_SIZE >= 0 && gRect[i].pivot.y + RECT_SIZE > 0)
//			{
//				min_y = HEIGHT - (((gRect[i].pivot.y + RECT_SIZE + 1.0f) / 2.0f) * HEIGHT);
//				max_y = ((gRect[i].pivot.y - RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
//			}
//			else if (gRect[i].pivot.y - RECT_SIZE < 0.0f && gRect[i].pivot.y + RECT_SIZE > 0.0f)
//			{
//				min_y = ((gRect[i].pivot.y - RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
//				max_y = ((gRect[i].pivot.y + RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
//			}
//			else
//			{
//				min_y = ((gRect[i].pivot.y + RECT_SIZE + 1.0f) / 2.0f) * HEIGHT;
//				max_y = HEIGHT - (((gRect[i].pivot.y - RECT_SIZE + 1.0f) / 2.0f) * HEIGHT);
//			}
//
//			if (min_x <= x && x <= max_x && min_y <= y && y <= max_y)
//			{
//				if (min_generate > gRect[i].generate)
//				{
//					min_generate = gRect[i].generate;
//				}
//			}
//		}
//	}
//
//	// 겹칠경우 가장 먼저 생성된 사각형 클릭
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		if (gRect[i].generate == min_generate)
//		{
//			g_cur_rect = i;
//			return;
//		}
//	}
//}
//
//GLvoid MouseClick(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		CheckRect(x, y);
//		UpdateOrder();
//		SortRectByOrder();
//		g_left_button = true;
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
//GLvoid MouseDrag(int x, int y)
//{
//	// && g_cur_rect != -1
//	if (g_left_button && g_cur_rect != -1)
//	{
//		// 마우스 드래그에 따른 도형 이동
//		gRect[0].pivot.x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
//		gRect[0].pivot.y = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
//	}
//
//	glutPostRedisplay();
//}
//
//GLvoid RandomChange(int idx)
//{
//	if (g_rect_num >= 5) return;
//
//	float r, g, b;
//	r = rand() % 255 + 1;
//	g = rand() % 255 + 1;
//	b = rand() % 255 + 1;
//	gRect[idx].color.red = r / 255.0f;
//	gRect[idx].color.green = g / 255.0f;
//	gRect[idx].color.blue = b / 255.0f;
//}
//
//GLvoid AddRect()
//{
//	g_rect_num++;
//	if (g_rect_num >= 5) return;
//
//	gRect[g_rect_num].isActive = true;
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'a':
//		AddRect();
//		RandomChange(g_rect_num);
//		break;
//	case 'q':
//		exit(EXIT_FAILURE);
//		break;
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}
