
#include "number_draw.h"


enum {
  FONT_SIZE = 40,
};


cNumber::cNumber() :
text_("res/MeiryoConsolas.ttf") {

  text_.size(FONT_SIZE);
}


void cNumber::draw(const int val, const Vec2f& pos) {

  std::stringstream score;
  score << "SCORE: " << val;

  text_.draw(score.str(), pos, Color(1, 1, 1));
}
