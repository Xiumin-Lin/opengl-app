#ifndef CAMERA_ORBITALE_H
#define CAMERA_ORBITALE_H

#include <algorithm>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include "Mat4.h"
#include "Vertex.h"

class CameraOrbitale {
public:
    CameraOrbitale() = default;
    CameraOrbitale(const vec3& target, float initialRadius, float initialPhi, float initialTheta);

    void update();
    const Mat4& getViewMatrix() const;

    const vec3& getPosition() const { return m_position; }
    const vec3 getDirection() const { return m_target - m_position; }
    void setTarget(const vec3& target) { m_target = target; }

    void mouseCallback(double xpos, double ypos, bool isFirstClick);
    void scrollCallback(double xoffset, double yoffset);
    void keyCallback(int key, int scancode, int action, int mods);

private:
    vec3 m_target;
    vec3 m_up;
    vec3 m_position;
    
    float m_radius;
    float m_phi;
    float m_theta;

    Mat4 m_viewMatrix;

    vec3 calculatePosition() const;
};

#endif // CAMERA_H

