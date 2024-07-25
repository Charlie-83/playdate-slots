#include "battle.h"
#include "game.h"
#include "utils.h"
#include <cJSON.h>

void SetupBattle(State *state) {
  PlaydateAPI *pd = state->pd;
  state->battle_state.next_battle = 0;
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
}

void ShowBattle(State *state) {
  state->scene = battle;
  PlaydateAPI *pd = state->pd;
  pd->sprite->removeAllSprites();
}

void UpdateBattle(State *state, float time) {

}
