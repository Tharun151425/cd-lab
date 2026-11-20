#include <iostream>
#include <algorithm>
#include <GL/glut.h>
using namespace std;

int n;
float x[100], y[100];

void scanFill() {
    glColor3f(0, 0, 1);
    for (int s = 0; s <= 500; s++) {
        float ix[100];
        int c = 0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            float xa = x[i], ya = y[i], xb = x[j], yb = y[j];
            if (yb < ya) { swap(xa, xb); swap(ya, yb); }
            if (s >= ya && s < yb)
                ix[c++] = xa + (s - ya) * (xb - xa) / (yb - ya);
        }
        sort(ix, ix + c);
        glBegin(GL_LINES);
        for (int i = 0; i + 1 < c; i += 2) {
            glVertex2f(ix[i], s);
            glVertex2f(ix[i + 1], s);
        }
        glEnd();
    }
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 1, 0);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; i++) glVertex2f(x[i], y[i]);
    glEnd();
    scanFill();
}

void myInit() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    cout << "Enter number of sides: ";
    cin >> n;
    cout << "Enter vertices (x y):\n";
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scanline Areafill");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
}