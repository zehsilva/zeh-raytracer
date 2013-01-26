#ifndef ORTHOCAMERA_HH_INCLUDED
#define ORTHOCAMERA_HH_INCLUDED


#include "Basic.hh"

class OrthogonalCamera : public Camera
{
public:
  Vec3f pos, dir, up, at;
  float angle,aspect,focus;
  int resX, resY;
  Vec3f dir_X, dir_Y;
  float res_X, res_Y;

  OrthogonalCamera()
  {};

  OrthogonalCamera(Vec3f p, Vec3f d, Vec3f vup, float ang, int rX, int rY)
  {
    pos = p;
    dir = d-p;
    at = d;
    Normalize(dir);
    up = vup;
    Normalize(up);
    angle = ang * 3.1416 / 180;
    resX = rX;
    resY = rY;

    aspect = resX / float(resY);

    focus = 1.f / tan(angle/2.f);


    if( ((ang-180) > Epsilon) || ((ang-180) < -Epsilon) )
    {
      res_Y = tan(angle/2) * 2;
      res_X = res_Y * resX / resY;
    }

    dir_X = Cross(up, dir);
    Normalize(dir_X);
    dir_Y = Cross(dir, dir_X);
    Normalize(dir_Y);
  };


  ~OrthogonalCamera() {};

  void InitRay(float x, float y, Ray &ray);
  virtual int getResX();
  virtual int getResY();
  virtual int getNumberSamples();

};


#endif // ORTHOCAMERA_HH_INCLUDED
