#ifndef DOMINUS_H
#define DOMINUS_H

#include <vector>
#include "particle.h"
using namespace sf;

namespace step {
    const float TIME_STEP = 1/60;
};

class Dominus {
public:

    // - - -
    std::vector<Particle*> p;
    Particle *encaptured = NULL;
    Vector2f mousePosition;
    Vector2u bounds;
    RenderWindow *ref = NULL;
    unsigned int counter = 5;
    
    // - - -
    Dominus( RenderWindow& reference, const int W, const int H );
    ~Dominus();
    void draw();
    void updateEvents();
    void mainLoop();
    void flickedMomentum();
    void addNewParticle();
    Particle* particleFound();
    Vector2f getNewMousePos();
};

#endif