//#include <iostream>
//#include <gl/glew.h> //--- �ʿ��� ������� include
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <vector>
//#include <algorithm>
//#include <time.h>
//#include <random>
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
//GLvoid Keyboard(unsigned char key, int x, int y);
//
////�簢���� �ִϸ��̼� �����ϱ�
//// ������ £�� ȸ������ ���ϰ�, �簢���� ������ ������ �׸���.
//// ���콺�� Ŭ���ϴ� ���� �߽��� �Ǿ� �簢���� �׸���.�ִ� 5���� �簢���� �׸���.
//// Ű���� �Է�(��ɾ�� �빮�� �Ǵ� �ҹ��� ������) :
////	 a : ��ġ ��ȭ1 : �簢������ ���� �밢������ �̵��ϰ� ���� ������ ƨ�� �ٸ� �������� �̵��Ѵ� / �����.
////	 i : ��ġ ��ȭ2 : �簢������ ������׷� �̵��Ѵ� / �����.
////	 c : ũ�� ��ȭ : �簢���� ũ�Ⱑ �پ��ϰ� ���Ѵ� / ������ �ʴ´�.
////	 o : ���� ��ȭ : �簢���� ������ �����ϰ� ���Ѵ� / ������ �ʴ´�.
////	 s : ��� �ִϸ��̼��� �����.
////	 m : ���� �׸� ��ġ�� �簢������ �̵��Ѵ�.
////	 r : �簢������ �����ϰ� �ٽ� ���콺 �Է��� ���� �� �ִ�.
////	 q : ���α׷��� �����Ѵ�.
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
//	Color origin_color;
//	Pivot pivot;
//	Pivot origin_pivot;
//	GLfloat dist_x;
//	GLfloat dist_y;
//	GLfloat moving1_x;
//	GLfloat moving1_y;
//	GLfloat moving2_y;
//	GLfloat moving2dist_y;
//	int generate;
//	int order;
//	bool isActive = false;
//};
//
//
//// �۷ι� ����
//Color bgColor;
//vector<Rect> gRect;
//int g_rect_num = -1;
//bool g_left_button = false;
//
//GLfloat RECT_SIZE = 0.2f;
//
//// �ִϸ��̼� üũ ����
//bool is_MovingAnim1 = false;
//bool is_MovingAnim2 = false;
//bool is_ChangingScaleAnim = false;
//bool is_ChangingColorAnim = false;
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
//	glutKeyboardFunc(Keyboard);
//	glutMainLoop();
//}
//
//GLvoid InitRect()
//{
//	srand(static_cast<unsigned int>(std::time(nullptr)));
//
//	GLfloat init_pos = 0.0f;
//
//
//	Rect temp;
//	temp.color = { 255.0f, 255.0f, 0.0f ,1.0f };
//	temp.pivot = { 0.0f, 0.0f };
//	temp.dist_x = RECT_SIZE;
//	temp.dist_y = RECT_SIZE;
//	temp.moving1_x = 0.01f;
//	temp.moving1_y = 0.01f;
//	temp.moving2_y = 0.02f;
//	temp.moving2dist_y = 0.0f;
//	temp.order = 0;
//	temp.generate = 0;
//	temp.isActive = false;
//
//	gRect.emplace_back(temp);
//
//	temp.color = { 255.0f, 0.0f, 0.0f ,1.0f };
//	temp.pivot = { init_pos, init_pos };
//	temp.dist_x = RECT_SIZE;
//	temp.dist_y = RECT_SIZE;
//	temp.moving1_x = 0.01f;
//	temp.moving1_y = 0.01f;
//	temp.moving2_y = 0.02f;
//	temp.moving2dist_y = 0.0f;
//	temp.order = 1;
//	temp.generate = 1;
//	temp.isActive = false;
//
//	gRect.emplace_back(temp);
//
//	temp.color = { 0.0f, 0.0f, 255.0f ,1.0f };
//	temp.pivot = { init_pos, init_pos };
//	temp.dist_x = RECT_SIZE;
//	temp.dist_y = RECT_SIZE;
//	temp.moving1_x = 0.01f;
//	temp.moving1_y = 0.01f;
//	temp.moving2_y = 0.02f;
//	temp.moving2dist_y = 0.0f;
//	temp.order = 2;
//	temp.generate = 2;
//	temp.isActive = false;
//
//	gRect.emplace_back(temp);
//
//
//	temp.color = { 0.0f, 255.0f, 0.0f ,1.0f };
//	temp.pivot = { init_pos, init_pos };
//	temp.dist_x = RECT_SIZE;
//	temp.dist_y = RECT_SIZE;
//	temp.moving1_x = 0.01f;
//	temp.moving1_y = 0.01f;
//	temp.moving2_y = 0.02f;
//	temp.moving2dist_y = 0.0f;
//	temp.order = 3;
//	temp.generate = 3;
//	temp.isActive = false;
//
//	gRect.emplace_back(temp);
//
//	temp.color = { 0.0f, 255.0f, 0.0f ,1.0f };
//	temp.pivot = { init_pos, init_pos };
//	temp.dist_x = RECT_SIZE;
//	temp.dist_y = RECT_SIZE;
//	temp.moving1_x = 0.01f;
//	temp.moving1_y = 0.01f;
//	temp.moving2_y = 0.02f;
//	temp.moving2dist_y = 0.0f;
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
//	glClearColor(74.0f/255.0f, 79.0f / 255.0f, 90.0f/255.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// �簢�� �׸���
//	for(int i = 0; i < gRect.size(); i++)
//	{
//		if (gRect[i].isActive)
//		{
//			glColor3f(gRect[i].color.red, gRect[i].color.green, gRect[i].color.blue);
//			glRectf(gRect[i].pivot.x - gRect[i].dist_x, gRect[i].pivot.y - gRect[i].dist_y, gRect[i].pivot.x + gRect[i].dist_x, gRect[i].pivot.y + gRect[i].dist_y);
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
//// �̵��ϴ� �簢�� ���� �浹 üũ �� ���� �ٲٱ�
//GLvoid CheckCollisionRect(int idx)
//{
//	if (gRect[idx].isActive == false) return;
//
//	// ���� ���� ���� ���
//	if (gRect[idx].pivot.y + gRect[idx].dist_y >= 1.0f)
//	{
//		gRect[idx].moving1_y = gRect[idx].moving1_y * (-1.0f);
//	}
//
//	// �Ʒ��� ���� ���� ���
//	if (gRect[idx].pivot.y - gRect[idx].dist_y <= -1.0f)
//	{
//		gRect[idx].moving1_y = gRect[idx].moving1_y * (-1.0f);
//	}
//
//	// ������ ���� ���� ���
//	if (gRect[idx].pivot.x + gRect[idx].dist_x >= 1.0f)
//	{
//		gRect[idx].moving1_x = gRect[idx].moving1_x * (-1.0f);
//	}
//
//	// ���� ���� ���� ���
//	if (gRect[idx].pivot.x - gRect[idx].dist_x <= -1.0f)
//	{
//		gRect[idx].moving1_x = gRect[idx].moving1_x * (-1.0f);
//	}
//}
//
//// �簢�� �밢������ �̵�
//GLvoid MovingRectDiagonal(int idx)
//{
//	if (gRect[idx].isActive == false) return;
//
//	gRect[idx].pivot.x += gRect[idx].moving1_x;
//	gRect[idx].pivot.y += gRect[idx].moving1_y;
//}
//
//// �簢�� ������׷� �̵�
//GLvoid MovingRectZigzag(int idx)
//{
//	if (gRect[idx].isActive == false) return;
//
//	if (gRect[idx].moving2dist_y >= 0.1f || gRect[idx].pivot.y + gRect[idx].dist_y >= 1.0f)
//	{
//		gRect[idx].moving2_y = gRect[idx].moving2_y * (-1.0f);
//	}
//
//	if (gRect[idx].moving2dist_y <= -0.1f || gRect[idx].pivot.y - gRect[idx].dist_y <= -1.0f)
//	{
//		gRect[idx].moving2_y = gRect[idx].moving2_y * (-1.0f);
//	}
//
//	gRect[idx].moving2dist_y += gRect[idx].moving2_y;
//	gRect[idx].pivot.y += gRect[idx].moving2_y;
//}
//
//// �簢�� ũ�� �������� ����
//GLvoid RandomChangeScale(int idx)
//{
//	if (gRect[idx].isActive == false) return;
//
//	GLfloat temp_x, temp_y;
//
//	random_device rd;
//	mt19937 gen(rd());
//
//	// ���� ���� (0.1, 0.3) ������ ����
//	uniform_real_distribution<float> dis(0.1f, 0.3f);
//
//	// ���� �� ����
//	temp_x = dis(gen);
//	temp_y = dis(gen);
//
//	gRect[idx].dist_x = temp_x;
//	gRect[idx].dist_y = temp_y;
//}
//
//// �簢�� ���� �������� ����
//GLvoid RandomChangeColor(int idx)
//{
//	if (gRect[idx].isActive == false) return;
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
//// Ŭ���� ������ �簢�� ����
//GLvoid GenerateRect(int _x, int _y)
//{
//	g_rect_num++;
//	if (g_rect_num >= 5) return;
//
//	gRect[g_rect_num].pivot.x = (2.0f * _x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
//	gRect[g_rect_num].pivot.y = 1.0f - (2.0f * _y) / glutGet(GLUT_WINDOW_HEIGHT);
//	gRect[g_rect_num].origin_pivot.x = (2.0f * _x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
//	gRect[g_rect_num].origin_pivot.y = 1.0f - (2.0f * _y) / glutGet(GLUT_WINDOW_HEIGHT);
//	gRect[g_rect_num].isActive = true;
//	RandomChangeColor(g_rect_num);
//
//	gRect[g_rect_num].origin_color.red = gRect[g_rect_num].color.red;
//	gRect[g_rect_num].origin_color.green = gRect[g_rect_num].color.green;
//	gRect[g_rect_num].origin_color.blue = gRect[g_rect_num].color.blue;
//	gRect[g_rect_num].origin_color.alpha = gRect[g_rect_num].color.alpha;
//}
//
//GLvoid MouseClick(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		GenerateRect(x, y);
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
//// ��ġ ��ȭ1
//GLvoid MovingAnim1(int is_anim)
//{
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		if (gRect[i].isActive)
//		{
//			CheckCollisionRect(i);
//			MovingRectDiagonal(i);
//		}
//	}
//
//	glutPostRedisplay();
//
//	if (is_anim) glutTimerFunc(30, MovingAnim1, is_MovingAnim1);
//}
//
//// ��ġ ��ȭ2
//GLvoid MovingAnim2(int is_anim)
//{
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		if (gRect[i].isActive)
//		{
//			MovingRectZigzag(i);
//		}
//	}
//
//	glutPostRedisplay();
//
//	if (is_anim) glutTimerFunc(30, MovingAnim2, is_MovingAnim2);
//}
//
//// ũ�� ��ȭ
//GLvoid ChangeAllScale(int is_anim)
//{
//	if (is_anim)
//	{
//		for (int i = 0; i < gRect.size(); i++)
//		{
//			if (gRect[i].isActive)
//			{
//				RandomChangeScale(i);
//			}
//		}
//	}
//
//	glutPostRedisplay();
//
//	if (is_anim) glutTimerFunc(120, ChangeAllScale, is_ChangingScaleAnim);
//}
//
//// ���� ��ȭ
//GLvoid ChangeAllColor(int is_anim)
//{
//	if (is_anim)
//	{
//		for (int i = 0; i < gRect.size(); i++)
//		{
//			if (gRect[i].isActive)
//			{
//				RandomChangeColor(i);
//			}
//		}
//	}
//
//	glutPostRedisplay();
//
//	if (is_anim) glutTimerFunc(120, ChangeAllColor, is_ChangingColorAnim);
//}
//
//// ��� �ִϸ��̼� ���߱�
//GLvoid StopAllAnimation()
//{
//	is_MovingAnim1 = false;
//	is_MovingAnim2 = false;
//	is_ChangingScaleAnim = false;
//	is_ChangingColorAnim = false;
//}
//
//// ���� �׸� ��ġ�� �簢������ �̵��Ѵ�
//GLvoid ResetOriginPos(int value)
//{
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		if (gRect[i].isActive)
//		{
//			gRect[i].dist_x = RECT_SIZE;
//			gRect[i].dist_y = RECT_SIZE;
//			gRect[i].moving1_x = 0.01f;
//			gRect[i].moving1_y = 0.01f;
//			gRect[i].moving2_y = 0.02f;
//			gRect[i].moving2dist_y = 0.0f;
//			gRect[i].pivot.x = gRect[i].origin_pivot.x;
//			gRect[i].pivot.y = gRect[i].origin_pivot.y;
//			gRect[i].color.red = gRect[i].origin_color.red;
//			gRect[i].color.green = gRect[i].origin_color.green;
//			gRect[i].color.blue = gRect[i].origin_color.blue;
//			gRect[i].color.alpha = gRect[i].origin_color.alpha;
//		}
//	}
//
//	glutPostRedisplay();
//}
//
//// �簢������ �����ϰ� �ٽ� ���콺 �Է��� ���� �� �ִ�.
//GLvoid Reset()
//{
//	StopAllAnimation();
//
//	for (int i = 0; i < gRect.size(); i++)
//	{
//		gRect[i].dist_x = RECT_SIZE;
//		gRect[i].dist_y = RECT_SIZE;
//		gRect[i].moving1_x = 0.01f;
//		gRect[i].moving1_y = 0.01f;
//		gRect[i].moving2_y = 0.02f;
//		gRect[i].moving2dist_y = 0.0f;
//		gRect[i].isActive = false;
//	}
//
//	g_rect_num = -1;
//}
//
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'A':
//	case 'a':
//		// ��ġ ��ȭ1 : �簢������ ���� �밢������ �̵��ϰ� ���� ������ ƨ�� �ٸ� �������� �̵��Ѵ�/�����.
//		if (is_MovingAnim1) is_MovingAnim1 = false;
//		else is_MovingAnim1 = true;
//		if (is_MovingAnim1) glutTimerFunc(30, MovingAnim1, is_MovingAnim1);
//		break;
//	case 'I':
//	case 'i':
//		// ��ġ ��ȭ2 : �簢������ ������׷� �̵��Ѵ�/�����.
//		if (is_MovingAnim2) is_MovingAnim2 = false;
//		else is_MovingAnim2 = true;
//		if (is_MovingAnim2) glutTimerFunc(30, MovingAnim2, is_MovingAnim2);
//		break;
//	case 'C':
//	case 'c':
//		// ũ�� ��ȭ : �簢���� ũ�Ⱑ �پ��ϰ� ���Ѵ�/������ �ʴ´�.
//		is_ChangingScaleAnim = !is_ChangingScaleAnim;
//		if(is_ChangingScaleAnim) glutTimerFunc(120, ChangeAllScale, is_ChangingScaleAnim);
//		break;
//	case 'O':
//	case 'o':
//		// ���� ��ȭ : �簢���� ������ �����ϰ� ���Ѵ�/������ �ʴ´�
//		is_ChangingColorAnim = !is_ChangingColorAnim;
//		if(is_ChangingColorAnim) glutTimerFunc(120, ChangeAllColor, is_ChangingColorAnim);
//		break;
//	case 'S':
//	case 's':
//		// ��� �ִϸ��̼��� �����.
//		StopAllAnimation();
//		break;
//	case 'M':
//	case 'm':
//		// ���� �׸� ��ġ�� �簢������ �̵��Ѵ�.
//		StopAllAnimation();
//		glutTimerFunc(120, ResetOriginPos, 1);
//		break;
//	case 'R':
//	case 'r':
//		// �簢������ �����ϰ� �ٽ� ���콺 �Է��� ���� �� �ִ�.
//		Reset();
//		break;
//	case 'Q':
//	case 'q':
//		exit(EXIT_FAILURE);
//		break;
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}