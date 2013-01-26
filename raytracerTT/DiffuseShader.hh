#ifndef DIFFSHADER_HXX
#define DIFFSHADER_HXX

#include "Basic.hh"

class DiffuseShader : public Shader
{
public:
  Vec3f color;
  Vec3f normal;
  DiffuseShader();
  DiffuseShader(Vec3f color)
    : color(color)
  {};

  ~DiffuseShader()
  {};

  virtual Vec3f Shade(Ray &ray,Vec3f normal);
  virtual Vec3f Shade(Ray &ray);
};

inline Vec3f DiffuseShader::Shade(Ray &ray,Vec3f normal)
{
  return color*fabs(Dot(ray.dir,normal));
}

inline Vec3f DiffuseShader::Shade(Ray &ray)
{
  return color*fabs(Dot(ray.dir,ray.hit->GetNormal(ray)));
}

#endif
