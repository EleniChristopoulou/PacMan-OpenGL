#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;
std::queue<int> dir_ghost1, dir_ghost2;
queue<pair<int,int>> squares_left;
int counterMov=1;
    
char message[60];
int score=0;
int num_of_lives = 3;

float square_size = 30, map_width = 21, map_height = 23;
float window_width = map_width*square_size, window_height = map_height*square_size;
    int counter = 0;
//Ghost Parametres
float ghost1X = 9*square_size, ghost1Y = 11*square_size;
float ghost2X = 9*square_size, ghost2Y = 10*square_size;

int len_ghost = 78;
float body_periemtre_ghost[78]= {0,0, 0,8,  1,8, 1,11
                    ,2,11, 2,12,  3,12, 3,13
                    ,5,13, 5,14,  7,14, 8,14
                    ,8,13, 10,13,  10,12, 11,12
                    ,11,11, 12,11,  12,8, 13,8

                    ,13,0, 13,0,  13,1, 12,1
                    ,12,2, 11,2,  11,1, 10,1
                    ,10,0, 8,0,  8,2, 6,2
                    ,6,0, 4,0,  4,1, 3,1
                    ,3,2, 2,2,  2,1};

//Pacman Parameters
float pacmanX = 10*square_size+square_size/2, pacmanY = 21*square_size+square_size/2, speed = 3, dx = 1, dy = 1;
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

void ghost_body(float x_cord, float y_cord){
    float x = 0, y = 0;
    
    glBegin(GL_LINE_LOOP);        

    for(int i=0; i<len_ghost; i=i+2){
        x = body_periemtre_ghost[i];
        y = body_periemtre_ghost[i+1];
        glVertex2f(2*x+1+x_cord,2*(15-1 - y)+y_cord); 
    } 

    glEnd();
}

void ghost_eyes(float x_cord, float y_cord){
    glColor3ub(255,255,255);
    glBegin(GL_POLYGON);       //left eye 
    float x=4 ,y=4;
    glVertex2f(2*x+1+x_cord,2*y + y_cord); 
    glVertex2f(2*(x+2)+1+x_cord ,2*y + y_cord); 

    glVertex2f(2*(x+2)+1+x_cord ,2*(y+3)+y_cord); 
    glVertex2f(2*x+1+x_cord,2*(y+3)+y_cord);
    glEnd();

    glBegin(GL_POLYGON);        //right eye
    x=14-6;
    glVertex2f(2*x+1 + x_cord,2*y + y_cord); 
    glVertex2f(2*(x+2)+1 + x_cord,2*y + y_cord); 

    glVertex2f(2*(x+2)+1 + x_cord,2*(y+3) + y_cord); 
    glVertex2f(2*x+1 + x_cord,2*(y+3) + y_cord);
    glEnd();
}

void drawGhost(int r, int g, int b, float x_cord, float y_cord){
    /*Creating Black Box To Hide Points, (if background is changed it will need to be changed too!)*/
        glColor3ub(0,0,0);        

        glBegin(GL_POLYGON);        
        glVertex2f(x_cord, y_cord); 
        glVertex2f(x_cord + square_size-1, y_cord); 

        glVertex2f(x_cord + square_size-1, y_cord + square_size-1); 
        glVertex2f(x_cord, y_cord + square_size-1);
        glEnd();
    /*end of balckbox creation*/

    glColor3ub(r,g,b);     //Ghost Color
    ghost_body(x_cord, y_cord);
    ghost_eyes(x_cord, y_cord);
}

void pacmanGhostColission(){   //more about checking collision with pacman 
    /*Check Colision With Ghost 1*/
        float x_diff = fabsl(pacmanX-square_size/2-ghost1X); 
        float y_diff = fabsl(pacmanY-square_size/2-ghost1Y);

        if(x_diff<30 && y_diff<30){
            printf("Gotcha By Ghost 1");
            num_of_lives--;
            pacmanX = 10*square_size+square_size/2;
            pacmanY = 21*square_size+square_size/2;

            ghost1X = 9*square_size, ghost1Y = 11*square_size;
            ghost2X = 9*square_size, ghost2Y = 10*square_size;

            while (!dir_ghost1.empty()) {
                dir_ghost1.pop();
            }

            while (!dir_ghost2.empty()) {
                dir_ghost2.pop();
            }

            counterMov = 1;
        }

    /*Check Colision With Ghost 2*/
        x_diff = fabsl(pacmanX-square_size/2-ghost2X); 
        y_diff = fabsl(pacmanY-square_size/2-ghost2Y);
    
        if(x_diff<30 && y_diff<30){
            printf("Gotcha By Ghost 2");
            num_of_lives--;
            pacmanX = 10*square_size+square_size/2;
            pacmanY = 21*square_size+square_size/2;

            ghost1X = 9*square_size, ghost1Y = 11*square_size;
            ghost2X = 9*square_size, ghost2Y = 10*square_size;

            while (!dir_ghost1.empty()) {
                dir_ghost1.pop();
            }

            while (!dir_ghost2.empty()) {
                dir_ghost2.pop();
            }

            counterMov = 1;
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
    glTranslatef(x, y, 0);
    glScalef(0.1 * s, 0.1 * -s, 0);
    for (c = str; *c != '\0'; c++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    glScalef(1, 1, 1);
    glPopMatrix();
}

void drawHeart(float square_num){
    float xc = square_size* square_num, yc = - 35;
    glColor3ub(255, 0, 0);

    glBegin(GL_POLYGON);
    glVertex2f(xc,yc);
    for(float theta=0; theta<=M_PI; theta=theta+M_PI/10){
        glVertex2f(xc + square_size/4*cos(theta) , yc - square_size/4*sin(theta));
    }
    glEnd(); 

    xc += square_size/2-1;
    glBegin(GL_POLYGON);
    glVertex2f(xc,yc);
    for(float theta=0; theta<=M_PI; theta=theta+M_PI/10){
        glVertex2f(xc + square_size/4*cos(theta) , yc - square_size/4*sin(theta));
    }
    glEnd(); 

    xc += square_size/4-1;
    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(xc,yc);
    glVertex2f(xc - square_size/2+1,yc + square_size/2);
    glVertex2f(xc - square_size + 2,yc);
    glEnd(); 

}

void moveGhost(int ghost, int move){
    if(ghost == 1){
        switch (move)
        {
        case 1:
            ghost1X--;
            break;
        case 2:
            ghost1Y--;
            break;
        case 3:
            ghost1X++;
            break;
        case 4:
            ghost1Y++;
            break;
        default:
            break;
        }
    }

    if(ghost == 2){
        switch (move)
        {
        case 1:
            ghost2X--;
            break;
        case 2:
            ghost2Y--;
            break;
        case 3:
            ghost2X++;
            break;
        case 4:
            ghost2Y++;
            break;
        default:
            break;
        }
    }
}

void setNeighbours(float dist_map[23][21], int x, int y){
    int x_neigh = x - 1, y_neigh = y ;
    if(x_neigh > -1){           //LEFT NEIGHBOUR
        //not out of bounds
        if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
            //means has no value yet, and its not a wall 
            dist_map[y_neigh][x_neigh] = dist_map[y][x] + 1;   //increase it by 1
            squares_left.push({y_neigh, x_neigh});
        }
    }

    x_neigh = x + 1;
    if(x_neigh < 21){           //RIGHT NEIGHBOUR
        //not out of bounds
        if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
            //means has no value yet, and its not a wall 
            dist_map[y_neigh][x_neigh] = dist_map[y][x] + 1;   //increase it by 1
            squares_left.push({y_neigh, x_neigh});
        }
    }

    x_neigh = x, y_neigh = y - 1;
    if(y_neigh > -1){           //UP NEIGHBOUR
        //not out of bounds
        if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
            //means has no value yet, and its not a wall 
            dist_map[y_neigh][x_neigh] = dist_map[y][x] + 1;   //increase it by 1
            squares_left.push({y_neigh, x_neigh});
        }
    }

    y_neigh = y + 1;
    if(y_neigh < 23){           //DOWN NEIGHBOUR
        //not out of bounds
        if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
            //means has no value yet, and its not a wall 
            dist_map[y_neigh][x_neigh] = dist_map[y][x] + 1;   //increase it by 1
            squares_left.push({y_neigh, x_neigh});
        }
    }
    
}

std::queue<int> getDircetions(int start_x, int start_y, int end_x, int end_y){
    std::queue<int> directions;

    if(start_x == end_x && start_y == end_y){
        directions.push(-1);
        return directions;
    }
    float dist_map[23][21];

    for (int i = 0; i < 23; ++i) {      //initialising everythin to 100.
        for (int j = 0; j < 21; ++j) {
            dist_map[i][j] = 100;
        }
    } 

    dist_map[end_y][end_x] = 0;
    squares_left.push({end_y, end_x});

    while (!squares_left.empty()){
        pair<int,int> square = squares_left.front();
        // setNeighbours(dist_map, square.second, square.first);
        int x_neigh = square.second - 1, y_neigh = square.first ;
        if(x_neigh > -1){           //LEFT NEIGHBOUR
            //not out of bounds
            if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
                //means has no value yet, and its not a wall 
                dist_map[y_neigh][x_neigh] = dist_map[square.first][square.second] + 1;   //increase it by 1
                squares_left.push({y_neigh, x_neigh});
            }
        }

        x_neigh = square.second + 1;
        if(x_neigh < 21){           //RIGHT NEIGHBOUR
            //not out of bounds
            if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
                //means has no value yet, and its not a wall 
                dist_map[y_neigh][x_neigh] = dist_map[square.first][square.second] + 1;   //increase it by 1
                squares_left.push({y_neigh, x_neigh});
            }
        }

        x_neigh = square.second, y_neigh = square.first - 1;
        if(y_neigh > -1){           //UP NEIGHBOUR
            //not out of bounds
            if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
                //means has no value yet, and its not a wall 
                dist_map[y_neigh][x_neigh] = dist_map[square.first][square.second] + 1;   //increase it by 1
                squares_left.push({y_neigh, x_neigh});
            }
        }

        y_neigh = square.first + 1;
        if(y_neigh < 23){           //DOWN NEIGHBOUR
            //not out of bounds
            if((dist_map[y_neigh][x_neigh]==100) && (map[y_neigh][x_neigh]!=0)){
                //means has no value yet, and its not a wall 
                dist_map[y_neigh][x_neigh] = dist_map[square.first][square.second] + 1;   //increase it by 1
                squares_left.push({y_neigh, x_neigh});
            }
        }

        squares_left.pop();
    }

    float value=100;
    int direction = 0;
    int x = start_x, y = start_y, x_check = start_x, y_check = start_y;

    while (value!=0){
        if(value > dist_map[y][x - 1]) {     //left
            value = dist_map[y][x - 1];
            x_check = x-1;
            y_check = y;
            direction = 1;
        }

        if(value > dist_map[y][x + 1]) {     //right
            value = dist_map[y][x + 1];
            y_check = y;
            x_check = x+1;
            direction = 3;
        }

        if(value > dist_map[y - 1][x]) {     //up
            value = dist_map[y - 1][x];
            x_check = x;
            y_check = y - 1;
            direction = 2;
        }

        if(value > dist_map[y + 1][x]) {     //down
            value = dist_map[y + 1][x];
            x_check = x;
            y_check = y + 1;
            direction = 4;
        }

        directions.push(direction);

        y = y_check;
        x = x_check;
    }

    return directions;
} 
//only for debugging purposes
void showq(queue<int> gq)
{
    queue<int> g = gq;
    while (!g.empty()) {
        cout << ' ' << g.front();
        g.pop();
    }
    cout << '\n';
}

void display(){

    glClearColor(0.0f,0.0f,0.0f, 0.0f);   //background color
    glClear(GL_COLOR_BUFFER_BIT);

    if(getPoints()!=0 && num_of_lives!=0){
        drawMap();
        drawPoints();

        drawPacMan(pacmanX,pacmanY);
        pacmanEat(); 

        drawGhost(255,165,0, ghost1X, ghost1Y);     //Inky
        drawGhost(0,165,255, ghost2X, ghost2Y);     //Clyde

        if(dir_ghost1.size() == 0){         
            dir_ghost1 = getDircetions(ceil(ghost1X/square_size),
            ceil(ghost1Y/square_size), 5,2);    
        }else{  
            moveGhost(1, (int) dir_ghost1.front());
            if((counterMov%((int)square_size))==0){
                dir_ghost1.pop();
            }
        }

        if(dir_ghost2.size() == 0){                 //move Clude
            dir_ghost2 = getDircetions(ceil(ghost2X/square_size),
            ceil(ghost2Y/square_size), 5,2);  
        }else{  
            moveGhost(2, (int) dir_ghost2.front());
            if((counterMov%((int)square_size))==0){
                dir_ghost2.pop();
            }
            counterMov ++;
        }

        pacmanGhostColission();

        char str[] = "Points: ";            //Add score
        glColor3ub(255,255,255);;         
        sprintf(message, "%d", (220-getPoints())*10);
        strcat(str, message);
        strcat(str, "/2200");
        type(str, 10, -25, 2);

        type("Lives: ", 30*15, -25, 2);
        for(int i=0; i<num_of_lives; i++){
            drawHeart(16.5 + (float) (i+1));
        }

        if(keySpecialStates[GLUT_KEY_UP] && checkColision(pacmanX, pacmanY-speed)!=0 ) {pacmanY -= speed; direction = 3*M_PI/2;}
        if(keySpecialStates[GLUT_KEY_DOWN] && checkColision(pacmanX, pacmanY+speed)!=0) {pacmanY += speed; direction = M_PI/2;}
        if(keySpecialStates[GLUT_KEY_LEFT] && checkColision(pacmanX-speed, pacmanY)!=0) {pacmanX -= speed; direction = M_PI;}
        if(keySpecialStates[GLUT_KEY_RIGHT] && checkColision(pacmanX+speed, pacmanY)!=0) {pacmanX += speed; direction = 0;}

    }else{
        glColor3ub(255,255,255);
        
        if(num_of_lives==0){    //Game Over
            type("Game Over", window_height/2, window_width/2, 2);
        }else{                  //Player Wins
            type("You Won!", window_height/2, window_width/2-70, 2);
        }
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
