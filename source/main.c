#include "game.h"
#include "pd_api.h"

int eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg) {
  (void)arg;

  if (event == kEventInit) {
    setup(pd);
  }
  return 0;
}
