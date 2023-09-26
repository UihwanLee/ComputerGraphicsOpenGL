//#include <iostream>
//#include <gl/glew.h> //--- 필요한 헤더파일 include
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
////사각형에 애니메이션 적용하기
//// 배경색은 짙은 회색으로 정하고, 사각형은 랜덤한 색으로 그린다.
//// 마우스를 클릭하는 곳이 중심이 되어 사각형을 그린다.최대 5개의 사각형을 그린다.
//// 키보드 입력(명령어는 대문자 또는 소문자 무관함) :
////	 a : 위치 변화1 : 사각형들은 각각 대각선으로 이동하고 벽에 닿으면 튕겨 다른 방향으로 이동한다 / 멈춘다.
////	 i : 위치 변화2 : 사각형들이 지그재그로 이동한다 / 멈춘다.
////	 c : 크기 변화 : 사각형의 크기가 다양하게 변한다 / 변하지 않는다.
////	 o : 색상 변화 : 사각형의 색상이 랜덤하게 변한다 / 변하지 않는다.
////	 s : 모든 애니메이션이 멈춘다.
////	 m : 원래 그린 위치로 사각형들이 이동한다.
////	 r : 사각형들을 삭제하고 다시 마우스 입력을 받을 수 있다.
////	 q : 프로그램을 종료한다.
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
//// 글로벌 변수
//Color bgColor;
//vector<Rect> gRect;
//int g_rect_num = -1;
//bool g_left_button = false;
//
//GLfloat RECT_SIZE = 0.2f;
//
//// 애니메이션 체크 변수
//bool is_MovingAnim1 = false;
//bool is_MovingAnim2 = false;
//bool is_ChangingScaleAnim = false;
//bool is_ChangingColorAnim = false;
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
//	// 변경된 배경색 설정
//	glClearColor(74.0f/255.0f, 79.0f / 255.0f, 90.0f/255.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// 사각형 그리기
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
//// 이동하는 사각형 벽과 충돌 체크 후 방향 바꾸기
//GLvoid CheckCollisionRect(int idx)
//{
//	if (gRect[idx].isActive == false) return;
//
//	// 위쪽 벽에 닿은 경우
//	if (gRect[idx].pivot.y + gRect[idx].dist_y >= 1.0f)
//	{
//		gRect[idx].moving1_y = gRect[idx].moving1_y * (-1.0f);
//	}
//
//	// 아래쪽 벽에 닿은 경우
//	if (gRect[idx].pivot.y - gRect[idx].dist_y <= -1.0f)
//	{
//		gRect[idx].moving1_y = gRect[idx].moving1_y * (-1.0f);
//	}
//
//	// 오른쪽 벽에 닿은 경우
//	if (gRect[idx].pivot.x + gRect[idx].dist_x >= 1.0f)
//	{
//		gRect[idx].moving1_x = gRect[idx].moving1_x * (-1.0f);
//	}
//
//	// 왼쪽 벽에 닿은 경우
//	if (gRect[idx].pivot.x - gRect[idx].dist_x <= -1.0f)
//	{
//		gRect[idx].moving1_x = gRect[idx].moving1_x * (-1.0f);
//	}
//}
//
//// 사각형 대각선으로 이동
//GLvoid MovingRectDiagonal(int idx)
//{
//	if (gRect[idx].isActive == false) return;
//
//	gRect[idx].pivot.x += gRect[idx].moving1_x;
//	gRect[idx].pivot.y += gRect[idx].moving1_y;
//}
//
//// 사각형 지그재그로 이동
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
//// 사각형 크기 무작위로 변경
//GLvoid RandomChangeScale(int idx)
//{
//	if (gRect[idx].isActive == false) return;
//
//	GLfloat temp_x, temp_y;
//
//	random_device rd;
//	mt19937 gen(rd());
//
//	// 분포 생성 (0.1, 0.3) 범위로 설정
//	uniform_real_distribution<float> dis(0.1f, 0.3f);
//
//	// 랜덤 값 생성
//	temp_x = dis(gen);
//	temp_y = dis(gen);
//
//	gRect[idx].dist_x = temp_x;
//	gRect[idx].dist_y = temp_y;
//}
//
//// 사각형 색깔 무작위로 변경
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
//// 클릭된 지점에 사각형 생성
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
//// 위치 변화1
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
//// 위치 변화2
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
//// 크기 변화
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
//// 색상 변화
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
//// 모든 애니메이션 멈추기
//GLvoid StopAllAnimation()
//{
//	is_MovingAnim1 = false;
//	is_MovingAnim2 = false;
//	is_ChangingScaleAnim = false;
//	is_ChangingColorAnim = false;
//}
//
//// 원래 그린 위치로 사각형들이 이동한다
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
//// 사각형들을 삭제하고 다시 마우스 입력을 받을 수 있다.
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
//		// 위치 변화1 : 사각형들은 각각 대각선으로 이동하고 벽에 닿으면 튕겨 다른 방향으로 이동한다/멈춘다.
//		if (is_MovingAnim1) is_MovingAnim1 = false;
//		else is_MovingAnim1 = true;
//		if (is_MovingAnim1) glutTimerFunc(30, MovingAnim1, is_MovingAnim1);
//		break;
//	case 'I':
//	case 'i':
//		// 위치 변화2 : 사각형들이 지그재그로 이동한다/멈춘다.
//		if (is_MovingAnim2) is_MovingAnim2 = false;
//		else is_MovingAnim2 = true;
//		if (is_MovingAnim2) glutTimerFunc(30, MovingAnim2, is_MovingAnim2);
//		break;
//	case 'C':
//	case 'c':
//		// 크기 변화 : 사각형의 크기가 다양하게 변한다/변하지 않는다.
//		is_ChangingScaleAnim = !is_ChangingScaleAnim;
//		if(is_ChangingScaleAnim) glutTimerFunc(120, ChangeAllScale, is_ChangingScaleAnim);
//		break;
//	case 'O':
//	case 'o':
//		// 색상 변화 : 사각형의 색상이 랜덤하게 변한다/변하지 않는다
//		is_ChangingColorAnim = !is_ChangingColorAnim;
//		if(is_ChangingColorAnim) glutTimerFunc(120, ChangeAllColor, is_ChangingColorAnim);
//		break;
//	case 'S':
//	case 's':
//		// 모든 애니메이션이 멈춘다.
//		StopAllAnimation();
//		break;
//	case 'M':
//	case 'm':
//		// 원래 그린 위치로 사각형들이 이동한다.
//		StopAllAnimation();
//		glutTimerFunc(120, ResetOriginPos, 1);
//		break;
//	case 'R':
//	case 'r':
//		// 사각형들을 삭제하고 다시 마우스 입력을 받을 수 있다.
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