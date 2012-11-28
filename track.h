class CTrack{
    private:
      double l[8];
      double r[8];
      int dist;
      int width;
      int diff;
      double frame;
      int speed;
      double last;
      
    public:
      void newtrack();
      void newpos();
      void render(int mc);
      void difficulty(double dist);
      void setlevel(int d);
      int getdiff();
      int getwidth();
      double getspeed();
      double* getl();
      double* getr();
};
