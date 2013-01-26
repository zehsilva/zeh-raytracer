#ifndef INFINITEPLANE_HXX
#define INFINITEPLANE_HXX

#include "Basic.hh"

class InfinitePlane : public Primitive
{
public:
  Vec3f origin, normal;

  InfinitePlane(Vec3f origin, Vec3f normal, Shader *shader)
    : origin(origin), normal(normal), Primitive(shader)
  {
    Normalize(normal);
  };

  ~InfinitePlane() {};

  virtual bool Intersect(Ray &ray);
  virtual Vec3f GetNormal(Ray &ray);
};



#endif


