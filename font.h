class font
{
private:
	unsigned int m_fontListBase;
    bool selected;
    double red, green, blue;
    double r,g,b;
    double sr, sg, sb;
    
	unsigned int CreateBitmapFont(char *fontName, int fontSize);
	void RenderFont(float xPos, float yPos, unsigned int base, char *str);
	void ReleaseFont(unsigned int base);

public:
	font();
	font(double r, double g, double b, double, double, double);
	virtual ~font();

    void setc(double, double, double);
    void setsc(double, double, double);
    void setcolour(double r, double g, double b);
	void init(int size, char *font);
	void shutdown();
    void select();
    void deselect();
	void render(float xpos, float ypos, char* string);
};
