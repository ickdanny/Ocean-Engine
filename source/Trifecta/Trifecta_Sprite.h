#ifndef TRIFECTA_SPRITE
#define TRIFECTA_SPRITE

#include <OpenGL/gl3.h>

#include "ZMath.h"

#define _minScale 0.01f
#define _defaultRotation 0.0f
#define _defaultScale 1.0f

/* Represents a 2D image */
typedef struct TFSprite{
    GLuint _textureHandle;
    uint32_t width;
    uint32_t height;
} TFSprite;

/* Specifies how a sprite is to be drawn */
typedef struct TFSpriteInstruction{
    /* The pointer to the actual texture */
    TFSprite *spritePtr;

    /*
     * The depth at which to draw the sprite
     * (with respect to overlapping sprites)
     */
    int depth;

    /* 
     * The offset to apply from the center of the
     * entity to be drawn
     */
    Vector2D offset;

    /* The rotation angle in degrees CCW */
    float rotation;

    /* 
     * The scale factor to stretch the sprite in
     * both directions 
     */
    float scale;
} TFSpriteInstruction;

/* Loads a sprite from the specified .bmp file */
TFSprite parseBitmapFile(const char *fileName);

/* 
 * Constructs and returns a TFSpriteInstruction
 * by value
 */
TFSpriteInstruction tfSpriteInstructionMake(
    TFSprite *spritePtr,
    int depth,
    Vector2D offset,
    float rotation,
    float scale
);

/* 
 * Constructs and returns a TFSpriteInstruction
 * by value
 */
#define tfSpriteInstructionMakeSimple( \
    SPRITEPTR, \
    DEPTH, \
    OFFSET \
) \
    tfSpriteInstructionMake( \
        (SPRITEPTR), \
        (DEPTH), \
        (OFFSET), \
        _defaultRotation, \
        _defaultScale \
    )

#endif