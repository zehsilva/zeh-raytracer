#ifndef BOUNDINGSPHERE_HH_INCLUDED
#define BOUNDINGSPHERE_HH_INCLUDED

#include "Basic.hh"
#include "Sphere.hh"

class BoundingSphere : public Primitive
{
public:
  Sphere * boundaries;
  vector<Primitive *> primitive;

  Group() : Primitive(NULL)
  {};

  ~Group();

  void Add(Primitive *prim)
  {
    primitive.push_back(prim);
  };

  virtual bool Intersect(Ray &ray);

  virtual Vec3f GetNormal(Ray &ray)
  {
    return boundaries->GetNormal(ray);
  };
};


#endif // BOUNDINGSPHERE_HH_INCLUDED
