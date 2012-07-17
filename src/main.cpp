#include <GL/glfw3.h>
#include <cstdio>
#include <iostream>
#include <vector>

#include "raytracer.h"

void drawImage(const std::vector<std::vector<Pixel_t>> &pixels, int width, int height)
{
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width,height,0,-1.0f, 1.f);
	glMatrixMode(GL_MODELVIEW); // doesnt matter if we are modelview or proj, as were not using translate, push, pop, etc.

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

	int width = 200;
	int height = 200;
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

	double traceStart = glfwGetTime();
	trace( pixels, width, height );
	double timeSpent = glfwGetTime() - traceStart;
	std::cout << "spent " << timeSpent << " seconds in trace" << std::endl;


	bool running = true;
	while ( running ) {
		drawImage(pixels, width, height);
		glfwSwapBuffers();
		glfwPollEvents();
		if ( glfwGetKey(wnd, GLFW_KEY_ESC ) ) running = false;
	}


	//glfwCloseWindow(wnd); //crash
	glfwTerminate();

	return 0;
}