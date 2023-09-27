#include "vec.h"

namespace MathUtil
{
	vec3 normalize(const vec3& v) { return v.normalized(); }
	vec3 sqrsub(const vec3& a, const vec3& b)
	{
		return (a - b).sqrLentgh();
	}
	float DistanceBetween(const vec3& a, const vec3& b)
	{
		return sqrtf(sqrsub(a, b).x + sqrsub(a, b).y + sqrsub(a, b).z);
	}
	float sign(float val) { return (0.f > val) ? -1.f : 1.f; }
	vec3 Randomvec3(float min, float max) { return vec3{ Rand(max) - min , Rand(max) - min, Rand(max) - min }; }
	int RandSeed = rand();
	float Fast_RandFloat()
	{
		RandSeed *= 16807;
		return (float)RandSeed * 4.6566129e-010f;
	}
	vec2 RandWithinUnitDisc()
	{
		vec2 p;
		float a = Fast_RandFloat() * 2.f * PI;
		p = vec2(cosf(a), sinf(a));

		return p;
	}
	vec3 FastRand()
	{
		return vec3{ Fast_RandFloat(), Fast_RandFloat(), Fast_RandFloat() };
	}
	vec3 RandWithinUnitSphere()
	{
		bool thisIsLoop = true;
		vec3 p = 0;
		while (thisIsLoop)
		{
		
			p = FastRand();
			thisIsLoop = p.sqrLentgh() >= 1.f;
		}
		return p;
	}
	vec3 cross(const vec3& a, const vec3& b) { return a.cross(b); }
	float dot(const vec3& a, const vec3& b) { return a.dot(b); }
	vec3 operator * (const float& s, const vec3& v) { return vec3(v.x * s, v.y * s, v.z * s); }
	vec3 operator * (const vec3& v, const float& s) { return vec3(v.x * s, v.y * s, v.z * s); }
	vec4 operator * (const float& s, const vec4& v) { return vec4(v.x * s, v.y * s, v.z * s, v.w * s); }
	vec4 operator * (const vec4& v, const float& s) { return vec4(v.x * s, v.y * s, v.z * s, v.w * s); }
	vec4 operator * (const vec4& v, const mat4& M)
	{
		vec4 mx(M.cell[0], M.cell[4], M.cell[8], M.cell[12]);
		vec4 my(M.cell[1], M.cell[5], M.cell[9], M.cell[13]);
		vec4 mz(M.cell[2], M.cell[6], M.cell[10], M.cell[14]);
		vec4 mw(M.cell[3], M.cell[7], M.cell[11], M.cell[15]);
		return v.x * mx + v.y * my + v.z * mz + v.w * mw;
	}

	mat4::mat4()
	{
		memset(cell, 0, 64);
		cell[0] = cell[5] = cell[10] = cell[15] = 1;
	}

	mat4 mat4::identity()
	{
		mat4 r;
		memset(r.cell, 0, 64);
		r.cell[0] = r.cell[5] = r.cell[10] = r.cell[15] = 1.0f;
		return r;
	}

	mat4 mat4::rotate(const vec3 l, const float a)
	{
		// http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation
		mat4 M;
		const float u = l.x, v = l.y, w = l.z, ca = cosf(a), sa = sinf(a);
		M.cell[0] = u * u + (v * v + w * w) * ca, M.cell[1] = u * v * (1 - ca) - w * sa;
		M.cell[2] = u * w * (1 - ca) + v * sa, M.cell[4] = u * v * (1 - ca) + w * sa;
		M.cell[5] = v * v + (u * u + w * w) * ca, M.cell[6] = v * w * (1 - ca) - u * sa;
		M.cell[8] = u * w * (1 - ca) - v * sa, M.cell[9] = v * w * (1 - ca) + u * sa;
		M.cell[10] = w * w + (u * u + v * v) * ca;
		M.cell[3] = M.cell[7] = M.cell[11] = M.cell[12] = M.cell[13] = M.cell[14] = 0, M.cell[15] = 1;
		return M;
	}
	mat4 mat4::rotatex(const float a)
	{
		mat4 M;
		const float ca = cosf(a), sa = sinf(a);
		M.cell[5] = ca, M.cell[6] = -sa;
		M.cell[9] = sa, M.cell[10] = ca;
		return M;
	}
	mat4 mat4::rotatey(const float a)
	{
		mat4 M;
		const float ca = cosf(a), sa = sinf(a);
		M.cell[0] = ca, M.cell[2] = sa;
		M.cell[8] = -sa, M.cell[10] = ca;
		return M;
	}
	mat4 mat4::rotatez(const float a)
	{
		mat4 M;
		const float ca = cosf(a), sa = sinf(a);
		M.cell[0] = ca, M.cell[1] = -sa;
		M.cell[4] = sa, M.cell[5] = ca;
		return M;
	}
	mat4 operator * (const mat4& M1, const mat4& M2)
	{
		mat4 m;
		m[0] = M1.cell[0] * M2.cell[0] + M1.cell[1] * M2.cell[4] + M1.cell[2] * M2.cell[8] + M1.cell[3] * M2.cell[12];
		m[1] = M1.cell[0] * M2.cell[1] + M1.cell[1] * M2.cell[5] + M1.cell[2] * M2.cell[9] + M1.cell[3] * M2.cell[13];
		m[2] = M1.cell[0] * M2.cell[2] + M1.cell[1] * M2.cell[6] + M1.cell[2] * M2.cell[10] + M1.cell[3] * M2.cell[14];
		m[3] = M1.cell[0] * M2.cell[3] + M1.cell[1] * M2.cell[7] + M1.cell[2] * M2.cell[11] + M1.cell[3] * M2.cell[15];

		m[4] = M1.cell[4] * M2.cell[0] + M1.cell[5] * M2.cell[4] + M1.cell[6] * M2.cell[8] + M1.cell[7] * M2.cell[12];
		m[5] = M1.cell[4] * M2.cell[1] + M1.cell[5] * M2.cell[5] + M1.cell[6] * M2.cell[9] + M1.cell[7] * M2.cell[13];
		m[6] = M1.cell[4] * M2.cell[2] + M1.cell[5] * M2.cell[6] + M1.cell[6] * M2.cell[10] + M1.cell[7] * M2.cell[14];
		m[7] = M1.cell[4] * M2.cell[3] + M1.cell[5] * M2.cell[7] + M1.cell[6] * M2.cell[11] + M1.cell[7] * M2.cell[15];

		m[8] = M1.cell[8] * M2.cell[0] + M1.cell[9] * M2.cell[4] + M1.cell[10] * M2.cell[8] + M1.cell[11] * M2.cell[12];
		m[9] = M1.cell[8] * M2.cell[1] + M1.cell[9] * M2.cell[5] + M1.cell[10] * M2.cell[9] + M1.cell[11] * M2.cell[13];
		m[10] = M1.cell[8] * M2.cell[2] + M1.cell[9] * M2.cell[6] + M1.cell[10] * M2.cell[10] + M1.cell[11] * M2.cell[14];
		m[11] = M1.cell[8] * M2.cell[3] + M1.cell[9] * M2.cell[7] + M1.cell[10] * M2.cell[11] + M1.cell[11] * M2.cell[15];

		m[12] = M1.cell[12] * M2.cell[0] + M1.cell[13] * M2.cell[4] + M1.cell[14] * M2.cell[8] + M1.cell[15] * M2.cell[12];
		m[13] = M1.cell[12] * M2.cell[1] + M1.cell[13] * M2.cell[5] + M1.cell[14] * M2.cell[9] + M1.cell[15] * M2.cell[13];
		m[14] = M1.cell[12] * M2.cell[2] + M1.cell[13] * M2.cell[6] + M1.cell[14] * M2.cell[10] + M1.cell[15] * M2.cell[14];
		m[15] = M1.cell[12] * M2.cell[3] + M1.cell[13] * M2.cell[7] + M1.cell[14] * M2.cell[11] + M1.cell[15] * M2.cell[15];

		return m;

	}
	vec3 operator * (const vec3& v, const mat4& M)
	{
		vec3 r;
		r.x = v.x * M.cell[0] + v.y * M.cell[4] + v.z * M.cell[8];
		r.y = v.x * M.cell[1] + v.y * M.cell[5] + v.z * M.cell[9];
		r.z = v.x * M.cell[2] + v.y * M.cell[6] + v.z * M.cell[10];

		return r;
	}

	vec3::vec3(vec4 v) : x(v.x), y(v.y), z(v.z) {}





}