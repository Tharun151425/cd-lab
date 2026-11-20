#include <iostream>
#include <cmath>
#include <GL/glut.h>
using namespace std;

int xc, yc, r, ch;
bool flag = false;

void drawCircle() {
    int x = 0, y = r, d = 3 - 2 * r;
    glBegin(GL_POINTS);
    while (x <= y) {
        glVertex2i(xc + x, yc + y); glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y); glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x); glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x); glVertex2i(xc - y, yc - x);

        if (d < 0) d += 4 * x + 6;
        else     { d += 4 * (x - y) + 10; y--; }
        x++;
    }
    glEnd();
    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!flag) { xc = x - 250; yc = 250 - y; cout << "Center: " << xc << " " << yc << "\n"; }
        else {
            int xb = x - 250, yb = 250 - y;
            r = hypot(xb - xc, yb - yc);
            cout << "Point: " << xb << " " << yb << " r=" << r << "\n";
            drawCircle();
        }
        flag = !flag;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (ch == 1) drawCircle();
    glFlush();
}

void myInit() {
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 1, 0);
    glPointSize(3);
    gluOrtho2D(-250, 250, -250, 250);
}

int main(int argc, char** argv) {
    cout << "1: Keyboard  2: Mouse : ";
    cin >> ch;
    if (ch == 1) { cout << "Enter xc yc r: "; cin >> xc >> yc >> r; }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Circle");
    myInit();
    glutDisplayFunc(display);
    if (ch == 2) glutMouseFunc(mouse);
    glutMainLoop();
}