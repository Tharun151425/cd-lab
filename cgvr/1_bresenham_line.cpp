#include <iostream>
#include <cmath>
#include <GL/glut.h>
using namespace std;

int xa, ya, xb, yb, ch;
bool flag = false;

void drawLine() {
    int dx = abs(xb - xa), dy = abs(yb - ya);
    int incx = (xa <= xb) ? 1 : -1, incy = (ya <= yb) ? 1 : -1;
    int x = xa, y = ya;

    glBegin(GL_POINTS);
    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            glVertex2i(x, y);
            x += incx;
            if (p > 0) { y += incy; p -= 2 * dx; }
            p += 2 * dy;
        }
    } else {
        int p = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            glVertex2i(x, y);
            y += incy;
            if (p > 0) { x += incx; p -= 2 * dy; }
            p += 2 * dx;
        }
    }
    glEnd();
    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!flag) { xa = x - 250; ya = 250 - y; cout << "P1: " << xa << " " << ya << "\n"; }
        else       { xb = x - 250; yb = 250 - y; cout << "P2: " << xb << " " << yb << "\n"; drawLine(); }
        flag = !flag;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (ch == 1) drawLine();
    glFlush();
}

void myInit() {
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 1);
    glPointSize(5);
    gluOrtho2D(-250, 250, -250, 250);
}

int main(int argc, char** argv) {
    cout << "1: Keyboard  2: Mouse : ";
    cin >> ch;
    if (ch == 1) { cout << "Enter x1 y1 x2 y2: "; cin >> xa >> ya >> xb >> yb; }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line");
    myInit();
    glutDisplayFunc(display);
    if (ch == 2) glutMouseFunc(mouse);
    glutMainLoop();
}