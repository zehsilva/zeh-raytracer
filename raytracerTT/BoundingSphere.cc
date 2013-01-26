#include "BoundingSphere.hh"

bool isInside(Sphere &s1, Sphere &s2)
//returns true if s2 is inside s1
{
  if((Length(s2.center-s1.center)+s2.radius)<=s1.radius)
    return true;
  return false;
}

void merge(BoundingSphere &s1,BoundingSphere &s2)
{
  if(!isInside(s1.boundaries,s2.boundaries))
  {

    float radius=s1.boundaries.radius+s2.boundaries.radius+Length(s1.boundaries.center-s2.boundaries.center);
    Vec3f c1=s1.boundaries.center;
    Vec3f c2=s2.boundaries.center;
    float r1=s1.boundaries.radius;
    float r2=s2.boundaries.radius;
    float distc1c2=Length(c1-c2);

    Vec3f center=(c1c2*(r1-r2)+(c1+c2)*distc1c2)/(2*distc1c2);

    Sphere novo(center,radius,NULL);
    s1.boundaries=novo;
  }

  vector<Primitive *> v(s1.primitive.size()+s2.primitive.size());
  copy(s1.primite.begin(),s1.primitive.end(),v.begin());
  copy(s2.primite.begin(),s2.primitive.end(),v.begin());
  s1.primitive=v;
}
