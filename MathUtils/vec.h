#pragma once
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

namespace MathUtil
{
	typedef unsigned int uint;

	inline float Rand(float range) { return (static_cast<float>(rand()) / RAND_MAX) * range; }
	inline int IRand(int range) { return rand() % range; }

	template <typename T>
	constexpr T Min(T a, T b) { return (a > b) ? b : a; }

	template <typename T>
	constexpr T Lerp(T a, T b, T t) { return a + t * (b - a); }
	template <typename T>
	constexpr T Max(T a, T b) { return (a > b) ? a : b; }

	template <typename T>
	constexpr T Clamp(T value, T min, T max) { return Min(max, Max(value, min)); }

	template <typename T>
	constexpr T Map(T  v, T min1, T max1, T min2, T max2) { return (v - min1) / (max1 - min1) * (max2 - min2) + min2; }

	constexpr float PI = 3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f;

	float sign(float val);
	class vec2 // adapted from https://github.com/dcow/RayTracer
	{
	public:
#pragma warning (push)
#pragma warning (disable:4201)
		union { struct { float x, y; }; float cell[2]; };
#pragma warning (pop)
		vec2() = default;
		vec2(float v) : x(v), y(v) {}
		vec2(float x, float y) : x(x), y(y) {}
		vec2 operator - () const { return vec2(-x, -y); }
		vec2 operator + (const vec2& addOperand) const { return vec2(x + addOperand.x, y + addOperand.y); }
		vec2 operator - (const vec2& operand) const { return vec2(x - operand.x, y - operand.y); }
		vec2 operator * (const vec2& operand) const { return vec2(x * operand.x, y * operand.y); }
		vec2 operator * (float operand) const { return vec2(x * operand, y * operand); }
		void operator -= (const vec2& a) { x -= a.x; y -= a.y; }
		void operator += (const vec2& a) { x += a.x; y += a.y; }
		void operator *= (const vec2& a) { x *= a.x; y *= a.y; }
		void operator *= (float a) { x *= a; y *= a; }
		float& operator [] (const int idx) { return cell[idx]; }
		float length() { return sqrtf(x * x + y * y); }

		float sqrLentgh() { return x * x + y * y; }
		vec2 normalized() { float r = 1.0f / length(); return vec2(x * r, y * r); }
		void normalize() { float r = 1.0f / length(); x *= r; y *= r; }
		static vec2 normalize(vec2 v) { return v.normalized(); }
		float dot(const vec2& operand) const { return x * operand.x + y * operand.y; }
	};
	class vec4;
	class vec3
	{
	public:
#pragma warning (push)
#pragma warning (disable:4201)
		union {
			float f[3];
			struct
			{
				float x;
				float y;
				float z;
				float dummy;
			};
			struct
			{
				float r;
				float g;
				float b;
				float dummy;
			};
			struct
			{
				float cell[4];
			};
		};
#pragma warning (pop)
		vec3() = default;
		vec3(float v) : x(v), y(v), z(v) {}
		vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		vec3(vec4 v);
		vec3 operator - () const { return vec3(-x, -y, -z); }
		vec3 operator + (const vec3& addOperand) const { return vec3(x + addOperand.x, y + addOperand.y, z + addOperand.z); }
		vec3 operator - (const vec3& operand) const { return vec3(x - operand.x, y - operand.y, z - operand.z); }
		vec3 operator * (const vec3& operand) const { return vec3(x * operand.x, y * operand.y, z * operand.z); }
		vec3 operator/(const vec3& other) const
		{
			return vec3(x / other.x, y / other.y, z / other.z);
		}
		vec3 operator/(const float& val) const
		{
			float v = 1.f / val;
			return vec3(x * v, y * v, z * v);
		}
		void operator -= (const vec3& a) { x -= a.x; y -= a.y; z -= a.z; }
		void operator += (const vec3& a) { x += a.x; y += a.y; z += a.z; }
		void operator += (const float& a) { x += a; y += a; z += a; }
		void operator *= (const vec3& a) { x *= a.x; y *= a.y; z *= a.z; }
		void operator *= (const float a) { x *= a; y *= a; z *= a; }
		float operator [] (const uint& idx) const { return cell[idx]; }
		float& operator [] (const uint& idx) { return cell[idx]; }
		float length() const { return sqrtf(x * x + y * y + z * z); }
		float sqrLentgh() const { return x * x + y * y + z * z; }
		vec3 normalized() const { float l = 1.0f / length(); return vec3(x * l, y * l, z * l); }
		void normalize() { float l = 1.0f / length(); x *= l; y *= l; z *= l; }
		static vec3 normalize(const vec3 v) { return v.normalized(); }

		vec3 cross(const vec3& operand) const
		{
			return vec3(y * operand.z - z * operand.y, z * operand.x - x * operand.z, x * operand.y - y * operand.x);
		}
		float dot(const vec3& operand) const { return x * operand.x + y * operand.y + z * operand.z; }
	};

	class vec4
	{
	public:
#pragma warning (push)
#pragma warning (disable:4201)
		union {
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			struct
			{
				vec3 xyz;
				float w2;
			};

			float cell[4] = { 0 };
		};
#pragma warning (pop)
		vec4() {}
		vec4(float v) : x(v), y(v), z(v), w(v) {}
		vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		vec4(vec3 a, float b) : x(a.x), y(a.y), z(a.z), w(b) {}
		vec4 operator - () const { return vec4(-x, -y, -z, -w); }
		vec4 operator + (const vec4& addOperand) const { return vec4(x + addOperand.x, y + addOperand.y, z + addOperand.z, w + addOperand.w); }
		vec4 operator - (const vec4& operand) const { return vec4(x - operand.x, y - operand.y, z - operand.z, w - operand.w); }
		vec4 operator / (const float& operand) const { return vec4(x / operand, y / operand, z / operand, w / operand); }
		vec4 operator * (const vec4& operand) const { return vec4(x * operand.x, y * operand.y, z * operand.z, w * operand.w); }
		void operator -= (const vec4& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
		void operator += (const vec4& a) { x += a.x; y += a.y; z += a.z; w += a.w; }
		void operator *= (const vec4& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
		void operator *= (float a) { x *= a; y *= a; z *= a; w *= a; }
		float& operator [] (const int idx) { return cell[idx]; }
		float operator [] (const uint& idx) const { return cell[idx]; }
		float length() { return sqrtf(x * x + y * y + z * z + w * w); }
		float sqrLentgh() { return x * x + y * y + z * z + w * w; }
		vec4 normalized() { float r = 1.0f / length(); return vec4(x * r, y * r, z * r, w * r); }
		void normalize() { float r = 1.0f / length(); x *= r; y *= r; z *= r; w *= r; }
		static vec4 normalize(vec4 v) { return v.normalized(); }
		float dot(const vec4& operand) const { return x * operand.x + y * operand.y + z * operand.z + w * operand.w; }
	};

	vec3 Randomvec3(float min, float max);
	float Fast_RandFloat();
	vec2 RandWithinUnitDisc();
	vec3 RandWithinUnitSphere();
	vec3 normalize(const vec3& v);
	vec3 sqrsub(const vec3& a, const vec3& b);
	float DistanceBetween(const vec3& a, const vec3& b);
	vec3 cross(const vec3& a, const vec3& b);
	float dot(const vec3& a, const vec3& b);
	vec3 operator * (const float& s, const vec3& v);
	vec3 operator * (const vec3& v, const float& s);
	vec4 operator * (const float& s, const vec4& v);
	vec4 operator * (const vec4& v, const float& s);

	class uint4
	{
	public:
#pragma warning (push)
#pragma warning (disable:4201)
		union { struct { uint x, y, z, w; }; uint cell[4]; };
#pragma warning (pop)

		uint4() = default;
		uint4(int v) : x(v), y(v), z(v), w(v) {}
		uint4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
		uint4 operator + (const uint4& addOperand) const { return uint4(x + addOperand.x, y + addOperand.y, z + addOperand.z, w + addOperand.w); }
		uint4 operator - (const uint4& operand) const { return uint4(x - operand.x, y - operand.y, z - operand.z, w - operand.w); }
		uint4 operator * (const uint4& operand) const { return uint4(x * operand.x, y * operand.y, z * operand.z, w * operand.w); }
		uint4 operator * (uint operand) const { return uint4(x * operand, y * operand, z * operand, w * operand); }
		void operator -= (const uint4& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
		void operator += (const uint4& a) { x += a.x; y += a.y; z += a.z; w += a.w; }
		void operator *= (const uint4& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
		void operator *= (uint a) { x *= a; y *= a; z *= a; w *= a; }
		uint& operator [] (const int idx) { return cell[idx]; }
	};

	class int4
	{
	public:
#pragma warning (push)
#pragma warning (disable:4201)
		union { struct { int x, y, z, w; }; int cell[4]; };
#pragma warning (pop)
		int4() = default;
		int4(int v) : x(v), y(v), z(v), w(v) {}
		int4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
		int4 operator - () const { return int4(-x, -y, -z, -w); }
		int4 operator + (const int4& addOperand) const { return int4(x + addOperand.x, y + addOperand.y, z + addOperand.z, w + addOperand.w); }
		int4 operator - (const int4& operand) const { return int4(x - operand.x, y - operand.y, z - operand.z, w - operand.w); }
		int4 operator * (const int4& operand) const { return int4(x * operand.x, y * operand.y, z * operand.z, w * operand.w); }
		int4 operator * (int operand) const { return int4(x * operand, y * operand, z * operand, w * operand); }
		void operator -= (const int4& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
		void operator += (const int4& a) { x += a.x; y += a.y; z += a.z; w += a.w; }
		void operator *= (const int4& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
		void operator *= (int a) { x *= a; y *= a; z *= a; w *= a; }
		int& operator [] (const int idx) { return cell[idx]; }
	};

	class mat4
	{
	public:
		mat4();
		float cell[16];
		float& operator [] (const int idx) { return cell[idx]; }
	
		static mat4 identity();
		static mat4 rotate(vec3 v, float a);
		static mat4 rotatex(const float a);
		static mat4 rotatey(const float a);
		static mat4 rotatez(const float a);
		void invert()
		{
			// from MESA, via http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
			const float inv[16] = {
				 cell[5] * cell[10] * cell[15] - cell[5] * cell[11] * cell[14] - cell[9] * cell[6] * cell[15] +
				 cell[9] * cell[7] * cell[14] + cell[13] * cell[6] * cell[11] - cell[13] * cell[7] * cell[10],
				-cell[1] * cell[10] * cell[15] + cell[1] * cell[11] * cell[14] + cell[9] * cell[2] * cell[15] -
				 cell[9] * cell[3] * cell[14] - cell[13] * cell[2] * cell[11] + cell[13] * cell[3] * cell[10],
				 cell[1] * cell[6] * cell[15] - cell[1] * cell[7] * cell[14] - cell[5] * cell[2] * cell[15] +
				 cell[5] * cell[3] * cell[14] + cell[13] * cell[2] * cell[7] - cell[13] * cell[3] * cell[6],
				-cell[1] * cell[6] * cell[11] + cell[1] * cell[7] * cell[10] + cell[5] * cell[2] * cell[11] -
				 cell[5] * cell[3] * cell[10] - cell[9] * cell[2] * cell[7] + cell[9] * cell[3] * cell[6],
				-cell[4] * cell[10] * cell[15] + cell[4] * cell[11] * cell[14] + cell[8] * cell[6] * cell[15] -
				 cell[8] * cell[7] * cell[14] - cell[12] * cell[6] * cell[11] + cell[12] * cell[7] * cell[10],
				 cell[0] * cell[10] * cell[15] - cell[0] * cell[11] * cell[14] - cell[8] * cell[2] * cell[15] +
				 cell[8] * cell[3] * cell[14] + cell[12] * cell[2] * cell[11] - cell[12] * cell[3] * cell[10],
				-cell[0] * cell[6] * cell[15] + cell[0] * cell[7] * cell[14] + cell[4] * cell[2] * cell[15] -
				 cell[4] * cell[3] * cell[14] - cell[12] * cell[2] * cell[7] + cell[12] * cell[3] * cell[6],
				 cell[0] * cell[6] * cell[11] - cell[0] * cell[7] * cell[10] - cell[4] * cell[2] * cell[11] +
				 cell[4] * cell[3] * cell[10] + cell[8] * cell[2] * cell[7] - cell[8] * cell[3] * cell[6],
				 cell[4] * cell[9] * cell[15] - cell[4] * cell[11] * cell[13] - cell[8] * cell[5] * cell[15] +
				 cell[8] * cell[7] * cell[13] + cell[12] * cell[5] * cell[11] - cell[12] * cell[7] * cell[9],
				-cell[0] * cell[9] * cell[15] + cell[0] * cell[11] * cell[13] + cell[8] * cell[1] * cell[15] -
				 cell[8] * cell[3] * cell[13] - cell[12] * cell[1] * cell[11] + cell[12] * cell[3] * cell[9],
				 cell[0] * cell[5] * cell[15] - cell[0] * cell[7] * cell[13] - cell[4] * cell[1] * cell[15] +
				 cell[4] * cell[3] * cell[13] + cell[12] * cell[1] * cell[7] - cell[12] * cell[3] * cell[5],
				-cell[0] * cell[5] * cell[11] + cell[0] * cell[7] * cell[9] + cell[4] * cell[1] * cell[11] -
				 cell[4] * cell[3] * cell[9] - cell[8] * cell[1] * cell[7] + cell[8] * cell[3] * cell[5],
				-cell[4] * cell[9] * cell[14] + cell[4] * cell[10] * cell[13] + cell[8] * cell[5] * cell[14] -
				 cell[8] * cell[6] * cell[13] - cell[12] * cell[5] * cell[10] + cell[12] * cell[6] * cell[9],
				 cell[0] * cell[9] * cell[14] - cell[0] * cell[10] * cell[13] - cell[8] * cell[1] * cell[14] +
				 cell[8] * cell[2] * cell[13] + cell[12] * cell[1] * cell[10] - cell[12] * cell[2] * cell[9],
				-cell[0] * cell[5] * cell[14] + cell[0] * cell[6] * cell[13] + cell[4] * cell[1] * cell[14] -
				 cell[4] * cell[2] * cell[13] - cell[12] * cell[1] * cell[6] + cell[12] * cell[2] * cell[5],
				 cell[0] * cell[5] * cell[10] - cell[0] * cell[6] * cell[9] - cell[4] * cell[1] * cell[10] +
				 cell[4] * cell[2] * cell[9] + cell[8] * cell[1] * cell[6] - cell[8] * cell[2] * cell[5]
			};
			const float det = cell[0] * inv[0] + cell[1] * inv[4] + cell[2] * inv[8] + cell[3] * inv[12];
			if (det != 0)
			{
				const float invdet = 1.0f / det;
				for (int i = 0; i < 16; i++) cell[i] = inv[i] * invdet;
			}
		}
	};
	mat4 operator * (const mat4& M1, const mat4& M2);

	vec3 operator * (const vec3& v, const mat4& M);

	vec4 operator * (const vec4& v, const mat4& M);
}