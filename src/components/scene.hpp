#ifndef __COMPONENTS_SCENE_HPP__
#define __COMPONENTS_SCENE_HPP__

#include <vector>

#include "gameobject.hpp"

using namespace std;

class Scene {
    private:
        vector<GameObject> objects_;
        float tickRate_;
        float lastUpdate_;
    public:
        Scene();
        void run();

};

#endif
