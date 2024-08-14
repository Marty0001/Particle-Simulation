#pragma once

#include <glm/glm.hpp>

class Particle {
public:
    glm::vec2 position; // Current position of the particle
    glm::vec2 velocity; // Current velocity of the particle
    glm::vec3 color;    // Color of the particle

    Particle(const glm::vec2& pos, const glm::vec2& vel, const glm::vec3& col)
        : position(pos), velocity(vel), color(col) {}

    // Update the position based on velocity and boundary constraints
    void Update(float deltaTime, const glm::vec2& windowSize) {
        position += velocity * deltaTime;

        // Bounce off the walls
        if (position.x <= 0.0f || position.x >= windowSize.x) {
            velocity.x *= -0.4f;
            position.x = glm::clamp(position.x, 0.0f, windowSize.x);
        }
        if (position.y <= 0.0f || position.y >= windowSize.y) {
            velocity.y *= -0.4f;
            position.y = glm::clamp(position.y, 0.0f, windowSize.y);
        }
    }

    // Apply force to the particle (e.g., mouse attraction)
    void ApplyForce(const glm::vec2& force) {
        velocity += force;
    }
};
