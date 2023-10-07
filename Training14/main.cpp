#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>
#include <random>
#include "Init.h"
#include "filebuf.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define WIDTH 800
#define HEIGHT 600

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

GLvoid InitBuffer();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLuint ShaderProgram;
GLuint VBO[2], EBO;
GLuint midVBO, midEBO;
GLuint upVBO, upEBO;
GLuint boardEBO, boardVBO;

GLfloat down_move = 0.0f, mid_move = 0.0f, up_move = 0.0f;

float angle = 0;

bool mid_rotate = false;
bool up_rotate = false;

bool is_move = true;
int move_dir = 1;

bool is_camera_rotate = false;
bool is_standard_camera = false;

GLfloat y_angle = 0.0f, z_angle = 0.0f, camera_y = 0.0f;
int y_dir = 1, z_dir = 1;
glm::vec3 CameraPos = glm::vec3(0.0f, 1.0f, 2.0f);
glm::vec3 AT = glm::vec3(0.0f, 0.0f, 0.0f);

GLvoid DrawObject(void* obj_pos, void* obj_index, void* obj_color);

void Timer(int a);
void y_Rotate_Timer(int a);
void z_Rotate_Timer(int a);
void Camera_Timer(int a);
void InputKey(unsigned char key, int x, int y);
void SpecialKey(int key, int x, int y);

// 사각형
GLfloat vertex[] = {
		-0.5f, 1.0f, -0.5f,  //0번점
		-0.5f, 1.0f, 0.5f,  //1번점
		0.5f, 1.0f, 0.5f,  //2번점
		0.5f, 1.0f, -0.5f,  //3번점

		-0.5f, 0.0f, -0.5f,  //4번점
		-0.5f, 0.0f, 0.5f,  //5번점
		0.5f, 0.0f, 0.5f,  //6번점
		0.5f, 0.0f, -0.5f,  //7번점
};

GLint gIndices[] = 
{
	0,1,2,
	0,2,3,  //윗면

	1,5,6,
	1,6,2, //앞면

	2,6,7,
	2,7,3, //우측면

	0,4,5,
	0,5,1, //좌측면

	5,4,6,
	4,7,6,// 밑면

	0,7,4, //뒷면
	0,3,7
};

GLfloat colors[] = {
		1.0f,0.0f,0.0f,  //0번점
		0.0f,1.0f,0.0f,  //1번점
		0.0f,0.0f,1.0f,  //2번점
		1.0f,0.0f,0.0f,  //3번점

		0.0f,1.0f,0.0f,  //4번점
		0.0f,0.0f,1.0f,  //5번점
		1.0f,0.0f,0.0f,  //6번점
		0.0f,1.0f,0.0f,  //7번점
};

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

	InitBuffer();

	InitProgram(ShaderProgram);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(InputKey);
	glutSpecialFunc(SpecialKey);
	glutTimerFunc(10, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid InitBuffer()
{
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);
}

void Timer(int a)
{
	if (is_move) {
		if (move_dir == 1) {
			down_move += 0.01f;
			mid_move += 0.025f;
			up_move += 0.05f;

			if (down_move > 1.0f)
				move_dir = -1;
		}

		else if (move_dir == -1) {
			down_move -= 0.01f;
			mid_move -= 0.025f;
			up_move -= 0.05f;

			if (down_move < -1.0f)
				move_dir = 1;
		}
	}

	glutPostRedisplay();

	if (is_move)
		glutTimerFunc(10, Timer, a);
}

void y_Rotate_Timer(int a)
{
	if (mid_rotate) {
		if (y_dir == 1) {
			y_angle += 1.0f;

			if (y_angle > 90.0f)
				y_dir = -1;
		}

		else if (y_dir == -1) {
			y_angle -= 1.0f;

			if (y_angle < -90.0f)
				y_dir = 1;
		}
	}

	glutPostRedisplay();

	if (mid_rotate)
		glutTimerFunc(20, y_Rotate_Timer, a);
}

void Camera_Timer(int a)
{
	if (is_camera_rotate) {
		camera_y += 0.5f;
	}

	glutPostRedisplay();

	if (is_camera_rotate)
		glutTimerFunc(20, Camera_Timer, a);
}

void z_Rotate_Timer(int a)
{
	if (up_rotate) {
		if (z_dir == 1) {
			z_angle += 1.0f;

			if (z_angle > 90.0f)
				z_dir = -1;
		}

		else if (z_dir == -1) {
			z_angle -= 1.0f;

			if (z_angle < -90.0f)
				z_dir = 1;
		}
	}

	glutPostRedisplay();

	if (up_rotate)
		glutTimerFunc(20, z_Rotate_Timer, a);
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(ShaderProgram);

	unsigned int projectionLocation = glGetUniformLocation(ShaderProgram, "projection");
	unsigned int viewLocation = glGetUniformLocation(ShaderProgram, "view");

	glm::mat4 view = glm::mat4(1.0f);		// 뷰변환
	view = glm::lookAt(glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z), glm::vec3(AT.x, AT.y, AT.z), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotate = glm::mat4(1.0f);
	rotate = glm::rotate(rotate, glm::radians(camera_y), glm::vec3(0.0f, 1.0f, 0.0f));


	glm::mat4 itx = glm::mat4(1.0f);
	itx = glm::translate(itx, glm::vec3(-CameraPos.x, -CameraPos.y, -CameraPos.z));		// 역변환

	glm::mat4 tx = glm::mat4(1.0f);
	tx = glm::translate(tx, glm::vec3(CameraPos.x, CameraPos.y, CameraPos.z));		// 제자리 변환

	if (!is_standard_camera)
		view = view * rotate;

	else if (is_standard_camera)
		view = view * tx * rotate * itx;

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);		// 뷰변환


	glm::mat4 projection = glm::mat4(1.0f);		// 투영변환
	projection = glm::perspective(glm::radians(60.0f), (float)WIDTH / (float)HEIGHT, 1.0f, 200.0f);	// 뷰잉각도, 종횡비, 가까운거리, 먼거리
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);		// 투영변환

	DrawObject(vertex, gIndices, colors);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

GLvoid DrawObject(void* obj_pos, void* obj_index, void* obj_color)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 96, obj_pos, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 144, obj_index, GL_STATIC_DRAW);


	unsigned int modelLocation = glGetUniformLocation(ShaderProgram, "trans");

	glm::mat4 model = glm::mat4(1.0f);		// 모델변환
	glm::mat4 scale = glm::mat4(1.0f);

	scale = glm::scale(scale, glm::vec3(0.5f, 0.3f, 0.5f));


	glm::mat4 tx = glm::mat4(1.0f);
	tx = glm::translate(tx, glm::vec3(0.0f, 0.5f, 1.0f));

	glm::mat4 move = glm::mat4(1.0f);
	move = glm::translate(move, glm::vec3(0.0f, 0.0f, 0.0f));

	model = model * scale * tx * move;


	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));		// 모델변환

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 96, obj_color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void InputKey(unsigned char key, int x, int y)
{
	GLfloat step = 0.5f;

	switch (key)
	{
	case'm':
		if (mid_rotate) {
			mid_rotate = false;
		}

		else if (!mid_rotate) {
			mid_rotate = true;
			glutTimerFunc(20, y_Rotate_Timer, 1);
		}

		break;

	case 't':
		if (up_rotate) {
			up_rotate = false;
		}

		else if (!up_rotate) {
			up_rotate = true;
			glutTimerFunc(20, z_Rotate_Timer, 2);
		}

		break;

	case's':
		if (is_move) {
			is_move = false;
			mid_rotate = false;
			up_rotate = false;
		}

		else if (!is_move) {
			is_move = true;
			mid_rotate = true;
			up_rotate = true;
			glutTimerFunc(10, Timer, 1);
		}
		break;

	case'c':
		if (is_camera_rotate) {
			is_camera_rotate = false;
			camera_y = 0.0f;
		}

		else if (!is_camera_rotate) {
			is_camera_rotate = true;
			glutTimerFunc(10, Camera_Timer, 1);
		}

		break;

	case 'r':
		if (is_camera_rotate && is_standard_camera) {
			is_camera_rotate = false;
			is_standard_camera = false;
			camera_y = 0.0f;

		}

		else if (!is_camera_rotate) {
			is_camera_rotate = true;
			is_standard_camera = true;
			glutTimerFunc(10, Camera_Timer, 1);
		}

		break;

	case 'q':
		glutLeaveMainLoop();
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
	GLfloat step = 0.5f;
	switch (key)
	{
	case GLUT_KEY_DOWN:
		CameraPos.z += step;
		break;

	case GLUT_KEY_UP:
		CameraPos.z -= step;
		break;

	case GLUT_KEY_RIGHT:
		CameraPos.x += step;
		AT.x += step;
		break;

	case GLUT_KEY_LEFT:
		CameraPos.x -= step;
		AT.x -= step;
		break;
	}

	glutPostRedisplay();
}