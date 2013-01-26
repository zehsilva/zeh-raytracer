#ifndef TEXTURE_HXX
#define TEXTURE_HXX

#include <Image.hxx>

class Texture : public Image
{
public:
	Texture(int x = 0, int y = 0) 
	 : Image(x, y) 
	 {};
	~Texture();

	Vec3f GetTexel(float u, float v);
};

Texture *ReadPPM(char *filename);

#endif










