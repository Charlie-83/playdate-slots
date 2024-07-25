#include "utils.h"
#include "game.h"

void drawRectWidth(PlaydateAPI *pd, int x, int y, int width, int height, int line_width,
                   LCDColor colour) {
  pd->graphics->drawLine(x, y, x + width, y, line_width, colour);
  pd->graphics->drawLine(x, y, x, y + height, line_width, colour);
  pd->graphics->drawLine(x, y + height, x + width, y + height, line_width, colour);
  pd->graphics->drawLine(x + width, y, x + width, y + height, line_width, colour);
}

const char *readFile(PlaydateAPI *pd, const char *path) {
  SDFile *file = pd->file->open(path, kFileRead);
  pd->file->seek(file, 0, SEEK_END);
  int file_size = pd->file->tell(file);
  char *content = (char *)malloc(file_size + 1);
  pd->file->seek(file, 0, SEEK_SET);
  pd->file->read(file, content, file_size);
  content[file_size] = '\0';
  pd->file->close(file);
  return content;
}

Units strToUnit(const char *str) {
  if (strcmp(str, "skeleton") == 0)
    return skeleton;
  if (strcmp(str, "ghoul") == 0)
    return ghoul;
  return -1;
}

