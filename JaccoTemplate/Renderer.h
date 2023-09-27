#pragma once
#include "vec.h"
#include "Ray.h"
#include "Camera.h"
#include "ColorHandler.h"
#include <vector>

namespace Tmpl8
{
	class Object;
	class Scene;
	struct HitData;
	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(Surface* screen) { m_Screen = screen; }

		void Init();
		void Render(float deltaTime, bool keys[], Scene& scene);
		void ShutDown(Scene& scene);

	private:

		//used with threadpool
		void tilePool(Scene& scene);
		void RenderTile(int, Scene&);

		//used without threadpool
		void RenderOneTile(int, int, Scene&);

		void UpdateCam(float deltaTime, bool keys[]);
	
		MathUtil::vec3 CalcColor(MathUtil::Ray& ray, Scene& scene, int depht);
		MathUtil::vec2 GetUVfromDir(const MathUtil::vec3& rd);
		MathUtil::vec3 SampleLight(const MathUtil::HitData& hd, Scene& scene);

		//key that shows the normals //+//
		bool m_Debugkey = false;

		float OneOverWidth = 1.f / ScreenWidth;
		float OneOverHeight = 1.f / ScreenHeight;

		int m_Depht = 35;

		Camera m_Cam;
		Surface* m_Screen;
		ColorHandler m_ColorHandling;

		
	};
	
	
};
