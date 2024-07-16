#include "OutboundSystem.h"

static Bitset accept;
static bool initialized = false;

/* destroys the outbound system */
static void destroy(){
    if(initialized){
        bitsetFree(&accept);
        initialized = false;
    }
}

/* inits the outbound system */
static void init(){
    if(!initialized){
        accept = bitsetMake(numComponents);
        bitsetSet(&accept, PositionID);
        bitsetSet(&accept, OutboundID);

        registerSystemDestructor(destroy);
        
        initialized = true;
    }
}

/*
 * Transforms a point to be within the specified
 * boundary from the outside of the game field
 */
static void inboundPoint(
    Point2D *pointPtr,
    Inbound bound
){
	float lowXBound = bound + config_gameOffsetX;
    float lowYBound = bound + config_gameOffsetY;
	float highXBound = config_gameWidth - bound
        + config_gameOffsetX;
	float highYBound = config_gameHeight - bound
        + config_gameOffsetY;
	if (pointPtr->x < lowXBound) {
		pointPtr->x = lowXBound;
	}
	else if (pointPtr->x > highXBound) {
		pointPtr->x = highXBound;
	}
	if (pointPtr->y < lowYBound) {
		pointPtr->y = lowYBound;
	}
	else if (pointPtr->y > highYBound) {
		pointPtr->y = highYBound;
	}
}

/*
 * Removes entities which are outside of a certain
 * boundary from the edge of the game
 */
void outboundSystem(Game *gamePtr, Scene *scenePtr){
    init();

    /* get entities with position and velocity */
    WindQueryItr itr = windWorldRequestQueryItr(
        &(scenePtr->ecsWorld),
        &accept,
        NULL
    );
    while(windQueryItrHasEntity(&itr)){
        Position *positionPtr = windQueryItrGetPtr(
            Position,
            &itr
        );
        Outbound bound = windQueryItrGet(
            Outbound,
            &itr
        );

        if(isOutOfBounds(
            positionPtr->currentPos,
            bound)
        ){
            windWorldQueueIDRemoveEntity(
                &(scenePtr->ecsWorld),
                windQueryItrCurrentID(&itr)
            );
        }
        
        windQueryItrAdvance(&itr);
    }
    windWorldHandleOrders(&(scenePtr->ecsWorld));
}