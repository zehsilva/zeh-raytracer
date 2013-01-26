#include "Triangle.hh"


inline Vec3f Triangule::GetNormal(Ray &ray)
{
  if(Dot(normal,ray.dir)>Epsilon)
  {
    normal=-normal;
  }
  return normal;
}

Triangule::Triangule(Vec3f a, Vec3f b, Vec3f c, Shader *s) : Primitive(s), A(a), B(b), C(c)
{
  Vec3f	v1 = b - a;
  Vec3f	v2 = c - a;
  who = "tri";
  normal = Cross(v1, v2);
  Normalize(normal);
}
///http://www.blackpawn.com/texts/pointinpoly/default.html
//formula derivada manualmente usando ideias similares do link superior
bool Triangule::PointInTriangle(Vec3f &pt, Ray &ray)
{
  Vec3f U=B-A;
  Vec3f V=C-A;
  Vec3f X=pt-A;
  float r,s;
  float Uyz=U.y+U.z;
  float Vyz=V.y+V.z;
  float Xyz=X.y+X.z;
  float den=Vyz*U.x - Uyz*V.x;

  r=(Xyz*U.x - Uyz*X.x)/den;
  s=-(Xyz*V.x - Vyz*X.x)/den;

  if(s>0 && r>0 && (r+s)<=1)
    return true;
  return false;
}

bool Triangule::Intersect(Ray &ray)
{
  // Calculo da intersecao
  Ray novo(ray);
  InfinitePlane planTri(A,normal,NULL);
  if(planTri.Intersect(novo))
  {
    if(PointInTriangle(novo.hitPoint,novo))
    {
      if(novo.t <= ray.t)
      {
        ray.t=novo.t;
        ray.hitPoint=novo.hitPoint;
        ray.hit=this;

        return true;
      }
    }
  }
  return false;
}








