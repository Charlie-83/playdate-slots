#include "game.h"
#include "inventory.h"
#include "pd_api.h"
#include "slots.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int starting_positions[3][3][2] = {{{88, 40}, {88, 120}, {88, 200}},
                                   {{200, 40}, {200, 120}, {200, 200}},
                                   {{312, 40}, {312, 120}, {312, 200}}};

void setup(PlaydateAPI *pd) {
  const char *err;
  const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
  LCDFont *font = pd->graphics->loadFont(fontpath, &err);
  if (font == NULL)
    pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

  State *state = (State *)malloc(sizeof(State));
  state->pd = pd;
  state->roller_speed = 1000;
  state->roller_state = spinning;
  state->roller_offset = 40;
  state->mana = 10;
  state->current_row = 1;
  state->mana_regen = 1;
  state->pull_cost = 10;
  state->scene = slots;
  for (int i = 0; i < NUMBER_OF_SLOT_SPRITES; ++i)
    state->inventory[i] = 0;

  pd->system->setUpdateCallback(update, state);
  LCDBitmap *machine_image = pd->graphics->loadBitmap("machine.png", &err);
  if (machine_image == NULL)
    pd->system->error(err);
  LCDSprite *machine_sprite = pd->sprite->newSprite();
  pd->sprite->setImage(machine_sprite, machine_image, kBitmapUnflipped);
  pd->sprite->addSprite(machine_sprite);
  pd->sprite->moveTo(machine_sprite, 200, 120);
  pd->sprite->setOpaque(machine_sprite, 0);
  pd->sprite->setZIndex(machine_sprite, 1);
  state->sprites[machine] = machine_sprite;

  LCDBitmap *skull_image = pd->graphics->loadBitmap("skull.png", &err);
  if (skull_image == NULL)
    pd->system->error(err);
  state->slot_images[skull] = skull_image;
  skull_image = pd->graphics->loadBitmap("small_skull.png", &err);
  if (skull_image == NULL)
    pd->system->error(err);
  state->small_slot_images[skull] = skull_image;
  LCDBitmap *bone_image = pd->graphics->loadBitmap("bone.png", &err);
  if (bone_image == NULL)
    pd->system->error(err);
  state->slot_images[bone] = bone_image;
  bone_image = pd->graphics->loadBitmap("small_bone.png", &err);
  if (bone_image == NULL)
    pd->system->error(err);
  state->small_slot_images[bone] = bone_image;

  LCDBitmap *skeleton_image = pd->graphics->loadBitmap("skeleton.png", &err);
  if (skeleton_image == NULL)
    pd->system->error(err);
  state->unit_images[skeleton] = skeleton_image;
  LCDBitmap *ghoul_image = pd->graphics->loadBitmap("ghoul.png", &err);
  if (ghoul_image == NULL)
    pd->system->error(err);
  state->unit_images[ghoul] = ghoul_image;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      LCDSprite *sprite = pd->sprite->newSprite();
      assignSprite(state, sprite);
      state->rollers[i][j] = sprite;
      pd->sprite->moveTo(sprite, starting_positions[i][j][0], starting_positions[i][j][1]);
      pd->sprite->addSprite(sprite);
    }
  }
  SetupInventory(state);
  pd->system->resetElapsedTime();
}

int update(void *userdata) {
  State *state = (State *)userdata;
  PlaydateAPI *pd = state->pd;
  float time = pd->system->getElapsedTime();

  // Update mana
  state->mana += time * state->mana_regen;

  if (state->scene == slots) {
    UpdateSlots(state, time);
  } else if (state->scene == inventory) {
    UpdateInventory(state, time);
  }
  pd->system->resetElapsedTime();
  return 1;
}
