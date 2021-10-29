#include <iostream>
#include <GL/glut.h>

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

int posx = 0, posy = 0, posz = 0;
int cn = 0;

struct cube {
    int x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;
    float r, g, b;
    int state;
    int total;
}; cube c[100];

// don't want to make truly random, need cubes to land in one area in a pattern, in a 100x100 area we should pick
// two 10x10 blocks and make it so that of those 100 spaces 80 of them are filled at random, we can then tweak this later
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

void addCube(int origx, int origz, int i) {
    c[i].state++; if(c[i].state > 4) {c[i].state = 1;}
    int st = c[0].state;
    // statement for a random number in range of the grid, then everyone else pulls off of it with c[cn].x1 being equal to it
    // y is always zero to keep the "cube" just a box on the grid

    if(st == 1) {c[i].total++; cn = c[i].total;}
    if(st == 1) {c[cn].x1 = origx; c[cn].y1 = 0; c[cn].z1 = origz;}
    if(st == 1 || st == 2) {c[cn].x2 = c[cn].x1; c[cn].y2 = 0; c[cn].z2 = c[cn].z1 + 1;}
    if(st == 1 || st == 2 || st == 3) {c[cn].x3 = c[cn].x1 + 1; c[cn].y3 = 0; c[cn].z3 = c[cn].z1 + 1;}
    if(st == 1 || st == 2 || st == 3 || st == 4) {c[cn].x4 = c[cn].x1 + 1; c[cn].y4 = 0; c[cn].z4 = c[cn].z1;}

}


void drawCube() {
    int i;
    for (i = 0;i < 5 + 1; i++){
        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        glVertex3f(c[i].x1, c[i].y1, c[i].z1);
        glVertex3f(c[i].x2, c[i].y2, c[i].z2);
        glVertex3f(c[i].x3, c[i].y3, c[i].z3);
        glVertex3f(c[i].x4, c[i].y4, c[i].z4);
        glEnd();
    }
}

void drawGrid(){
    int i;

    for (i=0;i<40;i++){
        glPushMatrix();
        if (i< 20){glTranslatef(0,0,i);}
        if (i>=20){glTranslatef(i-20, 0, 0); glRotatef(-90,0,1,0);}

        glBegin(GL_LINES);
        glColor3f(1,1,1);glLineWidth(1);
        glVertex3f(0,-0.1,0); glVertex3f(19,-0.1,0);
        glEnd();
        glPopMatrix();
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-13, 0, -45);
    glRotatef(40,1,1,0);
    for(int i = 0; i < 10; i++) {
        int origx = random(0, 19);
        int origz = random(0, 19);
        addCube(origx, origz, i);
        drawCube();
        cout << origx << endl;
        cout << origz << endl;
    }

    drawGrid();


    glutSwapBuffers();
}

void init(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1,1);
}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutCreateWindow("Conway's Game Of Life");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
