//#include <iostream>
//#include <gl/glew.h> //--- �ʿ��� ������� include
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
////�簢�� �̵��ϱ�
////ȭ�� �߾ӿ� �簢���� �׸� ��,
////���콺 ��ư�� �簢�� ���� Ŭ���� ä�� �巡�� �ϸ�
////�簢���� ��ġ�� �̵��ȴ�.
////���콺�� ������ �� �̻� �簢���� �̵����� �ʴ´�.
////Ű���带 ���� �簢���� �߰��� �����.
////Ű���� a : ȭ���� ������ ��ġ�� �ٸ� ������ �簢���� �����.�ִ� 5�� �����.���Ӱ� ���� �簢���� �̵��� �� �ִ�.
////�簢���� ������ ������ ���߿� ���� �簢���� ���� �ö���� �� �簢���� ���õȴ�
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
//// �۷ι� ����
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
//	// �簢�� �ʱ�ȭ
//	InitRect();
//
//	// ������ ����
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(0, 0);
//	glutInitWindowSize(WIDTH, HEIGHT);
//	glutCreateWindow("Example2");
//
//	// GLEW �ʱ�ȭ
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
//	// ����� ���� ����
//	glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// �簢�� �׸���
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
//// order ������� gRect ����
//GLvoid SortRectByOrder()
//{
//	sort(gRect.begin(), gRect.end(), compare);
//}
//
//// ���� Ŭ���� �簢���� order�� �� �տ� �ΰ� ������ �簢�� order ����
//GLvoid UpdateOrder()
//{
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		if (i == g_cur_rect) gRect[i].order = 0;
//		else gRect[i].order += 1;
//	}
//}
//
//// Ŭ���� ���콺 �����Ͱ� ��� �簢������ üũ
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
//	// ��ĥ��� ���� ���� ������ �簢�� Ŭ��
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
//		// ���콺 �巡�׿� ���� ���� �̵�
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
