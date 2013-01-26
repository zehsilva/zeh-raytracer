#include "InfinitePlane.hh"

bool InfinitePlane::Intersect(Ray &ray)
{
  float t;
  float nv=Dot(normal,ray.dir);

  if(fabs(nv) < Epsilon)
    return false;
  t=Dot(normal,origin-ray.org)/nv;
  if(t <= ray.t)
  {
    ray.hit=this;
    ray.hitPoint=ray.org+ray.dir*t;
    ray.t=t;
    return true;
  }
  return false;
}

inline Vec3f InfinitePlane::GetNormal(Ray &ray)
{
  return normal;
}
