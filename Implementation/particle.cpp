#include "../Headers/particle.h"
//#include <iostream>
//using namespace std;

Particle::Particle( float x, float y, float assignedRadius, int assignedMass = 1 ) {
    // Initialize several properties of the particle
    radius = assignedRadius;
    mass = assignedMass;
    angle = M_PI_2;
    speed = rand() % 10;

    color.r = rand() % 255;
    color.g = rand() % 255;
    color.b = rand() % 255;

    position.x = x;
    position.y = y;

    circle.setRadius( radius );
    circle.setFillColor( color );
    circle.setPosition( position );
    circle.setOrigin( Vector2f( radius, radius ) );
}

void Particle::move() {
    Vector2f gravity( Gravity::direction, Gravity::multiplier );
    Vector2f currentStats( angle, speed );
    
    Vector2f adjusted = addVectors( currentStats, gravity );
    this->angle = adjusted.x;
    this->speed = adjusted.y;

    position.x += sin( angle ) * speed;
    position.y -= cos( angle ) * speed;

    this->speed *= Gravity::DRAG;

    circle.setPosition( position );
}

void Particle::bounce( const int wallX, const int wallY ) {
    float x = position.x;
    float y = position.y;

    // - - -
    if ( x < radius ) {
        position.x = radius;
        angle = -angle;
        speed *= Gravity::ELASTICITY;
    } else if ( x >  wallX - radius ) {
        position.x = wallX - radius;
        angle = -angle;
        speed *= Gravity::ELASTICITY;
    }

    if ( y < radius ) {
        position.y = radius;
        angle = M_PI - angle;
        speed *= Gravity::ELASTICITY;
    } else if ( y > wallY - radius ) {
        position.y = wallY - radius;
        angle = M_PI - angle;
        speed *= Gravity::ELASTICITY;
    }
}

void Particle::draw( RenderWindow* sim ) {
    sim->draw( this->circle );
}

Vector2f Particle::addVectors( Vector2f a, Vector2f b ) {
    // 2D Vector Addition = <a1, b1> + <a2, b2>
    // Additive Total for One Side = ( Angle1 * Scalar1 ) + ( Angle2 * Scalar2 )
    // The hypoteneuse becomes the combined vector
    float x = ( sin( a.x ) * a.y ) + ( sin( b.x ) * b.y );
    float y = ( cos( a.x ) * a.y ) + ( cos( b.x ) * b.y );

    float newLength = hypot( x, y );
    float newAngle = M_PI_2 - atan2( y, x );

    return Vector2f( newAngle, newLength );
}

void Particle::collide( Particle* a, Particle* b ) {
    // Two circles collide if the distance between their centers
    // is shorter than the their radii combined
    float dx = a->position.x - b->position.x;
    float dy = a->position.y - b->position.y;

    float hypoteMOOSE = hypot( dx, dy );
    float radiiCombo = a->radius + b->radius;

    if ( hypoteMOOSE <= radiiCombo ) {
        // - - -
        float totalMass = a->mass + b->mass;
        float tangent = atan2( dy, dx ) + M_PI_2;
        float collisionElasticity = pow( Gravity::ELASTICITY, 2 );

        // To be combined for particle A
        Vector2f component1 = Vector2f( a->angle, a->speed * ( a->mass - b->mass ) / totalMass );
        Vector2f component2 = Vector2f( tangent, 2 * b->speed * b->mass / totalMass );
        // To be combine for particle B
        Vector2f component3 = Vector2f( b->angle, b->speed * ( b->mass - a->mass ) / totalMass );
        Vector2f component4 = Vector2f( tangent + M_PI, 2 * a->speed * a->mass / totalMass );

        // - - -
        Vector2f v1 = a->addVectors( component1, component2 );
        Vector2f v2 = a->addVectors( component3, component4 );

        a->angle = v1.x;
        a->speed = v1.y;
        b->angle = v2.x;
        b->speed = v2.y;

        a->speed *= collisionElasticity;
        b->speed *= collisionElasticity;
        
        // - - -
        // Handle any overlap that the time step didn't catch
        float overlap = 0.5 * ( radiiCombo - hypoteMOOSE + 1 );
        a->position.x += sin( tangent ) * overlap;
        a->position.y -= cos( tangent ) * overlap;
        b->position.x -= sin( tangent ) * overlap;
        b->position.y += cos( tangent ) * overlap;
    }
}
