#include <stdio.h>
#include <wchar.h>
#include <limits.h>
#include <time.h>

#include "ZMath.h"
#include "Constructure.h"
#include "MokyoMidi.h"
#include "Trifecta.h"

#include "Config.h"
#include "GameLoop.h"
#include "Settings.h"

#include <unistd.h>

void printInt(int *toPrint){
    printf("%d\n", *toPrint);
}

void printString(const String *toPrint){
    printf("%s\n", toPrint->_ptr);
}

void printWideString(const WideString *toPrint){
    printf("%ls\n", toPrint->_ptr);
}

size_t intHash(const void *intPtr){
    return *((int*)intPtr);
}

bool intEquals(
    const void *intPtr1, 
    const void *intPtr2
){
    return *((int*)intPtr1) == *((int*)intPtr2);
}

#define intToLong(i) ((long)i)

/* A struct holding all the elements of the engine */
typedef struct Engine{
    Settings settings;
    /* todo: resources */
    TFWindow window;
    /* todo: input */
    MidiHub midiHub;
    /* todo: game */
    GameLoop gameLoop;
} Engine;

/* Frees the specified Engine */
void engineFree(Engine *enginePtr){
    writeSettingsToFile(
        &(enginePtr->settings),
        config_settingsFileName
    );
    //todo: more frees
    tfWindowFree(&(enginePtr->window));
    midiHubFree(&(enginePtr->midiHub));
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
    //todo: update game
    //todo: pump messages? idk
}

/* Draws the specified Engine passed as a void ptr */
void renderCallback(void *voidPtr){
    Engine *enginePtr = (Engine*)voidPtr;
    //todo: render game
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

    /* init MIDI */
    engine.midiHub = midiHubMake(
        engine.settings.muted
    );

    //todo: init game
    //todo: set game fullscreen callback
    //todo: set game write settings callback

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
    /* todo: game set exit callback */

    /* make visible and start running */
    tfWindowMakeVisible(&(engine.window));
    gameLoopRun(&(engine.gameLoop));

    /* clean up after game ends */
    engineFree(&engine);

    printf("main completed\n");
    return 0;
}