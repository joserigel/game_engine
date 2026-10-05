#ifndef __COMPONENTS_SCENE_HPP__
#define __COMPONENTS_SCENE_HPP__

#include "gameobject.hpp"
#include "../rendering/camera.hpp"

#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

class Scene {
    private:
        Camera camera_;
        vector<GameObject> objects_;
        float tickRate_;
        float lastUpdate_;

        Shader shader_;
        
    public:
        Scene();
        void tick();
        void draw();
        string debugText();

        void mouseCallback(GLFWwindow* id);
        void keyboardCallback(GLFWwindow* id, float delta);
        void updateAspectRatio(int width, int height);

};

#endif
