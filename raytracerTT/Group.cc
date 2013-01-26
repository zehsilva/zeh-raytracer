#include "Group.hh"

Group::~Group()
{
  Primitive	*pri;
  vector<Primitive *>::iterator	iter = primitive.begin();
  for(int i = 0; i < primitive.size(); i++, iter++)
    {
      pri = (Primitive *)*iter;
      delete pri;
    }
}

bool Group::Intersect(Ray &ray)
{
  Primitive	*pri;
  vector<Primitive *>::iterator	iter = primitive.begin();

  for(int i = 0; i < primitive.size(); i++, iter++)
    {
      pri = (Primitive *)*iter;
      pri->Intersect(ray);
    }

  if(ray.hit==NULL)
  {
    return false;
  }
  return true;
}

