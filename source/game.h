#ifndef GAME_H
#define GAME_H
#include <pd_api.h>

#define SPRITE_SIZE 80
#define SMALL_SPRITE_SIZE 20
#define FONTSIZE 14
#define SCREEN_X 400
#define SCREEN_Y 240

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
#define NUMBER_OF_UNITS 1
typedef enum Units {
    skeleton
} Units;

typedef unsigned int Inventory[NUMBER_OF_SLOT_SPRITES];
typedef struct InventoryState {
    unsigned int selected;
    float flashing_progress;
} InventoryState;

typedef struct State {
  PlaydateAPI *pd;
  Roller rollers[3];
  LCDBitmap *slot_images[NUMBER_OF_SLOT_SPRITES];
  LCDBitmap *small_slot_images[NUMBER_OF_SLOT_SPRITES];
  LCDBitmap *unit_images[NUMBER_OF_UNITS];
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
  InventoryState inventory_state;
} State;

void setup(PlaydateAPI *pd);
int update(void *userdata);
#endif
