#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include "stdlib.h"
#include "math.h"

using namespace std;

void setPixel(int x, int y)
{
    glColor3f(0.0, 0.0, 0.0); //Set pixel to black  
    glBegin(GL_POINTS);
    glVertex2i(x, y); //Set pixel coordinates 
    glEnd();
    glFlush(); //Render pixel
}

class Line {
public:
    int round(const int a){
        return int(a + 0.5);
    }
    void lineDDA(int x0, int y0, int xEnd, int yEnd){
        int dx = xEnd - x0, dy = yEnd - y0, steps, k;

        int xIncrement, yIncrement, x = x0, y = y0;

        steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);

        xIncrement = int(dx) / int(steps);
        yIncrement = int(dy) / int(steps);
        setPixel(round(x), round(y));
        for(k = 0; k < steps; k++){
            x += xIncrement;
            y += yIncrement;
            setPixel(round(x), round(y));
        }
    }
    void polylineDDA(int v[][2], int n){
        if(n == 1){
            lineDDA(v[0][0], v[0][1], v[0][0], v[0][1]);
            return;
        }
        int x0 = v[0][0], y0 = v[0][1];
        for(int i = 1; i < n; i++){
            lineDDA(x0, y0, v[i][0], v[i][1]);
            x0 = v[i][0];
            y0 = v[i][1];
        }
        return;
    }
};



void init()
{
    glClearColor(0.0, 1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void testPolylineDDA()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);


    Line *line = new Line();
    line->lineDDA(0, 0, 100, 200);
    int vertexLine1[1][2] = { { 230, 30 } };
    line->polylineDDA(vertexLine1, 1);
    //int vertexLine2[5][2] = { {25, 100}, {60, 140}, {40, 20}, {80, 60}, {160, 70} };
    //line->polylineDDA(vertexLine2, 5);

    glFlush();
    return;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL");
    init();
    glutDisplayFunc(testPolylineDDA);
    glutMainLoop();
    return 0;
}
