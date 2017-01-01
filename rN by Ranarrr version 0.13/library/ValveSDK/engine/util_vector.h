#if !defined FILE_UTIL_VECTOR_H
#define FILE_UTIL_VECTOR_H

// Misc C-runtime library headers
#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"

// Header file containing definition of globalvars_t and entvars_t
typedef int	func_t;					//
typedef int	string_t;				// from engine's pr_comp.h;
typedef float vec_t;				// needed before including progdefs.h

class Vector2D {
public:
	inline Vector2D( void ) {}
	inline Vector2D( float X, float Y ) { x = X; y = Y; }
	inline Vector2D operator+( const Vector2D& v )	const { return Vector2D( x + v.x, y + v.y ); }
	inline Vector2D operator-( const Vector2D& v )	const { return Vector2D( x - v.x, y - v.y ); }
	inline Vector2D operator*( float fl )				const { return Vector2D( x*fl, y*fl ); }
	inline Vector2D operator/( float fl )				const { return Vector2D( x / fl, y / fl ); }

	inline float Length( void )						const { return ( float ) sqrt( x*x + y*y ); }

	inline Vector2D Normalize( void ) const {
		Vector2D vec2;

		float flLen = Length();
		if( flLen == 0 ) {
			return Vector2D( ( float ) 0, ( float ) 0 );
		} else {
			flLen = 1 / flLen;
			return Vector2D( x * flLen, y * flLen );
		}
	}

	vec_t	x, y;
};

inline float DotProduct( const Vector2D& a, const Vector2D& b ) { return( a.x*b.x + a.y*b.y ); }
inline Vector2D operator*( float fl, const Vector2D& v ) { return v * fl; }

//=========================================================
// 3D Vector
//=========================================================
class Vector						// same data-layout as engine's vec3_t,
{									// which is a vec_t[3]
public:
	// Construction/destruction
	inline Vector( void ) {}
	inline Vector( float X, float Y, float Z ) { x = X; y = Y; z = Z; }
	inline Vector( double X, double Y, double Z ) { x = ( float ) X; y = ( float ) Y; z = ( float ) Z; }
	inline Vector( int X, int Y, int Z ) { x = ( float ) X; y = ( float ) Y; z = ( float ) Z; }
	inline Vector( const Vector& v ) { x = v.x; y = v.y; z = v.z; }
	inline Vector( float rgfl[ 3 ] ) { x = rgfl[ 0 ]; y = rgfl[ 1 ]; z = rgfl[ 2 ]; }

	// Operators
	inline Vector operator-( void ) const { return Vector( -x, -y, -z ); }
	inline int operator==( const Vector& v ) const { return x == v.x && y == v.y && z == v.z; }
	inline int operator!=( const Vector& v ) const { return !( *this == v ); }
	inline Vector operator+( const Vector& v ) const { return Vector( x + v.x, y + v.y, z + v.z ); }
	inline Vector operator-( const Vector& v ) const { return Vector( x - v.x, y - v.y, z - v.z ); }
	inline Vector operator*( float fl ) const { return Vector( x*fl, y*fl, z*fl ); }
	inline Vector operator/( float fl ) const { return Vector( x / fl, y / fl, z / fl ); }

	// Methods
	inline void CopyToArray( float* rgfl ) const { rgfl[ 0 ] = x, rgfl[ 1 ] = y, rgfl[ 2 ] = z; }
	inline float Length( void ) const { return ( float ) sqrt( x*x + y*y + z*z ); }
	operator float *( ) { return &x; } // Vectors will now automatically convert to float * when needed
	operator const float *( ) const { return &x; } // Vectors will now automatically convert to float * when needed
	inline Vector Normalize( void ) const {
		float flLen = Length();
		if( flLen == 0 ) return Vector( 0, 0, 1 ); // ????
		flLen = 1 / flLen;
		return Vector( x * flLen, y * flLen, z * flLen );
	}

	float LengthSqr() const {
		return x * x + y * y + z * z;
	}

	float* Base() {
		return ( float* )this;
	}

	inline float DistTo( Vector from ) {
		float a = this->x - from[ 0 ];
		float b = this->y - from[ 1 ];
		float c = this->z - from[ 2 ];
		return sqrt( a*a + b*b + c*c );
	}

	inline Vector2D Make2D( void ) const {
		Vector2D	Vec2;

		Vec2.x = x;
		Vec2.y = y;

		return Vec2;
	}
	inline float Length2D( void ) const { return ( float ) sqrt( x*x + y*y ); }

	// Members
	vec_t x, y, z;
};
inline Vector operator*( float fl, const Vector& v ) { return v * fl; }
inline float DotProduct( const Vector& a, const Vector& b ) { return( a.x*b.x + a.y*b.y + a.z*b.z ); }
inline Vector CrossProduct( const Vector& a, const Vector& b ) { return Vector( a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x ); }

#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#define VectorCopy(from,to) {(to)[0]=(from)[0];(to)[1]=(from)[1];(to)[2]=(from)[2];}
#define VectorClear(a) {a[0]=0.0;a[1]=0.0;a[2]=0.0;}

#endif
