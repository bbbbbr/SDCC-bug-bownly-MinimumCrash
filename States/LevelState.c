#include <gb/gb.h>
#include <rand.h>
#include <string.h>

#include "../Engine/common.h"
#include "../Engine/enums.h"

#include "../Assets/Tiles/fontTiles.h"
#include "../Assets/Tiles/TadTiles.h"

#include "../Objects/PlayerObject.h"

extern UINT8 curJoypad;
extern UINT8 prevJoypad;
extern UINT8 i;  // Used mostly for loops
extern UINT8 j;  // Used mostly for loops
extern UINT8 k;  // Used for whatever
extern INT8 l;  // Used for whatever
extern UINT8 m;  // Used for menus generally
extern UINT8 n;  // Used for menus generally
extern UINT8 p;  // Used for passing values between states
extern UINT8 r;  // Used for randomization stuff

extern UINT8 animTick;
extern UINT8 animFrame;

extern PlayerObject player;
UINT8 playerstate;

extern UINT8 gamestate;
extern UINT8 substate;
static UINT8 playGrid[32U][32U];

#define STARTPOS 0U
#define STARTCAM 0U

static UINT8 roomId;
static UINT8 gridW;
static UINT8 gridH;
static UINT16 camera_max_x = 10U * 16U;
static UINT16 camera_max_y = 9U * 16U;
static UINT8 walkableTileCount = 6U;

// current and new positions of the camera in pixels
static WORD camera_x = STARTCAM, camera_y = STARTCAM, new_camera_x = STARTCAM, new_camera_y = STARTCAM;
// current and new position of the map in tiles
static UBYTE map_pos_x = STARTPOS, map_pos_y = STARTPOS, new_map_pos_x = STARTPOS, new_map_pos_y = STARTPOS;
// redraw flag, indicates that camera position was changed
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


/* SUBSTATE METHODS */
static void phaseInit(void);
static void phaseLoop(void);

/* INPUT METHODS */
static void inputs(void);

/* HELPER METHODS */
static void calcPhysics(void);
static void commonInit(void);

/* DISPLAY METHODS */
static void animatePlayer(void);


void LevelStateMain(void)
{
    curJoypad = joypad();

    switch (substate)
    {
        case SUB_INIT:
            phaseInit();
            break;
        case SUB_LOOP:
            phaseLoop();
            break;
        default:  // Abort to title in the event of unexpected state
            gamestate = STATE_TITLE;
            substate = SUB_INIT;
            break;
    }
    prevJoypad = curJoypad;
}


/******************************** SUBSTATE METHODS *******************************/
static void phaseInit(void)
{
    // Initializations
    animTick = 0U;
    
    HIDE_WIN;
    SHOW_SPRITES;
    SPRITES_8x16;

    // // Check levelId, pull appropriate level
    // loadLevel();

    player.xTile = 1U;
    player.yTile = 1U;
    player.xSpr = 88U;
    player.ySpr = 88U;

    commonInit();

    substate = SUB_LOOP;

    player.moveSpeed = 21U;
    set_bkg_tile_xy(18U,17U,player.moveSpeed/10U);
    set_bkg_tile_xy(19U,17U,player.moveSpeed%10U);

    // fadein();
    OBP0_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_WHITE, DMG_LITE_GRAY, DMG_BLACK);
}

static void phaseLoop(void)
{
    ++animTick;
    inputs();
    animatePlayer();
    if (player.state == PLAYER_WALKING)
        calcPhysics();
}


/******************************** INPUT METHODS *********************************/
static void inputs(void)
{
    // Movement
    if (player.state == PLAYER_IDLE || player.state == PLAYER_WALKING)
    {
        if (curJoypad & J_UP)
        {
            if (player.xSpr != TOP_BOUND)
            {
                player.xVel = 0;
                player.yVel = -player.moveSpeed;
                player.state = PLAYER_WALKING;
                player.dir = DIR_UP;
            }
        }
        else if (curJoypad & J_DOWN)
        {
            if (player.xSpr != BOTTOM_BOUND)
            {
                player.xVel = 0;
                player.yVel = player.moveSpeed;
                player.state = PLAYER_WALKING;
                player.dir = DIR_DOWN;
            }
        }
        else if (curJoypad & J_LEFT)
        {
            if (player.xSpr != LEFT_BOUND)
            {
                player.xVel = -player.moveSpeed;
                player.yVel = 0;
                player.state = PLAYER_WALKING;
                player.dir = DIR_LEFT;
            }
        }
        else if (curJoypad & J_RIGHT)
        {
            if (player.xSpr != RIGHT_BOUND)
            {
                player.xVel = player.moveSpeed;
                player.yVel = 0;
                player.state = PLAYER_WALKING;
                player.dir = DIR_RIGHT;
            }
        }

        else if (player.state == PLAYER_WALKING && !(curJoypad & 0x00001111))
        {
            player.state = PLAYER_IDLE;
            player.xVel = 0;
            player.yVel = 0;
        }
    }

}


/******************************** HELPER METHODS *********************************/
static void calcPhysics(void)
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
    // static UINT8 playerBottomMetatileIndex;
    // playerBottomMetatileIndex = pxToMetatile(subPxToPx(y));
    // static UINT8 playerLeftMetatileIndex;
    // playerLeftMetatileIndex = pxToMetatile(subPxToPx(x) - 8U + leftOffset);
    // static UINT8 playerRightMetatileIndex;
    // playerRightMetatileIndex = pxToMetatile(subPxToPx(x) + 8U - rightOffset);

    // ...Then you'll need to uncomment/comment the equivalent one of these
    // UINT8 playerTopMetatileIndex = pxToMetatile(subPxToPx(y) - 16U + topOffset);
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
    set_bkg_tile_xy(0,0,i);
    set_bkg_tile_xy(1,0,j);


set_bkg_tile_xy(1,5, playerTopMetatileIndex);
set_bkg_tile_xy(1,7, playerBottomMetatileIndex);
set_bkg_tile_xy(0,6, playerLeftMetatileIndex);
set_bkg_tile_xy(2,6, playerRightMetatileIndex);

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

static void commonInit(void)
{
    // Screen initializations
    init_bkg(0xFFU);
    for (i = 0U; i != 39U; ++i)
        move_sprite(i, 0U, 0U);

    // Initializations
    animTick = 0U;
    playerstate = PLAYER_IDLE;

    player.xSpr = 1000;
    player.ySpr = 1000;

    set_bkg_data(0x00U, 10U, fontTiles);
    set_sprite_data(0U, 48U, owTadTiles);

    // Check player coords/dir, draw player appropriately
    SCX_REG = camera_x; SCY_REG = camera_y;

    animatePlayer();

}

/******************************** DISPLAY METHODS ********************************/
static void animatePlayer(void)
{
    switch (player.state)
    {
        default:
            animFrame = 1U;
            animFrame += (player.dir >> 2U);
            break;
        case PLAYER_WALKING:
            animFrame = animTick % 32U;
            animFrame /= 8U;
            if (animFrame == 3U)
                animFrame = 1U;
            animFrame += (player.dir >> 2U);
            break;
    }
    animFrame <<= 2U;

    move_sprite(0U, subPxToPx(player.xSpr),      subPxToPx(player.ySpr));
    move_sprite(1U, subPxToPx(player.xSpr) + 8U, subPxToPx(player.ySpr));

    set_sprite_tile(0U, 0U + animFrame);
    set_sprite_tile(1U, 2U + animFrame);

}
