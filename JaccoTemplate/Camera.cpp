#include "precomp.h"
#include "Camera.h"

using namespace Tmpl8;

MathUtil::Ray Camera::ShootRay(const MathUtil::vec2& UV, bool depthoffield)
{
	MathUtil::vec2 uv = UV * 2.f - 1.f;
	uv.x += (MathUtil::Fast_RandFloat() - 0.5f) * pixelWidth;
	uv.y += (MathUtil::Fast_RandFloat() - 0.5f) * pixelHeight;
	MathUtil::Ray r;

	if (depthoffield)
	{
		MathUtil::vec2 rd = (MathUtil::RandWithinUnitDisc() * (0.05f));
		MathUtil::vec3 offset = u * rd.x + v * rd.y;

		r.Origin = m_Position + offset;
		r.Direction = m_bottomLeft + m_Horizontal * uv.x + m_Vertical * uv.y - offset;
	}
	else
	{
		MathUtil::vec3 offset = 0.f;
		r.Origin = m_Position;
		r.Direction = m_bottomLeft + m_Horizontal * uv.x + m_Vertical * uv.y;
	}
	r.Direction = MathUtil::normalize(r.Direction);
	return r;

}
void Camera::SetPercpectiveProjection(float fov, float n, float f)
{
	m_Projection = MathUtil::mat4::identity();

	float scale = 1.f / static_cast<float>(tanf(fov * 0.5f * (MathUtil::PI / 180.f)));
	float OneOvernearminfar = 1.f / (n - f);
	m_Projection[0] = scale; m_Projection[1] = 0.f;                    m_Projection[2] = 0.f;                                m_Projection[3] = 0.f;
	m_Projection[4] = 0.f;   m_Projection[5] = scale * aspectRatio;    m_Projection[6] = 0.f;                                m_Projection[7] = 0.f;
	m_Projection[8] = 0.f;   m_Projection[9] = 0.f;                    m_Projection[10] = (f + n) * OneOvernearminfar;       m_Projection[11] = -1.f;
	m_Projection[12] = 0.f;   m_Projection[13] = 0.f;                  m_Projection[14] = 2.f * f * n * OneOvernearminfar;   m_Projection[15] = 0.f;

}
void Camera::RecalculateTransform()
{
	//multiplying matrices is in flipped order x * y so y is first

	MathUtil::mat4 xRotMat = MathUtil::mat4::identity().rotatex(m_Rotation.x);
	MathUtil::mat4 yRotMat = MathUtil::mat4::identity().rotatey(m_Rotation.y);
	MathUtil::mat4 zRotMat = MathUtil::mat4::identity().rotatez(m_Rotation.z);

	
	MathUtil::mat4 RotationMatrix = zRotMat * xRotMat * yRotMat;
	MathUtil::mat4 translationMatrix = MathUtil::mat4::identity();
	translationMatrix.cell[3] = m_Position.x;
	translationMatrix.cell[7] = m_Position.y;
	translationMatrix.cell[11] = m_Position.z;

	m_View = translationMatrix * RotationMatrix;

	VP = m_Projection * m_View;
	
	m_bottomLeft = MathUtil::vec3{ 0.f,0.f,m_fd } * VP;
	m_Horizontal = MathUtil::vec3{ m_fd,0.f,0.f } * VP;
	m_Vertical = MathUtil::vec3{ 0.f,m_fd,0.f } * VP;
	u = MathUtil::vec3{ 1.f,0.f,0.f } *VP;
	v = MathUtil::vec3{ 0.f,1.f,0.f } *VP;


	
}