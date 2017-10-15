#ifndef GCONSOLE_CORE_CPP
	#define GCONSOLE_CORE_CPP

#include "core.hpp"

namespace gcons {
	string get_version()
	{
		return "0.1:d2017-10-15:l600";
	}

	void init()
	{
		ncurses::initscr();
		ncurses::start_color();
		ncurses::curs_set(0);
		ncurses::keypad(ncurses::stdscr, TRUE);
		ncurses::noecho();
		ncurses::halfdelay(1);
		init_pallette();
		{
			using namespace ncurses;
			int x, y;
			getmaxyx(stdscr, y, x);
			standart_screen.cells.resize(x, y);
			standart_screen.cells.fill({ { Color::black, Color::white }, ' ', true });
		}
	}

	void init_pallette()
	{
		for(int i = (int)Color::begin; i < (int)Color::end; ++i)
		for(int j = (int)Color::begin; j < (int)Color::end; ++j)
			ncurses::init_pair(1 + i + j * (int)Color::end, i, j);
	}

	void concluding()
	{
		ncurses::endwin();

		log_out += {"Version: gconsole ", get_version()};
		log_out();
	}

	Allign::Allign()
		: right{0}, left{0}, up{0}, down{0}
	{ }

	void Cells::resize(int x, int y)
	{
		cells.resize(x);
		for(vector<Cell>& in_cells_x : cells)
			in_cells_x.resize(y);
	}

	void Cells::fill(Cell cell)
	{
		for(int x = 0; x < size(0); x++)
		for(int y = 0; y < size(1); y++) {
			cells[x][y] = cell;
		}
	}

	int Cells::size()
	{
		return cells.size();
	}

	int Cells::size(int deep)
	{
		if (deep == 0) return cells.size();
		if (deep == 1) return cells[0].size();
	}

	vector<Cell>& Cells::operator [] (int n)
	{
		return cells[n];
	}

	void Screen_buffer::draw_cell(Coords xy)
	{
		cells[xy.x][xy.y].color_pair.background = Color::white;
		cells[xy.x][xy.y].is_draw = false;
		is_draw = false;
	}

	void Screen_buffer::draw_cell(Coords xy, Brush brush)
	{
		if (brush.color_pairs[0].background != Color::transparency)
			cells[xy.x][xy.y].color_pair.background = brush.color_pairs[0].background;
		if (brush.color_pairs[0].foreground != Color::transparency)
			cells[xy.x][xy.y].color_pair.foreground = brush.color_pairs[0].foreground;
		cells[xy.x][xy.y].ch = brush.ch;
		cells[xy.x][xy.y].is_draw = false;
		is_draw = false;
	}

	void Screen_buffer::draw_row(int row, Brush brush)
	{
		for(int x = 0; x <= cells.size(0); x++)
			draw_cell({x, row}, brush);
	}

	void Screen_buffer::draw_row(int row, Coords beg_end, Brush brush)
	{
		for(int x = beg_end.x; x <= beg_end.y; x++)
			draw_cell({x, row}, brush);
	}

	void Screen_buffer::draw_column(int column, Brush brush)
	{
		for(int y = 0; y <= cells.size(1); y++)
			draw_cell({column, y}, brush);
	}

	void Screen_buffer::draw_column(int column, Coords beg_end, Brush brush)
	{
		for(int y = beg_end.x; y <= beg_end.y; y++)
			draw_cell({column, y}, brush);
	}

	void Screen_buffer::draw_line(Coords xy1, Coords xy2, Brush brush)
	{
		int x = 0, y = 0;
		int x_len = xy2.x - xy1.x;
		int y_len = xy2.y - xy1.y;
		int max_len = (x_len > y_len) ? x_len : y_len;
		for(int i = 0; i <= max_len; i++) {
			x = xy1.x + x_len * i / max_len;
			y = xy1.y + y_len * i / max_len;
			draw_cell({x, y}, brush);
		}
	}

	void Screen_buffer::draw_area(Coords xy1, Coords xy2, Brush brush)
	{
		for(int x = xy1.x; x <= xy2.x; x++)
		for(int y = xy1.y; y <= xy2.y; y++)
			draw_cell({x, y}, brush);
	}

	void Screen_buffer::draw_frame(Coords xy1, Coords xy2, Brush brush)
	{
		draw_row(xy1.y, {xy1.x, xy2.x}, brush);
		draw_row(xy2.y, {xy1.x, xy2.x}, brush);
		draw_column(xy1.x, {xy1.y + 1, xy2.y - 1}, brush);
		draw_column(xy2.x, {xy1.y + 1, xy2.y - 1}, brush);
	}

	void Screen_buffer::draw_string(Coords xy, string str)
	{
		for(int i = 0; i < str.size(); i++)
			draw_cell({i + xy.x, xy.y}, {{{}}, str[i]});
	}

	template<typename T>
	void Screen_buffer::draw_string(Coords xy, T t)
	{
		string str = to_cast<string, T>(t);
		for(int i = 0; i < str.size(); i++)
			draw_cell({i + xy.x, xy.y}, {{{}}, str[i]});
	}

	void Screen_buffer::draw_string(Coords xy, String_builder string_builder)
	{
		for(int i = 0; i < string_builder.str.size(); i++)
			draw_cell({i + xy.x, xy.y}, {{{}}, string_builder.str[i]});
	}

	void Screen_buffer::draw_string(Coords xy, String_builder string_builder, Brush brush)
	{
		for(int i = 0; i < string_builder.str.size(); i++)
			draw_cell({i + xy.x, xy.y},
				{{{brush.color_pairs[0].background, brush.color_pairs[0].foreground}}, string_builder.str[i]});
	}

	void Screen_buffer::fill(Brush brush)
	{
		for(int x = 0; x < cells.size(0); x++)
		for(int y = 0; y < cells.size(1); y++)
			draw_cell({x, y}, brush);
	}

	void Screen_buffer::clear()
	{
		fill({{{Color::black, Color::white}}, ' '});
	}

	void Screen_buffer::draw(Coords xy1, Coords xy2)
	{
		if (is_draw == false) {
		for(int x = 0; x < (xy2.x - xy1.x); x++)
		for(int y = 0; y < (xy2.y - xy1.y); y++)
			if (cells[x][y].is_draw == false)
			{
				if (cells[x][y].color_pair.background != Color::transparency)
					standart_screen.cells[x + xy1.x][y + xy1.y].color_pair.background = 
						cells[x][y].color_pair.background;

				if (cells[x][y].color_pair.foreground != Color::transparency)
					standart_screen.cells[x + xy1.x][y + xy1.y].color_pair.foreground =
						cells[x][y].color_pair.foreground;

				if (cells[x][y].ch != nullable)
					standart_screen.cells[x + xy1.x][y + xy1.y].ch =
						cells[x][y].ch;
				
				cells[x][y].is_draw = true;
				standart_screen.cells[x + xy1.x][y + xy1.y].is_draw = false;
			}
		is_draw = true;
		standart_screen.is_draw = false;
		}
	}

	void Screen_buffer::draw(Coords xy)
	{
		draw(xy, {cells.size(0) - xy.x, cells.size(1) - xy.y});
	}

	void Screen_buffer::draw()
	{
		draw({0, 0}, {cells.size(0), cells.size(1)});
	}

	void Standart_screen::draw()
	{
		using namespace ncurses;
		int color_code;

		if (is_draw == false) {
		for(int x = 0; x < cells.size(0); x++)
		for(int y = 0; y < cells.size(1); y++)
		{
			if (cells[x][y].is_draw == false)
			{
				color_code = 1 + (int)cells[x][y].color_pair.foreground +
					(int)cells[x][y].color_pair.background * (int)Color::end;

				attron(COLOR_PAIR(color_code));
				if (cells[x][y].ch == nullable)
					mvaddch(y, x, ' ');
				else
					mvaddch(y, x, cells[x][y].ch);
				attroff(COLOR_PAIR(color_code));

				cells[x][y].is_draw = true;
			}
		}
		is_draw = true;
		}
	}

	void Item::draw()
	{
		;
	}
	
	void Items::draw()
	{
		for(Item& item : items)
			item.draw();
	}

	View::View(int n)
		: i{n}
	{ }

	View_* View::operator -> ()
	{
		return &views[i];
	}

	View_* View::operator & ()
	{
		return &views[i];
	}

	void View_::draw()
	{
		if (parent == nullptr)
			screen_buffer.draw({x, y}, {x + width, y + height});
		else
		{
			int parent_x = 0, parent_y = 0;
			View_* view_ = this;
			while(view_->parent != nullptr) {
				parent_x += view_->parent->x;
				parent_y += view_->parent->y;
				view_ = view_->parent;
			}
			screen_buffer.draw({x + parent_x, y + parent_y},
												 {x + parent_x + width, y + parent_y + height});
		}
		items.draw();
	}

	View Views::create_view(Coords xy, Coords wid_hei)
	{
		views.push_back(*new View_);
		View_* view = &views[views.size() - 1];
		view->x = xy.x;
		view->y = xy.y;
		view->width = wid_hei.x;
		view->height = wid_hei.y;
		view->is_viewed = true;
		view->screen_buffer.cells.resize(view->width, view->height);

		return *new View(views.size() - 1);	
	}

	View Views::create_view(Coords wid_hei)
	{
		return create_view({0, 0}, wid_hei);
	}
	
	View Views::create_view()
	{
		int width, height;
		{ using namespace ncurses; getmaxyx(stdscr, height, width); }
		return create_view({0, 0}, {width, height});
	}

	View Views::create_subview(View& view, Coords xy, Coords wid_hei)
	{
		View sub_view = create_view(xy, wid_hei);
		sub_view->parent = &view;
		view->children.push_back(&sub_view);

		return sub_view;
	}

	View Views::create_subview(View& view, Coords wid_hei)
	{
		return create_subview(view, {0, 0}, wid_hei);
	}

	View Views::create_subview(View& view)
	{
		return create_subview(view, {0, 0}, {view->width, view->height});
	}

	void Views::draw()
	{
		for(View_& view : views)
			view.draw();
		standart_screen.draw();
		ncurses::refresh();
	}

	int Views::size()
	{
		return views.size();
	}

	View_& Views::operator [] (int n)
	{
		return views[n];
	}

	template<typename... Args>
	String_builder::String_builder(Args... arg)
	{
		str += string_build(arg...);
	}

	char& String_builder::operator [] (int n)
	{
		return str[n];
	}

	void Log_out::operator () ()
	{
		std::cout << ">>>Log_out<<<\n";
		for(int i = 0; i < strs.size(); i++)
			std::cout << i + 1 << ": " << strs[i] << '\n';
	}

	template<typename T>
	void Log_out::operator += (T t)
	{
		strs.push_back(to_cast<string, T>(t));
	}
	
	void Log_out::operator += (String_builder string_builder)
	{
		strs.push_back(string_builder.str);
	}

	template<typename T>
	string string_build(T value)
	{
		return to_cast<string, T>(value);
	}

	template<typename T, typename... Args>
	string string_build(T value, Args... arg)
	{
		return to_cast<string, T>(value) += string_build(arg...);
	}

	template<typename T1, typename T2>
	T1 to_cast(T2 t2)
	{
		T1 t1;
		std::stringstream ss;
		ss << t2;
		getline(ss, t1);
		return t1;
	}
}

#endif
