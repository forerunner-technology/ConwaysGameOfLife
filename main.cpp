#include <iostream>
#include <GL/glut.h>

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string.h>

#include <unistd.h>  // maybe a sleep timer before implement the rules say for half a sec linux only at the moment

using namespace std;

int initCatcher = 0;

struct cube {
    int x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;
    // color of a dead cell
    float r = 0, g = 0, b = 0;
    int state;
    int total = 1;
};

cube c[9800];
cube tmp[9800];

// don't want to make truly random, need cubes to land in one area in a pattern, in a 100x100 area we should pick
// two 10x10 blocks and make it so that of those 100 spaces 80 of them are filled at random, we can then tweak this latergithu
int random(int min, int max) //range : [min, max]
{
    static bool first = true;
    if (first)
    {
        srand( time(NULL) ); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max ) - min);
}
/*initializes an array of empty cubes on the grid*/
void addCube() {
    // statement for a random number in range of the grid, then everyone else pulls off of it with c[cn].x1 being equal to it
    // y is always zero to keep the "cube" just a box on the grid
    int k = 0;
    int i = 0;

    while(i < 99) {
        int j=0;
        while(j < 99){
            c[k].state = 1;
            if(c[k].state > 4) {c[k].state = 1;}
            int st = c[k].state;
            if(st == 1) {c[k].total++;}
            if(st == 1) {c[k].x1 = i; c[k].y1 = 0; c[k].z1 = j;}
            if(st == 1 || st == 2) {c[k].x2 = c[k].x1; c[k].y2 = 0; c[k].z2 = c[k].z1 + 1;}
            if(st == 1 || st == 2 || st == 3) {c[k].x3 = c[k].x1 + 1; c[k].y3 = 0; c[k].z3 = c[k].z1 + 1;}
            if(st == 1 || st == 2 || st == 3 || st == 4) {c[k].x4 = c[k].x1 + 1; c[k].y4 = 0; c[k].z4 = c[k].z1;}
            //cout << c[k].x1 << " " << c[k].z1 << " " << k << endl;
            j++;
            k++;
        }
        i++;
    }
}
/* selects some cubes at random then turns them green*/
void randomInit(){
    for (int j = 0; j < 75; j++) {
        int origx = random(20,30);
        int origz = random(20,30);

        for (int i =0; i < 9800; i++){
            if((c[i].x1 == origx) && (c[i].z1 == origz)) {
                // color of live cells is based on what goes in this loop
                c[i].g = 1;
            }
        }
    }
}

void drawCube() {
    int i;
    for (i = 0;i < 9800; i++){
        glBegin(GL_QUADS);
        glColor3f(c[i].r, c[i].g, c[i].b);
        glVertex3f(c[i].x1, c[i].y1, c[i].z1);
        glVertex3f(c[i].x2, c[i].y2, c[i].z2);
        glVertex3f(c[i].x3, c[i].y3, c[i].z3);
        glVertex3f(c[i].x4, c[i].y4, c[i].z4);
        glEnd();
    }
}

void drawGrid(){
    int i;

    for (i=0;i<200;i++){
        glPushMatrix();
        if (i< 100){glTranslatef(0,0,i);}
        if (i>=100){glTranslatef(i-100, 0, 0); glRotatef(-90,0,1,0);}

        glBegin(GL_LINES);
        glColor3f(1,1,1);glLineWidth(1);
        glVertex3f(0,-0.1,0); glVertex3f(99,-0.1,0);
        glEnd();
        glPopMatrix();
    }
}

//    Any live cell with two or three live neighbours survives.
//    Any dead cell with three live neighbours becomes a live cell.
//    All other live cells die in the next generation. Similarly, all other dead cells stay dead.

int checkForNeighbors(cube cube) {  //checks for live neighbors only!!!
    int numNeighbors = 0;
    for(int i=0; i<=9800; i++) {
        if(c[i].g == 1) {
            if((cube.x1 == c[i].x2) && (cube.z1 == c[i].z2)){ // Bottom Center
                numNeighbors ++;
            }
            if((cube.x1 == c[i].x3) && (cube.z1 == c[i].z3)){ // Bottom Left
                numNeighbors ++;
            }
            if((cube.x1 == c[i].x4) && (cube.z1 == c[i].z4)){
                numNeighbors ++;
            }
            if((cube.x2 == c[i].x4) && (cube.z2 == c[i].z4)){
                numNeighbors ++;
            }
            if((cube.x2 == c[i].x1) && (cube.z2 == c[i].z1)){
                numNeighbors ++;
            }
            if((cube.x3 == c[i].x1) && (cube.z3 == c[i].z1)){
                numNeighbors ++;
            }
            if((cube.x3 == c[i].x2) && (cube.z3 == c[i].z2)){
                numNeighbors ++;
            }
            if((cube.x4 == c[i].x2) && (cube.z4 == c[i].z2)){
                numNeighbors ++;
            }
        }
    }
    return numNeighbors;
}

void ruleOne(){
    for (int i=0; i<9800; i++){
        if(c[i].g == 1){
            int numNeighbors = checkForNeighbors(c[i]);
            if((numNeighbors == 2) || (numNeighbors == 3)){
                tmp[i].g = 1;
            }
            else {
                tmp[i].g = 0;
            }
        }
    }
}

void ruleTwo(){
    for (int i=0; i<9800; i++){
        if(c[i].g == 0){
            int numNeighbors = checkForNeighbors(c[i]);
            if((numNeighbors == 3)){
                tmp[i].g = 1;
                cout << numNeighbors << endl;
            }
        }
    }
}

void ruleThree(){
    for (int i=0; i<9800; i++){
        if(c[i].g == 1){
            int numNeighbors = checkForNeighbors(c[i]);
            if((numNeighbors < 2)){
                tmp[i].g = 0;
            }
        }
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-30, 0, -90);
    glRotatef(40,1,0,0);
    glRotatef(60,0,1,0);
    if (initCatcher < 1) {
        addCube();  // add initial array of black cubes
        randomInit();  // init a few green ones as a starting point
        initCatcher++;
    }
    drawCube();  // draw the current version of the array
    drawGrid();  // draw the grid
    glutSwapBuffers();
    sleep(2);
    memcpy(&tmp, &c, sizeof(c));
    ruleOne();  // apply rule 1
    ruleTwo(); // apply rule 2
    ruleThree(); //apply rule 3
    memcpy(&c, &tmp, sizeof(tmp));
}

void init(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1,1);
}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1080,720);
    glutCreateWindow("Conway's Game Of Life");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    init();
    glutMainLoop();
    return 0;
}
