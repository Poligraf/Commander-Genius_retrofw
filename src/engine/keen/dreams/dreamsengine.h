#ifndef DREAMSENGINE_H
#define DREAMSENGINE_H

#include <base/GsEngine.h>
#include <engine/core/GameEngine.h>
#include <base/utils/ThreadPool.h>
#include <memory>

namespace dreams
{


class DreamsEngine : public GameEngine
{
public:
    DreamsEngine(const bool openedGamePlay,
                 const std::string &dataPath) :
    GameEngine(openedGamePlay, dataPath) {}

    /**
     * @brief loadResources Load DOS data.
     * @return true if everything went well otherwise false.
     */
    bool loadResources();


    void GameLoop();


    /**
     * @brief InitGame initializes all the resources so the main game can run.
     */
    void InitGame();


    /**
     * @brief start Starts the Dreams engine which refers to accessing RefKeen code
     */
    void start();

    /**
     * @brief ponder    Everything logic related here
     * @param deltaT    timestep
     */
    void ponder(const float deltaT);


    /**
     * @brief updateHostDisplay Will update the display of all the possible states the game runs into
     */
    void updateHostDisplay();

    /**
     * @brief render    Everything to ne drawn here!
     */
    void render();


private:

    enum GameState
    {
        INTRO_TEXT,     // The famous screen where hardware is detected and some notes about the versions are told
        INTRO_SCREEN    // Within the gameloop it will show the intro screen of the dreams game
    } mGameState = INTRO_TEXT;


    std::unique_ptr<Action> mpPlayLoopAction;
    std::unique_ptr<ThreadPoolItem> mpPlayLoopThread;

    // Because some routines have loops in Refkeen we pass the main game to a separate thread and catch the apropiate signals
    //ThreadPoolItem *mpThread = nullptr;

};

}

// Events
struct StartDreamsEngine : public SwitchEngineEvent
{
    StartDreamsEngine(const bool openedGamePlay,
                      const std::string &dataPath) :
        SwitchEngineEvent( new dreams::DreamsEngine(openedGamePlay, dataPath) )
        { }
};


#endif // DREAMSENGINE_H
