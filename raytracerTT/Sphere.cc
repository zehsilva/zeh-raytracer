#include "Sphere.hh"

bool Sphere::Intersect(Ray &ray)
{
  // Calculo da intersecao aqui
  // ray:  p = q + t*v
  // esfera: (p-C)*(p-C)=r^2
  Vec3f qC=ray.org-center;
  float a=Dot(ray.dir,ray.dir);
  float b=2*Dot(qC,ray.dir);
  float c=Dot(qC,qC)-radius*radius;

  float delta=b*b-4*a*c;
  if(delta < 0.0f)
    return false;
  float raiz=sqrt(delta);
  if(delta> 0.00f)
  {
    float t = (-b - raiz)/(2*a);

    if( t <  Epsilon)
    {
      t = (-b + raiz)/(2*a);
      if (t < Epsilon)
        return false;
    }
    if(t < ray.t)
    {
      ray.t=t;
      ray.hitPoint=ray.org+ray.t*ray.dir;
      ray.hit=this;
      normal=(ray.hitPoint-center)/radius;
      Normalize(normal);
      return true;
    }
  }

  return false;
}

Vec3f Sphere::GetNormal(Ray &ray)
{

  return normal;
}
