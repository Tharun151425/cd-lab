#include <iostream>
#include <GL/glut.h>
using namespace std;

typedef GLfloat point[3];
point v[4] = {{0, 0, 250}, {0, 250, -250}, {250, -250, -250}, {-250, -250, -250}};
int iter;

void tri(point a, point b, point c) {
    glVertex3fv(a); glVertex3fv(b); glVertex3fv(c);
}

void divide(point a, point b, point c, point d, int m) {
    if (m > 0) {
        point ab, ac, ad, bc, bd, cd;
        for (int j = 0; j < 3; j++) {
            ab[j] = (a[j] + b[j]) / 2;  bc[j] = (b[j] + c[j]) / 2;
            ac[j] = (a[j] + c[j]) / 2;  bd[j] = (b[j] + d[j]) / 2;
            ad[j] = (a[j] + d[j]) / 2;  cd[j] = (c[j] + d[j]) / 2;
        }
        divide(a,  ab, ac, ad, m - 1);
        divide(ab, b,  bc, bd, m - 1);
        divide(ac, bc, c,  cd, m - 1);
        divide(ad, bd, cd, d,  m - 1);
    } else {
        glColor3f(1, 0, 0); tri(a, b, c);
        glColor3f(0, 1, 0); tri(a, b, d);
        glColor3f(0, 0, 1); tri(a, c, d);
        glColor3f(1, 1, 0); tri(b, c, d);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
        divide(v[0], v[1], v[2], v[3], iter);
    glEnd();
    glFlush();
}

void myInit() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
    glOrtho(-250, 250, -250, 250, -250, 250);
}

int main(int argc, char** argv) {
    cout << "Enter number of subdivisions: ";
    cin >> iter;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sierpinski Gasket");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
}