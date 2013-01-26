#ifndef FLATSHADER_HXX
#define FLATSHADER_HXX

#include "Basic.hh"

class FlatShader : public Shader
{
public:
  Vec3f color;

  FlatShader();
  FlatShader(Vec3f color)
    : color(color)
  {};

  ~FlatShader()
  {};

  virtual Vec3f Shade(Ray &ray);
};

inline Vec3f FlatShader::Shade(Ray &ray)
{
    Vec3f normal = ray.hit->GetNormal(ray);
    Normalize(normal);
    Normalize(ray.dir);

    if (fabs(Dot(normal,ray.dir)) < 0.6) {

        color=0.4*color;
    }



    return color;
}

#endif
