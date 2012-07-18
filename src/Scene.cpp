#include "Scene.h"

#include "Material.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include "Trimesh.h"
#include "Light.h"

void Scene::createTestScene()
{
	Material* sjakkMat = new MatPhongChequered( vec3(1.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f) );
	Material* blackWhiteChequerMat = new MatChequered( vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f), 5.f );
	Material* normalMat = new MatNormal;
	Material* phongMat = new MatPhong( vec3(0.9f, 0.4f, 0.1f) );
	Material* greenMat = new MatPhong( vec3(0.1f, 0.9f, 0.1f) );
	
	Material* reflPhongMat = new MatPhong( vec3(0.8f, 0.8f, 0.8f) );
	reflPhongMat->isReflective = true;

	Material* refrPhongMat = new MatPhong( vec3(0.7f, 0.7f, 1.0f) );
	//refrPhongMat->isTransparent = true;
	refrPhongMat->isReflective = true;

	materials.push_back(sjakkMat);
	materials.push_back(blackWhiteChequerMat);
	materials.push_back(normalMat);
	materials.push_back(phongMat);
	materials.push_back(reflPhongMat);
	materials.push_back(refrPhongMat);
	materials.push_back(greenMat);

	// Add objects to the scene
	
	Plane *plane = new Plane( plane_t (vec3(0.f,1.0f,0.f), vec3(0.f, 1.f, 0.f))); // TODO. get epsilon working at origin
	plane->mat = sjakkMat;
	primitives.push_back( plane );

	//float dist = 50.0f;
	//Plane *plane2 = new Plane( plane_t (vec3(+dist,0.0f,0.f), vec3(1.f, 0.f, 0.f))); // TODO. get epsilon working at origin
	//plane2->mat = sjakkMat;
	//primitives.push_back( plane2 );

	//Plane *plane3 = new Plane( plane_t (vec3(-dist,0.0f,0.f), vec3(-1.f, 0.f, 0.f))); // TODO. get epsilon working at origin
	//plane3->mat = sjakkMat;
	//primitives.push_back( plane3 );

	//Plane *plane4 = new Plane( plane_t (vec3(0.f,0.f,+dist), vec3(0.f, 0.f, 1.f))); // TODO. get epsilon working at origin
	//plane4->mat = sjakkMat;
	//primitives.push_back( plane4 );

	//Plane *plane5 = new Plane( plane_t (vec3(0.f,0.0f,-dist), vec3(0.f, 0.f, -1.f))); // TODO. get epsilon working at origin
	//plane5->mat = sjakkMat;
	//primitives.push_back( plane5 );

	//vec3 boxPos( 2.f, 0.f, 0.f );
	//Box *box = new Box( boxPos+vec3(-.5f, -.5f, -.5f), boxPos+vec3(.5f, .5f, .5f) );
	//box->mat = normalMat;
	//primitives.push_back( box );

	Sphere *sphere = new Sphere( vec3(0.f, 0.f,0.f), 1.4f );
	sphere->mat = refrPhongMat;
	primitives.push_back( sphere );

	Sphere *sphere2 = new Sphere( vec3(0.f, 0.8f+0.8f,0.f), 0.4f );
	sphere2->mat = refrPhongMat;
	primitives.push_back( sphere2 );

	//Monkey *monkey = new Monkey;
	//monkey->mat = reflPhongMat;
	//primtives.push_back( monkey );
	
	//std::vector<vec3> tris;
	//tris.push_back( vec3(-.5f,-.5f,0.f) );
	//tris.push_back( vec3(+.5f,-.5f,0.f) );
	//tris.push_back( vec3(0.f,+0.5f,0.f) );
	//Trimesh* singleTriangle = new Trimesh( tris );
	//singleTriangle->mat = phongMat;
	//primitives.push_back( singleTriangle );

	int pointsInCircle = 8;
	for ( int i=0; i<pointsInCircle; i++ ){
		float a = 2.f*(float)M_PI * i/float(pointsInCircle);
		float radi = 3.f;
		const vec3 pos(cos(a)*radi,0.0f,sin(a)*radi);
		Sphere *sphere = new Sphere( pos, 0.4f );
		Material* sphereMat = new MatPhong( vec3( .5f+.5f*cos(a), 0.f, .5f+.5f*sin(a) ) );
		//sphereMat->isReflective = false;
		materials.push_back( sphereMat );
		sphere->mat = sphereMat;
		primitives.push_back( sphere );
	}

	

	//int pointsInLine = 20.f;
	//for ( int i=0; i<pointsInLine; i++ ){
	//	float pos =  i/float(pointsInLine);
	//	Sphere *sphere = new Sphere( vec3(-3.0f + 3.f*pos,1.32f,0.f), 0.32f );
	//	sphere->mat = phongMat;
	//	primitives.push_back( sphere );
	//}

	auto constColor = new MatConstant( vec3(1.f) ); materials.push_back(constColor);
	constColor->castsShadow = false;

	auto light0 = new Light_t( vec3(0.f, 2.f, -4.f) ); lights.push_back( light0 );
	auto light1 = new Light_t( vec3(0.f, 2.1f,-4.f) ); lights.push_back( light1 );
	//auto light2 = new Light_t( vec3(+2.f, 2.f, -2.f) ); lights.push_back( light2 );
	//auto light3 = new Light_t( vec3(+2.f, 2.f, +2.f) ); lights.push_back( light3 );
	
	auto lightSphere0 = new Sphere( light0->pos, 0.1f ); 
	auto lightSphere1 = new Sphere( light1->pos, 0.1f ); 
	//auto lightSphere2 = new Sphere( light2->pos, 0.1f ); 
	//auto lightSphere3 = new Sphere( light3->pos, 0.1f ); 

	lightSphere0->mat = constColor; primitives.push_back(lightSphere0);
	lightSphere1->mat = constColor; primitives.push_back(lightSphere1);
	//lightSphere2->mat = constColor; primitives.push_back(lightSphere2);
	//lightSphere3->mat = constColor; primitives.push_back(lightSphere3);
	
}