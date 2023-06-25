#include <gb/gb.h>
#include <rand.h>


typedef enum {
    DIR_UP = 0U,
    DIR_DOWN = 24U,
    DIR_LEFT = 36U,
    DIR_RIGHT = 12U
} DIRECTION;  // These are in increments of 12 to make the math faster when using directions for sprite frame indices


 
typedef struct PlayerObject {
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



PlayerObject player;


static void func_1(void);
static void func_2(void);

void main(void)
{

    DISPLAY_ON;
    SHOW_BKG;    
    func_1();
}


UINT8 i;  // Used mostly for loops
UINT8 j;  // Used mostly for loops
UINT8 k;  // Used for whatever


static UINT8 playGrid[32U][32U];

#define STARTPOS 0U
#define STARTCAM 0U

static UINT8 roomId;
static UINT8 gridW;
static UINT8 gridH;
static UINT16 camera_max_x = 10U * 16U;
static UINT16 camera_max_y = 9U * 16U;
static UINT8 walkableTileCount = 6U;

static WORD camera_x = STARTCAM, camera_y = STARTCAM, new_camera_x = STARTCAM, new_camera_y = STARTCAM;
static UBYTE map_pos_x = STARTPOS, map_pos_y = STARTPOS, new_map_pos_x = STARTPOS, new_map_pos_y = STARTPOS;
static UBYTE redraw;

#define PLAYER_X_LEFT_BOUND_PX   8U
#define PLAYER_X_CENTER_BOUND_PX 88U
#define PLAYER_X_RIGHT_BOUND_PX  160U
#define PLAYER_Y_UP_BOUND_PX     16U
#define PLAYER_Y_CENTER_BOUND_PX 80U
#define PLAYER_Y_DOWN_BOUND_PX   152U
#define PLAYER_SPEED    21
#define LEFT_BOUND 48
#define RIGHT_BOUND 652
#define TOP_BOUND 48
#define BOTTOM_BOUND 652

#define pxToSubpx(px) ((px) << 4U)
#define subPxToPx(subpx) ((subpx) >> 4U)

#define tileToPx(tile) ((tile) << 3U)
#define pxToTile(px) ((px) >> 3U)

#define metatileToPx(metatile) ((metatile) << 4U)
#define pxToMetatile(px) ((px) >> 4U)



static void func_1(void)
{
    player.xVel = player.moveSpeed;
    player.yVel = 0;
    player.dir = DIR_RIGHT;

    func_2();
}


/******************************** HELPER METHODS *********************************/
static void func_2(void)
{
    // Hypothetical coords that include velocity changes
    INT16 x = player.xSpr + player.xVel;
    INT16 y = player.ySpr + player.yVel;

    UINT8 topOffset = 8;
    UINT8 bottomOffset = 1;
    UINT8 leftOffset = 4;
    UINT8 rightOffset = 5;

    // If you comment/uncomment one of these...
    static UINT8 playerTopMetatileIndex;
    playerTopMetatileIndex = pxToMetatile(subPxToPx(y) - 16U + topOffset);

    UINT8 playerBottomMetatileIndex = pxToMetatile(subPxToPx(y));
    UINT8 playerLeftMetatileIndex = pxToMetatile(subPxToPx(x) - 8U + leftOffset);
    UINT8 playerRightMetatileIndex = pxToMetatile(subPxToPx(x) + 8U - rightOffset);

    UINT8 collided = TRUE;
    switch (player.dir)
    {
        case DIR_UP:
            if ((playGrid[playerTopMetatileIndex][playerLeftMetatileIndex] < walkableTileCount)
             && (playGrid[playerTopMetatileIndex][playerRightMetatileIndex] < walkableTileCount))
                collided = FALSE;
            break;
        case DIR_DOWN:
            if ((playGrid[playerBottomMetatileIndex][playerLeftMetatileIndex] < walkableTileCount)
             && (playGrid[playerBottomMetatileIndex][playerRightMetatileIndex] < walkableTileCount))
                collided = FALSE;
            break;
        case DIR_LEFT:
            if ((playGrid[playerTopMetatileIndex][playerLeftMetatileIndex] < walkableTileCount)
             && (playGrid[playerBottomMetatileIndex][playerLeftMetatileIndex] < walkableTileCount))
                collided = FALSE;
            break;
        case DIR_RIGHT:
            if ((playGrid[playerTopMetatileIndex][playerRightMetatileIndex] < walkableTileCount)
             && (playGrid[playerBottomMetatileIndex][playerRightMetatileIndex] < walkableTileCount))
                collided = FALSE;
            break;
    }


    // Check for left wall collisions
    i = pxToMetatile(subPxToPx(x) - 8U);
    j = pxToMetatile(subPxToPx(y) - 16U);

    if (collided == FALSE)
    {
        player.xSpr = x;
        player.ySpr = y;
    }
    else
    {
        switch (player.dir)
        {
            case DIR_UP:    player.ySpr = pxToSubpx(metatileToPx(playerTopMetatileIndex + 1U) + topOffset);   break;
            case DIR_DOWN:  player.ySpr = pxToSubpx(metatileToPx(playerBottomMetatileIndex) - bottomOffset);  break;
            case DIR_LEFT:  player.xSpr = pxToSubpx(metatileToPx(playerLeftMetatileIndex + 1U) + leftOffset); break;
            case DIR_RIGHT: player.xSpr = pxToSubpx(metatileToPx(playerRightMetatileIndex) - leftOffset);     break;
            default: break;
        }

        player.xVel = 0U;
        player.yVel = 0U;
    }
}


// DEBUG TOOLS
const uint8_t tile_crash[] = { 0x00u, 0x81u, 0x00u, 0x81u, 0x00u, 0x42u, 0x00u, 0x42u, 0x00u, 0x24u, 0x00u, 0x24u, 0x00u, 0x18u, 0x00u, 0x18u};

void small_crash_handler(void) NONBANKED {
    set_bkg_data(0u,1u, tile_crash);
    while(1);
}

void crash_jump(void) __naked NONBANKED {
    __asm \
    .area _CRASH_HEADER(ABS)
    .org    0x38
    di
    jp _small_crash_handler
    __endasm;
}
