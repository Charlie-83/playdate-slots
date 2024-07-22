#ifndef GAME_H
#define GAME_H
#include <pd_api.h>

typedef LCDSprite *Roller[3];
#define NUMBER_OF_SLOT_SPRITES 2
typedef enum SlotItems {
  skull,
  bone,
} SlotItems;
typedef enum RollerState {
  spinning,
  slowing,
  stopping,
  stopped,
} RollerState;
typedef enum Sprites {
  machine,
} Sprites;
typedef enum Scene {
  slots,
  inventory,
} Scene;

typedef unsigned int Inventory[NUMBER_OF_SLOT_SPRITES];

typedef struct State {
  PlaydateAPI *pd;
  Roller rollers[3];
  LCDBitmap *images[3];
  float roller_speed;
  RollerState roller_state;
  float roller_offset;
  unsigned int current_row;
  float mana;
  float mana_regen;
  unsigned int pull_cost;
  Inventory inventory;
  LCDSprite *sprites[3];
  Scene scene;
} State;

void setup(PlaydateAPI *pd);
int update(void *userdata);
#endif
