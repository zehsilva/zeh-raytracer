#include "OrthoCamera.hh"
void OrthogonalCamera::InitRay(float x, float y, Ray &ray)
{

  ray.dir= dir;
  ray.org = dir_X*2.0f * ((x/(float)resX - .5f)*aspect)+dir_Y*2.0f * (y/(float)resY - .5f)+dir*focus;


  Normalize(ray.dir);
}


  int OrthogonalCamera::getResY()
  {
    return this->resY;
  }

  int OrthogonalCamera::getResX()
  {
    return this->resX;
  }

  int OrthogonalCamera::getNumberSamples()
  {
    return 1;
  }
