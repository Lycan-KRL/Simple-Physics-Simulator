#include "../Headers/dominus.h"
//#include <iostream>
//using namespace std;

Dominus::Dominus( RenderWindow& reference, const int W, const int H ) {
    // Catch reference of the running window
    this->ref = &reference;
    bounds.x = W;
    bounds.y = H;

    // Push back new instances of particles
    for ( int i = 0; i < counter; i++ ) {
        addNewParticle();
    }
}

Dominus::~Dominus() {
    // The vector clears on its own
    // There will be no dangling pointers in my dominion
    ref = NULL;
}

void Dominus::draw() {
    ref->clear( Color( 50, 50, 50 ) ); // Clear the screen

    for ( int i = 0; i < counter; i++ ) {
        Particle *temp = p[i];
        temp->draw( this->ref );
    }

    ref->display(); // Refresh the window
}

Vector2f Dominus::getNewMousePos() {
    Vector2i temp = Mouse::getPosition( *ref );
    return ref->mapPixelToCoords( temp );
}

void Dominus::addNewParticle() {
    // - - -
    float radius = rand() % ( ( 50 - 30 + 1) ) + 30;
    float startingX = rand() % ( ( bounds.x - 0 + 1 ) ) + 0;
    float startingY = rand() % ( ( bounds.y - 0 + 1 ) ) + 0;
    float density = rand() % ( ( 20 - 1 + 1 ) ) + 1;

    // If you watched the show, you know the joke
    using Smarticle = Particle;
    Smarticle *particle = new Particle( startingX, startingY, radius, density * pow( radius, 2 ) );
    p.push_back( particle );
}

void Dominus::updateEvents() {
    for ( int i = 0; i < counter; i++ ) {
        Particle *temp = p[i];
        
        // - - -
        if ( temp == encaptured ) {
            temp->position = getNewMousePos();
            temp->circle.setPosition( temp->position );
        } else {
            temp->move();
            temp->bounce( bounds.x, bounds.y );
        }

        // - - -
        for ( int j = 0; j < counter; j++ ) {
            Particle *neighbor = p[j];

            if ( neighbor != temp ) {
                Particle::collide( temp, neighbor );
            }
        }
    }
}

Particle* Dominus::particleFound() {
    for ( int i = 0; i < counter; i++ ) {
        Particle *temp = p[i];
        FloatRect boundaries = temp->circle.getGlobalBounds();
        
        if ( boundaries.contains( mousePosition ) ) {
            return temp;
        }
    }

    return NULL; // No such particle was touched
}

void Dominus::flickedMomentum() {
    if ( not encaptured ) { return; }

    float dx = mousePosition.x - encaptured->position.x;
    float dy = mousePosition.y - encaptured->position.y;

    encaptured->angle = atan2( dy, dx ) - M_PI_2;
    encaptured->speed = hypot( dx, dy ) * 0.01;

    encaptured = NULL;
}

void Dominus::mainLoop() {
    Event e;
    Clock c;

    float timePassed = 0.f;

    while ( ref->isOpen() ) {
        timePassed += c.restart().asSeconds();

        // - - -
        while ( ref->pollEvent( e ) ) {

            if ( e.type == Event::Closed ) {
                ref->close();
            } else if ( e.type == Event::KeyPressed ) {
                switch ( e.key.code ) {
                    case Keyboard::Q:
                        ref->close();
                        break;
                    case Keyboard::N:
                        addNewParticle();
                        ++counter;
                        break;
                };
            }

            if ( e.type == Event::MouseButtonPressed ) {
                // Store where the mouse possibly
                // captured a particle
                mousePosition = getNewMousePos();
                encaptured = particleFound();
            } else if ( e.type == Event::MouseButtonReleased ) {
                // Particle has been released from its clicked imprisonment
                flickedMomentum();
            }
        }

        // - - -
        if ( timePassed >= step::TIME_STEP ) {
            updateEvents();
            timePassed = 0;
        }

        // - - -
        draw();
    }
}
