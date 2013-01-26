#ifndef IMAGE_HXX
#define IMAGE_HXX

#include <iostream>
#include <fstream>
#include "Vec3f.hh"

class Image
{
public:
  int resX, resY;
  Vec3f* data;

  // Constructor
  Image()
  {};

  // Constructor
  Image(int rX, int rY)
	: resX(rX), resY(rY)
  {
    data = new Vec3f[resY * resX]; // Creating a list of lines
  }

  int SavePPM(char *fileName);

  void Clear();

  void SetValuePixel(int x, int y, Vec3f &data);

  // Destructor
  ~Image()
  {
    delete []data;
	}
};

#endif



