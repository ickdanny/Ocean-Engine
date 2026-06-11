#ifndef CONFIG_H
#define CONFIG_H

/* The name of the game window */
#define config_windowName "OYCN v1.1"

/* The file name of the settings file */
#define config_settingsFileName "oycn.cfg"

/* The width of the game window */
#define config_windowWidth 640

/* The height of the game window */
#define config_windowHeight 480

/* The pixel width of the game */
#define config_graphicsWidth (config_windowWidth / 2)

/* The pixel height of the game */
#define config_graphicsHeight (config_windowHeight / 2)

/* The number of game updates per second */
#define config_updatesPerSecond 60

/* The maximum number of updates between frames */
#define config_maxUpdatesPerFrame 5

/* The maximum number of entities for the game */
#define config_gameMaxEntities 6000

/* Width of the playable area (maps to pixels) */
#define config_gameWidth 170.0f

/* Height of the playable area (maps to pixels) */
#define config_gameHeight 214.0f

/* X offset of the game from the upper left corner */
#define config_gameOffsetX 15.0f

/* Y offset of the game from the upper left corner */
#define config_gameOffsetY 13.0f

/*
 * Vector offset of the game from the upper left corner
 */
#define config_gameOffset \
    ((Vector2D){ \
        config_gameOffsetX, \
        config_gameOffsetY \
    })

/*
 * The square radius of the outer boundry from the
 * game after which collisions are not checked for
 */
#define config_collisionOutbound 100.0f

/*
 * The bounding box outside of which collisions are not
 * detected in screen pixel coordinates
 */
#define config_collisionBounds \
    ((AABB){ \
        config_gameOffsetX \
            - config_collisionOutbound, \
        config_gameOffsetX + config_gameWidth \
            + config_collisionOutbound, \
        config_gameOffsetY \
            - config_collisionOutbound, \
        config_gameOffsetY + config_gameHeight \
            + config_collisionOutbound \
    })

/* X coordinate of the player spawn position */
#define config_playerSpawnX \
    (config_gameWidth / 2.0f) + config_gameOffsetX

/* Y coordinate of the player spawn position */
#define config_playerSpawnY \
    12.0f + config_gameOffsetY

/* Point coordinate of the player spawn position */
#define config_playerSpawn \
    ((Point2D){ \
        config_playerSpawnX, \
        config_playerSpawnY \
    })

/* Square radius of the player hitbox */
#define config_playerHitboxRadius 1.5f

#define config_playerHitbox \
    aabbMakeRadius(config_playerHitboxRadius)

/* Speed of the player in pixels per second */
#define config_playerSpeed 166.0f

/*
 * Speed multiplier applied to the player when focused
 */
#define config_focusSpeedMultiplier 0.5f

/*
 * Speed of the player when focused in pixels per
 * second
 */
#define config_focusedSpeed \
    (config_playerSpeed * config_focusSpeedMultiplier)

/*
 * The square radius from the outer edge of the 
 * playable area which the player is not allowed to
 * move out of
 */
#define config_playerInbound 5.0f

/*
 * The number of game ticks the player is able to
 * death bomb
 */
#define config_deathBombTicks 15

/* The number of game ticks the player is dead for */
#define config_deathTicks 30

/*
 * The number of game ticks the player is respawning
 * for
 */
#define config_respawnTicks 20

/*
 * The number of game ticks the player has IFrames
 * after respawning for
 */
#define config_respawnIFrameTicks 180

/*
 * The number of game ticks the player has IFrames
 * when bombing
 */
#define config_bombIFrameTicks 300

/*
 * The initial number of lives given to players at the
 * start of gameplay
 */
#define config_initLives 3

/*
 * The initial number of bombs given to players at the
 * start of gameplay
 */
#define config_initBombs 2

/*
 * The initial number of continues given to players at
 * the start of gameplay
 */
#define config_initContinues 2
	
/* The maximum number of lives a player can accrue */
#define config_maxLives 6

/* The maximum number of bombs a player can accrue */
#define config_maxBombs config_maxLives	

/* The maximum shot power a player can reach */
#define config_maxPower 80

/*
 * The number of lives given to a player upon the use
 * of a continue
 */
#define config_continueLives 2

/*
 * The number of bombs given to a player upon
 * respawning
 */
#define config_respawnBombs 1

/*
 * The amount of power gained from acquiring a small
 * power item
 */	
#define config_smallPowerGain 1

/*
 * The amount of power gained from acquiring a large
 * power item
 */
#define config_largePowerGain 5

/*
 * The depth offset between different categories of
 * graphical objects
 */
#define config_depthOffset 5000

/* The minimum depth for game entities inclusive */
#define config_minDepth -30000

/* The base depth for background graphics */	
#define config_backgroundDepth config_minDepth

/* The base depth for effect graphics */
#define config_effectDepth \
    config_backgroundDepth + config_depthOffset

/* The base depth for player bullets */
#define config_playerBulletDepth \
    config_effectDepth + config_depthOffset

/* The base depth for enemies */
#define config_enemyDepth \
    config_playerBulletDepth + config_depthOffset

/* The base depth for pickups */
#define config_pickupDepth \
    config_enemyDepth + config_depthOffset

/* The base depth for the player */
#define config_playerDepth \
    config_pickupDepth + config_depthOffset

/* The base depth for enemy bullets category 0 */
#define config_enemyBulletDepth0 \
    config_playerDepth + config_depthOffset

/* The base depth for enemy bullets category 1 */
#define config_enemyBulletDepth1 \
    config_enemyBulletDepth0 + config_depthOffset

/* The base depth for enemy bullets category 2 */
#define config_enemyBulletDepth2 \
    config_enemyBulletDepth1 + config_depthOffset

/* The base depth for enemy bullets category 3 */
#define config_enemyBulletDepth3 \
    config_enemyBulletDepth2 + config_depthOffset

/* The base depth for enemy bullets category 4 */
#define config_enemyBulletDepth4 \
    config_enemyBulletDepth3 + config_depthOffset

/* The base depth for foreground graphics */
#define config_foregroundDepth \
    config_enemyBulletDepth4 + config_depthOffset

/* The maximum depth for game entities exclusive */
#define config_maxDepth \
    config_foregroundDepth + config_depthOffset

#endif