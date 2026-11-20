#include <GL/glut.h>

float xmin = -100, ymin = -100, xmax = 100, ymax = 100;
const int TOP = 8, BOTTOM = 4, RIGHT = 2, LEFT = 1; // TBRL = 8-4-2-1

int code(float x, float y) {
    int c = 0;
    if (y > ymax)      c |= TOP;
    else if (y < ymin) c |= BOTTOM;
    if (x > xmax)      c |= RIGHT;
    else if (x < xmin) c |= LEFT;
    return c;
}

void clip(float x0, float y0, float x1, float y1) {
    int c0 = code(x0, y0), c1 = code(x1, y1);
    while (true) {
        if (!(c0 | c1)) { // Trivial Accept
            glColor3f(0, 0, 1);
            glBegin(GL_LINES);
                glVertex2f(x0, y0); glVertex2f(x1, y1);
            glEnd();
            return;
        }
        if (c0 & c1) return; // Trivial Reject

        int c = c0 ? c0 : c1;
        float x, y, m = (y1 - y0) / (x1 - x0);
        if (c & TOP)         { x = x0 + (ymax - y0) / m; y = ymax; }
        else if (c & BOTTOM) { x = x0 + (ymin - y0) / m; y = ymin; }
        else if (c & RIGHT)  { y = y0 + (xmax - x0) * m; x = xmax; }
        else                 { y = y0 + (xmin - x0) * m; x = xmin; }

        if (c == c0) { x0 = x; y0 = y; c0 = code(x0, y0); }
        else         { x1 = x; y1 = y; c1 = code(x1, y1); }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    float x0 = -120, y0 = -120, x1 = 160, y1 = 80;
    float x2 = 60,   y2 = 30,   x3 = 90,  y3 = 120;

    glColor3f(1, 0, 0); // Red: original lines
    glBegin(GL_LINES);
        glVertex2f(x0, y0); glVertex2f(x1, y1);
        glVertex2f(x2, y2); glVertex2f(x3, y3);
    glEnd();

    glColor3f(0, 1, 0); // Green: clipping window
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin); glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax); glVertex2f(xmin, ymax);
    glEnd();

    clip(x0, y0, x1, y1); // Blue: clipped lines
    clip(x2, y2, x3, y3);
    glFlush();
}

void myInit() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-250, 250, -250, 250);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland Line Clipping");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
}