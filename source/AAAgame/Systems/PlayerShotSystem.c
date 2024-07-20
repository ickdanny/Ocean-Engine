#include "PlayerShotSystem.h"

#include "GameCommand.h"

static Bitset accept;
static bool initialized = false;

/* destroys the player shot system */
static void destroy(){
    if(initialized){
        bitsetFree(&accept);
        initialized = false;
    }
}

/* inits the player shot system */
static void init(){
    if(!initialized){
        accept = bitsetMake(numComponents);
        bitsetSet(&accept, PlayerDataID);

        registerSystemDestructor(destroy);
        
        initialized = true;
    }
}

/*
 * Adds the shot script to the player in script
 * slot 3
 */
static void addPlayerShot(
    Game *gamePtr,
    Scene *scenePtr
){
    WindQueryItr itr = windWorldRequestQueryItr(
        &(scenePtr->ecsWorld),
        &accept,
        NULL
    );
    while(windQueryItrHasEntity(&itr)){
        WindEntity handle = windWorldMakeHandle(
            &(scenePtr->ecsWorld),
            windQueryItrCurrentID(&itr)
        );
        /* if player has scripts, just add it */
        if(windWorldHandleContainsComponent(Scripts,
            &(scenePtr->ecsWorld),
            handle
        )){
            Scripts *scriptsPtr
                = windWorldHandleGetPtr(Scripts,
                    &(scenePtr->ecsWorld),
                    handle
                );
            /* if slot 3 is empty */
            if(!scriptsPtr->vm3){
                scriptsPtr->vm3 = vmPoolRequest();
                unVirtualMachineLoad(
                    scripts->vm3,
                    //todo: resources get script
                )
            }
        }
        windQueryItrAdvance(&itr);
    }

    windWorldHandleOrders(&(scenePtr->ecsWorld));
}

/*
 * Detects when the player inputs a shot and adds
 * the shot script to the player
 */
void playerShotSystem(Game *gamePtr, Scene *scenePtr){
    init();

    ArrayList *gameCommandsPtr
        = &(scenePtr->messages.gameCommands);

    for(int i = 0; i < gameCommandsPtr->size; ++i){
        if(arrayListGet(GameCommand,
                gameCommandsPtr,
                i
            ) == game_shoot
        ){
            addPlayerShot(gamePtr, scenePtr);
            break;
        }
    }
}