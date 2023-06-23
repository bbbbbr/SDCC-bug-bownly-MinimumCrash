#include <gb/gb.h>
#include <rand.h>

#include "Engine/enums.h"
#include "Objects/PlayerObject.h"
#include "States/LevelState.h"

UINT8 vbl_count;
UINT8 curJoypad;
UINT8 prevJoypad;
UINT8 i;  // Used mostly for loops
UINT8 j;  // Used mostly for loops
UINT8 k;  // Used for whatever
UINT8 l;  // Used for whatever
UINT8 m;  // Used for menus generally
UINT8 n;  // Used for menus generally
UINT8 p;  // Used for passing values between states
UINT8 r;  // Used for randomization stuff

UINT8 gamestate = STATE_TITLE;
UINT8 substate;

UINT8 animFrame = 0U;
UINT8 animTick = 0U;

UINT16 camera_x = 0U;
UINT16 camera_y = 0U;

PlayerObject player;

void initRAM(UINT8);

void main(void)
{
 	// initRAM(0U);

    // Sound stuff
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels
    set_interrupts(TIM_IFLAG | VBL_IFLAG);
 
    SPRITES_8x16;

    init_bkg(0xFFU);
    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;

    gamestate = STATE_LEVEL;
    substate = SUB_INIT;
    
    while(1U)
    {
        wait_vbl_done();

        SWITCH_ROM(2U);
        LevelStateMain();

    }
}
