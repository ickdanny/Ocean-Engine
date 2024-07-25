#include <stdio.h>
#include <wchar.h>
#include <limits.h>
#include <time.h>

#include "ZMath.h"
#include "Constructure.h"
#include "MokyoMidi.h"
#include "Trifecta.h"
#include "BLoader.h"
#include "WindECS.h"
#include "Unknown.h"

#include "Config.h"
#include "Game.h"
#include "GameLoop.h"
#include "Resources.h"
#include "Settings.h"

#include "SystemDestructors.h"
#include "Scripts.h" /* to destroy vm pool */

#include <unistd.h>

/* A struct holding all the elements of the engine */
typedef struct Engine{
    Settings settings;
    Resources resources;
    TFWindow window;
    TFKeyTable keyTable;
    MidiHub midiHub;
    Game game;
    GameLoop gameLoop;
} Engine;

/* Frees the specified Engine */
void engineFree(Engine *enginePtr){
    writeSettingsToFile(
        &(enginePtr->settings),
        config_settingsFileName
    );
    resourcesFree(&(enginePtr->resources));
    tfWindowFree(&(enginePtr->window));
    tfKeyTableFree(&(enginePtr->keyTable));
    midiHubFree(&(enginePtr->midiHub));
    gameFree(&(enginePtr->game));
}

/*
 * A callback which stops the gameloop in the
 * specified Engine passed as a void ptr
 */
void stopGameLoopCallback(void *voidPtr){
    Engine *enginePtr = (Engine*)voidPtr;
    gameLoopStop(&(enginePtr->gameLoop));
}

/* Updates the specified Engine passed as a void ptr */
void updateCallback(void *voidPtr){
    Engine *enginePtr = (Engine*)voidPtr;
    gameUpdate(&(enginePtr->game));
    tfWindowPumpMessages(&(enginePtr->window));
}

/* Draws the specified Engine passed as a void ptr */
void renderCallback(void *voidPtr){
    Engine *enginePtr = (Engine*)voidPtr;
    gameRender(&(enginePtr->game));
    tfWindowRender(&(enginePtr->window));
}

/* The entry point for the game */
int main(){
    Engine engine = {0};

    /* read settings */
    engine.settings = readOrCreateSettingsFromFile(
        config_settingsFileName
    );

    /* init window */
    engine.window = tfWindowMake(
        engine.settings.fullscreen,
        config_windowName,
        config_windowWidth,
        config_windowHeight,
        config_graphicsWidth,
        config_graphicsHeight,
        &engine /* user ptr to engine for callbacks */
    );

    /* init key table input */
    engine.keyTable = tfKeyTableMake(&(engine.window));

    /* init resources (after window for OpenGL) */
    engine.resources = resourcesMake();
    resourcesLoadDirectory(
        &(engine.resources),
        "res/image"
    );
    resourcesLoadDirectory(
        &(engine.resources),
        "res/midi"
    );
    resourcesLoadDirectory(
        &(engine.resources),
        "res/dialogue"
    );
    resourcesLoadDirectory(
        &(engine.resources),
        "res/script"
    );

    //todo: what about loading the symbol map?
    //manually create with symbol_char in image?

    /* init MIDI */
    engine.midiHub = midiHubMake(
        engine.settings.muted
    );

    engine.game = gameMake(
        &(engine.settings),
        &(engine.resources),
        &(engine.window),
        &(engine.keyTable),
        &(engine.midiHub),
        &engine /* pass the engine as user ptr */
    );

    engine.gameLoop = gameLoopMake(
        config_updatesPerSecond,
        config_maxUpdatesPerFrame,
        &engine, /* update user ptr */
        &engine, /* render user ptr */
        updateCallback,
        renderCallback
    );

    /* set callbacks */
    tfWindowSetExitCallback(
        &(engine.window),
        stopGameLoopCallback
    );
    gameSetExitCallback(
        &(engine.game),
        stopGameLoopCallback
    );

    /* make visible and start running */
    tfWindowMakeVisible(&(engine.window));
    gameLoopRun(&(engine.gameLoop));

    /* clean up after game ends */
    engineFree(&engine);
    freeSystems();
    vmPoolDestroy();

    #ifdef _DEBUG
    printf("main completed\n");
    #endif

    return 0;
}