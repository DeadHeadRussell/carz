#include <ctime>

class CCar;
class CTrack;
class font;
class CGame{
    private:
      double dist;
      int state, frame;
      int mc;
      int lev;
      char hsn[10][11];
      int hss[10];
      int name, length;
      char inputName[11];
      char title[8];
      char subtitle[24];
      double score;
      clock_t start, end;
      
      vector<font*> fonts;
      
      int decrypt(char *buf);
      void move();
      void hittest();
      void render();
      void update();
      void load();
      void text();
      void gameover();
      void reset();
      void loadMenu();
      void menu();
      void endMenu();
      void entername();
      void nameKeyInput();
      void keyCheck(int key, int out = -1);
      void submitScore();
      
      CCar *car;
      CTrack *track;
    public:
      CGame();
      ~CGame();
      void loop();
};
