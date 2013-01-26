#ifndef PHONGSHADER_HXX
#define PHONGSHADER_HXX

#include "Basic.hh"
#include "Scene.hh"

class PhongShader : public Shader
{
public:
  Vec3f color;
  float ka, ks, kd, kg;
  float eta_i,eta_t;
  Scene* scene;
  PhongShader(Vec3f color, float ka, float ks, float kd, float kg,Scene *scene,float rl,float tp)
    : color(color), ka(ka), ks(ks), kd(kd), kg(kg), scene(scene), eta_i(rl), eta_t(tp)
  {};

  ~PhongShader()
  {};

  Vec3f ReflectRefract(Ray &ray,Scene * sc);
  virtual Vec3f Shade(Ray &ray);
  float FrDielectric (float cosi, float cost, float etai, float etat);
  float FresnelEvaluate (float cosi,float eta_i,float eta_t);

};

#endif



