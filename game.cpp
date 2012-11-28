#include <windows.h>
#include <gl/gl.h>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;
#include "game.h"
#include "car.h"
#include "track.h"
#include "keys.h"
#include "font.h"

extern HDC hDC;
extern CKeys* keys;

double framerate;

CGame::CGame(){
    frame = 2;
    state = 0;
    car = new CCar();
    track = new CTrack();
    srand (time (NULL));
    mc = 0;
    name = 0;
    framerate = 0;
    strcpy(title, "CARZ");
    strcpy(subtitle, "CR4SH1NG N0T 4LL0W3D");
    
    for(int i = 0; i < 10; i++)
        hss[i] = 0;
    FILE* hscores;
    hscores = fopen("hs.bin", "rb");
    if(hscores != NULL)
    {
        for(int i = 0; i < 10; i++)
        {
            char buffer[7];
            fread(buffer, sizeof(char), 7, hscores);
            fread(hsn[i], sizeof(char), 11, hscores);
            if(feof(hscores))
                break;
            hss[i] = decrypt(buffer);
            for(int j = 0; j < 11; j++)
            {
                if(hsn[i][j] == (char)126)
                {
                    hsn[i][j] = '\0';
                    break;
                }
                hsn[i][j] += (i+1) * 3;
            }
        }
        fclose(hscores);
    }
}

CGame::~CGame()
{
    FILE* hscores;
    hscores = fopen("hs.bin", "wb");
    if(hscores != NULL)
    {
    for(int i = 0; i < 10; i++)
    {
        if(hss[i] > 0)
        {
            char buffer[7];
            int len = 0;
            for(int l = 6; l > 0; l--)
            {
                if(hss[i] % (int)round(pow(10.0, l)) != 0)
                {
                   len = l;
                   break;
                }
            }
            for(int j = 6; j >= 0; j--)
            {
                if(j < len)
                {
                    char temp[2];
                    sprintf(temp, "%d", hss[i]%10);
                    buffer[j] = temp[0];
                    buffer[j] += (j+1) * 10;
                    hss[i] /= 10;
                }
                else if(j == len)
                    buffer[j] = (char)47;
                else
                    buffer[j] = rand() % 94 + 33;
            }
            fwrite(buffer, sizeof(char), 7, hscores);
            for(int j = 10; j >= 0; j--)
            {
                if(j < strlen(hsn[i]))
                {
                    hsn[i][j] -= (i+1) * 3;
                }
                else if(j == strlen(hsn[i]))
                    hsn[i][j] = (char)126;
                else
                    hsn[i][j] = rand() % 94 + 33;
            }
            fwrite(hsn[i], sizeof(char), 11, hscores);
        }
    }
    fclose(hscores);
    }
}

int CGame::decrypt(char *buf)
{
    for(int i=0; i < 7; i++)
    {
        if(buf[i] == (char)47)
        {
            buf[i] = '\0';
            break;
        }
        buf[i] -= (i+1) * 10;
    }
    return atoi(buf);
}

void CGame::loop()
{
    start = clock();
    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    if(keys->getkey('M') < 0)
        mc = !mc;
    
    if(frame == 1)
    {
        if(state==0){
            load();
        }else if(state==1){
            move();
            hittest();
            render();
            text();
            update();
        }else if(state==2){
            gameover();
        }else if(state==3){
            reset();
        }
    }
    else if(frame == 2)
    {
        if(state == 0){
            loadMenu();
        }
        else if(state == 1)
        {
            menu();
        }
        else if(state == 2)
        {
            endMenu();
        }
    }
    keys->reset();
    SwapBuffers (hDC);
    end = clock();
    if(end - start > 0.0)
        framerate = 17.0/(end-start);
    else
    {
        framerate = 1.0;
        Sleep(1);
    }
}

void CGame::loadMenu()
{
    for(int i = 0; i < 3; i++)
        fonts.push_back(new font());
    fonts[0]->init(48, "Courier New");
    fonts[0]->select();
    fonts[1]->init(36, "Courier New");
    fonts[1]->setc(0.0, 1.0, 0.0);
    fonts[1]->deselect();
    fonts[2]->init(24, "Courier New");
    state = 1;
    lev = 0;
}

void CGame::menu()
{
    fonts[0]->render(-0.90, 0.7, title);
    fonts[2]->render(-0.85, 0.6, subtitle);
    fonts[2]->render(-0.80, 0.2, "Play -> Press 'P'");
    fonts[2]->render(-0.80, 0.1, "Quit -> Press ESC");
    
    fonts[2]->render(-0.95, -0.35, "Move Car  -> Left/Right Arrow Keys");
    fonts[2]->render(-0.95, -0.45, "Car Speed ->   Up/Down  Arrow Keys");
    if(hss[0] > 150000)
        fonts[2]->render(-0.95, -0.55, "Multi-Colour Game -> 'M'");
    
    fonts[1]->render(0.30, 0.52, "High Scores");
    
    char string[7];
    for(int i = 0; i < 10; i++)
    {
        sprintf(string, "%d.", i+1);
        fonts[2]->render(0.22, 0.40 - i/9.7, string);
        if(hss[i] > 0)
        {
            fonts[2]->render(0.325, 0.40 - i/9.7, hsn[i]);
            sprintf(string, "%d", hss[i]);
            fonts[2]->render(0.70, 0.40 - i/9.7, string);
        }    
    }
    
    glBegin(GL_LINES);
      glVertex3f(0.20, 0.46, 1);
      glVertex3f(0.90, 0.46, 1);
      /*glVertex3f(0.33, 0.46, 1);
      glVertex3f(0.33, -0.6, 1);
      glVertex3f(0.69, 0.46, 1);
      glVertex3f(0.69, -0.6, 1);*/
      
      glVertex3f(0.2, 0.6, 1);
      glVertex3f(0.2, -0.6, 1);
      glVertex3f(0.2, 0.6, 1);
      glVertex3f(0.9, 0.6, 1);
      glVertex3f(0.9, 0.6, 1);
      glVertex3f(0.9, -0.6, 1);
      glVertex3f(0.9, -0.6, 1);
      glVertex3f(0.2, -0.6, 1);
    glEnd();
    
    if(keys->getkey('L') < 0)
        if(keys->getkey('0') < 0)
            lev = 10;
    if(keys->getkey('P') < 0)
        state = 2;
}

void CGame::endMenu()
{
    fonts.clear();
    state = 0;
    frame = 1;
}

void CGame::load()
{
    car->init();
    track->newtrack();
    if(lev == 10)
    {
        dist = 10000;
        score = 0;
        track->setlevel(dist);
    }
    else
    {
        dist = 0;
        score = 0;
    }
    
    for(int i = 0; i < 3; i++)
        fonts.push_back(new font());
    fonts[0]->init(48, "Courier New");
    fonts[0]->select();
    fonts[1]->init(36, "Courier New");
    fonts[1]->setc(0.0, 1.0, 0.0);
    fonts[1]->deselect();
    fonts[2]->init(24, "Courier New");
    
    state = 1;
}

void CGame::text()
{
    fonts[0]->render(-0.90, 0.7, title);
    fonts[2]->render(-0.85, 0.6, subtitle);
    
    fonts[2]->render(0.2, 0.7, "Level:");
    fonts[2]->render(0.2, 0.6, "Score:");
    
    char string[9];
    sprintf(string, "%d", track->getdiff());
    fonts[2]->render(0.4, 0.7, string);
    sprintf(string, "%d", (int)score);
    fonts[2]->render(0.4, 0.6, string);
}

void CGame::move()
{
    car->move();
    track->newpos();
}

void CGame::hittest(){
    car->death(track->getl(), track->getr());
}

void CGame::render(){
    track->render(mc);
    car->render();
}

void CGame::update(){
    if(car->isdead()){
        state = 2;
    }
    track->difficulty(dist);
    dist += framerate;
    score += (track->getspeed() * track->getwidth()) * framerate;
}

void CGame::gameover(){
    track->render(0);
    car->render();
    
    fonts[0]->render(-0.90, 0.7, title);
    fonts[2]->render(-0.85, 0.6, subtitle);
    
    double xpos = ((track->getl()[4]/10.0-1) + (track->getr()[4]/10.0-1)) / 2.0;
    fonts[1]->render(xpos - 0.2, 0.08, "GAME OVER");
    
    char string[9];
    
    xpos = ((track->getl()[3]/10.0-1) + (track->getr()[3]/10.0-1)) / 2.0;
    fonts[2]->render(xpos - 0.1, -0.06, "Level:");
    sprintf(string, "%d", track->getdiff());
    fonts[2]->render(xpos + 0.1, -0.06, string);
    
    xpos = ((track->getl()[2]/10.0-1) + (track->getr()[2]/10.0-1)) / 2.0;
    fonts[2]->render(xpos - 0.2, -0.2, "Score:");
    sprintf(string, "%d", (int)score);
    fonts[2]->render(xpos, -0.2, string);
    
    fonts[2]->render(-0.7, -0.65, "Submit Score -> Press 'S'");
    fonts[2]->render(-0.7, -0.75, "Play Again   -> Press 'P'");
    fonts[2]->render(-0.7, -0.85, "Main Menu    -> Press 'B'");
    fonts[2]->render(0.20, -0.75, "Quit -> Press ESC");
    
    if(!name)
    {
        if(keys->getkey('P') < 0)
            state = 3;
        if(keys->getkey('B') < 0)
        {
            fonts.clear();
            frame = 2;
            state = 0;
        }
        if(keys->getkey('S') < 0)
        {
            fonts[1]->setsc(1.0, 1.0, 1.0);
            strcpy(inputName, "");
            name = 1;
        }
    }
    else
    {
        entername();
    }
}

void CGame::reset(){
    car->reset();
    track->newtrack();
    dist = 0;
    state = 1;
    score = 0;
}

void CGame::entername()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 0.85);
    glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(-1, -1, 1);
      glVertex3f(-1, 1, 1);
      glVertex3f(1, -1, 1);
      glVertex3f(1, 1, 1);
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    
    fonts[1]->select();
    fonts[1]->render(-0.4, 0.2, "ENTER YOUR NAME");
    fonts[1]->deselect();
    fonts[2]->render(-0.58, 0.0, "Hit ENTER when you have finished");
    nameKeyInput();
    fonts[1]->render(-length/40.0, -0.2, inputName);
    if(keys->getkey(VK_RETURN) == -1)
    {
        submitScore();
        name = 0;
        fonts.clear();
        frame = 2;
        state = 0;
    }
}

void CGame::nameKeyInput()
{
    if(length < 10)
    {
        for(int key = 'A'; key <= 'Z'; key++)
        {
            if(keys->getkey(VK_SHIFT) == 0)
                keyCheck(key, key + 32);
            else
                keyCheck(key);
        }
        for(int key = '0'; key <= '9'; key++)
        {
            keyCheck(key);
        }
        keyCheck(VK_SPACE);
    }
    if(length > 0)
        if(keys->getkey(VK_BACK) == -1)
        {
            inputName[length-1] = '\0';
            length--;
        }
}

void CGame::keyCheck(int key, int out)
{
    if(keys->getkey(key) == -1)
    {
        char temp[2];
        if(out == -1)
            temp[0] = (char)key;
        else
            temp[0] = (char)out;
        temp[1] = '\0';
        strcat(inputName, temp);
        length++;
    }
}

void CGame::submitScore()
{
    for(int i = 0; i < 10; i++)
    {
        if(score > hss[i])
        {
            for(int j = 9; j > i; j--)
            {
                hss[j] = hss[j-1];
                strcpy(hsn[j], hsn[j-1]);
            }
            hss[i] = (int)score;
            strcpy(hsn[i], inputName);
            break;
        }
    }
}
