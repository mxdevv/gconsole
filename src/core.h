#ifndef GCONSOLE_CORE_H
	#define GCONSOLE_CORE_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "consts.h"

namespace gcons {
	using namespace std;

	namespace ncurses {
		#include <ncurses.h>
	}
	enum struct Color;
	enum struct Direction;
	struct Color_pair;
	struct Coords;
	struct Lengths;
	struct Distance;
	struct Brush;
	struct Cell;
	struct Cells;
	struct Surface_base;
	struct Surface;
	struct Standart_screen;
	struct Item;
	struct Items;
	struct View_;
	struct View;
	struct Views;
	struct String_builder;
	struct Log_out;

	void init();
	void init_pallette();
	void concluding();
	string get_version();

	template<typename T>
		string string_build(T value);
	template<typename T, typename... Args>
		string string_build(T value, Args... arg);
	template<typename T1, typename T2>
		T1 to_cast(T2 t2);
}

#endif
