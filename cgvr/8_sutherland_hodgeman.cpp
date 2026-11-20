#include <GL/glut.h>

float xmin = -100, ymin = -100, xmax = 100, ymax = 100;
struct Point { float x, y; };

void drawPoly(Point p[], int n) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; i++) glVertex2f(p[i].x, p[i].y);
    glEnd();
}

bool inside(Point p, int e) {
    if (e == 0) return p.x >= xmin; // Left
    if (e == 1) return p.x <= xmax; // Right
    if (e == 2) return p.y >= ymin; // Bottom
    return p.y <= ymax;             // Top
}

Point intersect(Point s, Point p, int e) {
    float m = (p.y - s.y) / (p.x - s.x);
    if (e == 0) return {xmin, s.y + (xmin - s.x) * m};
    if (e == 1) return {xmax, s.y + (xmax - s.x) * m};
    if (e == 2) return {s.x + (ymin - s.y) / m, ymin};
    return {s.x + (ymax - s.y) / m, ymax};
}

void clipPoly(Point in[], int n) {
    Point out[20];
    for (int e = 0; e < 4; e++) {
        int m = 0;
        if (n == 0) break;
        Point s = in[n - 1];
        for (int i = 0; i < n; i++) {
            Point p = in[i];
            bool inS = inside(s, e), inP = inside(p, e);
            if (inS && inP)      out[m++] = p;
            else if (inS)        out[m++] = intersect(s, p, e);
            else if (inP)      { out[m++] = intersect(s, p, e); out[m++] = p; }
            s = p;
        }
        for (int i = 0; i < m; i++) in[i] = out[i];
        n = m;
    }
    glColor3f(0, 0, 1); // Blue: clipped polygon
    drawPoly(in, n);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Point poly[20] = {{-120, -120}, {160, 80}, {60, 30}, {90, 120}};

    glColor3f(1, 0, 0); // Red: original polygon
    drawPoly(poly, 4);

    glColor3f(0, 1, 0); // Green: clipping window
    Point win[4] = {{xmin, ymin}, {xmax, ymin}, {xmax, ymax}, {xmin, ymax}};
    drawPoly(win, 4);

    clipPoly(poly, 4);
    glFlush();
}

void myInit() {
    glClearColor(1, 1, 1, 1);
    glLineWidth(3);
    gluOrtho2D(-250, 250, -250, 250);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
}