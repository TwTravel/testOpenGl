//////////////////////////////////////////////////////////////////////////////////////////
//	BITMAP_FONT.h
//	Bitmap font class
//	Downloaded from: www.paulsprojects.net
//	Created:	13th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

class BITMAP_FONT
{
public:
	BITMAP_FONT()	:	startTextModeList(0), textBase(0)
	{}
	~BITMAP_FONT()	{}

	bool Init();
	void Shutdown()
	{	glDeleteLists(textBase, 96);	}

	//Text writing functions
	void StartTextMode(void);
	void Print(int x, int y, const char * string, ...);
	void EndTextMode(void);

protected:
	//Display lists for text
	GLuint startTextModeList, textBase;
};

#endif	//BITMAP_FONT_H
