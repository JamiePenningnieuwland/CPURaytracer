#include "vec.h"
#include "Ray.h"
#include "Camera.h"

using namespace MathUtil;

Ray Camera::shootRay(const vec2& UV)
{
	vec2 uv = UV * 2.f-1.f;
	float pixelWidth = 1.f / static_cast<float>(screenWidth);
	float pixelHeight = 1.f / static_cast<float>(screenWidth);
	uv.x += (Fast_RandFloat() - 0.5f) * pixelWidth;
	uv.y += (Fast_RandFloat() - 0.5f) * pixelHeight;
	Ray r;

	//vec2 rd = (RandWithinUnitDisc() * (0.04f));
	vec3 offset = 0.f;// u* rd.x + v * rd.y;

	r.Origin = m_Position + offset;
	r.Direction = m_bottomLeft + m_Horizontal * uv.x + m_Vertical *  uv.y - offset;

	r.Direction = normalize(r.Direction);

	return r;

}
void Camera::SetPercpectiveProjection(float fov, float aspectratio, float n, float f)
{
	m_Projection = mat4::identity();

	float scale = 1.f / static_cast<float>(tanf(fov * 0.5f * (PI / 180.f)));
	float OneOvernearminfar = 1.f / (n - f);
	m_Projection[0] = scale; m_Projection[1] = 0.f;                    m_Projection[2] = 0.f;                                m_Projection[3] = 0.f;
	m_Projection[4] = 0.f;   m_Projection[5] = scale * aspectratio;    m_Projection[6] = 0.f;                                m_Projection[7] = 0.f;
	m_Projection[8] = 0.f;   m_Projection[9] = 0.f;                    m_Projection[10] = (f + n) * OneOvernearminfar;       m_Projection[11] = -1.f;
	m_Projection[12] = 0.f;   m_Projection[13] = 0.f;                  m_Projection[14] = 2.f * f * n * OneOvernearminfar;   m_Projection[15] = 0.f;

	/*m_Projection[0] = scale;    m_Projection[4] = 0.f;                    m_Projection[8] = 0.f;                             m_Projection[12] = 0.f;
	m_Projection[1] = 0.f;      m_Projection[5] = scale * aspectRatio;    m_Projection[9] = 0.f;                             m_Projection[13] = 0.f;
	m_Projection[2] = 0.f;      m_Projection[6] = 0.f;                    m_Projection[10] = (f + n) * OneOvernearminfar;    m_Projection[14] = 2.f * f * n * OneOvernearminfar;
	m_Projection[3] = 0.f;      m_Projection[7] = 0.f;                    m_Projection[11] = -1.f;                           m_Projection[15] = 0.f;*/
}
//vec3 Camera::GetRigth() const
//{
//	vec3 right = vec3(1.f, 0.f, 0.f) * m_View;
//	return right;
//}
//vec3 Camera::GetUp() const
//{
//	vec3 up = vec3(0.f, 1.f, 0.f) * m_View;
//	return up;
//}
//vec3 Camera::GetForward() const
//{
//	vec3 forward = vec3(0.f, 0.f, 1.f) * m_View;
//	return forward;
//}
void Camera::RecalculateTransform()
{
	//multiplying matrices is in flipped order x * y so y is first

	mat4 xRotMat = mat4::identity().rotatex(m_Rotation.x);
	mat4 yRotMat = mat4::identity().rotatey(m_Rotation.y);
	mat4 zRotMat = mat4::identity().rotatez(m_Rotation.z);

	
	mat4 RotationMatrix = zRotMat * xRotMat * yRotMat;
	mat4 translationMatrix = mat4::identity();
	translationMatrix.cell[3] = m_Position.x;
	translationMatrix.cell[7] = m_Position.y;
	translationMatrix.cell[11] = m_Position.z;

	m_View = translationMatrix * RotationMatrix;

	VP = m_Projection * m_View;
	
	m_bottomLeft = vec3{ 0.f,0.f,m_fd } * VP;
	m_Horizontal = vec3{ m_fd,0.f,0.f } * VP;
	m_Vertical = vec3{ 0.f,m_fd,0.f } * VP;
	u = vec3{ 1.f,0.f,0.f } *VP;
	v = vec3{ 0.f,1.f,0.f } *VP;


	
}