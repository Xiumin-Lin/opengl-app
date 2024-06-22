#ifndef CAMERA_ORBITALE_H
#define CAMERA_ORBITALE_H

#include <algorithm>
#include <GLFW/glfw3.h>
#include <cmath>
#include "Mat4.h"
#include "Vertex.h"

class CameraOrbitale {
public:
    CameraOrbitale() = default;
    CameraOrbitale(const vec3& target, float initialRadius, float initialPhi, float initialTheta);

    void update();
    void setProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
    const Mat4& getViewMatrix() const;
    const Mat4& getProjectionMatrix() const;

    const vec3& getPosition() const { return m_position; }

    void mouseCallback(double xpos, double ypos, bool isFirstClick);
    void scrollCallback(double xoffset, double yoffset);

private:
    vec3 m_target;
    vec3 m_up;
    vec3 m_position;
    
    float m_radius;
    float m_phi;
    float m_theta;

    Mat4 m_viewMatrix;
    Mat4 m_projectionMatrix;

    vec3 calculatePosition() const;
};

#endif // CAMERA_H

