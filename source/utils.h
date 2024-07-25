#pragma once
#include "game.h"
#include <pd_api.h>

void drawRectWidth(PlaydateAPI *pd, int x, int y, int width, int height, int line_width,
                   LCDColor colour);

const char* readFile(PlaydateAPI *pd, const char* path);

Units strToUnit(const char *str);
