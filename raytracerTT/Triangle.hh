#ifndef TRIANGULE_HXX
#define TRIANGULE_HXX

#include "Basic.hh"
#include "InfinitePlane.hh"

class Triangule : public Primitive
{
private:
  Vec3f	A, B, C;
  Vec3f	normal;
  bool PointInTriangle(Vec3f &pt, Ray &ray);

public:
  Triangule(Vec3f a, Vec3f b, Vec3f c, Shader *s);
  ~Triangule() {};

  virtual bool Intersect(Ray &ray);
  virtual Vec3f GetNormal(Ray &ray);
};


#endif








