#ifndef BASIC_HXX
#define BASIC_HXX

#include "Vec3f.hh"

float frand();

class Primitive;

class Ray
{
public:
  Vec3f org, dir,hitPoint;
  Ray* lightRay;
  float t;
  int bounce;
  Primitive *hit;
  float u, v;

  Ray()
    : hit(NULL)
  {
    t = Infinity;
    bounce=0;
  };

  Ray(Vec3f org, Vec3f dir, float t)
    : org(org), dir(dir), t(t), hit(NULL)
  {
    lightRay=NULL;
    bounce=0;

    };

  Ray(const Ray &r)
    : org(r.org), dir(r.dir), t(r.t), hit(NULL)
  {};

  ~Ray()
  {};
};

class Shader
{
public:
  virtual Vec3f Shade(Ray &ray) = 0;
};

class Primitive
{
public:
  Shader *shader;
  string who;
  Primitive(Shader *sh)
    : shader(sh)
  {};

  ~Primitive()
  {};

  // Virtual ray/primitive intersection routine
  virtual bool Intersect(Ray &ray) = 0;


  // Virtual Normal routine
  virtual Vec3f GetNormal(Ray &ray) = 0;
};

class Camera
{

public:
  // Virtual camera routine
  virtual void InitRay(float x, float y, Ray &ray)
  {};
  virtual int getResX()
  {};
  virtual int getResY()
  {};

  virtual int getNumberSamples()
  {};

};

class Light : public Primitive
{
public:
  Vec3f pos, color;
  Primitive* shape;
  Light(Vec3f pos, Vec3f color,Primitive *sp,Shader * sh)
    : pos(pos),color(color), shape(sp),Primitive(sh)
  {};
  virtual Vec3f Illuminate(Ray &ray, Vec3f &intensity)=0;
};

class PointLight : public Light
{
  public:

  PointLight(Vec3f pos, Vec3f color,Primitive *sp,Shader * sh)
    : Light(pos,color,sp,sh)
  {

  };
  Vec3f Illuminate(Ray &ray, Vec3f &intensity)
  {
     // raio aponta para a luz
     ray.dir = pos - ray.org;
     ray.hit = NULL;
     ray.t = Length(ray.dir) - Epsilon;
     Normalize(ray.dir);


     // http://www.gamasutra.com/features/20050729/lacroix_01.shtml
     float att = 1.0f / (ray.t*ray.t+ray.t);
     intensity = att * this->color;
     return intensity;
  };
  bool Intersect(Ray &ray){
    Vec3f dirLuz = pos-ray.org;
    Normalize(dirLuz);
    Normalize(ray.dir);
    float cosT = Dot(dirLuz,ray.dir);
    if(cosT >= 0.998f)
    {
      if(Length(pos-ray.org) < ray.t)
      {
        ray.t = Length(pos-ray.org);
        ray.hit = this;
        ray.hitPoint = pos;
        return true;
      }
    }
    return false;
  }

  Vec3f GetNormal(Ray &ray){
    return Vec3f(0,0,1);
  }


};
#endif






