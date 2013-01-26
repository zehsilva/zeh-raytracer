#ifndef VOLUMEREGION_HXX
#define VOLUMEREGION_HXX

#include "Shader.hxx"
#include "Vec3f.hxx"
#include "Triangle.hxx"
#include "Box.hxx"

class VolumeRegion{

	//************************************************ Fields **************************************************
	
	protected:
	
	//This is the constant value for the isotropic phase function
	static const float phase_isotropic = 1.0f / (4.0f * M_PI);
	
	//This is the scene, used to shade
	Scene* scene;
	
	//Triangle used to set in the ray
	Triangle* help_prim;
	
	//The boundings of the region
	Box boundingBox;
	
	//********************************************** Constructors **********************************************
	
	public:
	
	/* This constructor sets the values of the fields as the Object constructor. The difference is the fact
	 * that no acceleration structure is needed */
	VolumeRegion(Scene* scene) : scene(scene){
		boundingBox = Box();
		help_prim = new Triangle(Vec3f(1.0f), Vec3f(1.0f), Vec3f(1.0f));
	}
	
	
	/* This constructor sets the bounding box of the volume to the given bounds */
	VolumeRegion(Vec3f min, Vec3f max, Scene* scene) : scene(scene){
		boundingBox = Box();
		boundingBox.Extend(min);
		boundingBox.Extend(max);
		help_prim = new Triangle(Vec3f(1.0f), Vec3f(1.0f), Vec3f(1.0f));
	}
	
	
	//Destructor
	~VolumeRegion(){
		delete help_prim;
	}
	
	
	//**************************************************** Methods ***********************************************
	
	/* Since this object is defined by its bounds, these are not calculated but only returned */
	Box CalcBounds();
	
	/* Method to set the minimum value of the boundings of the volume */
	void SetMin(Vec3f min);
	
	/* Method to set the maximum value of the boundings of the volume */
	void SetMax(Vec3f max);
	
	/* This Method is like the Intersect method of the Object class. There is an intersection if the bounding box is hit */
	bool Intersect(Ray &ray, Shader* shader);
	
	
	/* This Method returns the value for a isotropic phase function */
	Vec3f Phase_iso(const Vec3f w, const Vec3f v);
	
	
	/* This Methods calculates the value for the Henyey-Greenstein Phase Function. */
	Vec3f Phase_phg(const Vec3f w, const Vec3f v, float g);
	
	
	/* This Method calculates the value for the Schlick Phase function, which avoids the powf call in the PHG Funtion */
	Vec3f Phase_schlick(const Vec3f w, const Vec3f v, float g);
	
	
	/* This method returns 'true' if the point p lies within the volume and false otherwise */
	inline bool IsWithin(const Vec3f p){
		return (boundingBox.min.x() <= p.x() && boundingBox.max.x() >= p.x() &&
				boundingBox.min.y() <= p.y() && boundingBox.max.y() >= p.y() &&
				boundingBox.min.z() <= p.z() && boundingBox.max.z() >= p.z());
	}
	
	
	/* This method intersects the ray with the bounds of the volume and sets t0 and t1 to the intersection
	 * times. It is also determined if the ray hits some obect within the volume. If this is the case the value
	 * of t1 is adopted. */
	bool Intersect(const Ray &ray, float &t0, float &t1);
	
		
	/* Method to get the absorption cross section value of the volume media at the given point and for the given direction */
	virtual Vec3f GetSigma_A(const Vec3f p, const Vec3f dir) = 0;
	
	
	/* Method to get the scattering coefficient of the media at the given point and for the given direction */
	virtual Vec3f GetSigma_S(const Vec3f p, const Vec3f dir) = 0;
	
	
	/* Method to get the extinction coefficient of the volume media at the given point and for the given direction */
	virtual Vec3f GetSigma_T(const Vec3f p, const Vec3f dir) = 0;
	
	
	/* Method to get the volume emission function value for the volume at the given point and for the given direction */
	virtual Vec3f GetLve(const Vec3f p, const Vec3f dir) = 0;
	
	
	/* Method to get the optical thickness for the volume measured in the given ray direction and calculated by the step width and offset.
	 * The test argument can be used to skip the test if the ray travels during [t0, t1] throug the region without hitting something*/
	virtual Vec3f GetTau(const Ray ray, float &tmin, float &tmax, float step = 1.0f, float offset = 0.5f, bool test = true) = 0;
	
};

#endif
