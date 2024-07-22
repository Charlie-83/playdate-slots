#include "inventory.h"
#include "slots.h"
#include "pd_api.h"
#include <stdio.h>

void ShowInventory(State *state) {
  PlaydateAPI *pd = state->pd;
  pd->sprite->removeAllSprites();
  for (int i = 0; i < NUMBER_OF_SLOT_SPRITES; ++i) {
    LCDSprite *sprite = pd->sprite->newSprite();
    pd->sprite->setImage(sprite, state->images[i], kBitmapUnflipped);
    pd->sprite->moveTo(sprite, 100 + 100 * i, 120);
    pd->sprite->addSprite(sprite);
  }
}

void UpdateInventory(State *state) {
  PlaydateAPI *pd = state->pd;
  PDButtons current, pushed, released;
  pd->system->getButtonState(&current, &pushed, &released);
  if (pushed & kButtonRight) {
      ShowSlots(state);
    state->scene = slots;
  }
  pd->sprite->drawSprites();
  char str[6];
  for (int i = 0; i < NUMBER_OF_SLOT_SPRITES; ++i) {
    sprintf(str, "%d", state->inventory[i]);
    pd->graphics->drawText(str, strlen(str), kASCIIEncoding, 100 + 100 * i, 200);
  }
}
