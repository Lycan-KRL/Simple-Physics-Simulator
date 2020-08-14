#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
using namespace sf;

namespace Gravity {
    const float direction = M_PI;
    const float multiplier = 0.015f;

    const float DRAG = 0.999;
    const float ELASTICITY = 0.75;
};

class Particle {
public:

    // - - -
    float mass;
    float radius;
    float angle;
    float speed;

    // - - -
    Color color;
    Vector2f position;

    CircleShape circle;

    // - - -
    Particle( float x, float y, float assignedRadius, int assignedMass );
    void move();
    void bounce( const int wallX, const int wallY );
    void draw( RenderWindow* sim );
    static void collide( Particle* a, Particle* b );
    Vector2f addVectors( Vector2f a, Vector2f b );
};

#endif