#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <cstdio>

char message[60];
int score=0;
bool gameOver = false;

float square_size = 30, map_width = 21, map_height = 23;
float window_width = map_width*square_size, window_height = map_height*square_size;
    int counter = 0;
//Pacman Parameters
float pacmanX = 10*square_size+square_size/2, pacmanY = 11*square_size+square_size/2, speed = 3, dx = 1, dy = 1;
float mouth_start = M_PI/4;
    //animation
float direction = 0;
bool increase = true;

//keyboard
bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

float map[23][21] = {
    {0,0,0,0,0,0,0,  0,0,0, 0 ,0,0,0,  0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,  1,1,1, 0 ,1,1,1,  1,1,1,1,1,1,0},
    {0,1,0,0,0,1,0,  0,0,1, 0 ,1,0,0,  0,1,0,0,0,1,0},
    {0,1,0,0,0,1,0,  0,0,1, 0 ,1,0,0,  0,1,0,0,0,1,0},
    {0,1,1,1,1,1,1,  1,1,1, 1 ,1,1,1,  1,1,1,1,1,1,0},

    {0,1,0,0,0,1,0,  1,0,0, 0 ,0,0,1,  0,1,0,0,0,1,0},
    {0,1,1,1,1,1,0,  1,1,1, 0 ,1,1,1,  0,1,1,1,1,1,0},
    {0,0,0,0,0,1,0,  0,0,1, 0 ,1,0,0,  0,1,0,0,0,0,0},
    {2,2,2,2,0,1,0,  1,1,1, 1 ,1,1,1,  0,1,0,2,2,2,2},
    {0,0,0,0,0,1,0,  1,0,0, 1 ,0,0,1,  0,1,0,0,0,0,0},
    {1,1,1,1,1,1,1,  1,0,1, 1 ,1,0,1,  1,1,1,1,1,1,0},
    {0,0,0,0,0,1,0,  1,0,1, 1 ,1,0,1,  0,1,0,0,0,0,0},

    {2,2,2,2,0,1,0,  1,0,0, 0 ,0,0,1,  0,1,0,2,2,2,2},
    {2,2,2,2,0,1,0,  1,1,1, 1 ,1,1,1,  0,1,0,2,2,2,2},
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
    
    glColor3ub(200, 0, 0);

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

int getPoints(){
    int points = 0;

    for(int i=0; i<map_height; i++){
        for(int j=0; j<map_width; j++){
            if(map[i][j] == 1){
                points++;
            }
        }
    }

    return points;
}

int checkColision(float nextPositionX, float nextPositionY){
    
    int up_right,down_right, up_left, down_left;
    int val = 1;    //in order to narrow the borders of pacman

    // center = map[(int) round((nextPositionY-square_size/2)/square_size)][(int) round((nextPositionX-square_size/2)/square_size)];

    up_right = map[(int) round((nextPositionY-square_size + val)/square_size)][(int) round((nextPositionX -val)/square_size)];
    down_right = map[(int) round((nextPositionY- val)/square_size)][(int) round((nextPositionX -val)/square_size)];

    up_left = map[(int) round((nextPositionY-square_size + val)/square_size)][(int) round((nextPositionX -square_size + val)/square_size)];
    down_left = map[(int) round((nextPositionY-val)/square_size)][(int) round((nextPositionX -square_size + val)/square_size)];


    return down_left*up_left*up_right*down_right;
}

void pacmanAnimation(){

    if((counter%5)==0){
        if(increase){
            mouth_start = mouth_start + M_PI/10;
            if(mouth_start >= M_PI / 4){
                increase = false;
            }
        }else{
            mouth_start = mouth_start - M_PI/10;
            if(mouth_start <= 0){
                increase = true;
            }
        }
    }
    counter ++ ;
}

void drawPacMan(int xc, int yc){
    glColor3ub(255, 255, 0);

    glBegin(GL_POLYGON);
    glVertex2f(xc,yc);

    pacmanAnimation();
    
    for(float theta=direction+mouth_start; theta<2*M_PI-mouth_start+direction; theta=theta+M_PI/10){
        glVertex2f(xc + square_size/2*cos(theta),yc + square_size/2*sin(theta));
    }

    glEnd();
}

void pacmanEat(){
    int x,y;
    
    y = (int) round((pacmanY-square_size/2)/square_size); 
    x = (int) round((pacmanX-square_size/2)/square_size);

    if(map[y][x] == 1){
        map[y][x] = 2;
        // score++;
    }
}

void drawPoints(){
    glColor3ub(255, 255, 255);

    int i=0, j=0;
    for(int i=0; i<map_height; i++){
        for(int j=0; j<map_width; j++){
        if(map[i][j] == 1){
            glBegin(GL_POLYGON);        //DIAMOND SHAPED POINTS

            glVertex2f(j*square_size+square_size/2,i*square_size+square_size/2 - 3);              //top
            glVertex2f(j*square_size+square_size/2 - 3,i*square_size+square_size/2);              //left
            
            glVertex2f(j*square_size+square_size/2,i*square_size+square_size/2 + 3);  //bottom
            glVertex2f(j*square_size+square_size/2 + 3,i*square_size+square_size/2);    //right

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

void type(char *str, float x, float y, float s)
{
    glLineWidth(3);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xFFFF);
    char *c;
    glPushMatrix();
    glTranslatef(y, x, 0);
    glScalef(0.1 * s, 0.1 * -s, 0);
    for (c = str; *c != '\0'; c++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    glScalef(1, 1, 1);
    glPopMatrix();
}

void display(){
    
    glClearColor(0.0f,0.0f,0.0f, 0.0f);   //background color
    glClear(GL_COLOR_BUFFER_BIT);

    if(getPoints()!=0){
        drawMap();
        drawPoints();

        drawPacMan(pacmanX,pacmanY);
        pacmanEat();    
        
        char str[] = "Points: ";            //Add score
        glColor3ub(255,255,255);;         
        sprintf(message, "%d", 220-getPoints());
        strcat(str, message);
        strcat(str, "/220");
        type(str, -25, 10, 2);

        if(keySpecialStates[GLUT_KEY_UP] && checkColision(pacmanX, pacmanY-speed)!=0 ) {pacmanY -= speed; direction = 3*M_PI/2;}
        if(keySpecialStates[GLUT_KEY_DOWN] && checkColision(pacmanX, pacmanY+speed)!=0) {pacmanY += speed; direction = M_PI/2;}
        if(keySpecialStates[GLUT_KEY_LEFT] && checkColision(pacmanX-speed, pacmanY)!=0) {pacmanX -= speed; direction = M_PI;}
        if(keySpecialStates[GLUT_KEY_RIGHT] && checkColision(pacmanX+speed, pacmanY)!=0) {pacmanX += speed; direction = 0;}
    }else{
        glColor3ub(255,255,255);
        
        type("Game Over", window_height/2, window_width/2-70, 2);
    }

    glutSwapBuffers();
}

void specialKeyPressed(int key, int x, int y) {keySpecialStates[key] = true; }
void specialKeyReleased(int key, int x, int y){ keySpecialStates[key] = false; }

int main(int argc, char** argv)
{
 glutInit(&argc,argv);
 glutInitWindowPosition(350, 0);
 glutInitWindowSize(window_width, window_height+70); // window_height -> window_height+50
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 glutCreateWindow("OpenGL Pacman");
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D(0,window_width,window_height,-70);        // 0 -> -50
 glMatrixMode(GL_MODELVIEW);

 glutDisplayFunc(display);
 //keyboard
 glutSpecialFunc(specialKeyPressed);
 glutSpecialUpFunc(specialKeyReleased);

 glutTimerFunc(1000.0 / 60.0, timer, 0);
 glutMainLoop();

 return 0;
}
