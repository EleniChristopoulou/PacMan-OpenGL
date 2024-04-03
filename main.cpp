#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <cstdio>

bool gameOver = false;


float square_size = 30, map_width = 21, map_height = 23;
float window_width = map_width*square_size, window_height = map_height*square_size;

float map[23][21] = {
    {0,0,0,0,0,0,0,  0,0,0, 0 ,0,0,0,  0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,  1,1,1, 0 ,1,1,1,  1,1,1,1,1,1,0},
    {0,1,0,0,0,1,0,  0,0,1, 0 ,1,0,0,  0,1,0,0,0,1,0},
    {0,1,0,0,0,1,0,  0,0,1, 0 ,1,0,0,  0,1,0,0,0,1,0},
    {0,1,1,1,1,1,1,  1,1,1, 1 ,1,1,1,  1,1,1,1,1,1,0},

    {0,1,0,0,0,1,0,  1,0,0, 0 ,0,0,1,  0,1,0,0,0,1,0},
    {0,1,1,1,1,1,0,  1,1,1, 0 ,1,1,1,  0,1,1,1,1,1,0},
    {0,0,0,0,0,1,0,  0,0,1, 0 ,1,0,0,  0,1,0,0,0,0,0},
    {1,1,1,1,0,1,0,  1,1,1, 1 ,1,1,1,  0,1,0,1,1,1,1},
    {0,0,0,0,0,1,0,  1,0,0, 1 ,0,0,1,  0,1,0,0,0,0,0},
    {1,1,1,1,1,1,1,  1,0,1, 1 ,1,0,1,  1,1,1,1,1,1,0},
    {0,0,0,0,0,1,0,  1,0,1, 1 ,1,0,1,  0,1,0,0,0,0,0},

    {1,1,1,1,0,1,0,  1,0,0, 0 ,0,0,1,  0,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,  1,1,1, 1 ,1,1,1,  0,1,0,1,1,1,1},
    {0,0,0,0,0,1,1,  1,0,0, 0 ,0,0,1,  1,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,  1,1,1, 0 ,1,1,1,  1,1,1,1,1,1,0},
    {0,1,0,0,0,1,0,  0,0,1, 0 ,1,0,0,  0,1,0,0,0,1,0},
    {0,1,1,1,0,1,1,  1,1,1, 1 ,1,1,1,  1,1,0,1,1,1,0},
    {0,0,1,1,0,1,0,  1,0,0, 0 ,0,0,1,  0,1,0,1,1,0,0},
    {0,1,1,1,1,1,0,  1,1,1, 0 ,1,1,1,  0,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,  0,0,1, 0 ,1,0,0,  0,0,0,0,0,1,0},
    {0,1,1,1,1,1,1,  1,1,1, 1 ,1,1,1,  1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,  0,0,0, 0 ,0,0,0,  0,0,0,0,0,0,0}

};

void drawMap(){
    glColor3ub(0, 0, 255);
    
    glColor3ub(200, 0, 0);

    int i=0, j=0;
    for(int i=0; i<map_height; i++){
        for(int j=0; j<map_width; j++){
        if(map[i][j] == 0){
            glBegin(GL_POLYGON);
            glVertex2f(j*square_size,i*square_size);
            glVertex2f(j*square_size+square_size,i*square_size);
            
            glVertex2f(j*square_size+square_size,i*square_size+square_size);
            glVertex2f(j*square_size,i*square_size+square_size);
            glEnd();
        }
        }
    }
}


void timer(int) //1 fps
{
    glutPostRedisplay();
    glutTimerFunc(1000.0 / 60.0, timer, 0);
}

void display(){
    
    glClearColor(0.0f,0.0f,0.0f, 0.0f);   //background color
    glClear(GL_COLOR_BUFFER_BIT);

    if(!gameOver){
        drawMap();    
    }else{
        
    }

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
 glutInit(&argc,argv);
 glutInitWindowPosition(350, 0);
 glutInitWindowSize(window_width, window_height);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 glutCreateWindow("OpenGL Pacman");
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D(0,window_width,window_height,0);
 glMatrixMode(GL_MODELVIEW);

 glutDisplayFunc(display);

 glutTimerFunc(1000.0 / 60.0, timer, 0);
 glutMainLoop();

 return 0;
}