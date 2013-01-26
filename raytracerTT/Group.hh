#ifndef GROUP_HXX
#define GROUP_HXX


#include <vector>

// for stl template 'vector' that realizes a dynamic array
#include "Basic.hh"

using namespace std;


class Group : public Primitive
{
public:
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
    cerr << "cannot call getnormal of a group !" << endl;
    return Vec3f(0,0,0);
  };
};



#endif




