#include "SceneUpdater.h"

#include "InitSystem.h"
#include "MessageCleanupSystem.h"
#include "InputSystem.h"
#include "MenuNavigationSystem.h"
#include "ButtonSpriteSystem.h"
#include "GameBuilderSystem.h"
#include "LoadingScreenSystem.h"
#include "DialogueSystem.h"
#include "ScriptSystem.h"
#include "PlayerMovementSystem.h"
#include "VelocitySystem.h"
#include "InboundSystem.h"
#include "CollisionDetectionSystem.h"
#include "CollisionHandlerSystem.h"
#include "ClearSystem.h"
#include "PlayerLifeAddSystem.h"
#include "PlayerBombAddSystem.h"
#include "PlayerShotSystem.h"
#include "PlayerStateSystem.h"
#include "PlayerBombSystem.h"
#include "PlayerDeathSystem.h"
#include "ContinueSystem.h"
#include "PlayerRespawnSystem.h"
#include "PlayerReactivateSystem.h"
#include "OverlaySystem.h"
#include "DeathHandlerSystem.h"
#include "PauseSystem.h"
#include "AnimationSystem.h"
#include "RotateSpriteForwardSystem.h"
#include "SpriteSpinSystem.h"
#include "TileScrollSystem.h"
#include "OutboundSystem.h"
#include "GameOverSystem.h"
#include "CreditsSystem.h"

/* Updates the specified scene */
void updateScene(Game *gamePtr, Scene *scenePtr){
    initSystem(gamePtr, scenePtr);
    messageCleanupSystem(gamePtr, scenePtr);
    inputSystem(gamePtr, scenePtr);
    menuNavigationSystem(gamePtr, scenePtr);
    buttonSpriteSystem(gamePtr, scenePtr);
    gameBuilderSystem(gamePtr, scenePtr);
    loadingScreenSystem(gamePtr, scenePtr);
    dialogueSystem(gamePtr, scenePtr);
    scriptSystem(gamePtr, scenePtr);
    playerMovementSystem(gamePtr, scenePtr);
    velocitySystem(gamePtr, scenePtr);
    inboundSystem(gamePtr, scenePtr);
    collisionDetectionSystem(gamePtr, scenePtr);
    collisionHandlerSystem(gamePtr, scenePtr);
    clearSystem(gamePtr, scenePtr);
    playerLifeAddSystem(gamePtr, scenePtr);
    playerBombAddSystem(gamePtr, scenePtr);
    playerShotSystem(gamePtr, scenePtr);
    playerStateSystem(gamePtr, scenePtr);
    playerBombSystem(gamePtr, scenePtr);
    playerDeathSystem(gamePtr, scenePtr);
    continueSystem(gamePtr, scenePtr);
    playerRespawnSystem(gamePtr, scenePtr);
    playerReactivateSystem(gamePtr, scenePtr);
    overlaySystem(gamePtr, scenePtr);
    deathHandlerSystem(gamePtr, scenePtr);
    pauseSystem(gamePtr, scenePtr);
    animationSystem(gamePtr, scenePtr);
    rotateSpriteForwardSystem(gamePtr, scenePtr);
    spriteSpinSystem(gamePtr, scenePtr);
    tileScrollSystem(gamePtr, scenePtr);
    outboundSystem(gamePtr, scenePtr);
    gameOverSystem(gamePtr, scenePtr);
    creditsSystem(gamePtr, scenePtr);

    printf("address of midihub: %p\n", gamePtr->midiHubPtr);
    printf("address of midiout: %p\n", gamePtr->midiHubPtr->midiSequencer.midiOutPtr);
}