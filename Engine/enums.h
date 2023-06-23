#ifndef ENUMS_H
#define ENUMS_H

typedef enum {
    STATE_TITLE,
    STATE_LEVEL,
    STATE_PAUSEMENU,
} GAMESTATE;

typedef enum {
    SUB_INIT,
    SUB_LOOP
} SUBSTATE;

typedef enum {
    PLAYER_IDLE,
    PLAYER_WALKING,
    PLAYER_DEAD,
    PLAYER_WON
} PLAYERSTATE;

typedef enum {
    ENEMY_IDLE,
    ENEMY_WALKING,
    ENEMY_DEAD,
    ENEMY_WON
} ENEMYSTATE;

typedef enum {
    DIR_UP = 0U,
    DIR_DOWN = 24U,
    DIR_LEFT = 36U,
    DIR_RIGHT = 12U
} DIRECTION;  // These are in increments of 12 to make the math faster when using directions for sprite frame indices

#endif
