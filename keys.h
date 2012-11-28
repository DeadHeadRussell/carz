class CKeys{
    private:
      int keys[256];
      
    public:
      void setkey(int key, int pos);
      int getkey(int key);
      void reset();
};
