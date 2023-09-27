#include "precomp.h"
#include "Renderer.h"

#include "Sphere.h"
#include "Material.h"
#include "pow4Sphere.h"
#include "Primitive.h"
#include "Object.h"
#include "Scene.h"


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace Tmpl8;

#define NUMTHREADS 12
#define MULTITHREADING_NO_POOL    0
#define MULTITHREADING_WITH_POOL  1
#define NO_MULTITHREADING         0
#define DEPHT_OF_FIELD            false

//tiles
#if MULTITHREADING_NO_POOL

const int numTilesx = 4;
const int numTilesy = 4;
const int TileW = ScreenWidth / numTilesx;
const int TileH = ScreenHeight / numTilesy;

#endif // MULTITHREADING_NO_POOL 
#if MULTITHREADING_WITH_POOL
std::thread threads[NUMTHREADS];


struct Tile
{
	 bool completed = false;
	std::atomic<int> pixelX = -1;
	std::atomic<int> pixelY = -1;
};

Tile tiles[16];
std::atomic<bool> finishedAll = false;

//width height ratio should be same as screenW and Height
const int TileW = 8;
const int TileH = 8;
const int numTilesx = ScreenWidth / TileW;
const int numTilesy = ScreenHeight / TileH;
const int totalNumberOfTiles = numTilesx * numTilesy;

#endif // Multithreading with a pool

void Renderer::Init()
{
	m_ColorHandling.Init();

	// saving the camera's position and rotation when closing the program
	// and having this same position when starting the program
	float camData[6] = { 5.21678, 0.905972,- 15.1318,0.196905, 3.47946, 0 };
	std::string Line;
	std::fstream MyFile("save.txt");
	if(MyFile.is_open())
	{
		int i = 0;
		while (std::getline(MyFile, Line))
		{
			camData[i] = std::stof(Line);

			i++;
		}
		MyFile.close();

	}
		m_Cam.SetPosition({camData[0], camData[1] , camData[2] });
		m_Cam.SetRotation({ camData[3], camData[4] , camData[5] });

}

MathUtil::vec3 Renderer::CalcColor(MathUtil::Ray& ray, Scene& theScene, int depth)
{
	if (depth < 0)
		return MathUtil::vec3(0.f);

	MathUtil::HitData hd;
	if(theScene.Intersect(ray, hd))
	{

		MathUtil::Ray scatteredRay;
		MathUtil::vec3 attenuation;
		
		if (!hd.material->ScatteredRay(ray, hd, attenuation, scatteredRay))
			return attenuation ;

		if (m_Debugkey == true)
			return hd.Normal;

		return attenuation * CalcColor(scatteredRay, theScene, depth - 1) + attenuation * SampleLight(hd, theScene);
	}
	//skybox
	if (theScene.GetTexture() != nullptr)
	{
		MathUtil::vec2 UV = GetUVfromDir(ray.Direction);
		return theScene.GetTexture()->SampleTexture(UV) * 1.f;
	}
	else return theScene.GetSkyColor();
}

//for hdr uv
MathUtil::vec2 Renderer::GetUVfromDir(const MathUtil::vec3& rd)
{

	float u = 0.5f + atan2(rd.x, rd.z) / (2 * MathUtil::PI);
	float v = 0.5f - asin(rd.y) / MathUtil::PI;
	u = MathUtil::Clamp(u, 0.f, 1.f);
	v = MathUtil::Clamp(v, 0.f, 1.f);

	return MathUtil::vec2(u,v);
}

// adding colors that are affected by direct lightsampling
MathUtil::vec3 Renderer::SampleLight( const MathUtil::HitData& hd, Scene& theScene)
{
	MathUtil::Ray scatteredRay;
	std::vector<MathUtil::vec3> colors = theScene.IntersectLight(hd);

	if (colors.size() < 0)
		return MathUtil::vec3(0.f);
	MathUtil::vec3 attenuation = MathUtil::vec3(0.f);

	MathUtil::vec3 color = MathUtil::vec3(0.f);
	for (const auto& lightcolor : colors)
	{
		
		color += lightcolor;
	}
	return color;
	
}

//keys for camera
void Renderer::UpdateCam(float deltaTime, bool keys[])
{
	MathUtil::vec3 rotation(0.f);
	float sens = 0.8f;
	//wsda
	if (keys[82])
	{
		rotation.x -= 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[81])
	{
		rotation.x += 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[80])
	{
		rotation.y -= 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[79])
	{
		rotation.y += 1.f;
		m_ColorHandling.ResetFrame();
	}
	// q and e
	if (keys[8])
	{
		rotation.z += 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[20])
	{
		rotation.z -= 1.f;
		m_ColorHandling.ResetFrame();
	}
	rotation *= deltaTime * sens;
	MathUtil::vec3 camRotation = m_Cam.GetRotation();
	camRotation += rotation;
	m_Cam.SetRotation(camRotation);
	
	MathUtil::vec3 movement(0.f);
	if (keys[4])
	{
		movement.x -= 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[7])
	{
		movement.x += 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[225])
	{
		movement.y -= 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[44])
	{
		movement.y += 1.f;
		m_ColorHandling.ResetFrame();
	}
	// q and e
	if (keys[22])
	{
		movement.z += 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (keys[26])
	{
		movement.z -= 1.f;
		m_ColorHandling.ResetFrame();
	}
	if (movement.sqrLentgh() > 0.1f)
	{
		movement.normalize();
		movement = movement * m_Cam.GetView();
		MathUtil::vec3 camPosition = m_Cam.GetPosition();
		camPosition += movement * deltaTime;
		m_Cam.SetPosition(camPosition);

	}
	if (keys[87]) { m_Debugkey = true; m_ColorHandling.ResetFrame();};
	if (keys[86])
	{
		
		m_Debugkey = false;
		m_ColorHandling.ResetFrame();
	}
	
	
}

#if MULTITHREADING_WITH_POOL
void Renderer::RenderTile(int i, Scene& scene)
{
	while(!finishedAll)
	{
		if (!tiles[i].completed) 
		{
			int startx = tiles[i].pixelX * TileW;
			int starty = tiles[i].pixelY * TileH;

			//loop through one tile
			for (int x = startx; x < startx + TileW; x++)
			{
				for (int y = starty; y < starty + TileH; y++)
				{
					
					MathUtil::vec3 outColor(0.f);
					MathUtil::vec2 UV(static_cast<float>(x) * OneOverWidth, 1.f
						- static_cast<float>(y) * OneOverHeight);

					MathUtil::Ray ray = m_Cam.ShootRay(UV, DEPHT_OF_FIELD);
					MathUtil::vec3 color = CalcColor(ray, scene, m_Depht);
					m_ColorHandling.MapAndSampleColor(m_Screen,color, outColor, x, y, 10.f);
				}
			}
			tiles[i].completed = true;
		}
	}
}

static bool Initializedthreads = false;
void Renderer::tilePool(Scene& scene)
{
	bool AlltilesDone= false;
	int indexTile = 0;
	finishedAll = false;
	
	for (int i = 0; i < NUMTHREADS; i++)
	{
	
		if (indexTile < totalNumberOfTiles)
		{
			int x = indexTile / numTilesx;
			int y = indexTile % numTilesx;
			
			tiles[i].pixelX = x;
			tiles[i].pixelY = y;
			tiles[i].completed = false;
			if(Initializedthreads!=true)
				threads[i] = std::thread([this, &scene, i]() {RenderTile(i, scene); });

			indexTile++;
		}

	}
	Initializedthreads = true;
	while (!AlltilesDone) 
	{
		for (int i = 0; i < NUMTHREADS; i++)
		{
		
			if (tiles[i].completed && indexTile < totalNumberOfTiles)
			{
					
				int x = indexTile / numTilesx;
				int y = indexTile % numTilesx;
				tiles[i].pixelX = x;
				tiles[i].pixelY = y;
				tiles[i].completed = false;

				indexTile++;
			}
		}
		
		if (indexTile >= totalNumberOfTiles)
		{
			AlltilesDone = true;
		}
	}
}
#endif // MULTITHREADING_WITH_POOL

#if MULTITHREADING_NO_POOL
void Renderer::RenderOneTile(int tx, int ty, Scene& scene)
{
	int startx = ty * TileW;
	int starty = tx * TileH;

	//loop through one tile
	for (int x = startx; x < startx + TileW; x++)
	{
		for (int y = starty; y < starty + TileH; y++)
		{
			MathUtil::vec3 outColor(0.f);

			MathUtil::vec2 UV(static_cast<float>(x) * OneOverWidth, 1.f - static_cast<float>(y) * OneOverHeight);
			MathUtil::Ray ray = m_Cam.ShootRay(UV, DEPHT_OF_FIELD);
			MathUtil::vec3 color = CalcColor(ray, scene, m_Depht);
			m_ColorHandling.MapAndSampleColor( m_Screen,color, outColor, x, y, 7.f);
		}
	}
}
#endif // MULTITHREADING_NO_POOL

void Renderer::Render(float deltaTime, bool keys[], Scene& scene)
{
	UpdateCam(deltaTime, keys);

#if MULTITHREADING_WITH_POOL
	tilePool(scene);


#elif NO_MULTITHREADING

	for (int y = 0; y < ScreenHeight; y++)
	{
		for (int x = 0; x < ScreenWidth; x++)
		{
			MathUtil::vec3 outColor(0.f);

			MathUtil::vec2 UV(x * OneOverWidth, 1.f - y * OneOverHeight);
			MathUtil::Ray ray = m_Cam.ShootRay(UV, DEPHT_OF_FIELD);
			MathUtil::vec3 color = CalcColor(ray, scene, m_Depht);
			m_ColorHandling.MapAndSampleColor(m_Screen,color, outColor, x, y, 7.f);
		}
	}
#elif MULTITHREADING_NO_POOL
	std::thread threads[numTilesx][numTilesy];
	//loop through initializing thread
	for (int x = 0; x < numTilesx; x++)
	{
		for (int y = 0; y < numTilesy; y++) 
		{
			threads[x][y] = std::thread([this, &scene, x, y]() {RenderOneTile(x,y, scene); });
		}
	}

	//join all threads for cleanup
	for (int x = 0; x < numTilesx; x++)
	{
		for (int y = 0; y < numTilesy; y++)
		{
			threads[x][y].join();
		}
	}

#endif // MULTITHREADING_NO_POOL

}
//CAMERA pos and rot saved
void Renderer::ShutDown(Scene& scene)
{
#if MULTITHREADING_WITH_POOL
	finishedAll = true;
	for (int i = 0; i < NUMTHREADS; i++)
	{
			threads[i].join();
	}
#endif
	scene.Delete();
	std::ofstream myFile;
	myFile.open("save.txt");
	myFile << m_Cam.GetPosition().x << "\n" << m_Cam.GetPosition().y << "\n" << m_Cam.GetPosition().z << "\n";
	myFile << m_Cam.GetRotation().x << "\n" << m_Cam.GetRotation().y << "\n" << m_Cam.GetRotation().z << "\n";
	myFile.close();

}