#include "utils.h"

void drawRectWidth(PlaydateAPI *pd, int x, int y, int width, int height, int line_width,
                   LCDColor colour) {
  pd->graphics->drawLine(x, y, x + width, y, line_width, colour);
  pd->graphics->drawLine(x, y, x, y + height, line_width, colour);
  pd->graphics->drawLine(x, y + height, x + width, y + height, line_width, colour);
  pd->graphics->drawLine(x + width, y, x + width, y + height, line_width, colour);
}
