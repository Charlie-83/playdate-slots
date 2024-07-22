#ifndef SLOTS_H
#define SLOTS_H
#include "game.h"
#include <pd_api.h>

void ShowSlots(State *state);
void UpdateSlots(State *state, float time);
void assignSprite(State *state, LCDSprite *sprite);
#endif
