//
//  Game.hpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#ifndef Game_hpp
#define Game_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include "Renderer.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Actor.hpp"
#include "World.hpp"
#include "PlayerHero.hpp"
#include "InputHandler.hpp"
#include <sstream>
#include <vector>
#include "Ability.hpp"
#include "Sketch.hpp"
#include "Billow.hpp"
#include "Mist.hpp"
#include "LoadingScreen.hpp"
#include "InkEffect.hpp"
#include "Audio.hpp"
#include "Map.hpp"
#include "Numberable.hpp"
#include "Fireworks.hpp"
#include "ScriptOne.hpp"

class Dialogue;

class Game {
GLFWwindow* window; // Windowed
Camera camera;
World world;
    World realWorld;
Renderer* renderer;
    Renderer* realRenderer;
    Renderer* activeRenderer;
InputHandler inputHandler;
    Audio audio;
    
    std::stringstream input;
    std::string i;
    int nextBranch;
    bool printing = false;
   // boost::mutex mutex;
    
    int scheme = 0;
    double lastMX = 500;
    double lastMY = 400;
    double xOffset;
    double yOffset;
    bool firstMouse = true;
    bool mouseMoved = false;
    double lastTime = 0;
    
    std::shared_ptr<Sketch> activeSketch;
    Dialogue* activeDialogue = NULL;
    
    Ball ball;
    PlayerHero pHero;
    PlayerHero rHero;
    InkEffect inkGlyphs;
    Fireworks* fireworks;
    Map map;
    Map realMap;
    Mist mist;
    
    ScriptOne* script;
    
    std::vector<std::shared_ptr<Ability>> abilities;
    Numberable* numberables[100];
    
    GLuint fbo, fvao, ftexture;
    unsigned char* paint;
    unsigned char* blank;
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    
public:
    bool running = false;
    Game();
    ~Game();
    void tick();
    void moveMouse(double mouseX_, double mouseY_);
    int processInput(int key, int action, int mods);
    void processInput2(int key, int action);
    void print();
    void setActionScheme(int id);
    void newDialogue(Dialogue& dialogue_);
    
    Numberable* getNumberable(unsigned int ID_);
};

#endif /* Game_hpp */
