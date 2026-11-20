#include <cmath>
#include <GL/glut.h>

void display1() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 200; i += 3)
        for (float t = 0; t < 2 * 3.142; t += 0.01)
            glVertex2f(200 + 50 * cos(t), 200 + i + 50 * sin(t));
    glEnd();
    glFlush();
}

void display2() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);
    for (int i = 0; i < 100; i += 2) {
        glBegin(GL_LINE_LOOP);
            glVertex2f(100 + i, 100 + i);
            glVertex2f(300 + i, 100 + i);
            glVertex2f(300 + i, 300 + i);
            glVertex2f(100 + i, 300 + i);
        glEnd();
    }
    glFlush();
}

void myInit() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(500, 500);

    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cylinder");
    glutDisplayFunc(display1);
    myInit();

    glutInitWindowPosition(600, 100);
    glutCreateWindow("Parallelepiped");
    glutDisplayFunc(display2);
    myInit();

    glutMainLoop();
}