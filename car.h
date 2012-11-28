class CCar{
    private:
      double x,y;
      double xvel;
      double width,height;
      bool dead;
      
    public:
      void init();
      void move();
      void death(double* l, double* r);
      void render();
      bool isdead();
      void reset();
};
