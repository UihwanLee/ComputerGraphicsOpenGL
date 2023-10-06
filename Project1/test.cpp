//#include <iostream>
//#include <gl/glew.h> 
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <gl/glm/glm/glm.hpp>
//#include <gl/glm/glm/gtc/matrix_transform.hpp>
//#include <vector>
//
//const GLuint WIDTH = 800, HEIGHT = 600;
//
//// ���� ��ǥ�� �ε��� ������ ����
//std::vector<glm::vec3> vertices = {
//    {-0.5f, -0.5f, -0.5f}, // 0
//    { 0.5f, -0.5f, -0.5f}, // 1
//    { 0.5f,  0.5f, -0.5f}, // 2
//    {-0.5f,  0.5f, -0.5f}, // 3
//    {-0.5f, -0.5f,  0.5f}, // 4
//    { 0.5f, -0.5f,  0.5f}, // 5
//    { 0.5f,  0.5f,  0.5f}, // 6
//    {-0.5f,  0.5f,  0.5f}  // 7
//};
//
//std::vector<GLuint> indices = {
//    0, 1, 2, 2, 3, 0, // �ո�
//    4, 5, 6, 6, 7, 4, // �޸�
//    7, 3, 0, 0, 4, 7, // ���ʸ�
//    1, 5, 6, 6, 2, 1, // �����ʸ�
//    7, 6, 5, 5, 4, 7, // ����
//    3, 2, 1, 1, 0, 3  // �Ʒ���
//};
//
//float angle = 0.0f; // ȸ�� ����
//
//void renderScene();
//void reshape(int width, int height);
//
//int main(int argc, char** argv) {
//    // GLUT �ʱ�ȭ
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//    glutInitWindowSize(WIDTH, HEIGHT);
//    glutCreateWindow("3D ������ü ȸ���ϱ�");
//
//    // GLEW �ʱ�ȭ
//    glewExperimental = GL_TRUE;
//    if (glewInit() != GLEW_OK) {
//        std::cerr << "GLEW �ʱ�ȭ�� �����߽��ϴ�." << std::endl;
//        return -1;
//    }
//
//    // ������ �Լ� �� ������¡ �Լ� ���
//    glutDisplayFunc(renderScene);
//    glutReshapeFunc(reshape);
//
//    // Ÿ�̸� �Լ� ���
//    glutTimerFunc(10, [](int value) {
//        angle += 1.0f; // ȸ�� ���� ����
//        if (angle > 360.0f) angle -= 360.0f;
//        glutPostRedisplay(); // ���� �������� �׸����� ��û
//        glutTimerFunc(10, nullptr, 0); // Ÿ�̸� ����
//        }, 0);
//
//    // ���� ���� ����
//    glutMainLoop();
//
//    return 0;
//}
//
//void renderScene() {
//    // ȭ�� �����
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // ���� ����
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glTranslatef(0.0f, 0.0f, -3.0f); // ī�޶� ��ġ ����
//    glRotatef(angle, 0.0f, 1.0f, 0.0f); // y�� ������ ȸ��
//
//    // ���� ��ǥ ����
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
//
//    // ������ü �׸���
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
//
//    // ȭ�� ������Ʈ
//    glutSwapBuffers();
//}
//
//void reshape(int width, int height) {
//    // ����Ʈ ������Ʈ
//    glViewport(0, 0, width, height);
//
//    // ���� ��� ������Ʈ
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
//    glMatrixMode(GL_MODELVIEW);
//}