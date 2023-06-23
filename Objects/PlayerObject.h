#include <gb/gb.h>
#include "../Engine/enums.h"

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H
 
typedef struct PlayerObject {
    PLAYERSTATE state;
    UINT8 spriteId;
    UINT8 animFrame;
    WORD xSpr;  // Assumes 4 left shifted bits of subpixels
    WORD ySpr;
    INT16 xMap;
    INT16 yMap;
    UINT8 xTile;
    UINT8 yTile;
    UINT8 topOffsetInPx;  // Offset from top to center in px
    UINT8 bottomOffsetInPx;
    UINT8 leftOffsetInPx;
    UINT8 rightOffsetInPx;
    DIRECTION dir;
    UINT8 moveSpeed;
    INT8 xVel;
    INT8 yVel;
    UINT8 hpMax;
    UINT8 hpCur;
} PlayerObject;

#endif
