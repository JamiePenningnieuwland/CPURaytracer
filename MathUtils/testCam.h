#pragma once

namespace MathUtil
{

	class Ray;
	class Camera
	{
	public:

		// constructors
		Camera() :m_Position(0.f, 0.f, 5.f), m_Rotation(0.f, 0.f, 0.f), m_fd(9.f), screenWidth(0), screenHeight(0)
		{
			SetPercpectiveProjection(m_Fov, aspectRatio, 0.01f, 1000.f);
			RecalculateTransform();
		}
		Camera(int scrwidth, int scrheight):m_Position(0.f,0.f,5.f), m_Rotation(0.f,0.f,0.f), m_fd(9.f)
		{
			screenHeight = scrheight;
			screenWidth = scrwidth;
			SetPercpectiveProjection(m_Fov, aspectRatio, 0.01f, 1000.f);
			RecalculateTransform();
		}
		Camera(int scrwidth, int scrheight, const vec3& p, const float& fd): m_Position(p), m_Rotation(0.f, 0.f, 0.f), m_fd(fd)
		{
			screenHeight = scrheight;
			screenWidth = scrwidth;
			SetPercpectiveProjection(m_Fov, aspectRatio, 0.01f, 1000.f);
			RecalculateTransform();
		}
		Camera(int scrwidth, int scrheight, const vec3& p, const vec3& r, const float& fd): m_Position(p), m_Rotation(r), m_fd(fd)
		{
			screenHeight = scrheight;
			screenWidth = scrwidth;
			SetPercpectiveProjection(m_Fov, aspectRatio, 0.01f, 1000.f);
			RecalculateTransform();
		}
		const float aspectRatio = static_cast<float>(screenHeight) / static_cast<float>(screenWidth);

		void SetPercpectiveProjection(float fov, float aspectRatio, float near, float far);
		void RecalculateTransform();

		inline void SetPosition(const vec3& p) { m_Position = p; RecalculateTransform(); }
		inline const vec3& GetPosition() const { return m_Position; };

		inline void SetRotation(const vec3& r) { m_Rotation = r; RecalculateTransform(); }
		inline const vec3& GetRotation() { return m_Rotation; }

		/*inline vec3 GetForward() const;
		vec3 GetRigth() const;
		vec3 GetUp() const;*/
		inline const mat4& GetView() const { return m_View; }
		Ray shootRay(const vec2& UV);
		float m_fd = 8.f;
	

	private:
		int screenWidth;
		int screenHeight;
		//float pixelWidth = 1.f / static_cast<float>(ScreenWidth);
		//float pixelHeight = 1.f / static_cast<float>(ScreenWidth);
		vec3 m_Position = { 0.f,0.f,0.f };
		vec3 Direction = { 0,0,1 };
		vec3 m_Rotation = {};
		vec3 m_Horizontal;
		vec3 m_Vertical;
		vec3 u;
		vec3 v;

		vec3 m_bottomLeft;

		float m_Fov = 110.f;
		// Orientation and location
		mat4 m_View;

		mat4 m_Projection;

		//viewProjection
		mat4 VP;
	};

}
