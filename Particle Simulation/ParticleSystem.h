#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <random>
#include "Particle.h"

class ParticleSystem {
public:
    std::vector<Particle> particles;
    glm::vec2 windowSize;
    const float maxVelocity = 200.0f; // Maximum velocity cap

    ParticleSystem(size_t numParticles, const glm::vec2& windowSize)
        : windowSize(windowSize) {
        particles.reserve(numParticles);
        InitParticles();
    }

    // Initialize particles to form a square with a gradient color and slight randomization
    void InitParticles() {
        size_t sideLength = static_cast<size_t>(sqrt(particles.capacity())); // Determine the side length of the square
        float squareSize = glm::min(windowSize.x, windowSize.y) * 0.5f; // Set the square size as half of the smaller window dimension
        glm::vec2 squareOrigin = (windowSize - glm::vec2(squareSize)) / 2.0f; // Center the square in the window
        float jitterAmount = squareSize / sideLength * 0.5f; // Amount of random jitter

        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> jitterDist(-jitterAmount, jitterAmount);

        for (size_t i = 0; i < sideLength; ++i) {
            for (size_t j = 0; j < sideLength; ++j) {
                glm::vec2 basePos = squareOrigin + glm::vec2(i * (squareSize / sideLength), j * (squareSize / sideLength));
                glm::vec2 pos = basePos + glm::vec2(jitterDist(rng), jitterDist(rng)); // Apply random jitter

                // Calculate color based on position within the square
                float tX = static_cast<float>(i) / (sideLength - 1); // Normalized position along the x-axis
                float tY = static_cast<float>(j) / (sideLength - 1); // Normalized position along the y-axis

                // Red to yellow gradient
                glm::vec3 color = glm::mix(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), tX);

                glm::vec2 vel(0.0f, 0.0f); // Initially at rest

                particles.emplace_back(pos, vel, color);
            }
        }
    }


    // Update all particles
    void Update(float deltaTime, const glm::vec2& mousePos, bool mousePressed) {
        for (auto& particle : particles) {
            if (mousePressed) {
                glm::vec2 direction = mousePos - particle.position;
                float distance = glm::length(direction);
                if (distance > 0) {
                    direction = glm::normalize(direction);
                    float strength = 500.0f / glm::max(distance, 1.0f); // Attraction strength, clamped to avoid infinite force
                    particle.ApplyForce(direction * strength);
                }
            }
         
            particle.velocity *= 0.995f; // Small reduction in velocity
            

            // Cap the velocity
            if (glm::length(particle.velocity) > maxVelocity) {
                particle.velocity = glm::normalize(particle.velocity) * maxVelocity;
            }

            // Update particle with the new velocity
            particle.Update(deltaTime, windowSize);
        }
    }

    // Render particles
    void Render() {
        glPointSize(0.5f); // Set particle size
        glBegin(GL_POINTS);
        for (const auto& particle : particles) {
            glColor3f(particle.color.r, particle.color.g, particle.color.b); // Set particle color
            glVertex2f(particle.position.x, particle.position.y);
        }
        glEnd();
    }

};
