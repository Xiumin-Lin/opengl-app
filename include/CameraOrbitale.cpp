#include "CameraOrbitale.h"

CameraOrbitale::CameraOrbitale(const vec3 &target, float initialRadius, float initialPhi, float initialTheta)
    : m_target(target), m_up(0.0f, 1.0f, 0.0f), m_radius(initialRadius), m_phi(initialPhi), m_theta(initialTheta)
{
    update();
}

void CameraOrbitale::update()
{
    m_position = calculatePosition();
    m_viewMatrix.lookAt(m_position, m_target, m_up);
}

void CameraOrbitale::setProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    Mat4::perspective(&m_projectionMatrix, fov, aspectRatio, nearPlane, farPlane);
}

const Mat4 &CameraOrbitale::getViewMatrix() const
{
    return m_viewMatrix;
}

const Mat4 &CameraOrbitale::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

vec3 CameraOrbitale::calculatePosition() const
{
    float x = m_radius * cos(m_theta) * cos(m_phi);
    float y = m_radius * sin(m_theta);
    float z = m_radius * cos(m_theta) * sin(m_phi);
    return vec3(x, y, z);
}

static double lastX = 0;
static double lastY = 0;
void CameraOrbitale::mouseCallback(double xpos, double ypos, bool isFirstClick)
{
    if (isFirstClick) {
        lastX = xpos;
        lastY = ypos;
        return;
    }

    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.004f;
    m_phi += deltaX * sensitivity;
    m_theta -= deltaY * sensitivity;
    m_theta = std::max(-M_PI_2, std::min(M_PI_2, static_cast<double>(m_theta)));

    if (m_phi > 2 * M_PI) m_phi -= 2 * M_PI;
    else if (m_phi < -2 * M_PI) m_phi += 2 * M_PI;
    update();
}

void CameraOrbitale::scrollCallback(double xoffset, double yoffset)
{
    m_radius -= yoffset * 0.1f;
    m_radius = std::max(1.0f, std::min(100.0f, m_radius));
    update();
}
