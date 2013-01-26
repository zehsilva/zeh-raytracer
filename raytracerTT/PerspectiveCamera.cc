#include "PerspectiveCamera.hh"

void PerspectiveCamera::InitRay(float x, float y, Ray &ray)
{
  // Escreva o codigo aqui

  int i,j;

  i=-1+atual%3;
  j=-1+atual/3;
  x=x+(0.0001)*(float)i;
  y=y+(0.0001)*(float)j;
  atual++;
  atual%nSamples;

  Vec3f vp = dir_X*2.0f * ((x/(float)resX - .5f)*aspect)+dir_Y*2.0f * (y/(float)resY - .5f)+dir*focus;
  //ray.dir  = vp + dir*tan(angle/2);
  //ray.dir  = Vec3f(10,x,y);
  ray.dir= vp;
  ray.org = pos;


  Normalize(ray.dir);
}

   int PerspectiveCamera::getResY()
  {
    return this->resY;
  }

   int PerspectiveCamera::getResX()
  {
    return this->resX;
  }

  int PerspectiveCamera::getNumberSamples()
  {
    return nSamples;
  }
