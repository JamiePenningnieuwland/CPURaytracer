#include "precomp.h"
#include "vec.h"
#include "game.h"
#include "Scene.h"
#include "surface.h"
#include "Primitive.h"
#include "Material.h"
#include "Sphere.h"
#include "Ellepsiod.h"
#include "pow4Sphere.h"

#include "Renderer.h"
#include <iostream>
#include<vector>

// determine sky -- only one can be defined
#define SKYBOX           1
#define BLACKSKY         0
#define STATIC_COLOR     0

//define which scene -- only one can be defined
#define BEST_SCENE       1
#define GREENGLASS       0
#define WEEK_6_SCENE     0
#define FRACTAL          0
#define  MANY_SPHERES    0
#define EMISSIVE_SPHERE  0

namespace Tmpl8
{
	Scene scene;
	Renderer* myRayTracer;

	//textures for primitives
	
	static Sprite* moon = new Sprite(new Surface("assets/moon.jpg"), 1);
	static Sprite* moon_normal = new Sprite(new Surface("assets/moon_normal.jpg"), 1);
	
	static Sprite* brick = new Sprite(new Surface("assets/castle_brick.jpg"), 1);
	static Sprite* normalmapbrick = new Sprite(new Surface("assets/normal_castle_brick.png"), 1);

	static Sprite* metalplate = new Sprite(new Surface("assets/metal_plate.png"), 1);
	static Sprite* metalnormal = new Sprite(new Surface("assets/metal_plate_normal.png"), 1);
	static Sprite*	metalmap = new Sprite(new Surface("assets/metal_plate_metalmap.png"), 1);

	static Sprite*	fabric = new Sprite(new Surface("assets/fabric_pattern_07_col_2_2k.png"), 1);
	static Sprite*	fabric_normal = new Sprite(new Surface("assets/fabric_pattern_07_nor_gl_2k.png"), 1);

	//only ice fractal
	void Game ::Fractal(MathUtil::vec3 centerpoint, float radius, int depth, std::string mat, MathUtil::vec3 parentcenter)
	{
		if (depth < 0)
			return;

		MathUtil::vec3 parentdir = MathUtil::normalize(parentcenter - centerpoint);
		float halfRadius = radius / 2.f;
		MathUtil::vec3 newCenter0;	
		MathUtil::vec3 newCenter1;
		MathUtil::vec3 newCenter2;
		MathUtil::vec3 newCenter3;
		MathUtil::vec3 newCenter4;
		MathUtil::vec3 newCenter5;
		newCenter0.x = centerpoint.x + radius+ halfRadius;
		newCenter0.y = centerpoint.y;
		newCenter0.z = centerpoint.z;
		MathUtil::vec3 dir0 = MathUtil::normalize(newCenter0 - centerpoint);
		

		newCenter1.x = centerpoint.x - radius- halfRadius;
		newCenter1.y = centerpoint.y;
		newCenter1.z = centerpoint.z;
		MathUtil::vec3 dir1 = MathUtil::normalize(newCenter1 - centerpoint);

		newCenter2.x = centerpoint.x;
		newCenter2.y = centerpoint.y + radius+ halfRadius;
		newCenter2.z = centerpoint.z;
		MathUtil::vec3 dir2 = MathUtil::normalize(newCenter2 - centerpoint);

		newCenter3.x = centerpoint.x;
		newCenter3.y = centerpoint.y - radius- halfRadius;
		newCenter3.z = centerpoint.z;
		MathUtil::vec3 dir3 = MathUtil::normalize(newCenter3 - centerpoint);

		newCenter4.x = centerpoint.x;
		newCenter4.y = centerpoint.y;
		newCenter4.z = centerpoint.z + radius+ halfRadius;
		MathUtil::vec3 dir4 = MathUtil::normalize(newCenter4 - centerpoint);

		newCenter5.x = centerpoint.x;
		newCenter5.y = centerpoint.y;
		newCenter5.z = centerpoint.z - radius- halfRadius;
		MathUtil::vec3 dir5 = MathUtil::normalize(newCenter5 - centerpoint);

		if(MathUtil::dot(dir0, parentdir) < 0.9f)
		{
			scene.CreateObject(new MathUtil::Sphere(newCenter0, halfRadius), mat);
			Fractal(newCenter0, halfRadius, depth - 1, mat, centerpoint);
		}
		if (MathUtil::dot(dir1, parentdir) < 0.9f) 
		{
			scene.CreateObject(new MathUtil::Sphere(newCenter1, halfRadius), mat);
			Fractal(newCenter1, halfRadius, depth - 1, mat, centerpoint);
		}
		if(MathUtil::dot(dir2, parentdir) < 0.9f)
		{
			scene.CreateObject(new MathUtil::Sphere(newCenter2, halfRadius), mat);
			Fractal(newCenter2, halfRadius, depth - 1, mat, centerpoint);
		}
		if(MathUtil::dot(-dir3, parentdir) < 0.9f)
		{

			scene.CreateObject(new MathUtil::Sphere(newCenter3, halfRadius), mat);
			Fractal(newCenter3, halfRadius, depth - 1, mat, centerpoint);
		}
		if (MathUtil::dot(dir4, parentdir) < 0.9f)
		{

			scene.CreateObject(new MathUtil::Sphere(newCenter4, halfRadius), mat);
			Fractal(newCenter4, halfRadius, depth - 1, mat, centerpoint);
		}
		if (MathUtil::dot(dir5, parentdir) < 0.9f)
		{
			scene.CreateObject(new MathUtil::Sphere(newCenter5, halfRadius), mat);
			Fractal(newCenter5, halfRadius, depth - 1, mat, centerpoint);
		}
	}
	
	//keys
	void Game::KeyDown(int key)
	{
	
		keys[key] = true;
	}
	void Game::KeyUp(int key)
	{
		keys[key] = false;
	}

	void Game::Init()
	{
#if SKYBOX
		//different skyboxes
		//give any texture you want (hdr)
		scene = Scene(new Texture("assets/umhlanga_sunrise_4k.hdr"));

#elif STATIC_COLOR
		
		// give any color you want
		scene = Scene(MathUtil::vec3(0.f,1.f,0.f));

#elif BLACKSKY
		scene = Scene(MathUtil::vec3(0.f, 0.f, 0.f));
#endif // BLACKSKY


		//some common materials

		//diffuse
		scene.CreateMaterial("white", new MathUtil::Diffuse(MathUtil::vec3(0.98f)));
		scene.CreateMaterial("pink", new  MathUtil::Diffuse(MathUtil::vec3(0.9f, 0.1f, 0.9f)));

		//glass
		scene.CreateMaterial("salmon", new MathUtil::Dielectric(MathUtil::vec3(1.f, .412f, .569f), 1.4f));
		scene.CreateMaterial("Glass", new MathUtil::Dielectric(MathUtil::vec3(0.98f), 1.4f));
		scene.CreateMaterial("greenglass", new MathUtil::Dielectric(MathUtil::vec3(0.82f, 1.f, 0.82f), 1.4f));
		scene.CreateMaterial("blueglass", new MathUtil::Dielectric(MathUtil::vec3(0.58f, 0.89f, 0.96f), 1.4f));


		//metallics
		scene.CreateMaterial("metal", new MathUtil::Metallic(MathUtil::vec3(0.4f), 0.2f));
		scene.CreateMaterial("bluemetal", new MathUtil::Metallic(MathUtil::vec3(0.4f, 0.1f,0.9f), 0.2f));
		scene.CreateMaterial("metalchecker", new MathUtil::MetalCheckerboard(MathUtil::vec3(0.8f), MathUtil::vec3(0.2f), 0.2f));
		
		
		//textures
		scene.CreateMaterial("brick", new Textured(brick, MathUtil::eMaterial::diffuse, normalmapbrick));
		scene.CreateMaterial("moon", new Textured(moon, MathUtil::eMaterial::diffuse, moon_normal));
		scene.CreateMaterial("rust", new Textured(metalplate , MathUtil::eMaterial::diffuse,metalnormal, metalmap));
		scene.CreateMaterial("fabric", new Textured(fabric, MathUtil::eMaterial::diffuse, fabric_normal, nullptr, 4.f));
		
		//emissive
		scene.CreateMaterial("lightBulb", new MathUtil::Emissive(MathUtil::vec3(0.98f, 0.79f, 0.03f), 10.f));
		scene.CreateMaterial("greylight", new MathUtil::Emissive(MathUtil::vec3(0.97f), 10.f));
		scene.CreateMaterial("BlueLight", new MathUtil::Emissive(MathUtil::vec3(0.078f, 0.207f, 1.f), 1.f));
		
#if BEST_SCENE
		//ground
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.f, -107.f, 0.f), 100.f), "metalchecker");

		scene.CreateObject(new MathUtil::Ellepsoid(MathUtil::vec3(5.f, -0.5f, 5.f), MathUtil::vec3(3.f, 2.f, 2.f)), "Glass");
		scene.CreateObject(new MathUtil::pow4Sphere(MathUtil::vec3(5.f, -5.0f, 5.f), 2.f), "salmon");

		scene.CreateObject(new MathUtil::pow4Sphere(MathUtil::vec3(-3.f, 1.f, 5.f), 2.f), "metal");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-3.f, -4.5f, 5.f), 3.f), "rust");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-2.f, -5.5f, -1.f), 1.5f), "fabric");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-8.f, -5.5f, 3.f), 2.f), "brick");
	
		scene.CreateObject(new MathUtil::Ellepsoid(MathUtil::vec3(2.5f, -5.5f, -2.f), MathUtil::vec3(1.f, 1.5f, 1.f)), "bluemetal");




#elif GREENGLASS
		scene.CreateMaterial("ground", new MathUtil::Diffuse(MathUtil::vec3(0.8f, 0.65f, 0.51f)));
	
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.f, -201.5f, 0.f), 200.f), "ground");

		//pile of glass orbs
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-1.0f, -1.0f, -1.0f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.0f, -1.0f, -1.0f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(1.0f, -1.0f, -1.0f), 0.5f), "greenglass");

		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-1.0f, -1.0f, -2.0f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.0f, 0.5f, -2.0f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(1.0f, -1.0f, -2.0f), 0.5f), "greenglass");

		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-1.0f, -1.0f, -3.0f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.0f, -1.0f, -3.0f), 0.5f),  "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(1.0f, -1.0f, -3.0f), 0.5f), "greenglass");

		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.5f, -0.25f, -1.5f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.5f, -0.25f, -2.5f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-0.5f, -0.25f, -1.5f), 0.5f), "greenglass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-0.5f, -0.25f, -2.5f), 0.5f), "greenglass");


#elif WEEK_6_SCENE

		scene.CreateObject(new MathUtil::pow4Sphere(MathUtil::vec3(0.f, -5.5f, 5.f), 2.f), "metal");
		scene.CreateObject(new MathUtil::pow4Sphere(MathUtil::vec3(0.f, -18.f, 5.f), 10.f), "white");
		scene.CreateObject(new MathUtil::Ellepsoid(MathUtil::vec3(0.f, -0.5f, 5.f), MathUtil::vec3(3.f, 2.f, 2.f)), "blueglass");
	


#elif FRACTAL
		
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.f, 0.f, 0.f), 2.f), "Glass");
		Fractal(MathUtil::vec3(0.f, 0.f, 0.f), 2.f, 2, "Glass", MathUtil::vec3(0.f, -1.f, 0.f));
		//ground
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.f, -101.f, 0.f), 100.f), "metalchecker");

#elif MANY_SPHERES
		for (int x = 0; x < 200; x++)
			for (int y = 0; y < 500; y++)
				//	//	for(int  z = 0; z < 42; z++)
			{
				scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.f + x * 2.5f, -8.f, -4.f + y * 2.5f), 1.f), "pink");
			}

#elif EMISSIVE_SPHERE
		scene.CreateMaterial("wall", new MathUtil::Checkerboard(MathUtil::vec3(0.8f, 0.65f, 0.51f), MathUtil::vec3(0.f)));
	
		//lights
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(4.5f, 2.5f, 6.f), 0.5f), "lightBulb");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-90.f, 200.f, 0.f), 100.f), "greylight");

		//ground
	   scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(0.f, -102.5f, 0.f), 100.f), "wall");
		
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(5.f, -0.5f, 5.f), 2.f), "Glass");
		scene.CreateObject(new MathUtil::Sphere(MathUtil::vec3(-1.f, -0.5f, 5.f), 2.f), "Glass");
#endif // EMISSIVE_SPHERE

		myRayTracer = new Renderer(screen);
		myRayTracer->Init();
		scene.Init();
	}
	void Game::Shutdown()
	{
		myRayTracer->ShutDown(scene);
	}

	void Game::Tick(float deltaTime)
	{
		myRayTracer->Render(deltaTime, keys, scene);

		//printing fps to screen
		float fps = 1.0f / deltaTime;
		char counter[20];
		sprintf(counter, "%f", fps);
		screen->Print(counter, 10, 10, 0xffffff);

	};

}