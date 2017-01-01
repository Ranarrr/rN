#ifndef _DRAWING_H_
#define _DRAWING_H_

#include "main.hpp"

class CDrawing {
private:
	CDrawing();
public:
	virtual ~CDrawing();
	static CDrawing*	Get();

	void TintRGBA( int x, int y, int w, int h, Color col );
	void DrawRectangle( int x, int y, int w, int h, Color col );
	void DrawBorderBox( float x, float y, float w, float h, float t, Color col );
	void DrawBorderBoxOutlined( float x, float y, float w, float h, float t, Color col, Color out );
	void DrawRectangleOutlined( float x, float y, float w, float h, Color col, Color out );

	int GetStringLenght( const char *fmt, ... );
	int GetStringHeight( void );
	void DrawString( int x, int y, Color col, const char *fmt, ... );
	void DrawStringCenter( int x, int y, Color col, const char *fmt, ... );
};

#endif // DRAWING_H