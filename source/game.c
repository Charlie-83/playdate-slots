#include "game.h"
#include "pd_api/pd_api_gfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int starting_positions[3][3][2] = {{{88, 40}, {88, 120}, {88, 200}},
                                   {{200, 40}, {200, 120}, {200, 200}},
                                   {{312, 40}, {312, 120}, {312, 200}}};
/*int starting_positions[3][3][2] = {{{128, 40}, {108, 120}, {88, 200}},*/
/*                                   {{240, 40}, {220, 120}, {200, 200}},*/
/*                                   {{352, 40}, {332, 120}, {312, 200}}};*/
static void assignSprite(State *state, LCDSprite *sprite);

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
  state->score = 0;
  state->current_row = 1;
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

  LCDBitmap *skull_image = pd->graphics->loadBitmap("skull.png", &err);
  if (skull_image == NULL)
    pd->system->error(err);
  state->sprites[skull] = skull_image;
  LCDBitmap *bone_image = pd->graphics->loadBitmap("bone.png", &err);
  if (bone_image == NULL)
    pd->system->error(err);
  state->sprites[bone] = bone_image;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      LCDSprite *sprite = pd->sprite->newSprite();
      assignSprite(state, sprite);
      state->rollers[i][j] = sprite;
      pd->sprite->moveTo(sprite, starting_positions[i][j][0], starting_positions[i][j][1]);
      pd->sprite->addSprite(sprite);
    }
  }
  pd->system->resetElapsedTime();
}

int update(void *userdata) {
  State *state = (State *)userdata;
  PlaydateAPI *pd = state->pd;
  float time = pd->system->getElapsedTime();

  PDButtons current, pushed, released;
  pd->system->getButtonState(&current, &pushed, &released);
  if (pushed & kButtonA)
    state->roller_state = slowing;
  if (pushed & kButtonB) {
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
        Sprites sprite_tag = pd->sprite->getTag(state->rollers[i][state->current_row]);
        if (sprite_tag == skull)
          state->score += 10;
        else if (sprite_tag == bone)
          state->score += 5;
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
  pd->system->resetElapsedTime();
  pd->sprite->drawSprites();
  char score_str[8];
  sprintf(score_str, "%d", state->score);
  pd->graphics->drawText(score_str, strlen(score_str), kASCIIEncoding, 1, 1);
  return 1;
}

static void assignSprite(State *state, LCDSprite *sprite) {
  int sprite_number = (int)((float)rand() / RAND_MAX * NUMBER_OF_SLOT_SPRITES);
  PlaydateAPI *pd = state->pd;
  pd->sprite->setImage(sprite, state->sprites[sprite_number], kBitmapUnflipped);
  pd->sprite->setTag(sprite, sprite_number);
}
