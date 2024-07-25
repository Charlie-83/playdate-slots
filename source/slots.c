#include "slots.h"
#include "battle.h"
#include "inventory.h"
#include <stdio.h>

void ShowSlots(State *state) {
  state->scene = slots;
  PlaydateAPI *pd = state->pd;
  pd->sprite->removeAllSprites();
  pd->sprite->addSprite(state->sprites[machine]);
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      pd->sprite->addSprite(state->rollers[i][j]);
}

void UpdateSlots(State *state, float time) {
  PlaydateAPI *pd = state->pd;
  PDButtons current, pushed, released;
  pd->system->getButtonState(&current, &pushed, &released);
  if (pushed & kButtonLeft)
    ShowInventory(state);
  else if (pushed & kButtonRight)
    ShowBattle(state);

  if (pushed & kButtonA && state->roller_state == spinning)
    state->roller_state = slowing;
  if ((pushed & kButtonB) && state->mana >= state->pull_cost && state->roller_state == stopped) {
    state->mana -= state->pull_cost;
    state->roller_state = spinning;
    state->roller_speed = 1000;
  }

  if (state->roller_state == slowing) {
    state->roller_speed -= time * 400;
    if (state->roller_speed < 50) {
      state->roller_speed = 50;
      state->roller_state = stopping;
    }
  }

  if (state->roller_state == stopping) {
    float offset = state->roller_offset - 40;
    float move_y;
    if (offset < 5 && offset > -5) {
      move_y = -offset;
      state->roller_state = stopped;
      for (int i = 0; i < 3; ++i) {
        SlotItems sprite_tag = pd->sprite->getTag(state->rollers[i][state->current_row]);
        ++(state->inventory_state.inventory[sprite_tag]);
      }
    } else
      move_y = -offset * time * 3;

    state->roller_offset += move_y;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        pd->sprite->moveBy(state->rollers[i][j], 0, move_y);
      }
    }
  }

  if (state->roller_state != stopping && state->roller_state != stopped) {
    state->roller_offset += time * state->roller_speed;
    while (state->roller_offset > 80) {
      state->roller_offset -= 80;
      state->current_row -= 1;
      if (state->current_row == -1)
        state->current_row = 2;
    }

    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        float current_x, current_y;
        pd->sprite->getPosition(state->rollers[i][j], &current_x, &current_y);
        float new_y = current_y + time * state->roller_speed;
        while (new_y > 240) {
          new_y -= 240;
          assignSprite(state, state->rollers[i][j]);
        }
        pd->sprite->moveTo(state->rollers[i][j], current_x, new_y);
      }
    }
  }
  pd->sprite->drawSprites();
  char mana_str[8];
  sprintf(mana_str, "%d", (int)state->mana);
  pd->graphics->drawText(mana_str, strlen(mana_str), kASCIIEncoding, 1, 1);
}

void assignSprite(State *state, LCDSprite *sprite) {
  int sprite_number = (int)((float)rand() / RAND_MAX * NUMBER_OF_ITEMS);
  PlaydateAPI *pd = state->pd;
  pd->sprite->setImage(sprite, state->slot_images[sprite_number], kBitmapUnflipped);
  pd->sprite->setTag(sprite, sprite_number);
}
