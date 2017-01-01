#include "drawing.hpp"

CDrawing::CDrawing() {
}

CDrawing::~CDrawing() {
}

CDrawing* CDrawing::Get() {
	static CDrawing sDrawing;
	return &sDrawing;
}

void CDrawing::TintRGBA( int x, int y, int w, int h, Color col ) {
	g_Engine.pfnTintRGBA( x, y, w, h, col.r(), col.g(), col.b(), col.a() );
}

void CDrawing::DrawRectangle( int x, int y, int w, int h, Color col ) {
	TintRGBA( x, y, w, h, col );
}

void CDrawing::DrawBorderBox( float x, float y, float w, float h, float t, Color col ) {
	DrawRectangle( x, y, w, t, col );
	DrawRectangle( x, y, t, h, col );
	DrawRectangle( x + w, y, t, h, col );
	DrawRectangle( x, y + h, w + t, t, col );
}

void CDrawing::DrawBorderBoxOutlined( float x, float y, float w, float h, float t, Color col, Color out ) {
	DrawBorderBox( x, y, w, h, t, col );
	DrawBorderBox( x - 1, y - 1, w + 2, h + 2, 1, out );
	DrawBorderBox( x + 1, y + 1, w - 2, h - 2, 1, out );
}

void CDrawing::DrawRectangleOutlined( float x, float y, float w, float h, Color col, Color out ) {
	DrawRectangle( x, y, w, h, col );
	DrawBorderBox( x - 1, y - 1, w + 1, h + 1, 1, out );
}

int CDrawing::GetStringLenght( const char *fmt, ... ) {
	va_list va_alist;
	char buf[ 256 ];
	va_start( va_alist, fmt );
	_vsnprintf_s( buf, sizeof( buf ), fmt, va_alist );
	va_end( va_alist );
	int iWidth, iHeight;
	g_Engine.pfnDrawConsoleStringLen( buf, &iWidth, &iHeight );
	return iWidth;
}

int CDrawing::GetStringHeight( void ) {
	int iWidth, iHeight;
	g_Engine.pfnDrawConsoleStringLen( "F", &iWidth, &iHeight );
	return iHeight;
}

void CDrawing::DrawString( int x, int y, Color col, const char *fmt, ... ) {
	va_list va_alist;
	char buf[ 256 ];
	va_start( va_alist, fmt );
	_vsnprintf_s( buf, sizeof( buf ), fmt, va_alist );
	va_end( va_alist );
	g_Engine.pfnDrawSetTextColor( ( float ) col.r() / 255.0f, ( float ) col.g() / 255.0f, ( float ) col.b() / 255.0f );
	g_Engine.pfnDrawConsoleString( x, y, buf );
}

void CDrawing::DrawStringCenter( int x, int y, Color col, const char *fmt, ... ) {
	va_list va_alist;
	char buf[ 256 ];
	va_start( va_alist, fmt );
	_vsnprintf_s( buf, sizeof( buf ), fmt, va_alist );
	va_end( va_alist );
	int iWidth = GetStringLenght( "%s", buf );
	g_Engine.pfnDrawSetTextColor( ( float ) col.r() / 255.0f, ( float ) col.g() / 255.0f, ( float ) col.b() / 255.0f );
	g_Engine.pfnDrawConsoleString( x - iWidth / 2, y, buf );
}