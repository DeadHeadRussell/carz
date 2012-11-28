#include <windows.h>
#include <gl/gl.h>
#include "car.h"
#include "keys.h"

extern CKeys* keys;
extern double framerate;

void CCar::init(){
    x = 0;
    y = -0.5 + (1.0/14);
    xvel = .0125;
    width = 0.02;
    height = 0.04;
    dead = false;
}

void CCar::move(){
    if(keys->getkey(VK_UP))
        xvel += 0.0005 * framerate;
    if(keys->getkey(VK_DOWN))
        xvel -= 0.0005 * framerate;
    if(xvel <= 0)
        xvel = 0.0005;
    if(keys->getkey(VK_LEFT))
        x -= xvel * framerate;
    if(keys->getkey(VK_RIGHT))
        x += xvel * framerate;
}

void CCar::death(double* l, double* r){
    if((x-width/2 < ((l[0]/10-1 + l[1]/10-1)/2.0)) || (x+width/2 > ((r[0]/10-1 + r[1]/10-1)/2.0)))
    {
        dead = true;
    }
}

void CCar::render(){
    glPushMatrix();
    glColor3f(1.0,0.0,1.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(x - width/2.0, y - height/2.0, 1);
    glVertex3f(x - width/2.0, y + height/2.0, 1);
    glVertex3f(x + width/2.0, y - height/2.0, 1);
    glVertex3f(x + width/2.0, y + height/2.0, 1);
    glEnd();
    glPopMatrix();
}

bool CCar::isdead(){
    return dead;
}

void CCar::reset(){
    x = 0;
    xvel = .0125;
    dead = false;
}
