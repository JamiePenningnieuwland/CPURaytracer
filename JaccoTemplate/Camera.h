#pragma once
#include "vec.h"
#include "Ray.h"
namespace Tmpl8
{
	//class Ray;
	class Camera
	{
	public:

		// constructors
		Camera():m_Position(0.f,0.f,5.f), m_Rotation(0.f,0.f,0.f), m_fd(9.f)
		{
			SetPercpectiveProjection(m_Fov, 0.01f, 1000.f);
			RecalculateTransform();
		}
		Camera(const MathUtil::vec3& p, const float& fd): m_Position(p), m_Rotation(0.f, 0.f, 0.f), m_fd(fd)
		{
			SetPercpectiveProjection(m_Fov, 0.01f, 1000.f);
			RecalculateTransform();
		}
		Camera(const MathUtil::vec3& p, const MathUtil::vec3& r, const float& fd): m_Position(p), m_Rotation(r), m_fd(fd)
		{
			SetPercpectiveProjection(m_Fov, 0.01f, 1000.f);
			RecalculateTransform();
		}

		const float aspectRatio = static_cast<float>(ScreenHeight) / static_cast<float>(ScreenWidth);

		void SetPercpectiveProjection(float fov, float near, float far);
		void RecalculateTransform();

		inline void SetPosition(const MathUtil::vec3& p) { m_Position = p; RecalculateTransform(); }
		inline const MathUtil::vec3& GetPosition() const { return m_Position; };

		inline void SetRotation(const MathUtil::vec3& r) { m_Rotation = r; RecalculateTransform(); }
		inline const MathUtil::vec3& GetRotation() const { return m_Rotation; }

		inline const void SetFd(float n) { m_fd = n; }
		inline const float& GetFd() const { return m_fd; }

		inline const MathUtil::mat4& GetView() const { return m_View; }

		MathUtil::Ray ShootRay(const MathUtil::vec2& UV, bool depthoffield);
	

	private:
		float pixelWidth = 1.f / static_cast<float>(ScreenWidth);
		float pixelHeight = 1.f / static_cast<float>(ScreenHeight);
		float m_fd = 8.f;

		MathUtil::vec3 m_Position = { 5.21f, 0.90f, -15.1f };
		MathUtil::vec3 m_Rotation = { 0.19f, 3.47f,0.f };
		MathUtil::vec3 m_Horizontal;
		MathUtil::vec3 m_Vertical;
		MathUtil::vec3 u;
		MathUtil::vec3 v;

		MathUtil::vec3 m_bottomLeft;

		float m_Fov = 110.f;

		// Orientation and location
		MathUtil::mat4 m_View;
		MathUtil::mat4 m_Projection;

		//viewProjection
		MathUtil::mat4 VP;
	};

};
