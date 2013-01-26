#ifndef SPHERE_HXX
#define SPHERE_HXX

#include "Basic.hh"


class Sphere : public Primitive
{
public:
  Vec3f center,normal;
  float radius;

  Sphere(Vec3f center, float radius, Shader *shader)
    : center(center), radius(radius), Primitive(shader)
  {who="sss";};

  ~Sphere() {};

  virtual bool Intersect(Ray &ray);

  virtual Vec3f GetNormal(Ray &ray);
};


#endif




