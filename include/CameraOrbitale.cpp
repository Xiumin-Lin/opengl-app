#include "CameraOrbitale.h"

using namespace std;

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

const Mat4 &CameraOrbitale::getViewMatrix() const
{
    return m_viewMatrix;
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
    if (isFirstClick)
    {
        lastX = xpos;
        lastY = ypos;
        return;
    }

    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.002f;
    m_phi += deltaX * sensitivity;
    m_theta -= deltaY * sensitivity;
    m_theta = std::max(-M_PI_2, std::min(M_PI_2, static_cast<double>(m_theta)));

    if (m_phi > 2 * M_PI)
        m_phi -= 2 * M_PI;
    else if (m_phi < -2 * M_PI)
        m_phi += 2 * M_PI;
}

void CameraOrbitale::scrollCallback(double xoffset, double yoffset)
{
    m_radius -= yoffset * 0.3f;
    m_radius = std::max(1.0f, std::min(100.0f, m_radius));
}

void CameraOrbitale::keyCallback(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        float m_speed = 1.0f;
        switch (key)
        {
        case GLFW_KEY_Z:
        case GLFW_KEY_W:
            m_target += m_up * m_speed; // Forward
            break;
        case GLFW_KEY_S:
            m_target -= m_up * m_speed; // Backward
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_Q:
            m_target -= vec3::normalize(vec3::cross(getDirection(), m_up)) * m_speed; // Left
            break;
        case GLFW_KEY_D:
            m_target += vec3::normalize(vec3::cross(getDirection(), m_up)) * m_speed; // Right
            break;
        case GLFW_KEY_SPACE:
            m_target.y -= m_speed; // Up    
            break;
        case GLFW_KEY_LEFT_CONTROL:
            m_target.y += m_speed; // Down
            break;
        }
    }
}
