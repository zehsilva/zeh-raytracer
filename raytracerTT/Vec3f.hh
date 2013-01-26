#ifndef VEC3F_HXX
#define VEC3F_HXX

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <sys/times.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <strings.h>
#include <stdarg.h>
#include <errno.h>

#include <iostream>
#include <fstream>

#define Epsilon 1E-5
#define Infinity HUGE_VAL

using namespace std;

class Vec3f
{
public:
  float x,y,z;

  Vec3f()
  {};

  Vec3f(float x,float y, float z)
    : x(x),y(y),z(z)
  {};

  inline const float &operator[](const int i) const
  { return *(&x+i); };

  inline float &operator[](const int i)
  { return *(&x+i); }; //((float *)(this))[i]; };

  inline Vec3f &operator=(const Vec3f &b)
  { x = b.x; y = b.y; z = b.z; return *this;};

  float dot(const Vec3f &b)
  { return x*b.x+y*b.y+z*b.z; };

  float norm()
  { return sqrt(x*x+y*y+z*z); };

  void normalize()
  {
    float len=this->norm();
    x/=len;
    y/=len;
    z/=len;
  };
};

/*! dot product */
inline float Dot(const Vec3f &a, const Vec3f &b)
{ return a.x*b.x+a.y*b.y+a.z*b.z; };

/*! component-wise product */
inline Vec3f Product(const Vec3f &a, const Vec3f &b)
{ return Vec3f(a.x*b.x,a.y*b.y,a.z*b.z); };

/*! vector product */
inline Vec3f Cross(const Vec3f &a, const Vec3f &b)
{ return Vec3f(a.y*b.z-a.z*b.y,
	       a.z*b.x-a.x*b.z,
	       a.x*b.y-a.y*b.x); };


inline Vec3f operator-(const Vec3f &v)
{ return Vec3f(-v.x,-v.y,-v.z); };

inline float Length(const Vec3f &v)
{ return sqrt(Dot(v,v)); };

inline Vec3f operator*(const float f, const Vec3f &v)
{ return Vec3f(f*v.x, f*v.y, f*v.z); };

inline Vec3f operator*(const Vec3f &v, const float f)
{ return Vec3f(f*v.x, f*v.y, f*v.z); };

inline Vec3f operator*(const Vec3f &v, const Vec3f &f)
{ return Vec3f(v.x*f.x, v.y*f.y, v.z*f.z); };

inline void operator*=(Vec3f &v, const float f)
{ v.x *= f; v.y*=f; v.z*=f; };

inline void operator*=(Vec3f &v, const Vec3f &f)
{ v.x *= f.x; v.y*=f.y; v.z*=f.z; };

inline void operator+=(Vec3f &v, const float f)
{ v.x += f; v.y+=f; v.z+=f; };

inline void operator+=(Vec3f &v, const Vec3f &f)
{ v.x += f.x; v.y+=f.y; v.z+=f.z; };

inline void operator<<(ostream& os,Vec3f &v)
{  os << "{" << v.x << "," << v.y << "," << v.z << "}"; };

inline Vec3f operator/(const Vec3f &v, const float f)
{ return (1/f)*v; };

inline void operator/=(Vec3f &v, const float f)
{ v *= (1/f); };

inline bool operator!=(Vec3f &v, const Vec3f &f)
{ return v.x!=f.x || v.y!=f.y || v.z!=f.z ;};

inline Vec3f operator+(const Vec3f &a, const Vec3f &b)
{ return Vec3f(a.x+b.x, a.y+b.y, a.z+b.z); };

inline Vec3f operator-(const Vec3f &a, const Vec3f &b)
{ return Vec3f(a.x-b.x, a.y-b.y, a.z-b.z); };

inline void Normalize(Vec3f &v)
{ v *= (1./Length(v)); };

#endif


