
#include "score.h"


void scoreSave(const int score) {
  std::ofstream fstr("res/score.txt");
  if (fstr) { fstr << score; }
}
