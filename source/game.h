#ifndef GAME_H
#define GAME_H
#include <pd_api.h>

typedef LCDSprite *Roller[3];
#define NUMBER_OF_SLOT_SPRITES 2
typedef enum Sprites {
  skull,
  bone,
  machine,
} Sprites;
typedef enum RollerState {
    spinning,
    slowing,
    stopping,
    stopped,
} RollerState;

typedef struct State {
  PlaydateAPI *pd;
  Roller rollers[3];
  LCDBitmap *sprites[3];
  float roller_speed;
  RollerState roller_state;
  float roller_offset;
  unsigned int current_row;
  unsigned int score;
} State;

void setup(PlaydateAPI *pd);
int update(void *userdata);
#endif
