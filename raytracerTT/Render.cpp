#include "Image.hh"
#include "Basic.hh"
#include "Sphere.hh"
#include "Triangle.hh"
#include "InfinitePlane.hh"
#include "PerspectiveCamera.hh"
#include "FlatShader.hh"
#include "DiffuseShader.hh"
#include "Scene.hh"
#include <stdlib.h>
#include <time.h>

using namespace std;

float frand()
{
  return (float)rand()/(float)RAND_MAX;
}




Image *Render(char *filename)
{
  int depth = 3;
  bool antialiasing=true;
  Scene	scene(depth);
  scene.ParseNFF(filename);

  Image	*img;
  int	RES_X, RES_Y;
  RES_X = scene.camera->getResX();
  RES_Y = scene.camera->getResY();

  cout << RES_X << " " << RES_Y << endl;
  img = new Image(RES_X, RES_Y);
  if(!img)
    return NULL;
  img->Clear();


  for(int j = 0; j < RES_Y; j++)
    for(int i = 0; i < RES_X; i++)
      {

        int k;
        Vec3f coltemp(0,0,0);
        int nSamples=scene.camera->getNumberSamples();
        for(k=0;k<nSamples;k++)
        {
          Ray	ray;
          scene.camera->InitRay(i, j, ray);
          coltemp=coltemp+scene.RayTrace(ray);

        }
	      Vec3f	color = coltemp/nSamples;
	      //if(color!=scene.bgColor)
	       // cout << " pixel x y = " << i << "," << j << " ";

	      img->SetValuePixel(i, j, color);
      }
  return img;
}

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
      printf("render <inputfile> <outputfile>\n");
      return 0;
    }

  srand ( time(NULL) );
  printf("Rendering %s...\n", argv[1]);
  time_t t1 = time(&t1);
  Image	*img = Render(argv[1]);
  time_t t2 = time(&t2);
  printf("Now saving %s...\n", argv[2]);

  printf("Rendering Time = %d seg\n", (int)t2 - (int)t1);

  if(img->SavePPM(argv[2]) < 0)
    printf("Cannot save the file %s.\n", argv[2]);
  delete img;

  return 0;
}



