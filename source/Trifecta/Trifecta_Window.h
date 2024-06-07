#ifndef TRIFECTA_WINDOW_H
#define TRIFECTA_WINDOW_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include "Constructure.h"

#include "Trifecta_Sprite.h"
#include "Trifecta_GlyphMap.h"

/* 
 * Represents a window which is capable of being
 * drawn to
 */
typedef struct TFWindow{
    GLFWwindow* _windowPtr;
    void *userPtr;
    void (*exitCallback)(void*);
} TFWindow;

/* Constructs and returns a TFWindow by value */
TFWindow tfWindowMake(
    bool fullscreen,
    const char* windowName,
    int graphicsWidth,
    int graphicsHeight,
    void *userPtr
);

/* Makes the given TFWindow visible */
void tfWindowMakeVisible(TFWindow *windowPtr);

/* 
 * Sets the exit callback for the given TFWindow;
 * the callback function takes a parameter of type
 * void* and will be passed the userPtr of the
 * given TFWindow
 */
void tfWindowSetExitCallback(
    TFWindow *windowPtr,
    void (*exitCallback)(void*)
);

/* Has the specified TFWindow render */
void tfWindowRender(TFWindow *windowPtr);

/* Draws a sprite onto the specified TFWindow */
void tfWindowDrawSprite(
    TFWindow *windowPtr,
    Point2D preOffsetCenter,
    const TFSpriteInstruction *spriteInstrPtr
);

/*
 * Draws a portion of a sprite onto the 
 * specified TFWindow
 */
void tfWindowDrawSubSprite(
    TFWindow *windowPtr,
    Point2D preOffsetCenter,
    const TFSpriteInstruction *spriteInstrPtr,
    const Rectangle *srcRectPtr
);

/* Draws a tiled sprite onto the specified TFWindow */
void tfWindowDrawTileSprite(
    TFWindow *windowPtr,
    const Rectangle *drawRectPtr,
    const TFSpriteInstruction *spriteInstrPtr,
    Point2D pixelOffset
);

/* Draws text onto the specified TFWindow */
void tfWindowDrawText(
    TFWindow *windowPtr,
    Point2D pos,
    const WideString *wideStringPtr,
    int rightBound,
    TFGlyphMap *glyphMapPtr
);

/* Frees the given TFWindow */
void tfWindowFree(TFWindow *windowPtr);

#endif