#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
using namespace chrono;

#include "gconsole.h"
using namespace gcons;

int main()
{
	init();
	View view1 = views.create_view({0, 0}, {30, 30});
	View view2 = views.create_subview(view1, {2, 2}, {25, 25});
	View view3 = views.create_subview(view2, {2, 2}, {20, 20});
	View view4 = views.create_subview(view3 ,{2, 2}, {15, 15});

	for(int i = 0; i < 60; i++) {
		view1->screen_buffer.fill({{{(Color)((1 + i) % 7 + 1)}}, ' '});
		view2->screen_buffer.fill({{{(Color)((2 + i) % 7 + 1)}}, ' '});
		view3->screen_buffer.fill({{{(Color)((3 + i) % 7 + 1)}}, ' '});
		view4->screen_buffer.fill({{{(Color)((4 + i) % 7 + 1)}}, ' '});
		
		views.draw();
		this_thread::sleep_for(milliseconds(200));
	}

	// ==================== debug out ================
	log_out += {"standart screen width: ", standart_screen.cells.size(0),
	               "; height: ", standart_screen.cells.size(1)};
	for(int i = 0; i < views.views.size(); i++)
	{
		log_out += {"view", i + 1, " x: ", views[i].x, "; y: ", views[i].y,
		               "; width: ", views[i].width, "; height: ", views[i].height};	
		log_out += {"screen_buffer", i + 1, " width: ", views[i].screen_buffer.cells.size(0),
		               "; height: ", views[i].screen_buffer.cells.size(1)};
	}
	log_out += {"Color::transparency ", (int)Color::transparency};
	log_out += {"Color::black ",        (int)Color::black};
	log_out += {"Color::red ",          (int)Color::red};
	log_out += {"Color::green ",        (int)Color::green};
	log_out += {"Color::yellow ",       (int)Color::yellow};
	log_out += {"Color::blue ",         (int)Color::blue};
	log_out += {"Color::magenta ",      (int)Color::magenta};
	log_out += {"Color::cyan ",         (int)Color::cyan};
	log_out += {"Color::white ",        (int)Color::white};
	log_out += {"Color::end ",          (int)Color::end};
	log_out += {"Color::begin ",        (int)Color::begin};
	
	concluding();
}
