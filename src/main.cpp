#include <GL/glfw3.h>
#include <cstdio>
#include <iostream>
#include <vector>

#include "raytracer.h"
#include "vec_math.h"
#include "Scene.h"

void drawImage( const std::vector<std::vector<Pixel_t>> &pixels, int width, int height)
{	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width,height,0,-1.0f, 1.f);
	glMatrixMode(GL_MODELVIEW); // doesnt matter if we are modelview or proj, as were not using translate, push, pop, etc.

	//glDrawPixels(width, height, GL_RGB  , GL_UNSIGNED_BYTE, &(pixies[0]) ); // hardly an faster

	
	glBegin(GL_POINTS);
	for ( int i=0; i<width; i++ ){
	for ( int j=0; j<height; j++ ){
		glColor3ubv( &(pixels[i][j].r) );
		glVertex2i(i,j);
	}}
	glEnd();
	
}

int main()
{
	if ( glfwInit() != GL_TRUE ) 
	{
		std::cerr << "glfw error:" << glfwErrorString(glfwGetError()) << std::endl;
		return 1;
	}

	int width = 600;
	int height = 400;
	GLFWwindow wnd = glfwOpenWindow(width, height, GLFW_WINDOWED, "R First Raytracer", nullptr);

	if (!wnd)
	{
		fprintf(stderr, "Failed to open GLFW window: %s\n",
			glfwErrorString(glfwGetError()));
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(wnd);
	glfwSwapInterval(1); // vsync

	std::vector<std::vector<Pixel_t>> pixels(width, height);

	Scene myScene;
	myScene.createTestScene();

	bool reTrace = true;
	bool running = true;
	while ( running ) {

		if ( reTrace ) 
		{
			reTrace = false;
			int mouse_x, mouse_y;
			glfwGetMousePos(wnd, &mouse_x, &mouse_y);
			float normalized_mouse_x = mouse_x / float(width);
			float normalized_mouse_y = mouse_y / float(height);
			float TWO_PI = 2.f * float(M_PI);

			float zoom = 0.5f + normalized_mouse_y * 10.f;
			// Create easy camera definition, and the basis vectors (u,v,n)
			Camera_t camera;
			camera.position = vec3( cos( TWO_PI*normalized_mouse_x)*zoom, 0.5f, sin(TWO_PI*normalized_mouse_x)*zoom );
			camera.lookAt =   vec3(0.f, 0.f, 0.f);
			camera.lookUp =   vec3(0.f, 1.f, 0.f);
	
			
			double traceStart = glfwGetTime();
			std::cout << "Start trace..." << std::endl;
			trace( myScene, camera, pixels, width, height );
			double timeSpent = glfwGetTime() - traceStart;
			std::cout << "spent " << timeSpent << " seconds in trace" << std::endl;
		}

		drawImage(pixels, width, height);
		glfwSwapBuffers();
		glfwPollEvents();
		if ( glfwGetKey(wnd, GLFW_KEY_ESC ) ) running = false;

		if ( glfwGetKey(wnd, GLFW_KEY_SPACE ) ) reTrace = true;
	}


	//glfwCloseWindow(wnd); //crash
	glfwTerminate();

	return 0;
}