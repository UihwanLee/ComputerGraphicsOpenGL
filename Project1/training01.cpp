//#include <iostream>
//#include <gl/glew.h> //--- 필요한 헤더파일 include
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <random>
//#include <time.h>
//
//using namespace std;
//
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLvoid Keyboard(unsigned char key, int x, int y);
//GLvoid TimerFunction(int value);
//
//struct GColor
//{
//	float red = 1.0f;
//	float green = 1.0f;
//	float blue = 1.0f;
//	float alpha = 1.0;
//};
//
//// 글로벌 변수
//GColor gColor;
//bool g_ChangColorByTime = false;
//
//
//void main(int argc, char** argv)
//{
//	// 윈도우 생성
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(0, 0);
//	glutInitWindowSize(800, 600);
//	glutCreateWindow("Example1");
//
//	// GLEW 초기화
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		cerr << "Unable to initialize GLEW" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	else
//		cout << "GLEW Initialized\n";
//
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(Reshape);
//	glutKeyboardFunc(Keyboard);
//	glutMainLoop();
//}
//
//GLvoid drawScene()
//{
//	// 변경된 배경색 설정
//	glClearColor(gColor.red, gColor.green, gColor.blue, gColor.alpha);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glutSwapBuffers();
//}
//
//GLvoid Reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//}
//
//GLvoid ChangeBGColor(float _red, float _green, float _blue)
//{
//	gColor.red = _red/255.0;
//	gColor.green = _green/255.0;
//	gColor.blue = _blue/255.0;
//}
//
//GLvoid RandomChange()
//{
//	srand(time(NULL));
//	int r, g, b;
//	r = rand() % 255 + 1;
//	g = rand() % 255 + 1;
//	b = rand() % 255 + 1;
//	ChangeBGColor(r, g, b);
//}
//
//GLvoid TimerFunction(int value)
//{
//	RandomChange();
//	glutPostRedisplay();
//	if (value) glutTimerFunc(30, TimerFunction, g_ChangColorByTime);
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'c':
//		ChangeBGColor(0.0f, 86.0f, 102.0f);
//		break;
//	case 'm':
//		ChangeBGColor(255.0f, 0.0f, 255.0f);
//		break;
//	case 'y':
//		ChangeBGColor(255.0f, 255.0f, 0.0f); 
//		break;
//	case 'a':
//		RandomChange();
//		break;
//	case 'w':
//		ChangeBGColor(255.0f, 255.0f, 255.0f); 
//		break;
//	case 'k':
//		ChangeBGColor(0.0f, 0.0f, 0.0f); 
//		break;
//	case 't':
//		g_ChangColorByTime = true;
//		glutTimerFunc(30, TimerFunction, g_ChangColorByTime);
//		break;
//	case 's':
//		g_ChangColorByTime = false;
//		break;
//	case 'q':
//		exit(EXIT_FAILURE);
//		break;
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}