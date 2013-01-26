#include "PhongShader.hh"
#include <math.h>
#include <algorithm>

//http://en.wikipedia.org/wiki/Fresnel_equations
float PhongShader::FrDielectric (float cosi, float cost, float etai, float etat) {
   float r_par = (etat*cosi - etai*cost) / (etat*cosi + etai*cost);
   float r_per = (etai*cosi - etat*cost) / (etai*cosi + etat*cost);
   //  Assumption: The light is unpolarised and therefore the Fresnel reflectance is the average
   //  of the squares of the parallel and perpendicular polarization term.
   return  0.5 * (r_par*r_par + r_per*r_per);
}

float PhongShader::FresnelEvaluate (float cosi,float eta_i,float eta_t) {
  bool entering = cosi > 0;
  float ei = entering ? eta_i : eta_t;
  float et = entering ? eta_t : eta_i;
  // Lei de Snell
  float sint = ei/et * sqrtf(max(0.0f, 1 - cosi*cosi));
  if (sint > 1) {
    // Total internal reflection!
    return 1;
  } else {
    float cost = sqrtf(max(0.f, 1.f - sint*sint));
    return FrDielectric(fabs(cosi), cost, ei, et);
  }
}


Vec3f PhongShader::ReflectRefract(Ray &ray,Scene * scen)
//http://en.wikipedia.org/wiki/Snell's_law
{
  Vec3f reflectCol(0,0,0),transmitCol(0,0,0);

  Primitive* pHit=ray.hit;
  Vec3f normal = pHit->GetNormal(ray);



  Vec3f reflected=ray.dir-2.0*Dot(ray.dir,pHit->GetNormal(ray))*pHit->GetNormal(ray);
  Ray rayr(ray.hitPoint,reflected,Infinity);

  rayr.bounce=ray.bounce+1;

  reflectCol = scen->RayTrace(rayr);

  /*
  // The cosine of the angle of the incoming ray;
  float cosi = fabs(Dot(ray.dir,-normal))/Length(ray.dir) * Length(normal);
  // Snells Law
  float sint = eta_i/eta_t * sqrtf(max(0.f, 1.f - cosi*cosi));


  // The cosine of the angle of the reflected ray;
  float cosr = fabs(Dot(reflected,normal))/Length(reflected)*Length(normal);
  // The cosine of the angle of the transmitted ray;
  float cost = sqrtf(max(0.f, 1.f - sint*sint));

  Vec3f transmitted=(eta_i/eta_t * cosi - cost) * normal + eta_i/eta_t * ray.dir;

  Ray transmitRay(ray.hitPoint,transmitted,Infinity);
  transmitRay.bounce = ray.bounce+1;

  float F = FresnelEvaluate(cosi,eta_i,eta_t);// refletância


  reflectCol *= F / fabs(cosr);

  if (F == 1)
  {
    // reflexão total
    return reflectCol;
  }
  else
  {
    transmitCol = scene->RayTrace(transmitRay);
    transmitCol *= (eta_i*eta_i)/(eta_t*eta_t) * ( 1.f - F) / fabs(cost);
    return 0.6*reflectCol + transmitCol;
  }*/
  return reflectCol;
}

Vec3f PhongShader::Shade(Ray &ray)
{

  Vec3f normal = ray.hit->GetNormal(ray);


  if (Dot(normal,ray.dir) > 0) {
    normal = -normal;
  }

  Vec3f reflect = ray.dir - 2*Dot(normal,ray.dir)*normal;

  // Ia
  Vec3f ambientIntensity(1,1,1);
  Vec3f ambientColor = ka * color;
  Vec3f result = ambientColor*ambientIntensity;


  // shadow ray
  Ray shadow;
  shadow.org = ray.org + ray.t * ray.dir;

  // iterate over all light sources
  for (unsigned int l=0; l < scene->lights.size(); l++)
  {
    // get direction to light, and intensity
    Vec3f lightIntensity;
    Vec3f result_local = Vec3f(0.0f,0.0f,0.0f);

    lightIntensity=scene->lights[l]->Illuminate(shadow, lightIntensity);
    Vec3f reflect = shadow.dir - 2*Dot(normal,shadow.dir)*normal;
    // compute distance to the light source
    float distance = shadow.t;

    // diffuse term, also used as a check if illuminating the front-side
    float cosLightNormal = Dot(shadow.dir,normal);
    if (cosLightNormal > 0)
    {
      if(scene->primitives.Intersect(shadow))
      {
        if (shadow.t < distance)
          continue;
      }

      // compute diffuse term
      Vec3f diffuseColor = kd * color;
      result_local = result_local + (diffuseColor * cosLightNormal)*lightIntensity;

      // specular term is computed only if shading the front-side
      float cosLightReflect = Dot(ray.dir,reflect);
      if (cosLightReflect > 0)
      {
        Vec3f specularColor = ks * Vec3f(1,1,1); // white highlight;
        result_local = result_local + specularColor * powf(cosLightReflect,kg) * lightIntensity;
      }
    }
    result=result+ result_local;
    //result += (result_local / float(scene->lights.size()));
  }

  return result;
}










