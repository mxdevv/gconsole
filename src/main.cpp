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

	for(int i = 0; i < 60; i++) {
		view3->resize({20 + (i % 4), 20});
		view3->screen_buffer.fill({{{Color::white}}});
		view2->screen_buffer.fill({{{(Color)((2 + i) % 7 + 1), (Color)((2 + i) % 7 + 1)}}});
		view1->screen_buffer.draw_line({0, 0}, {29, (i % 30)}, {{{Color::transparency, Color::white}}, '*'});
		view3->screen_buffer.draw_line({0, (i % 20)}, {(i % 20), 19}, {{{Color::black, Color::transparency}}, '#'});

		views.draw();
		this_thread::sleep_for(milliseconds(200));
		standart_screen.clear();
	}

	// ==================== debug out ================
	log_out += {"standart screen width: ", standart_screen.cells.size(0),
	               "; height: ", standart_screen.cells.size(1)};
	for(int i = 0; i < views.views.size(); i++)
	{
		log_out += {"view", i + 1, " x: ", views[i].xy.x, "; y: ", views[i].xy.y,
		               "; width: ", views[i].lengths.width, "; height: ", views[i].lengths.height};	
		log_out += {"screen_buffer", i + 1, " width: ", views[i].screen_buffer.cells.size(0),
		               "; height: ", views[i].screen_buffer.cells.size(1)};
	}	
	
	concluding();
}
