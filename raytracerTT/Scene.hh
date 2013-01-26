#ifndef SCENE_HXX
#define SCENE_HXX


#include "PerspectiveCamera.hh"
#include "Group.hh"
#include "Basic.hh"




class Scene
{
  int  maxDepth;
  // parse an nff file 'fileName', add all its primitives
  // to the specified group...
  void ParseNFF(FILE *file, char *fileName, Group *groupToAddTo);

public:
  Group primitives; // group of all the primitives of this scene
  Vec3f bgColor; // background color, as long as we don't have an environment shader ...
  vector<Light *> lights; // lista de luzes;
  Camera *camera;
  int rayDepth;

  Scene(int depth = 0)
    : bgColor(Vec3f(0,0,0)), maxDepth(10),
      camera(new PerspectiveCamera(Vec3f(0,0,0),Vec3f(0,0,-1),
				   Vec3f(0,1,0),60,640,480))
  { rayDepth = 0;};

  ~Scene()
  {  if(camera) delete camera;  };

  void ParseNFF(char *fileName);

  Vec3f RayTrace(Ray &ray);

  void Cluster();
};

#endif
