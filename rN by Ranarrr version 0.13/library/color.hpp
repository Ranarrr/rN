#ifndef COLOR_H
#define COLOR_H

#ifdef _WIN32
#pragma once
#endif

class Color {
public:
	Color() : red( 0 ), green( 0 ), blue( 0 ), alpha( 0 ) {}

	Color( int r, int g, int b, int a ) : red( r ), green( g ), blue( b ), alpha( a ) {}

	Color( int r, int g, int b ) : red( r ), green( g ), blue( b ), alpha( 255 ) {}

	static Color White( void ) { return Color( 255, 255, 255, 255 ); }
	static Color Black( void ) { return Color( 35, 35, 35, 255 ); }
	static Color Red( void ) { return Color( 255, 30, 30, 255 ); }
	static Color Green( void ) { return Color( 0, 255, 0, 255 ); }
	static Color Blue( void ) { return Color( 0, 180, 255, 255 ); }
	static Color Cyan( void ) { return Color( 0, 127, 255, 255 ); }
	static Color Yellow( void ) { return Color( 255, 255, 0, 255 ); }

	int r( void ) { return red; }
	int g( void ) { return green; }
	int b( void ) { return blue; }
	int a( void ) { return alpha; }

	Color& operator= ( Color& c ) {
		red = c.r();
		green = c.g();
		blue = c.b();
		alpha = c.a();
		return *this;
	}

	/*Color& ConvToHex(Color& col)
	{
		int r = col.r(), g = col.g(), b = col.b(), a = col.a();

		string retr, retg, retb, reta;

		if (r < 10 && r >= 0)
			retr = r;

		if (r <= 16 && r >= 10)
		{
			retr = Instruments::Get()->ToHex(r);
		}
		else
		{
			for (int i = 0; i < 2; ++i)
			{
				int tempr = r;

				tempr /= 16;
				if (tempr < 16 && i == 1)
				{
					retr = Instruments::Get()->ToHex(tempr);
				}
				else if (tempr < 16 && i == 2)
				{
					retr += Instruments::Get()->ToHex(tempr);
				}
				else
				{
				}
			}
		}
	}*/

	Color ApplyR( int r ) {
		red = r;
		return *this;
	}

	Color ApplyG( int g ) {
		green = g;
		return *this;
	}

	Color ApplyB( int b ) {
		blue = b;
		return *this;
	}

	Color ApplyA( int a ) {
		alpha = a;
		return *this;
	}

private:
	int red, green, blue, alpha;
};

#endif // COLOR_H