#include <Texture.hxx>
#include <math.h>
using namespace std;

Vec3f Texture::GetTexel(float u, float v)
{
	float	u_, v_;
	u_ = floor(u);
	v_ = floor(v);
	u_ = u - u_;
	v_ = v - v_;

	int	x = (int)(u_ * resX);
	int y = resY - (int)(v_ * resY) - 1;

	return GetValue(x, y);
}

void eatComments(FILE *f)
{
  int ch;
  while((ch=getc(f))=='#')
    {
      char str[1000];
      fgets(str,1000,f);
    }
  ungetc(ch,f);
}

void eatWhitespace(FILE *f)
{
  int ch;
  ch=getc(f);
  while(ch==' ' || ch=='\t' || ch=='\n' || ch=='\f' || ch=='\r')
    {
      ch=getc(f);
    }
  ungetc(ch,f);
}

Texture* ReadPPM(char *fileName)
{
  cout << "Reading ppm image " << fileName << endl;

  FILE *f;
  char ch;
  int width, height, colres;
  Texture *tex;

  f = fopen(fileName,"r");

  if (f == NULL) {
    cerr << "ReadPPM : could not open file '" << fileName <<endl;
    exit(-1);
  }

  char str[100];
  eatWhitespace(f);
  eatComments(f);
  eatWhitespace(f);

  fscanf(f,"%s",str);
  if (!strcmp(str,"P3")) {
    eatWhitespace(f);
    eatComments(f);
    eatWhitespace(f);
    fscanf(f,"%d %d",&width,&height);
    if(width<=0 || height<=0)
      {
	printf("Error: width and height of the image must be greater than zero. File: %s\n",fileName);
	exit(1);
      }
    eatWhitespace(f);
    eatComments(f);
    eatWhitespace(f);
    fscanf(f,"%d",&colres);

    tex = new Texture(width, height);

    ch=0;
    while(ch!='\n') fscanf(f,"%c",&ch);

    for (int y=0;y<height;y++) {
      for (int x=0;x<width;x++) {
	int c[3];
	fscanf(f,"%d %d %d",c+0,c+1,c+2);

	Vec3f color = Vec3f( float(c[0]) / float(colres), float(c[1]) / float(colres), float(c[2]) / float(colres));

	tex->SetValuePixel(x, y, color);
      }
    }
    fclose(f);
  } else {
    cerr << "Wrong format !" << endl;
    exit(0);
  }
  return tex;
}






