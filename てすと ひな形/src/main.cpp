#include "number_draw.h"
#include "score.h"


bool PointToRect(float point_x, float point_y,
	float obj_pos_x, float obj_pos_y,
	float obj_size_w, float obj_size_h)
{
	if (point_x > obj_pos_x){
		if (point_x < obj_pos_x + obj_size_w){
			if (point_y > obj_pos_y){
				if (point_y < obj_pos_y + obj_size_h){
					// 矩形に入っているのでtrue
					return true;
				}
			}
		}
	}
	// 矩形に入っていないのでfalse
	return false;
}


struct Box{
	bool is_hit;
	float x;
	float y;
	float size_wh;
	Color color;
	int move_count;
	bool trriger;
} box;

Random random;


void BoxDraw(){
	if (box.is_hit==false){
		drawFillBox(box.x, box.y, box.size_wh, box.size_wh, box.color);
	}
	else{
		drawFillBox(box.x + random.fromFirstToLast(-5, 5),
			box.y + random.fromFirstToLast(-5, 5),
			box.size_wh, box.size_wh, box.color);
	}
}

void BoxUpdate(AppEnv& env, int& val, Media& hit_se){

	if (!box.is_hit){
		if (PointToRect(env.mousePosition().x(), env.mousePosition().y(),
			box.x, box.y, box.size_wh, box.size_wh))
		{
			if (env.isPushButton(Mouse::LEFT)){
				box.is_hit = true;
				if (env.isPushButton(Mouse::LEFT)) { ++val; }
			}
		}
	}

	if (box.is_hit){
		static int anime = 0;
		anime++;
		box.color = Color(1, 0, 0);
		if (!hit_se.isPlaying()){
			hit_se.play();
		}
		if (anime > 40){
			anime = 0;
			box.is_hit = false;
			box.trriger = true;
			box.color = Color(1, 1, 1);
			hit_se.stop();
		}
	}

	if (box.trriger){
		box.x = random.fromFirstToLast(-512.0f / 2, 512.0f / 2 - box.size_wh);
		box.y = random.fromFirstToLast(-512.0f / 2, 512.0f / 2 - box.size_wh);
		box.trriger = false;
	}
}

enum{
	PLAY_TIME_MAX = 300
};

struct Gauge{
	float x;
	float y;
	float width;
	float height;
	Color color;
	int count_speed;
	int play_time;
} gauge;

void GaugeDraw(){
	drawFillBox(gauge.x, gauge.y, gauge.width, gauge.height, gauge.color);
}

void GaugeUpdate(){
	if (gauge.play_time <= PLAY_TIME_MAX){
		gauge.play_time -= gauge.count_speed;
		gauge.width -= PLAY_TIME_MAX % 10;
		if (gauge.play_time <= PLAY_TIME_MAX / 5){
			gauge.color = Color(1, 0, 0);
		}
	}
	gauge.width = gauge.play_time;
}

void GameOver(int& mode){
	if (gauge.play_time <= 0){
		mode = 2;
	}
}

void Init(int& val, int& mode){
	box.is_hit = false;
	box.size_wh = 80;
	box.x = random.fromFirstToLast(-512.0f / 2, 512.0f / 2 - box.size_wh);
	box.y = random.fromFirstToLast(-512.0f / 2, 512.0f / 2 - box.size_wh);
	box.color = Color(1, 1, 1);

	gauge.width = 300;
	gauge.height = 30;
	gauge.x = -gauge.width / 2;
	gauge.y = 200;
	gauge.color = Color(0, 1, 0);
	gauge.count_speed = 1;
	gauge.play_time = PLAY_TIME_MAX;

	val = 0;
	mode = 0;
}

int main() {
	AppEnv env(Window::WIDTH, Window::HEIGHT, false, false);
	cNumber score;

	Media hit_se("res/hit.wav");
	Texture click_to_start("res/click_to_start.png");

	int val = 0;
	int mode = 0;

	Init(val, mode);

	while (env.isOpen()) {

		switch (mode)
		{
		case 0:{
			if (env.isPushButton(Mouse::LEFT)){
				Init(val, mode);
				mode = 1;
			}
		}break;

		case 1:{
			GaugeUpdate();
			BoxUpdate(env, val, hit_se);
			GameOver(mode);
		}break;

		case 2:{
			if (env.isPushButton(Mouse::LEFT)){
				mode = 0;
				scoreSave(val);
			}
		}break;
		}

		//////////////////////////////////////////////////////////
		env.setupDraw();
		//////////////////////////////////////////////////////////

		switch (mode)
		{
		case 0:{

		}break;

		case 1:{
			GaugeDraw();
			BoxDraw();
			score.draw(val, Vec2f(-80, 200));
		}break;

		case 2:{
			score.draw(val, Vec2f(-80, 0));
		}break;
		}

		env.update();
	}
}