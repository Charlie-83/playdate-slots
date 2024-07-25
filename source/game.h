#pragma once
#include <pd_api.h>

#define SPRITE_SIZE 80
#define SMALL_SPRITE_SIZE 20
#define FONTSIZE 14
#define SCREEN_X 400
#define SCREEN_Y 240

typedef LCDSprite *Roller[3];
#define NUMBER_OF_ITEMS 2
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
  battle,
} Scene;
#define NUMBER_OF_UNITS 2
typedef enum Units {
  skeleton,
  ghoul,
} Units;

typedef unsigned int Inventory[NUMBER_OF_ITEMS];
typedef struct InventoryState {
  Inventory inventory;
  unsigned int selected;
} InventoryState;
typedef unsigned int Army[NUMBER_OF_UNITS];
typedef struct ArmyState {
  Army army;
} ArmyState;

typedef struct BattleState {
  unsigned int next_battle;
  int (*battles)[NUMBER_OF_UNITS];
  int total_battles;
  LCDSprite *sprites[2][5];
  Units selected_units[5];
  unsigned int currently_selected;
} BattleState;

typedef struct State {
  PlaydateAPI *pd;
  Roller rollers[3];
  float flashing_progress;
  LCDBitmap *slot_images[NUMBER_OF_ITEMS];
  LCDBitmap *small_slot_images[NUMBER_OF_ITEMS];
  LCDBitmap *unit_images[NUMBER_OF_UNITS];
  float roller_speed;
  RollerState roller_state;
  float roller_offset;
  unsigned int current_row;
  float mana;
  float mana_regen;
  unsigned int pull_cost;
  LCDSprite *sprites[3];
  Scene scene;
  InventoryState inventory_state;
  ArmyState army_state;
  BattleState battle_state;
} State;

void setup(PlaydateAPI *pd);
int update(void *userdata);
