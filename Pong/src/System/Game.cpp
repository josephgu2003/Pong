//
//  Game.cpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#include "Game.hpp"
#include "FallingLetters.hpp"
#include <iostream>
#include <cctype>
#include "Speech.hpp"
#include "stb_image.h"
#include "Dialogue.hpp"
#include "Fish.hpp"

extern void char_callback(GLFWwindow* window, unsigned int key);
extern void onetap_callback0(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_);

Game::Game() {
    running = true;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(1000, 800, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    
    glewInit();
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LEQUAL);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetTime(0);
    
    glfwSetWindowUserPointer(window, this);
    LoadingScreen screen;
    
    inputHandler.setWindow(window);
    
    //billow.posVec = glm::vec3(0,5,0);
    stbi_set_flip_vertically_on_load(1);
   mist.init(1.0, glm::vec3(0,0,0), glm::vec3(15, 0.3, 15), 900, 9, 1000, 0.995);
  //  inkGlyphs.init(4, glm::vec3(0,-0.5,0), glm::vec3(0.5, 0.5, 0.5), 1, 9, 1000);
    Fireworks* fireworks = new Fireworks(glm::vec4(0.1,0.3,0.9,1.0));
    fireworks->init(0.20, glm::vec3(0,27,0), glm::vec3(0,0,0), 500, 20, 1000, 0.99);

    inkGlyphs.setActor(&ball); 
    realMap.init();
    map.init();
    camera.setActor(&pHero);
    pHero.setWorld(&world);
    ball.setWorld(&world);
    rHero.setWorld(&realWorld);
    
    world.setID(0);
    numberables[0] = &world;
    
    world.insertCamera(&camera);
    world.insertActor(&pHero);
    world.insertActor(&ball);
  //  world.insertActor(&billow)
    world.insertParticleEffect(&mist);
  world.insertParticleEffect(fireworks);

   // world.insertParticleEffect(&inkGlyphs);
    world.setMap(map);
    
    DirectionalLight dl(glm::vec3(0.1,0.1,0.1),glm::vec3(0.2,0.2,0.2),glm::vec3(1.0,1.0,1.0),glm::vec3(0,-1,-1));
    world.setWeather(dl);

   /** realWorld.insertActor(&rHero);
    realWorld.setMap(realMap);
    DirectionalLight dl2(glm::vec3(0.5,0.5,0.5),glm::vec3(0.5,0.5,0.5),glm::vec3(1.0,1.0,1.0),glm::vec3(0,-1,-1));
    realWorld.setWeather(dl2);**/
    

    screen.print("Preparing the brushes...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
  /**  realRenderer = new Renderer;
    realRenderer->setWorld(&realWorld);
    realRenderer->setCamera(&camera);**/
    
    renderer = new Renderer;
    renderer->setWorld(&world);
    renderer->setCamera(&camera);
    
    screen.print("Opening the books...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    stbi_set_flip_vertically_on_load(0);
    
    ball.init();
    ball.setID(1);
    numberables[1] = &ball;
    
    screen.print("Flipping the pages...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    pHero.init();
    pHero.setID(2);
    numberables[2] = &pHero;
    
    pHero.setPos(glm::vec3(10,35,10));
    ball.setPos(glm::vec3(0,0.5,0));
   // billow.loadModel();
    rHero.init();

    screen.print("Putting on a fresh canvas...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    renderer->loadActorData();
    renderer->loadMapData();
    renderer->loadSkyBoxData();
    
 /**   realRenderer->loadActorData();
    realRenderer->loadMapData();
    realRenderer->loadSkyBoxData();**/
    
    stbi_set_flip_vertically_on_load(1);

    printf("%s\n", glGetString(GL_VERSION));

    int i;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &i);
    printf("%i\n", i);
    

    
    blank = stbi_load("Resources/Particles/rosa.png", &imageWidth, &imageHeight, &channels, 0);
     
    glGenTextures(1, &ftexture);
    glBindTexture(GL_TEXTURE_2D, ftexture);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,blank);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     glBindTexture(GL_TEXTURE_2D, 0);

    paint = stbi_load("Resources/Particles/pencil.jpg", &imageWidth, &imageHeight, &channels, 0);
  //  audio.playMusic();
    activeRenderer = renderer;
    
 //   BallScriptOne* bso = new BallScriptOne();
  //  ball.setScript(bso);
    script = new ScriptOne();
    script->init(this);
}

Game::~Game() {
    
}


void Game::tick() {
    glfwSetTime(glfwGetTime()-lastTime);
    lastTime = glfwGetTime();
    if (scheme == 0) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        pHero.posDir(0.015);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        pHero.posDir(-0.015);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        pHero.posRight(0.015);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        pHero.posRight(-0.015);
    }
    }
    
    inputHandler.tick();
    
    double mx, my;
    
    if (scheme == 2)     {
        glfwGetCursorPos(window, &mx, &my);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                //quad and framebuffer
                //write to framebuffer at that position
                for (int j = 0; j < 50; j++) {
           //  glTexSubImage2D(GL_TEXTURE_2D, 0, 800*(1-trail.at(i).x/1000), 800*(1-trail.at(i).y/800),      imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, paint);
                    float ratio1 = (float)(j/50.0f);
                    float ratio2 = 1.0f - ratio1;
                    glBindTexture(GL_TEXTURE_2D, ftexture);
                    //   glTexSubImage2D(GL_TEXTURE_2D, 0, 800*(1-(ratio2*lastMX+ratio1*mx)/1000), 800*(1-((ratio2*lastMY+ratio1*my)/800)), imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, paint);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 2342*(1-(ratio2*lastMX+ratio1*mx)/1000), 1982*(1-((ratio2*lastMY+ratio1*my)/800)), imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, paint);
                }
            }
    lastMY = my;
    lastMX = mx;
    }

    if(glfwWindowShouldClose(window)) {
        running = false;
    }
    
    if(ball.abilityQ.size()>0) {
        for(int i = 0; i < ball.abilityQ.size(); i++) {
            ball.abilityQ.at(i)->call(this);
            abilities.push_back(ball.abilityQ.at(i));
        }
        ball.abilityQ.clear();
    }
    
    if(pHero.abilityQ.size()>0) {
        for(int i = 0; i < pHero.abilityQ.size(); i++) {
            pHero.abilityQ.at(i)->call(this);
            abilities.push_back(pHero.abilityQ.at(i));
        }
        pHero.abilityQ.clear();
    }
    
    if (abilities.size() > 0) {
        for(int i = 0; i < abilities.size(); i++) {
            if(abilities.at(i)->on == true) {
                abilities.at(i)->tick();
            }
            
    }
        
    }
    
    script->tick();
    world.tick();
    
    if (abilities.size() > 0) {
        for(int i = 0; i < abilities.size(); i++) {
            if(abilities.at(i)->on == false) {
                abilities.at(i).reset();
                abilities.erase(abilities.begin()+i);
            }
    }
    }
   // printf("%f\n", glfwGetTime());
    if(printing)
    {print();}
    
    activeRenderer->render();

    glfwPollEvents();
        glfwSwapBuffers(window);
}

void Game::moveMouse(double mouseX_, double mouseY_) {
    if (scheme != 2) {
    if (firstMouse) {
        lastMX = mouseX_;
        lastMY = mouseY_;
        firstMouse = false;
    }
    xOffset = mouseX_ - lastMX;
    yOffset = lastMY - mouseY_;
    lastMX = mouseX_;
    lastMY = mouseY_;
    camera.incYaw(xOffset*0.03);
    camera.incPitch(yOffset*0.03);
    xOffset = 0;
    yOffset = 0;
    }
}

int Game::processInput(int key, int action, int mods) {
//    unique_lock<mutex> lock()
    if (scheme == 3) {
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            nextBranch = 0;
            if(activeDialogue != NULL) activeDialogue->branch(nextBranch);
            setActionScheme(0);
        }
            if (key == GLFW_KEY_B && action == GLFW_PRESS) {
                nextBranch = 1;
                if(activeDialogue != NULL) activeDialogue->branch(nextBranch);
                setActionScheme(0);
            }
    }
    if (scheme == 1) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE)
        {
          //  input.put(' ');
            i.append(" ");
        }
    const char* key_name = glfwGetKeyName((key), 0);
    if (key_name != NULL)
       // input << (key_name);
        if (mods == GLFW_MOD_SHIFT){
        char c = toupper(*key_name);
            if (key == GLFW_KEY_1) c = '!';
            if (key == GLFW_KEY_SLASH) c = '?';
            if (key == GLFW_KEY_APOSTROPHE) c = '\"';
        i.append(std::string(1, c));
        } else {
            i.append(key_name);
        }
    }
        
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
            scheme = 0;
          //  std::string test;
          //  input >> test;
            std::cout << i;
        printing = true;
            return 1;
        }
    }
    
    if (scheme == 0) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pHero.jump();
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&world, &pHero, 6);
        letters->call(this);
        abilities.push_back(letters);
    }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
            std::vector<std::string> lines = { "Joseph Gu - Programmer", "Yirou Guo - Creative Consultant and Artist", "Jonathan Ran - Mathematical and Physics Consultant"};
            std::shared_ptr<Ability> speech = std::make_shared<Speech>(&world, &pHero, 6.0, lines);
            abilities.push_back(speech);
            speech->call(this);
        }
        
        if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
            std::shared_ptr<Ability> fish = std::make_shared<Fish>(&world, &pHero, 18.0);
            abilities.push_back(fish);
            fish->call(this);
        }
        
        if (key == GLFW_KEY_X && action == GLFW_PRESS) {
       //     activeRenderer = realRenderer;
        //    camera.setActor(&rHero);
        }
        
        if (key == GLFW_KEY_E && action == GLFW_PRESS) {
            int newScheme = (-1)*(scheme-2);
            setActionScheme(newScheme);
            if (activeSketch.get() == NULL) {
            std::shared_ptr<Ability> sketch = std::make_shared<Sketch>(&world, &pHero, 6, ftexture);
            activeSketch = static_pointer_cast<Sketch>(sketch);
            abilities.push_back(sketch);
            sketch->call(this);
            }
            return 0;
        }
        
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            glBindTexture(GL_TEXTURE_2D, ftexture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2342, 1982, GL_RGB, GL_UNSIGNED_BYTE, blank);
         //   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 800, 800, GL_RGBA, GL_UNSIGNED_BYTE, blank);
            glBindTexture(GL_TEXTURE_2D, 0);
            if (activeSketch.get() != NULL) {
            abilities.erase(std::remove(abilities.begin(), abilities.end(), activeSketch));
            activeSketch.reset();
            }
        }
        
        if (key == GLFW_KEY_T && action == GLFW_PRESS) {
            if (activeSketch.get() != NULL)
            activeSketch->call2();
        }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
    //    inputHandler.setCharCallback(char_callback);
 //       inputHandler.setKeyCallback(onetap_callback0);
        scheme = 1;
    }
    }
    if (scheme == 2) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        int newScheme = (-1)*(scheme-2);
        setActionScheme(newScheme);
        firstMouse = true;
    }
    }
    return 0;
}

void Game::processInput2(int key, int action) {

}

void Game::print() {
    renderer->print(i);
    printing = false;
    i = "";
}

void Game::setActionScheme(int id) {
    if(id == 2) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    if(scheme == 2 && id !=2) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    scheme = id;
}

void Game::newDialogue(Dialogue& dialogue_) {
    activeDialogue = &dialogue_;
}

Numberable* Game::getNumberable(unsigned int ID_) {
    return numberables[ID_];
}
