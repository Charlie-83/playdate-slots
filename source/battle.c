#include "battle.h"
#include "game.h"
#include "slots.h"
#include "utils.h"
#include <cJSON.h>

static void SetEnemySprites(State *state);

void SetupBattle(State *state) {
  PlaydateAPI *pd = state->pd;
  state->battle_state.next_battle = 0;
  state->battle_state.currently_selected = 0;
  const char *battles_str = readFile(pd, "battles.json");
  cJSON *battles_json = cJSON_Parse(battles_str);
  if (!cJSON_IsArray(battles_json))
    pd->system->error("Expected battles.json to be array");
  int number_of_battles = cJSON_GetArraySize(battles_json);
  state->battle_state.battles = malloc(number_of_battles * sizeof(int) * NUMBER_OF_UNITS);
  for (int i = 0; i < number_of_battles; ++i)
    for (int j = 0; j < NUMBER_OF_UNITS; ++j)
      state->battle_state.battles[i][j] = 0;
  state->battle_state.total_battles = number_of_battles;
  cJSON *battle = NULL;
  int i = 0;
  cJSON_ArrayForEach(battle, battles_json) {
    cJSON *unit = NULL;
    cJSON_ArrayForEach(unit, battle) {
      Units unit_enum = strToUnit(unit->string);
      if (unit_enum == -1)
        pd->system->error("Unrecognised unit %s", unit->string);
      state->battle_state.battles[i][unit_enum] = unit->valueint;
    }
    ++i;
  }
  for (int i = 0; i < 5; ++i) {
    LCDSprite *sprite = pd->sprite->newSprite();
    state->battle_state.sprites[0][i] = sprite;
    pd->sprite->moveTo(sprite, (i + 0.5f) * SPRITE_SIZE, SPRITE_SIZE / 2.0f);
    sprite = pd->sprite->newSprite();
    state->battle_state.sprites[1][i] = sprite;
    pd->sprite->moveTo(sprite, (i + 0.5f) * SPRITE_SIZE, SCREEN_Y - SPRITE_SIZE / 2.0f);
  }
  SetEnemySprites(state);
}

void ShowBattle(State *state) {
  state->scene = battle;
  PlaydateAPI *pd = state->pd;
  pd->sprite->removeAllSprites();
  pd->graphics->clear(kColorWhite);
  for (int i = 0; i < 5; ++i) {
    pd->sprite->addSprite(state->battle_state.sprites[0][i]);
    pd->sprite->addSprite(state->battle_state.sprites[1][i]);
  }
}

void UpdateBattle(State *state, float time) {
  PlaydateAPI *pd = state->pd;

  PDButtons current, pushed, released;
  pd->system->getButtonState(&current, &pushed, &released);
  if (pushed & kButtonB)
    ShowSlots(state);
  else if (pushed & kButtonA &&
           state->battle_state.next_battle < state->battle_state.total_battles - 1) {
    ++(state->battle_state.next_battle);
    SetEnemySprites(state);
  }

  if (pushed & kButtonLeft && state->battle_state.currently_selected > 0)
    --(state->battle_state.currently_selected);
  else if (pushed & kButtonRight && state->battle_state.currently_selected < 4)
    ++(state->battle_state.currently_selected);

  pd->sprite->drawSprites();
  int width = pulsingWidth(state, time);
  int selected = state->battle_state.currently_selected;
  drawRectWidth(pd, selected * SPRITE_SIZE, SCREEN_Y - SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE, width,
                kColorBlack

  );
}

static void SetEnemySprites(State *state) {
  PlaydateAPI *pd = state->pd;
  int *battle = state->battle_state.battles[state->battle_state.next_battle];
  int current = 0;
  for (int i = 0; i < NUMBER_OF_UNITS; ++i)
    for (int j = 0; j < battle[i]; ++j)
      pd->sprite->setImage(state->battle_state.sprites[0][current++], state->unit_images[i],
                           kBitmapUnflipped);
}
