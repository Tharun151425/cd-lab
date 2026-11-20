#include <iostream>
#include <cmath>
#include <GL/glut.h>
using namespace std;

int opt;
float m, c, t, xr, yr;
float house[11][2] = {
    {100,100}, {140,100}, {140,150}, {160,150}, {160,100}, {200,100}, // Base + Door
    {200,200}, {220,200}, {150,300}, {80,200},  {100,200}             // Walls + Roof
};

void drawHouse() {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 11; i++) glVertex2fv(house[i]);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0); // Red: original house
    drawHouse();

    if (opt == 2) {     // Green: mirror line y = mx + c
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
            glVertex2f(0, c);
            glVertex2f(400, 400 * m + c);
        glEnd();
    }

    glPushMatrix();
    if (opt == 1) {
        glTranslatef(xr, yr, 0);
        glRotatef(t, 0, 0, 1);
        glTranslatef(-xr, -yr, 0);
    } else {
        float r = atan(m) * 180 / 3.1415;
        glTranslatef(0, c, 0);
        glRotatef(r, 0, 0, 1);
        glScalef(1, -1, 1);
        glRotatef(-r, 0, 0, 1);
        glTranslatef(0, -c, 0);
    }
    glColor3f(0, 0, 1); // Blue: transformed house
    drawHouse();
    glPopMatrix();
    glFlush();
}

void myInit() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    cout << "1: Rotation  2: Reflection : ";
    cin >> opt;
    if (opt == 1) { cout << "Enter t xr yr: "; cin >> t >> xr >> yr; }
    else          { cout << "Enter m c: ";     cin >> m >> c; }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("House Transformation");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
}