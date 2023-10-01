//#include <GL/glew.h>
//#include <GL/freeglut.h>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//
//GLuint programID;
//
//// �ﰢ�� 2���� ���� ��ǥ�� ������ �����մϴ�.
//GLfloat vertices[][6] = {
//    { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f }, // ������
//    {  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f }, // ���
//    {  0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f }, // �Ķ���
//    { -0.8f, -0.8f, 0.0f, 1.0f, 1.0f, 0.0f }, // �����
//    {  0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f }, // ���ֻ�
//    {  0.0f,  0.8f, 0.0f, 0.0f, 1.0f, 1.0f }  // û��
//};
//
//// �ﰢ���� �׸��� �Լ�
//void drawTriangle(int a, int b, int c) {
//    glBegin(GL_TRIANGLES);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(a * 6 * sizeof(GLfloat)));
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)((a * 6 + 3) * sizeof(GLfloat)));
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glDisableVertexAttribArray(0);
//    glDisableVertexAttribArray(1);
//    glEnd();
//}
//
//// ���÷��� �ݹ� �Լ�
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // GLSL ���α׷� ���
//    glUseProgram(programID);
//
//    // �� ���� �ﰢ�� �׸���
//    drawTriangle(0, 1, 2);
//    drawTriangle(3, 4, 5);
//
//    glUseProgram(0);
//
//    glutSwapBuffers();
//}
//
//// GLSL ���̴��� �а� �������ϴ� �Լ�
//GLuint loadShader(const char* filename, GLenum shaderType) {
//    GLuint shaderID = glCreateShader(shaderType);
//    std::ifstream shaderFile(filename);
//    std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
//    const char* shaderCode = shaderText.c_str();
//    glShaderSource(shaderID, 1, &shaderCode, NULL);
//    glCompileShader(shaderID);
//
//    // ������ ���� Ȯ��
//    GLint success;
//    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        GLint logSize;
//        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
//        std::vector<GLchar> errorLog(logSize);
//        glGetShaderInfoLog(shaderID, logSize, NULL, errorLog.data());
//        std::cerr << "Shader compilation failed:\n" << errorLog.data() << std::endl;
//        glDeleteShader(shaderID);
//        return 0;
//    }
//
//    return shaderID;
//}
//
//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("OpenGL �ﰢ�� 2��");
//
//    glewInit();
//
//    // GLSL ���α׷� ����
//    GLuint vertexShaderID = loadShader("vertex.glsl", GL_VERTEX_SHADER);
//    GLuint fragmentShaderID = loadShader("fragment.glsl", GL_FRAGMENT_SHADER);
//    programID = glCreateProgram();
//    glAttachShader(programID, vertexShaderID);
//    glAttachShader(programID, fragmentShaderID);
//    glLinkProgram(programID);
//    glUseProgram(programID);
//
//    glutDisplayFunc(display);
//
//    glutMainLoop();
//
//    // ���α׷� ���� �� ���̴� �� ���α׷� ����
//    glDeleteProgram(programID);
//    glDeleteShader(vertexShaderID);
//    glDeleteShader(fragmentShaderID);
//
//    return 0;
//}