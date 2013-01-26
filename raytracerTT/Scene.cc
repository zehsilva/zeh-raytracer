#include "Scene.hh"
#include "PhongShader.hh"
#include "FlatShader.hh"
#include "Sphere.hh"
#include "Triangle.hh"
#include "OffObjectNEW.hh"
#include "PerspectiveCamera.hh"
#include "OrthoCamera.hh"
#include "DiffuseShader.hh"
#include "DepthOfField.hh"


#define LINESIZE 1000

#define NORM(x)   if((x) > 1) x = 1; else if((x) < 0) x = 0;

Vec3f Scene::RayTrace(Ray &ray)
{

  float tHit;
  Primitive * pHit;
  Vec3f color=bgColor;

  if(ray.bounce < maxDepth)
  {
    if(primitives.Intersect(ray))
    {
      ray.hitPoint=ray.org+ray.t*ray.dir;

      pHit=ray.hit;
      //cout << "entrou"<<endl;

      //Shader* sh=pHit->shader;
      //if(pHit == NULL)
        //cout << "y "<<endl;
      color=pHit->shader->Shade(ray);
      PhongShader* psh=(PhongShader*)pHit->shader;

      color=color+0.3*psh->ReflectRefract(ray,this);

    }

  }
  return color;
}

void Scene::ParseNFF(char *fileName)
{
  // parse file, add all items to 'primitives'
  ParseNFF(NULL,fileName,&primitives);
}

void Scene::ParseNFF(FILE *fileToUse, char *fileName, Group *groupToAddTo)
{
  FILE *file = fileToUse;
  if(!file) {
    file = fopen(fileName,"r");
    if(!file) {
      cerr << "could not open input file " << fileName << endl;
      exit(1);
    }
  }

  char line[LINESIZE+1];
  char token[LINESIZE+1];
  char *str;

  // just to have a default shader, in case the file doesn't define one !
  static Shader *currentShader = new FlatShader(Vec3f(1,0,0));

  while((str = fgets(line,LINESIZE,file)) && !feof(file)) {

    if(str[0] == '#')	// '#' : comment
      continue;

    int numtokens = sscanf(line,"%s",token);
    if(numtokens <= 0)
      continue; // empty line, except for whitespaces

    if(!strcmp(token,"begin_hierarchy")) {
      line[strlen(line)-1] = 0; // remove trailing eol indicator '\n'
      Group *subGroup = new Group;
      groupToAddTo->Add(subGroup);
      ParseNFF(file,fileName,subGroup);
      continue;
    }

    if(!strcmp(token,"end_hierarchy"))
      return;

    if(!strcmp(token,"v")) {
      // FORMAT:
      //     v
      //     from %g %g %g
      //     at %g %g %g
      //     up %g %g %g
      //     angle %g
      //     hither %g
      //     resolution %d %d
      Vec3f pos, at, up;
      float angle, hither;
      int resX, resY;
      fscanf(file,"from %g %g %g\n",&pos.x,&pos.y,&pos.z);
      fscanf(file,"at %g %g %g\n",&at.x,&at.y,&at.z);
      fscanf(file,"up %g %g %g\n",&up.x,&up.y,&up.z);
      fscanf(file,"angle %g\n",&angle);
      fscanf(file,"hither %g\n",&hither);
      fscanf(file,"resolution %d %d\n",&resX,&resY);

      if (camera) delete camera;
      camera = new PerspectiveCamera(pos,at,up,angle,resX,resY);

      continue;
    }

    if(!strcmp(token,"vo")) {
      // FORMAT:
      //     v
      //     from %g %g %g
      //     at %g %g %g
      //     up %g %g %g
      //     angle %g
      //     hither %g
      //     resolution %d %d
      Vec3f pos, at, up;
      float angle, hither;
      int resX, resY;
      fscanf(file,"from %g %g %g\n",&pos.x,&pos.y,&pos.z);
      fscanf(file,"at %g %g %g\n",&at.x,&at.y,&at.z);
      fscanf(file,"up %g %g %g\n",&up.x,&up.y,&up.z);
      fscanf(file,"angle %g\n",&angle);
      fscanf(file,"hither %g\n",&hither);
      fscanf(file,"resolution %d %d\n",&resX,&resY);

      if (camera) delete camera;
      camera = new OrthogonalCamera(pos,at,up,angle,resX,resY);
      continue;
    }

        if(!strcmp(token,"vd")) {
      // FORMAT:
      //     v
      //     from %g %g %g
      //     at %g %g %g
      //     up %g %g %g
      //     angle %g
      //     hither %g
      //     resolution %d %d
      Vec3f pos, at, up;
      float angle, hither;
      int resX, resY;
      float lensRadius;
      float focalDistance;


      fscanf(file,"from %g %g %g\n",&pos.x,&pos.y,&pos.z);
      fscanf(file,"at %g %g %g\n",&at.x,&at.y,&at.z);
      fscanf(file,"up %g %g %g\n",&up.x,&up.y,&up.z);
      fscanf(file,"angle %g\n",&angle);
      fscanf(file,"lensRadius %g\n",&lensRadius);
      fscanf(file,"focalDistance %g\n",&focalDistance);
      fscanf(file,"resolution %d %d\n",&resX,&resY);

      if (camera) delete camera;

      camera = new DepthOfFieldCamera(pos,at,up,angle,resX,resY,lensRadius,focalDistance);

      continue;
    }

    if(!strcmp(token,"s")) {
      Vec3f pos;
      float rad;
      sscanf(str,"s %g %g %g %g",&pos.x,&pos.y,&pos.z,&rad);
      groupToAddTo->Add(new Sphere(pos,rad,currentShader));
      continue;
    }

    if(!strcmp(token,"b")) {
      sscanf(line,"b %g %g %g",&bgColor.x,&bgColor.y,&bgColor.z);
      continue;
    }

    if(!strcmp(token,"p")) {
      int vertices;
      sscanf(str,"p %d",&vertices);
      Vec3f *vertex = new Vec3f[vertices];
      for(int i=0; i<vertices; i++)
	      fscanf(file,"%g %g %g\n",&vertex[i].x,&vertex[i].y,&vertex[i].z);

      assert(currentShader != NULL);
      for(int i=2; i<vertices; i++)
	      groupToAddTo->Add(new Triangule(vertex[0], vertex[i-1], vertex[i], currentShader));

      delete[] vertex;
      continue;
    }

    if(!strcmp(token,"include")) {
      if (!fgets(line,LINESIZE,file)) {
	cerr << " error in include, cannot read filename to include" << endl;
	exit(0);
      }
      line[strlen(line)-1] = 0; // remove trailing eol indicator '\n'
      cout << "including file " << line << endl;

      Group *subGroup = new Group;
      groupToAddTo->Add(subGroup);
      ParseNFF(NULL,line,subGroup);
      continue;
    }

    if(!strcmp(token,"includeO")) {
      if (!fgets(line,LINESIZE,file)) {
	cerr << " error in include, cannot read filename to include" << endl;
	exit(0);
      }
      line[strlen(line)-1] = 0; // remove trailing eol indicator '\n'
      cout << "including off file " << line << endl;

      OffObject mesh;

      if(!mesh.loadObject(line,0))
        cout << "Erro ao abrir " << line << endl;

      for (int j=0;j<mesh.getNumberTriangles();++j)
      {
          int v0 = mesh.getTriangleObject(j).getIndex(0);
          int v1 = mesh.getTriangleObject(j).getIndex(1);
          int v2 = mesh.getTriangleObject(j).getIndex(2);

          Vec3f v = mesh.getVertex( v0 );
          Vec3f u = mesh.getVertex( v1 );
          Vec3f w = mesh.getVertex( v2 );
          cout << " v= " << v ;
          cout << " u= " << v ;
          cout << " w= " << v ;
          cout << endl;
          groupToAddTo->Add(new Triangule(v, u, w,  currentShader));
      }
      continue;
    }

    if(!strcmp(token,"f")) {
      float r,g,b,kd,ks,shine,t,ior;

      sscanf(str,"f %g %g %g %g %g %g %g %g\n", &r,&g,&b,&kd,&ks,&shine,&t,&ior);
      Vec3f color(r,g,b);
      color = color * kd;
      currentShader = new PhongShader(color, 0.2, kd, ks, shine,this,t,ior);
      //currentShader = new FlatShader(color);
      //currentShader = new DiffuseShader(color);
      continue;
    }

    if(!strcmp(token,"fl")) {
      float r,g,b;

      sscanf(str,"fl %g %g %g \n", &r,&g,&b);
      Vec3f color(r,g,b);
      //currentShader = new PhongShader(color, 0.2, kd, ks, shine,this,t,ior);
      currentShader = new FlatShader(color);
      //currentShader = new DiffuseShader(color);
      continue;
    }

    if(!strcmp(token,"flam")) {
      float r,g,b;

      sscanf(str,"flam %g %g %g \n", &r,&g,&b);
      Vec3f color(r,g,b);
      //currentShader = new PhongShader(color, 0.2, kd, ks, shine,this,t,ior);
      //currentShader = new FlatShader(color);
      currentShader = new DiffuseShader(color);
      continue;
    }



    if(!strcmp(token,"l"))
    {
      // light source
      Vec3f pos, col;
      int num = sscanf(line,"l %g %g %g %g %g %g", &pos.x,&pos.y,&pos.z,&col.x,&col.y,&col.z);
      PointLight * pointLight;
      if(num == 3) {
	      // light source with position only
	      col = Vec3f(0,1,1);
	      pointLight=new PointLight(pos,col,NULL,new FlatShader(col));
	      lights.push_back(pointLight);
        groupToAddTo->Add(pointLight);
      }
      else
        if(num == 6) {
          pointLight=new PointLight(pos,col,NULL,new FlatShader(col));
          lights.push_back(pointLight);
          //groupToAddTo->Add(pointLight);
        }
        else
          cout << "error in " << fileName << " : " << line << endl;
      continue;
    }

    cout << "error in " << fileName << " : " << line << endl;
    exit(0);
  }
  if(fileToUse)
    fclose(file);
}
