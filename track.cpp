#include <cstdlib>
#include <gl/gl.h>
#include <cmath>
#include "track.h"

extern double framerate;

void CTrack::newtrack()
{
    for(int i=0;i<8;i++)
    {
        l[i] = 6;
        r[i] = 14;
    }
    width = 9;
    dist = 1;
    diff = 0;
    frame = 0;
    speed = 15;
    last = -1000;
}

void CTrack::newpos()
{
    if(frame >= speed)
    {
        for(int i=0;i<7;i++)
        {
            l[i] = l[i+1];
            r[i] = r[i+1];
        }
        
        if(l[6] > l[5])
        {
            if(rand() % 2 == 0)
                l[7] += rand() % 3 + 1;
        }
        else if(l[6] < l[5])
        {
            if(rand() % 2 == 0)
                l[7] += rand() % 3 - 3;
        }
        else
        {
            if(rand() % 2 == 0)
                if(l[7] == (19 - width))
                    l[7] += rand() % 3 - 3;
                else if(l[7] == 1)
                    l[7] += rand() % 3 + 1;
                else
                    l[7] += rand() % 7 - 3;
        }
        
        if(l[7] < 1)
            l[7] = 1;
        else if(l[7] > (19 - width))
            l[7] = (19 - width);
        
        r[7] = l[7]+width;
        
        frame = 0;
    }
    frame += framerate;
}

void CTrack::render(int mc){
    glPushMatrix();
    if(mc){
        glColor3f((rand()%11)/10.0,(rand()%11)/10.0,(rand()%11)/10.0);
        glBegin(GL_QUADS);
        glVertex3f(-1,-1,1);
        glVertex3f(-1,1,1);
        glVertex3f(1,1,1);
        glVertex3f(1,-1,1);
        glEnd();
    }else{
    glColor3f(1,1,1);
    }
    
    for(int i=0;i<7;i++){
        glBegin(GL_LINES);
        if(mc) glColor3f((rand()%11)/10.0,(rand()%11)/10.0,(rand()%11)/10.0);
        glVertex3f(l[i]/10-1,(double)i/7-.5,1);
        if(mc) glColor3f((rand()%11)/10.0,(rand()%11)/10.0,(rand()%11)/10.0);
        glVertex3f(l[i+1]/10-1,(double)(i+1)/7-.5,1);
        glEnd();
        glBegin(GL_LINES);
        if(mc) glColor3f((rand()%11)/10.0,(rand()%11)/10.0,(rand()%11)/10.0);
        glVertex3f(r[i]/10-1,(double)i/7-.5,1);
        if(mc) glColor3f((rand()%11)/10.0,(rand()%11)/10.0,(rand()%11)/10.0);
        glVertex3f(r[i+1]/10-1,(double)(i+1)/7-.5,1);
        glEnd();
    }    
    glPopMatrix();
}

void CTrack::difficulty(double dist){
    if(dist - last < 900)
        return;
    dist = ((int)(dist/10))*10;
    if((int)dist % 3000 == 0 && width > 3)
    {
        diff++;
        width--;
        last = dist;
    }
    else if(((int)dist % 3000 == 2000 || (int)dist % 3000 == 1000) && speed > 5)
    {
        diff++;
        speed--;
        last = dist;
    }
}

void CTrack::setlevel(int d)
{
    for(int a = 0; a < d; a++)
        difficulty(a);
}

int CTrack::getdiff()
{
    return diff;
}

int CTrack::getwidth()
{
    return 9 - width;
}

double CTrack::getspeed()
{
    return 16 - speed;
}

double* CTrack::getl(){
    return l;
}

double* CTrack::getr(){
    return r;
}
