#include <GL/glfw3.h>
#include <cstdio>
#include <iostream>
#include <vector>

#include "raytracer.h"

int main()
{
	if ( glfwInit() != GL_TRUE ) 
	{
		std::cerr << "glfw error:" << glfwErrorString(glfwGetError()) << std::endl;
		return 1;
	}

	int width = 600;
	int height = 600;
	GLFWwindow wnd = glfwOpenWindow(width, height, GLFW_WINDOWED, "R First Raytracer", nullptr);

	if (!wnd)
	{
		fprintf(stderr, "Failed to open GLFW window: %s\n",
			glfwErrorString(glfwGetError()));
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(wnd);

	
	{ 
		using std::vector;
		vector<vector<Pixel_t>> pixels(width, height);
		trace( pixels, width, height );

		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
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
	
	
	glfwSwapBuffers();

	bool running = true;
	while ( running ) {

		//glfwPollEvents();
		glfwWaitEvents();
		if ( glfwGetKey(wnd, GLFW_KEY_ESC ) ) running = false;
	}


	//glfwCloseWindow(wnd); //crash
	glfwTerminate();

	return 0;
}