#ifndef UTIL_VECTOR_HPP
#define UTIL_VECTOR_HPP

// Misc C-runtime library headers
#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"

#define MAX_PHYSINFO_STRING 256				// Physics info string definition

class Vector2D {
public:
	inline Vector2D() {}
	inline Vector2D( float X, float Y )						{ x = X; y = Y; }
	inline Vector2D operator+( const Vector2D v )	const	{ return Vector2D( x + v.x, y + v.y ); }
	inline Vector2D operator-( const Vector2D v )	const	{ return Vector2D( x - v.x, y - v.y ); }
	inline Vector2D operator*( float fl )			const	{ return Vector2D( x * fl, y * fl ); }
	inline Vector2D operator/( float fl )			const	{ return Vector2D( x / fl, y / fl ); }

	inline float Length()							const	{ return ( float ) sqrt( x * x + y * y ); }

	inline Vector2D Normalize() const {
		float flLen = Length();

		if( flLen == 0.f )
			return Vector2D( 0.f, 0.f );
		else {
			flLen = 1 / flLen;
			return Vector2D( x * flLen, y * flLen );
		}
	}

	inline float DistTo( Vector2D from ) {
		float a = this->x - from.x;
		float b = this->y - from.y;
		return sqrt( a * a + b * b );
	}

	inline float DotProduct( const Vector2D a, const Vector2D b ) { return( a.x * b.x + a.y * b.y ); }

	float x, y;
};

//=========================================================
// 3D Vector
//=========================================================
class Vector {
public:
	// Construction/destruction
	inline Vector()									{}
	inline Vector( float XYZ )						{ x = XYZ; y = XYZ; z = XYZ; }
	inline Vector( float X, float Y, float Z )		{ x = X; y = Y; z = Z; }
	inline Vector( double X, double Y, double Z )	{ x = ( float ) X; y = ( float ) Y; z = ( float ) Z; }
	inline Vector( int X, int Y, int Z )			{ x = ( float ) X; y = ( float ) Y; z = ( float ) Z; }
	inline Vector( const Vector &v )				{ x = v.x; y = v.y; z = v.z; }
	inline Vector( float rgfl[ 3 ] )				{ x = rgfl[ 0 ]; y = rgfl[ 1 ]; z = rgfl[ 2 ]; }

	// Operators
	inline int		operator==( const Vector v ) const	{ return x == v.x && y == v.y && z == v.z; }
	inline int		operator!=( const Vector v ) const	{ return !( *this == v ); }

	inline void		operator+=( const Vector v )		{ x + v.x; y + v.y; z + v.z; }
	inline void		operator=( const Vector v )			{ x = v.x; y = v.y; z = v.z; }

	inline Vector	operator-()	const { return Vector( -x, -y, -z ); }
	inline Vector	operator+( const Vector v )	const	{ return Vector( x + v.x, y + v.y, z + v.z ); }
	inline Vector	operator-( const Vector v )	const	{ return Vector( x - v.x, y - v.y, z - v.z ); }
	inline Vector	operator*( const Vector v )	const	{ return Vector( x * v.x, y * v.y, z * v.z ); }
	inline Vector	operator*( const float fl )	const	{ return Vector( x * fl, y * fl, z * fl ); }
	inline Vector	operator/( const Vector v )	const	{ return Vector( x / v.x == 0.f ? 1.f : v.x, y / v.y == 0.f ? 1.f : v.y, z / v.z == 0.f ? 1.f : v.z ); }
	inline Vector	operator/( const float fl )	const	{ return Vector( x / fl == 0.f ? 1.f : fl, y / fl == 0.f ? 1.f : fl, z / fl == 0.f ? 1.f : fl ); }

	// Methods
	inline void				CopyToArray( float *rgfl ) const	{ rgfl[ 0 ] = x, rgfl[ 1 ] = y, rgfl[ 2 ] = z; }
	inline float			Length() const						{ return ( float ) sqrt( x*x + y*y + z*z ); }
	operator float			*( )								{ return &x; } // Vectors will now automatically convert to float * when needed
	operator const float	*( ) const							{ return &x; } // Vectors will now automatically convert to float * when needed
	
	inline Vector Normalize() const {
		float flLen = Length();
		if( flLen == 0 ) return Vector( 0, 0, 1 ); // ????
		flLen = 1 / flLen;
		return Vector( x * flLen, y * flLen, z * flLen );
	}

	inline float LengthSqr() const {
		return x * x + y * y + z * z;
	}

	inline float *Base() {
		return ( float * ) this;
	}

	inline float DistTo( Vector from ) {
		float a = this->x - from[ 0 ];
		float b = this->y - from[ 1 ];
		float c = this->z - from[ 2 ];
		return sqrt( a * a + b * b + c * c );
	}

	inline Vector2D Make2D() const {
		return Vector2D( x, y );
	}

	inline void Init() { x = 0.f; y = 0.f; z = 0.f; }

	inline float Length2D() const { return sqrtf( x * x + y * y ); }

	inline float DotProduct( const Vector a, const Vector b ) { return( a.x * b.x + a.y * b.y + a.z * b.z ); }

	inline Vector CrossProduct( const Vector a, const Vector b ) { return Vector( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x ); }

	float x, y, z;
};

#endif // !UTIL_VECTOR_HPP