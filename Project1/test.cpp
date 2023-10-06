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
//// 정점 좌표와 인덱스 데이터 정의
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
//    0, 1, 2, 2, 3, 0, // 앞면
//    4, 5, 6, 6, 7, 4, // 뒷면
//    7, 3, 0, 0, 4, 7, // 왼쪽면
//    1, 5, 6, 6, 2, 1, // 오른쪽면
//    7, 6, 5, 5, 4, 7, // 윗면
//    3, 2, 1, 1, 0, 3  // 아랫면
//};
//
//float angle = 0.0f; // 회전 각도
//
//void renderScene();
//void reshape(int width, int height);
//
//int main(int argc, char** argv) {
//    // GLUT 초기화
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//    glutInitWindowSize(WIDTH, HEIGHT);
//    glutCreateWindow("3D 정육면체 회전하기");
//
//    // GLEW 초기화
//    glewExperimental = GL_TRUE;
//    if (glewInit() != GLEW_OK) {
//        std::cerr << "GLEW 초기화에 실패했습니다." << std::endl;
//        return -1;
//    }
//
//    // 렌더링 함수 및 리사이징 함수 등록
//    glutDisplayFunc(renderScene);
//    glutReshapeFunc(reshape);
//
//    // 타이머 함수 등록
//    glutTimerFunc(10, [](int value) {
//        angle += 1.0f; // 회전 각도 증가
//        if (angle > 360.0f) angle -= 360.0f;
//        glutPostRedisplay(); // 다음 프레임을 그리도록 요청
//        glutTimerFunc(10, nullptr, 0); // 타이머 재등록
//        }, 0);
//
//    // 메인 루프 진입
//    glutMainLoop();
//
//    return 0;
//}
//
//void renderScene() {
//    // 화면 지우기
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // 시점 설정
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glTranslatef(0.0f, 0.0f, -3.0f); // 카메라 위치 조정
//    glRotatef(angle, 0.0f, 1.0f, 0.0f); // y축 주위로 회전
//
//    // 정점 좌표 설정
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
//
//    // 정육면체 그리기
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
//
//    // 화면 업데이트
//    glutSwapBuffers();
//}
//
//void reshape(int width, int height) {
//    // 뷰포트 업데이트
//    glViewport(0, 0, width, height);
//
//    // 투영 행렬 업데이트
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
//    glMatrixMode(GL_MODELVIEW);
//}