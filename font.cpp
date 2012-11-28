
#include <gl/gl.h>
#include <windows.h>
#include "font.h"

extern HDC hDC;

font::font()
{
    setc(1.0, 1.0, 1.0);
    setsc(1.0, 1.0, 0.0);
}

font::font(double cr, double cg, double cb, double csr, double csg, double csb)
{
    setc(cr, cg, cb);
    setsc(csr, csg, csb);
}

font::~font()
{
    shutdown();
}

void font::setc(double cr, double cg, double cb)
{
    r = cr;
    g = cg;
    b = cb;
}

void font::setsc(double csr, double csg, double csb)
{
    sr = csr;
    sg = csg;
    sb = csb;
}

void font::setcolour(double r, double g, double b)
{
    red = r;
    green = g;
    blue = b;
}

void font::init(int size, char *font)
{		
	m_fontListBase = CreateBitmapFont(font, size);
    deselect();
}

void font::shutdown()
{
	ReleaseFont(m_fontListBase);
}

void font::select(){
    selected = true;
    setcolour(sr, sg, sb);
}

void font::deselect(){
    selected = false;
    setcolour(r, g, b);
}

void font::render(float xpos, float ypos, char* string)
{
    glColor3f(red, green, blue);
    RenderFont(xpos, ypos, m_fontListBase, (char*)string);
}

unsigned int font::CreateBitmapFont(char *fontName, int fontSize)
{
	HFONT hFont;     // the windows font
	unsigned int base;

	base = glGenLists(96);

	if (!stricmp(fontName, "symbol"))
	{
		hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
							SYMBOL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
							ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
							fontName);
	}
	else
	{
		hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
							ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
							ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
							fontName);
	}

	if (!hFont)
		return 0;

	SelectObject(hDC, hFont);
	wglUseFontBitmaps(hDC, 32, 96, base);

	return base;
}

void font::ReleaseFont(unsigned int base)
{
	if (base != 0)
		glDeleteLists(base, 96);
}

void font::RenderFont(float xPos, float yPos, unsigned int base, char *str)
{
	if ((base == 0) || (!str))
		return;

	glRasterPos2f(xPos, yPos);

	glPushAttrib(GL_LIST_BIT);
		glListBase(base - 32);
		glCallLists((int)strlen(str), GL_UNSIGNED_BYTE, str);
	glPopAttrib();
}
