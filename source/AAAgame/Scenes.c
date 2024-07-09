#include "Scenes.h"

/*
 * Constructs and returns a new SceneMessages by
 * value
 */
SceneMessages sceneMessagesMake(){
    //todo scene messages make
    SceneMessages toRet /* = {0} */;
    return toRet;
}

/* Clears the given SceneMessages */
void sceneMessagesClear(
    SceneMessages *sceneMessagePtr
){
    //todo scene messages clear
}

/*
 * Frees the memory associated with the specified
 * SceneMessgaes
 */
void sceneMessagesFree(
    SceneMessages *sceneMessagesPtr
){
    //todo scene message free
    memset(
        sceneMessagesPtr,
        0,
        sizeof(*sceneMessagesPtr)
    );
}

/* Constructs and returns a new Scene by value */
Scene sceneMake(
    SceneID id,
    size_t entityCapacity,
    WindComponents *componentsPtr,
    bool refresh,
    bool updateTransparent,
    bool renderTransparent
){
    Scene toRet = {
        id,
        refresh,
        updateTransparent,
        renderTransparent
    };
    toRet.ecsWorld = windWorldMake(
        entityCapacity,
        componentsPtr
    );
    toRet.sceneMessages = sceneMessagesMake();
    return toRet;
}

/*
 * Refreshes a scene if it was set to need refreshing,
 * does nothing otherwise
 */
void sceneRefresh(Scene *scenePtr){
    if(scenePtr->_refresh){
        /* clear ecs world */
        windWorldClear(&(scenePtr->ecsWorld));

        /* clear messages */
        sceneMessagesClear(&(scenePtr->sceneMessages));
    }
}

/* Frees the memory associated with the given Scene */
void sceneFree(Scene *scenePtr){
    windWorldFree(&(scenePtr->ecsWorld));
    sceneMessagesFree(&(scenePtr->sceneMessages));
    memset(scenePtr, 0, sizeof(*scenePtr));
}

/*
 * helper function to construct a scene for the
 * given Scenes object
 */
void scenesDefineScene(
    Scenes *scenesPtr,
    SceneID id,
    size_t entityCapacity,
    WindComponents *componentsPtr,
    bool refresh,
    bool updateTransparent,
    bool renderTransparent
){
    arraySet(Scene,
        &(scenesPtr->_sceneStorage),
        id,
        sceneMake(
            id,
            entityCapacity,
            componentsPtr,
            refresh,
            updateTransparent,
            renderTransparent
        )
    );
}

/*
 * Constructs and returns a new Scenes object by value
 */
Scenes scenesMake(WindComponents *componentsPtr){
    Scenes toRet = {0};
    toRet._sceneStorage = arrayMake(Scene,
        scene_numScenes
    );
    toRet._sceneStack = arrayListMake(SceneID,
        scene_numScenes
    );

    #define defineScene( \
        ID, \
        ENTITYCAPACITY, \
        REFRESH, \
        TRANSPARENCY \
    ) \
        arraySet(Scene, \
            &(toRet._sceneStorage), \
            ID, \
            sceneMake( \
                ID, \
                ENTITYCAPACITY, \
                componentsPtr, \
                REFRESH, \
                TRANSPARENCY \
            ) \
        )
    #define notTransparent false, false
    #define renderTransparent false, true
    #define fullyTransparent true, true

    defineScene(scene_main,
        20,     false,  notTransparent
    );
    defineScene(scene_difficulty,
        20,     false,  notTransparent
    );
    defineScene(scene_stage,
        20,     false,  notTransparent
    );
    defineScene(scene_music,
        20,     true,   renderTransparent
    );
    defineScene(scene_options,
        20,     true,   renderTransparent
    );
    defineScene(scene_loading,
        5,      false,  renderTransparent
    );
    defineScene(scene_game,
        1000,   true,   notTransparent
    );
    defineScene(scene_dialogue,
        5,      true,   fullyTransparent
    );
    defineScene(scene_pause,
        5,      true,   renderTransparent
    );
    defineScene(scene_continues,
        10,     true,   renderTransparent
    );
    defineScene(scene_credits,
        20,     true,   notTransparent
    );

    return toRet;

    #undef defineScene
    #undef noTransparency
    #undef renderTransparent
    #undef fullyTransparent
}

/*
 * Pushes the requested scene onto the top of the scene
 * stack in the given Scenes object
 */
void scenesPush(Scenes *scenesPtr, SceneID sceneID){
    arrayListPushBack(SceneID,
        &(scenesPtr->_sceneStack),
        sceneID
    );
}

/*
 * Pops the top scene off the scene stack in the given
 * Scenes object
 */
void scenesPop(Scenes *scenesPtr){
    arrayListPopBack(SceneID,
        &(scenesPtr->_sceneStack)
    );
}

/*
 * Pops scenes off the scene stack in the given Scenes
 * object until the requested scene is reached; error
 * if stack underflow
 */
void scenesPopTo(
    Scenes *scenesPtr,
    SceneID sceneToFind
){
    SceneID topSceneID = arrayListBack(SceneID,
        &(scenesPtr->_sceneStack)
    );
    while(topSceneID != sceneToFind){
        scenesPop(scenesPtr);
        topSceneID = arrayListBack(SceneID,
            &(scenesPtr->_sceneStack)
        );
    }
}

/*
 * Returns the current number of scenes on the stack in
 * the specified Scenes object
 */
int scenesCurrentCount(Scenes *scenesPtr){
    return scenesPtr->_sceneStack.size;
}

/*
 * Returns a pointer to the scene at the ith position
 * on the scene stack in the specified Scenes pointer
 * (where higher is closer to the top, zero indexed);
 * error if invalid index
 */
Scene *scenesGetScene(Scenes *scenesPtr, int i){
    if(i < 0 || i >= scenesPtr->_sceneStorage.size){
        pgError(
            "requested scene not on stack; "
            SRC_LOCATION
        );
    }
    SceneID sceneID = arrayListGet(SceneID,
        &(scenesPtr->_sceneStack),
        i
    );
    return arrayGetPtr(Scene,
        &(scenesPtr->_sceneStorage),
        sceneID
    );
}

/*
 * Frees the memory associated with the given Scenes
 * object
 */
void scenesFree(Scenes *scenesPtr){
    arrayApply(Scene,
        &(scenesPtr->_sceneStorage),
        sceneFree
    );
    arrayFree(Scene, &(scenesPtr->_sceneStorage));
    arrayListFree(SceneID, &(scenesPtr->_sceneStack));
    memset(scenesPtr, 0, sizeof(*scenesPtr));
}