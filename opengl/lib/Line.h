#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

class Line {
public:
    int round(const float a){
        return int(a + 0.5);
    }
    void lineDDA(int x0, int y0, int xEnd, int yEnd){
        int dx = xEnd - x0, dy = yEnd - y0, steps, k;
        float xIncrement, yIncrement, x = x0, y = y0;
        steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
        xIncrement = float(dx) / float(steps);
        yIncrement = float(dy) / float(steps);
        for(k = 0; k < steps; k++){
            x += xIncrement;
            y += yIncrement;
        }
    }
};
