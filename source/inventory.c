#include "inventory.h"
#include "slots.h"
#include "utils.h"
#include <math.h>
#include <pd_api.h>
#include <stdio.h>

static unsigned int costs[NUMBER_OF_UNITS][NUMBER_OF_ITEMS] = {{1, 5}, {3, 3}};
static void Buy(State *state);

void SetupInventory(State *state) {
  state->inventory_state.selected = 0;
  state->inventory_state.flashing_progress = 0;
  for (int i = 0; i < NUMBER_OF_UNITS; ++i) state->army_state.army[i] = 0;
}

void ShowInventory(State *state) {
  PlaydateAPI *pd = state->pd;
  pd->sprite->removeAllSprites();
  for (int i = 0; i < NUMBER_OF_ITEMS; ++i) {
    LCDSprite *sprite = pd->sprite->newSprite();
    // TODO: Memory leak because these are never freed
    pd->sprite->setImage(sprite, state->slot_images[i], kBitmapUnflipped);
    pd->sprite->moveTo(sprite, SPRITE_SIZE / 2.0f + SPRITE_SIZE * i, SPRITE_SIZE / 2.0f);
    pd->sprite->addSprite(sprite);
  }
  for (int i = 0; i < NUMBER_OF_UNITS; ++i) {
    LCDSprite *sprite = pd->sprite->newSprite();
    pd->sprite->setImage(sprite, state->unit_images[i], kBitmapUnflipped);
    pd->sprite->moveTo(sprite, SCREEN_X - SPRITE_SIZE / 2.0f, SPRITE_SIZE / 2.0f + i * SPRITE_SIZE);
    pd->sprite->addSprite(sprite);
  }
}

void UpdateInventory(State *state, float time) {
  PlaydateAPI *pd = state->pd;
  PDButtons current, pushed, released;
  pd->system->getButtonState(&current, &pushed, &released);
  if (pushed & kButtonB) {
    ShowSlots(state);
    state->scene = slots;
  }

  if (pushed & kButtonA) {
    Buy(state);
  }

  if (pushed & kButtonDown && state->inventory_state.selected < NUMBER_OF_UNITS - 1)
    ++(state->inventory_state.selected);
  if (pushed & kButtonUp && state->inventory_state.selected > 0)
    --(state->inventory_state.selected);

  pd->graphics->clear(kColorWhite);
  pd->sprite->drawSprites();

  char str[6];
  for (int i = 0; i < NUMBER_OF_ITEMS; ++i) {
    sprintf(str, "%d", state->inventory_state.inventory[i]);
    pd->graphics->drawText(str, strlen(str), kASCIIEncoding, SPRITE_SIZE * i, 0);
  }

  for (int i = 0; i < NUMBER_OF_UNITS; ++i) {
    sprintf(str, "%d", state->army_state.army[i]);
    pd->graphics->drawText(str, strlen(str), kASCIIEncoding, SCREEN_X - 10, SPRITE_SIZE * i);
    for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
      pd->graphics->drawBitmap(state->small_slot_images[j],
                               SCREEN_X - SPRITE_SIZE - SMALL_SPRITE_SIZE,
                               SPRITE_SIZE * i + SMALL_SPRITE_SIZE * j + 2, kBitmapUnflipped);
      sprintf(str, "%d", costs[i][j]);
      pd->graphics->drawText(str, strlen(str), kASCIIEncoding,
                             SCREEN_X - SPRITE_SIZE - SMALL_SPRITE_SIZE - 15,
                             SPRITE_SIZE * i + SMALL_SPRITE_SIZE * j + 2);
    }
  }

  pd->graphics->drawLine(SPRITE_SIZE * 2, 0, SPRITE_SIZE * 2, SCREEN_Y, 2, kColorBlack);
  // TODO: Rect not visible
  state->inventory_state.flashing_progress += time * 8;
  if (state->inventory_state.flashing_progress > 2 * (float)M_PI)
    state->inventory_state.flashing_progress -= 2 * (float)M_PI;
  int width = (int)(sinf(state->inventory_state.flashing_progress) * 1.5f + 3);
  drawRectWidth(pd, SCREEN_X - SPRITE_SIZE - SMALL_SPRITE_SIZE - 20,
                state->inventory_state.selected * SPRITE_SIZE, SPRITE_SIZE + SMALL_SPRITE_SIZE + 20,
                SPRITE_SIZE, width, kColorBlack);
}

static void Buy(State *state) {
  for (int i = 0; i < NUMBER_OF_ITEMS; ++i) {
    if (state->inventory_state.inventory[i] < costs[state->inventory_state.selected][i])
      return;
  }
  for (int i = 0; i < NUMBER_OF_ITEMS; ++i) {
    state->inventory_state.inventory[i] -= costs[state->inventory_state.selected][i];
  }
  ++(state->army_state.army[state->inventory_state.selected]);
}
