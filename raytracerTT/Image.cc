#include "Image.hh"

int Image::SavePPM(char *fileName)
{
  FILE *outfile;

  // Open a file to write
  outfile = fopen(fileName, "w");

  // Verifing the file
  if (outfile == NULL)
    {
      fprintf(stderr, "ERROR: Image::SavePPM couldn't open file '%s' for output.\n",fileName);
      return 0;
    }

  // Putting the data

  fprintf(outfile, "P3\n");  // Heading
  fprintf(outfile, "%d	%d	255", resX, resY); // width, height and maximum color value

  int position;

  // Put the Colors for each pixel
  for(int contY=0; contY < resY; contY++)
    for(int contX=0; contX < resX; contX++)
      {
	position = contY*resX + contX;
	if ((position % 8) == 0)
	  fprintf(outfile,"\n");

	fprintf(outfile, "%d %d %d ", (int) data[position].x, (int) data[position].y, (int) data[position].z);
      }

  // Close the file
  fclose(outfile);

  // All is correct...
  return 1;
}

void Image::Clear()
{
  for(int contY=0; contY < resY; contY++)
    for(int contX=0; contX < resX; contX++)
      {
	int position = contY*resX + contX;

	data[position].x = 0;
	data[position].y = 0;
	data[position].z = 0;
      }
}

void Image::SetValuePixel(int x, int y, Vec3f &color)
{
  int position = (resY - y - 1)*resX + x;

  data[position].x = (255.0f * color.x);
  data[position].y = (255.0f * color.y);
  data[position].z = (255.0f * color.z);

  data[position].x = data[position].x > 255 ? 255 : data[position].x ;
  data[position].y = data[position].y > 255 ? 255 : data[position].y ;
  data[position].z = data[position].z > 255 ? 255 : data[position].z ;
}
