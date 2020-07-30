#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

const int WIDTH = 1200;
const int HEIGHT = 1000;

const float GRAVITY = 10.f;
const float AIR_DENSITY = 1.225;
const float TIME_STEP = 1 / 100.f;

class BasicForm {
public:
    Vector2f position;
    Vector2f velocity;
    Vector2f force;

    float mass = 10.f;
    float airCoefficient = 0;
    float density = 0;
    float area = 0;

    virtual void update( const float ) = 0;
    virtual Shape& whatAmI( void ) = 0;
};

class MyRect : public BasicForm {
public:
    RectangleShape rect;

    MyRect( const Vector2f& v = Vector2f( rand() % WIDTH, rand() % HEIGHT ) );

    void update( const float ) override;
    inline Shape& whatAmI( void ) override {
        return rect;
    }
};

MyRect::MyRect( const Vector2f& v ) {

}

void MyRect::update( const float delta ) {
    force.y = mass * GRAVITY;
    Vector2f acceleration( 0.f, force.y / mass );

    velocity.y += acceleration.y * delta;
    position.y += velocity.y * delta;
}

class MyCircle : public BasicForm {
public:
    CircleShape circle;

    MyCircle( const Vector2f& v = Vector2f( rand() % WIDTH, rand() % HEIGHT ) );

    void update( const float ) override;
    inline Shape& whatAmI( void ) override {
        return circle;
    }
};

MyCircle::MyCircle( const Vector2f& v ) {
    circle.setRadius( rand() % 100 );
    circle.setFillColor( Color( rand() % 255, rand() % 255, rand() % 255 ) );
    circle.setPosition( v );

    density = rand() % 200;
    airCoefficient = 0.5f;
    area = 3.14 * pow( circle.getRadius(), 2 );
    mass = density * area;
}

void MyCircle::update( const float delta ) {
    float drag = airCoefficient * area * ( ( AIR_DENSITY * pow( velocity.y, 2 ) ) / 2 );

    force.y = this->mass * GRAVITY - drag;
    float acceleration = force.y / mass;

    velocity.y += acceleration * delta;
    circle.move( 0.f, velocity.y * delta );
}

class Collisions {
public:
    static void Bounce( BasicForm* b ) {
        Shape *s = &b->whatAmI();

        Vector2f currPos = s->getPosition();
        FloatRect bounds = s->getLocalBounds();

        float noGoRight = WIDTH - bounds.width;
        float noGoDown = HEIGHT - bounds.height;
        float noGoLeft = bounds.width;
        float noGoUp = bounds.height;

        if ( currPos.x < noGoLeft ) {
            currPos.x = noGoLeft;
        } else if ( currPos.x > noGoRight ) {
            currPos.x = noGoRight;
        } else if ( currPos.y < noGoUp ) {
            currPos.y = noGoUp;
        } else if ( currPos.y > noGoDown ) {
            b->velocity.y *= -0.7;
            currPos.y = noGoDown;
        }

        s->setPosition( currPos );
    }
};

int main() {
    srand( time( NULL ) );
    RenderWindow app( VideoMode( WIDTH, HEIGHT ), ":)" );

    // - - -
    Event e;
    Clock c;
    float time = 0;

    vector<BasicForm*> bodies;

    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );
    bodies.push_back( new MyCircle );

    // - - -
    while ( app.isOpen() ) {
        time += c.restart().asSeconds();

        while ( app.pollEvent( e ) ) {

            switch ( e.type ) {
                // Closing the window
                case Event::Closed:
                    app.close();
                    break;
                // Adding a shape
                case Event::MouseButtonPressed: {
                    Vector2i mousePos = Mouse::getPosition( app );
                    Vector2f mouseWindow = app.mapPixelToCoords( mousePos );

                    switch ( e.mouseButton.button ) {
                        case Mouse::Left:
                            bodies.push_back( new MyRect( mouseWindow ) );
                            break;
                        case Mouse::Right:
                            bodies.push_back( new MyCircle( mouseWindow ) );
                            break;
                    }; // Inner Switch End
                    break;
                }
                // Also closing the window
                case Event::KeyPressed:
                    switch ( e.key.code ) {
                        case Keyboard::Q:
                            app.close();
                            break;
                        case Keyboard::R:
                            bodies.clear();
                            for (int i = 0; i < 10; i++)
                                bodies.push_back( new MyCircle );
                            break;
                    }; // Inner Switch End
            }; 
        }

        if ( time >= TIME_STEP ) {
            for (int i = 0; i < bodies.size(); i++) {
                bodies[i]->update( TIME_STEP * 10 );
                Collisions::Bounce( bodies[i] );
            }

            time = 0;
        }

        app.clear();

        for (int i = 0; i < bodies.size(); i++) {
            Shape *s = &bodies[i]->whatAmI();
            app.draw( *s );
        }

        app.display();
    }

    return 0;
}