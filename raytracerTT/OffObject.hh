// ==========================================================================

#ifndef NEWINTERACTIVEBASE_OFFOBJECT_HH
#define NEWINTERACTIVEBASE_OFFOBJECT_HH


#include <iostream>
#include <fstream>
#include <vector>
#include <GL/gl.h>
#include "Vec3f.hh"


class OffVertex {
public:
	OffVertex( float _f0=0, float _f1=0, float _f2=0, float _n0=0, float _n1=0, float _n2=0 )
		: v(_f0,_f1,_f2), n(_n0,_n1,_n2)
	{}


	friend std::istream& operator>>(std::istream& _is, OffVertex& _v) {
		_is >> _v.v[0] >> _v.v[1] >> _v.v[2];

		return _is;
	}

	friend std::ostream& operator<<(std::ostream& _os, OffVertex& _v)  {
		_os << _v.v[0] << ' ' << _v.v[1] << ' ' << _v.v[2];
		return _os;
	}

	//!! order here is important!
	//changed to work with gcc-3.2
	Vec3f v;  // vertex
	Vec3f n;  // normal
};


class OffTriangle {
public:
	OffTriangle( int _i0=0, int _i1=0, int _i2=0 )
		: d_i0(_i0), d_i1(_i1), d_i2(_i2)
	{}

	friend std::istream& operator>>(std::istream& _is, OffTriangle& _t) {
		_is >> _t.d_i0 >> _t.d_i1 >> _t.d_i2;
		return _is;
	}

	friend std::ostream& operator<<(std::ostream& _os, OffTriangle& _t)  {
		_os << "3 " << _t.d_i0 << ' ' << _t.d_i1 << ' ' << _t.d_i2;
		return _os;
	}

	int getIndex( int i ) const {
		assert( i >= 0 && i <= 2 );

		switch(i) {
case 0:
	return d_i0;
case 1:
	return d_i1;
case 2:
	return d_i2;
		}

		return -1;
	}

	unsigned int d_i0, d_i1, d_i2; // indices to vertex buffer
	Vec3f n; // face normal
};

class OffObject {
public:
	// default constructor
	OffObject() :
	  vertices(),
		  triangles(),
		  vertexArray(NULL),
		  normalArray(NULL),
		  texArray(NULL),
		  indexArray(NULL),
		  scale(1.),
		  doCenter(true)
	  {};

	  // constructor (if size = 0.0, no scaling, only centering!)
	  OffObject( const char *filename, float size = 1.0, bool _doCenter = true ) :
	  vertices(),
		  triangles(),
		  vertexArray(NULL),
		  normalArray(NULL),
		  texArray(NULL),
		  indexArray(NULL),
		  scale(1.),
		  doCenter( _doCenter )
	  {
		  bool res;
		  res = loadObject( filename, size );
		  if (res==false)
		  {
			  std::cerr<<"ERROR: OFF file does not exist!"<<std::endl;
			  // exit(-1);
		  }
	  };

	  // copy-constructor
	  OffObject(const OffObject &off) {
		  vertices = off.vertices;
		  triangles = off.triangles;
		  faceNormals = off.faceNormals;
		  texcoords = off.texcoords;
		  doCenter = off.doCenter;

		  // keep them empty, will be generate on demand
		  vertexArray = NULL;
		  normalArray = NULL;
		  texArray = NULL;
		  indexArray = NULL;
	  }

	  // operator=
	  OffObject& operator=(const OffObject &off) {
		  vertices = off.vertices;
		  triangles = off.triangles;
		  faceNormals = off.faceNormals;
		  texcoords = off.texcoords;

		  if( vertexArray ) {
			  delete[] vertexArray;
		  }
		  if( normalArray ) {
			  delete[] normalArray;
		  }
		  if( texArray ) {
			  delete[] texArray;
		  }
		  if( indexArray ) {
			  delete[] indexArray;
		  }
		  vertexArray = NULL;
		  normalArray = NULL;
		  indexArray = NULL;
		  texArray = NULL;

		  return *this;
	  }

	  // desconstructor
	  virtual ~OffObject() {
		  if( vertexArray ){
			  delete [] vertexArray;
		  }
		  if( normalArray ){
			  delete [] normalArray;
		  }
		  if( indexArray ){
			  delete [] indexArray;
		  }
	  }

	  // get trangle object i
	  const OffTriangle &getTriangleObject( int i ) {
		  return triangles[i];
	  }


	  // add triangle (with three new vertices)
	  //!! Note, that the Index- and VertexArrays will NOT be updated automatically
	  int addTriangle( const Vec3f &v0,
		  const Vec3f &v1,
		  const Vec3f &v2 ) {
			  int i[3];
			  i[0] = addVertex( v0 );
			  i[1] = addVertex( v1 );
			  i[2] = addVertex( v2 );

			  triangles.push_back( OffTriangle( i[0], i[1], i[2] ) );
			  return triangles.size()-1;
	  }

	  // add triangle (i.e. only indices)
	  //!! Note, that the Index- and VertexArray will NOT be updated automatically
	  int addTriangle( const int i0,
		  const int i1,
		  const int i2 ) {
			  triangles.push_back( OffTriangle( i0, i1, i2 ) );
			  return triangles.size()-1;
	  }

  // add vertex, return index to vertex
  int addVertex( const Vec3f &v )
  {
    bool found = false;
    int idx = -1;
    for(unsigned int a=0; a<vertices.size(); a++)
      {
	if( (vertices[a].v-v).norm() < 0.0001)
	  {
	    found = true;
	    idx = a;
	  }
      }

    if(found)
      return idx;
    else
      {
	vertices.push_back( OffVertex(v[0], v[1], v[2]) );
	texcoords.push_back( Vec3f( 0.0, 0.0, 0.0 ) );
	return vertices.size()-1;
      }
  }

  // add vertex + nor + tex, return index to vertex
  int addVertex( const Vec3f &v, const Vec3f &n )
  {
    vertices.push_back( OffVertex(v[0], v[1], v[2], n[0], n[1], n[2]) );
    texcoords.push_back( Vec3f( 0.0, 0.0, 0.0 ) );
    return vertices.size()-1;
  }

	  // add vertex + nor + tex, return index to vertex
// 	  int addVertex( const Vec3f &v, const Vec3f &n, const Vec3f &t ) {
// 		  vertices.push_back( OffVertex(v[0], v[1], v[2], n[0], n[1], n[2]) );
// 		  texcoords.push_back( t );
// 		  return vertices.size()-1;
// 	  }

	  // set vertex i
	  void setVertex( const int i, const Vec3f &v ) {
		  vertices[i].v = v;
	  }

	  // set normal i
	  void setNormal( const int i, const Vec3f &n ) {
		  vertices[i].n = n;
	  }

	  // set tex coord
	  void setTexCoord( const int i, const Vec3f &t ) {
		  texcoords[i] = t;
	  }

	  // get vertex object i
	  const OffVertex &getVertexObject( int i ) {
		  return vertices[i];
	  }

	  // query number of triangles
	  int getNumberTriangles() const {
		  return triangles.size();
	  }

	  // query number of vertices
	  int getNumberVertices() const {
		  return vertices.size();
	  }

	  // get vertex i
	  const Vec3f &getVertex( int i ) {
		  return vertices[i].v;
	  }

	  // get normal for vertex i
	  const Vec3f &getNormal( int i ) {
		  return vertices[i].n;
	  }

	  // return face normal
	  const Vec3f &getFaceNormal( int i ) {
		  return faceNormals[i];
	  }

	  // return face normal
	  const Vec3f &getTexCoords( int i ) {
		  return texcoords[i];
	  }

	  // return vertex/normal array
	  //!! not supported anymore!!!
	  void *getVertexNormalArray() {
		  assert( !"getVertexNormalArray not supported anymore!" );
		  return (void*)NULL;
	  }

	  // return normal/vertex array (used for OpenGL rendering)
	  void *getNormalVertexArray() {
		  return (void *)&vertices[0];
	  }

	  // return vertex array (used for OpenGL rendering)
	  void *getVertexArray() {
		  if( !vertexArray ) createVertexArray();
		  return (void *)vertexArray;
	  }

	  // return normal array (used for OpenGL rendering)
	  void *getNormalArray() {
		  if( !normalArray ) createNormalArray();
		  return (void *)normalArray;
	  }

	  // return tex coords array (used for OpenGL rendering)
	  void *getTexArray() {
		  if( !texArray ) createTexArray();
		  return (void *)texArray;
	  }

	  // return index array (used for OpenGL rendering)
	  void *getIndexArray() {
		  if( !indexArray ) createIndexArray();
		  return (void *)indexArray;
	  }

	  // create the vertex array and copy vertices over
	  // (automatically called if array is needed)
	  void createVertexArray() {
		  // delete old array if already exists
		  if( vertexArray ) delete[] vertexArray;

		  // create new array
		  vertexArray = new Vec3f[getNumberVertices()];

		  for( int i = 0; i < getNumberVertices(); ++i ) {
			  vertexArray[i] = vertices[i].v;
		  }
	  }

	  // create the normal array and copy normals over
	  void createNormalArray() {
		  // delete old array if already exists
		  if( normalArray ) delete[] normalArray;

		  // create new array
		  normalArray = new Vec3f[getNumberVertices()];

		  for( int i = 0; i < getNumberVertices(); ++i ) {
			  normalArray[i] = vertices[i].n;
		  }
	  }

	  // create the tex array and copy tex coords over
	  void createTexArray() {
		  // delete old array if already exists
		  if( texArray ) delete[] texArray;

		  // create new array
		  texArray = new Vec3f[getNumberVertices()];

		  for( int i = 0; i < getNumberVertices(); ++i ) {
			  texArray[i] = texcoords[i];
		  }
	  }

	  // create the index array and copy indices overaz
	  void createIndexArray() {
		  long num_triangles = getNumberTriangles();

		  if(num_triangles <= 0)
			  return;

		  if(indexArray)
			  delete [] indexArray;

		  /* create index array */
		  indexArray = new int[num_triangles*3];

		  for( int i=0; i < num_triangles; ++i ) {
			  indexArray[i*3  ] = triangles[i].d_i0;
			  indexArray[i*3+1] = triangles[i].d_i1;
			  indexArray[i*3+2] = triangles[i].d_i2;
		  }
	  }
	  // just to be backward compatible
	  void recreateIndexArray() {
		  createIndexArray();
	  }

	  // set the GL array pointers (all of them, if not yet generated, do generate)
	  // does NOT enable client state.
	  void setGLArrayPointers() {
		  glVertexPointer( 3, GL_FLOAT, 0, (float*)getVertexArray() );
		  glNormalPointer( GL_FLOAT, 0, (float*)getNormalArray() );
		  glTexCoordPointer( 3, GL_FLOAT, 0, (float*)getTexArray() );
	  }

	  // clear the normal, vertex, texcoord, index arrays
	  // this is needed, *after* you add vertices/triangles/texcoords
	  // or after moving parts of the geometry.
	  void clearArrays() {
		  if( vertexArray )
			  delete[] vertexArray;
		  if( normalArray )
			  delete[] normalArray;
		  if( texArray )
			  delete[] texArray;
		  if( indexArray )
			  delete[] indexArray;

		  vertexArray = NULL;
		  normalArray = NULL;
		  texArray = NULL;
		  indexArray = NULL;
	  }

	  // If vertices one vertex is used multiple times,
	  // you have to create a new triangle vector.
	  void clearTriangleVector() {
		  triangles.clear();
	  }

	  // save object
	  bool saveObjectInvNorm( const char *filename ) {
		  int i;
		  std::ofstream out( filename ); // open it

		  if( !out ) {
			  std::cerr << "Couldn't open " << filename << " for saving OFF file." << std::endl;
			  return false;
		  }

		  out << "OFF" << std::endl;
		  out << getNumberVertices() << " " << getNumberTriangles() << " 0" << std::endl;

		  for( i=0; i<getNumberVertices(); ++i ) {
			  out << vertices[i] << std::endl;
		  }
		  for( i=0; i<getNumberTriangles(); ++i ) {
			  //out << triangles[i] << std::endl;
			  out << "3 "<< triangles[i].d_i0 << " " << triangles[i].d_i2 << " " << triangles[i].d_i1 << std::endl;
		  }

		  out.close();

		  return true;
	  }

	  // save object
	  bool saveObject( const char *filename ) {
		  int i;
		  std::ofstream out( filename ); // open it

		  if( !out ) {
			  std::cerr << "Couldn't open " << filename << " for saving OFF file." << std::endl;
			  return false;
		  }

		  out << "OFF" << std::endl;
		  out << getNumberVertices() << " " << getNumberTriangles() << " 0" << std::endl;

		  for( i=0; i<getNumberVertices(); ++i ) {
			  out << vertices[i] << std::endl;
		  }
		  for( i=0; i<getNumberTriangles(); ++i ) {
			  out << triangles[i] << std::endl;
			  //out << "3 "<< triangles[i].d_i0 << " " << triangles[i].d_i2 << " " << triangles[i].d_i1 << std::endl;
		  }

		  out.close();

		  return true;
	  }

	  // load object
	  // centers object and
	  // also resizes object if desired
	  bool loadObjectOBJ( char *filename, float size = 1.0 ) {
		  GLMmodel *model = glmReadOBJ( filename );
		  GLMtriangle *triangle;
		  unsigned int i;

		  assert(model);
		  assert(model->vertices);

		  vertices.reserve( model->numvertices );
		  texcoords.reserve( model->numvertices );
		  triangles.reserve( model->numtriangles );
		  faceNormals.reserve( model->numtriangles );

		  // create some vertices & texcoords
		  for( i = 0; i <= model->numvertices; ++i ) {
			  vertices.push_back( OffVertex(0.0,0.0,0.0, 0.0,0.0,0.0) );
			  texcoords.push_back( Vec3f( 0.0, 0.0, 0.0 ) );
		  }

		  for( i = 0; i < model->numtriangles; i++ ) {
			  triangle = &model->triangles[i];
			  int idx;

			  // now set the actual vertex/vertex-normals and texcoord.
			  for( int t = 0; t < 3; t++ ) {
				  idx = triangle->vindices[t] - 1;
				  int vidx = triangle->vindices[t];
				  vertices[idx].v[0] = model->vertices[vidx*3 + 0];
				  vertices[idx].v[1] = model->vertices[vidx*3 + 1];
				  vertices[idx].v[2] = model->vertices[vidx*3 + 2];

				  if( model->normals ) { // potentially no normals
					  int nidx = triangle->nindices[t];
					  vertices[idx].n[0] = model->normals[nidx*3 + 0];
					  vertices[idx].n[1] = model->normals[nidx*3 + 1];
					  vertices[idx].n[2] = model->normals[nidx*3 + 2];
				  }

				  if( model->texcoords ) { // potentially no tex-coords
					  int tidx = triangle->tindices[t];
					  texcoords[idx][0] = model->texcoords[tidx*2 + 0];
					  texcoords[idx][1] = model->texcoords[tidx*2 + 1];
					  texcoords[idx][2] = 0.0;
				  }
			  }

			  Vec3f fn;
			  if( model->facetnorms ) {
				  idx = triangle->findex;
				  fn = Vec3f( model->facetnorms[idx*3 + 0],
					  model->facetnorms[idx*3 + 1],
					  model->facetnorms[idx*3 + 2] );
			  } // TODO: what to	 do if none available
			  faceNormals.push_back( fn );
			  OffTriangle tri( triangle->vindices[0]-1,
				  triangle->vindices[1]-1,
				  triangle->vindices[2]-1 );
			  tri.n = fn;
			  triangles.push_back( tri );
		  }

		  if( !model->normals ) {
			  calculateNormals();
		  }

		  // center object
		  //centerObject( size );

		  glmDelete(model);  // dumps core
		  return true;
	  }

	  // load object
	  // centers object and
	  // also resizes object if desired
	  // normals (face and vertex) are also computed
	  bool loadObject( const char *filename, float size = 1.0 ) {
		  std::ifstream in( filename ); // open it
		  if( !in ) return false;

		  triangles.clear();
		  vertices.clear();

		  //char c1,c2,c3;
		  int num_vertices, num_triangles, dummy, i;
		  OffVertex v;
		  OffTriangle t;

		  // read off while ignoring comments
		  char l[1024];
		  do
		  {
			  in.getline(l,1024);
		  }
		  while (l[0]=='#');

		  if( l[0] != 'O' || l[1] != 'F' || l[2] != 'F' ) {
			  std::cerr << "no OFF input.\n";
			  return false;
		  }

		  //    // read off
		  //    in >> c1 >> c2 >> c3;
		  //    if( c1 != 'O' || c2 != 'F' || c3 != 'F' ) {
		  //      std::cerr << "no OFF input.\n";
		  //      return false;
		  //    }

		  // how many vertices and triangles do we have
		  in >> num_vertices >> num_triangles >> dummy;
		  vertices.reserve( num_vertices );
		  texcoords.reserve( num_vertices );
		  triangles.reserve( num_triangles );
		  faceNormals.reserve( num_triangles );

		  // get all the vertices
		  for( i=0; i<num_vertices; ++i ) {
			  in >> v;
			  vertices.push_back( v );
			  texcoords.push_back( Vec3f( 0.0, 0.0, 0.0 ) );
		  }

		  // Every triangle consist of three indices to vertices
		  for( i=0; i<num_triangles; ++i ) {
			  in >> dummy >> t;
			  if( dummy == 3 ) {
				  triangles.push_back( t );
			  }
			  else {
				  std::cerr << "polygon is not a triangle.\n";
				  return false;
			  }
		  }

		  in.close();

		  // now calculate the face normals
		  calculateNormals();

		  // center object
		  //centerObject( size );

		  // create index array
		  /* changed: do not create by default, only on demand!
		  indexArray = new int[num_triangles*3];
		  for( i=0; i < num_triangles; ++i ) {
		  indexArray[i*3  ] = triangles[i].d_i0;
		  indexArray[i*3+1] = triangles[i].d_i1;
		  indexArray[i*3+2] = triangles[i].d_i2;
		  }
		  */
		  return true;
	  };

	  // flip all normals
	  void flipNormals() {
		  for(unsigned int i = 0; i < vertices.size(); i++ ) {
			  vertices[i].n = -vertices[i].n;
		  }
	  }

	  // go over all the triangles and calculate face and vertex normals
	  void calculateNormals() {
		  std::vector<OffTriangle>::iterator tIt;
		  faceNormals.clear();

		  for( tIt=triangles.begin(); tIt!=triangles.end(); ++tIt ) {
			  OffTriangle *tri;
			  OffVertex *v0, *v1, *v2;

			  tri = &(*tIt);

			  float f1_x, f1_y, f1_z;
			  float f2_x, f2_y, f2_z;
			  Vec3f n;

			  v0 = &vertices[tri->d_i0];
			  v1 = &vertices[tri->d_i1];
			  v2 = &vertices[tri->d_i2];

			  f1_x = v1->v[0] - v0->v[0];
			  f1_y = v1->v[1] - v0->v[1];
			  f1_z = v1->v[2] - v0->v[2];

			  f2_x = v2->v[0] - v0->v[0];
			  f2_y = v2->v[1] - v0->v[1];
			  f2_z = v2->v[2] - v0->v[2];

			  n[0] = f1_y*f2_z - f1_z*f2_y;
			  n[1] = f1_z*f2_x - f1_x*f2_z;
			  n[2] = f1_x*f2_y - f1_y*f2_x;

			  if( n.norm() != 0.0 )
				  n.normalize();
			  else
				  n = Vec3f( 1.0, 0.0, 0.0 );

			  // store in triangle and in faceNormals array
			  faceNormals.push_back( n );
			  tri->n = n;

			  // add face normal to all vertex normals
			  v0->n += n;
			  v1->n += n;
			  v2->n += n;
		  }

		  // now go over all vertices and normalize vertex normals
		  for( int i = 0; i < getNumberVertices(); i++ ) {
			  if( vertices[i].n.norm() != 0.0 )
				  vertices[i].n.normalize();
			  else
				  vertices[i].n = Vec3f( 1.0, 0.0, 0.0 );
		  }
	  }

	  // compute bounding box and then center object
	  // also scale it down to a 'size' sized cube!
	  void centerObject( float size = 1.0 ) {
		  std::vector<OffVertex>::iterator vIt;


		  for( int i = 0; i < 3; i++ ) {
			  bbMin[i] =  99999999.9f;
			  bbMax[i] = -99999999.9f;
		  }

		  // compute bounding box (axis aligned)
		  for( vIt=vertices.begin(); vIt != vertices.end(); ++vIt ) {
			  if( (*vIt).v[0] < bbMin[0] ) bbMin[0] = (*vIt).v[0];
			  if( (*vIt).v[0] > bbMax[0] ) bbMax[0] = (*vIt).v[0];

			  if( (*vIt).v[1] < bbMin[1] ) bbMin[1] = (*vIt).v[1];
			  if( (*vIt).v[1] > bbMax[1] ) bbMax[1] = (*vIt).v[1];

			  if( (*vIt).v[2] < bbMin[2] ) bbMin[2] = (*vIt).v[2];
			  if( (*vIt).v[2] > bbMax[2] ) bbMax[2] = (*vIt).v[2];
		  }

		  origbbMin = bbMin;
		  origbbMax = bbMax;

		  // translate (x,y,z)
		  float tx, ty, tz;
		  tx = (bbMax[0] + bbMin[0])/2;
		  ty = (bbMax[1] + bbMin[1])/2;
		  tz = (bbMax[2] + bbMin[2])/2;

		  // scale (x,y,z)
		  float sx, sy, sz;
		  sx = bbMax[0] - bbMin[0];
		  sy = bbMax[1] - bbMin[1];
		  sz = bbMax[2] - bbMin[2];

#define fmax( a, b ) ((a>b)?a:b)

		  // scale only if size != 0.0
		  if( size != 0.0 ) {
			  scale = size/fmax( fmax( sx, sy ), sz );
		  } else {
			  scale = 1.0;
		  }

		  if ( !doCenter) {
			  tx = 0.; ty = 0.; tz = 0.;
		  }

		  for( vIt=vertices.begin(); vIt != vertices.end(); ++vIt ) {
			  (*vIt).v[0] = ((*vIt).v[0]-tx)*scale;
			  (*vIt).v[1] = ((*vIt).v[1]-ty)*scale;
			  (*vIt).v[2] = ((*vIt).v[2]-tz)*scale;
		  }
		  bbMax[0] = (bbMax[0] -tx)*scale;
		  bbMax[1] = (bbMax[1] -ty)*scale;
		  bbMax[2] = (bbMax[2] -tz)*scale;

		  bbMin[0] = (bbMin[0] -tx)*scale;
		  bbMin[1] = (bbMin[1] -ty)*scale;
		  bbMin[2] = (bbMin[2] -tz)*scale;

	  };

	  // Store all the vertices and triangles in here
	  std::vector<OffVertex> vertices;      // vertices (pos + normal)
	  std::vector<OffTriangle> triangles;   // triangle indices

	  Vec3f *vertexArray;
	  Vec3f *normalArray;
	  Vec3f *texArray;
	  int *indexArray;
	  float scale;
	  bool doCenter;

	  Vec3f bbMin, bbMax;
	  Vec3f origbbMin, origbbMax;

	  std::vector<Vec3f> faceNormals; // face normal
	  std::vector<Vec3f> texcoords;  // texture coordinates
};

#endif
