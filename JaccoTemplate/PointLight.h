#pragma once

namespace Tmpl8
{
	class PointLight
	{
	public:
		vec3 Direction = { 0.f, -1.f,-0.25f };
		vec3 Position = { 0.f,-10.f,0.f };
		void init() { Direction.normalize(); }
	};
}

