all: 
	g++ -c  InfinitePlane.cc Image.cc Sphere.cc Group.cc PhongShader.cc Scene.cc PerspectiveCamera.cc Triangle.cc orthoCamera.cc Render.cpp DepthOfField.cc
	g++ -s  InfinitePlane.o Image.o Sphere.o Group.o PhongShader.o Scene.o PerspectiveCamera.o Triangle.o  Render.o orthoCamera.o DepthOfField.o -o Render  -O3 -lrt 

render:
	g++ -c Render.cpp
	g++ -s  InfinitePlane.o Image.o Sphere.o Group.o PhongShader.o Scene.o PerspectiveCamera.o Triangle.o  Render.o orthoCamera.o DepthOfField.o -o Render  -O3 -lrt 

scene:
	g++ -c Scene.cc
	g++ -s  InfinitePlane.o Image.o Sphere.o Group.o PhongShader.o Scene.o PerspectiveCamera.o Triangle.o  Render.o orthoCamera.o DepthOfField.o -o Render  -O3 -lrt 
phong:
	g++ -c PhongShader.cc
	g++ -s  InfinitePlane.o Image.o Sphere.o Group.o PhongShader.o Scene.o PerspectiveCamera.o Triangle.o  Render.o orthoCamera.o DepthOfField.o -o Render  -O3 -lrt 

clean:
	rm *.o Render
