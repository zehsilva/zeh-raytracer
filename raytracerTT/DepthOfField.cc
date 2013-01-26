#include "DepthOfField.hh"
#include <cmath>
#include <stdlib.h>




     void DepthOfFieldCamera::InitRay(float x, float y,Ray& ray)
     {
         ray.org = pos;
         ray.dir = dir_X*2.0f * ((x/(float)resX - .5f)*aspect)+dir_Y*2.0f * (y/(float)resY - .5f)+dir*focus;

         ray.dir.normalize();

         // depth of field

         if (lensRadius > Epsilon) {

             /*float r = frand() * lensRadius;
             float theta = 2.0f * 3.1415 * (frand() + atual)/nSamples;
             float x = r * cosf(theta);
             float y = r * sinf(theta);


             float ft = (focalDistance - focus) / ray.dir.z;
             Vec3f fp = ray.org - ft * ray.dir;

             // update ray for effect of lens
             ray.org.x += x;
             ray.org.y += y;
             ray.dir = fp - ray.org;*/


              float ft = (focalDistance - focus) ;
              Vec3f focusedPoint = ray.org + ft * ray.dir;



               // Move origin of camera ray on one random point on circle with radius of circleOfConfusionSize

               double fi = 2*M_PI*frand();

               Vec3f newOrigin = ray.org + cos(fi)*lensRadius * xAxis + sin(fi) * lensRadius * yAxis;



               ray.dir = focusedPoint - newOrigin;

               Normalize(ray.dir);

               ray.org = newOrigin;
         }

     }

    int DepthOfFieldCamera::getResY()
    {
      return this->resY;
    }

    int DepthOfFieldCamera::getResX()
    {
      return this->resX;
    }

    int DepthOfFieldCamera::getNumberSamples()
    {
      return nSamples;
    }

