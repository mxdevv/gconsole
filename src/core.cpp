#ifndef GCONSOLE_CORE_CPP
	#define GCONSOLE_CORE_CPP

#include "core.hpp"

namespace gcons {
	using namespace std;

	string get_version()
	{
		return "0.1.3";
	}

	void init()
	{
		ncurses::initscr();
		ncurses::start_color();
		ncurses::curs_set(0);
		ncurses::keypad(ncurses::stdscr, TRUE);
		ncurses::noecho();
		//ncurses::halfdelay(1);
		//ncurses::nocbreak();
		ncurses::nodelay(ncurses::stdscr, TRUE);
		init_pallette();
		{
			using namespace ncurses;
			Lengths lengths;
			getmaxyx(stdscr, lengths.height, lengths.width);
			standart_screen.cells.resize(lengths);
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

	Coords::Coords(int n)
		: x{n}, y{n}
	{ }

	Coords::Coords(int x, int y)
		: x{x}, y{y}
	{ }

	Coords Coords::operator + (Lengths const& lengths)
	{
		return *new Coords{x + lengths.width, y + lengths.height};
	}

	Coords Coords::operator + (Distance const& distance)
	{
		return *new Coords{x + distance.begin, y + distance.end};
	}

	Coords Coords::operator + (Coords const& coords)
	{
		return *new Coords{x + coords.x, y + coords.y};
	}

	void Coords::operator += (Lengths const& lengths)
	{
		x += lengths.width;
		y += lengths.height;
	}

	void Coords::operator += (Distance const& distance)
	{
		x += distance.begin;
		y += distance.end;
	}

	void Coords::operator += (Coords const& coords)
	{
		x += coords.x;
		y += coords.y;
	}

	Coords Coords::operator - (Coords const& coords)
	{
		return *new Coords{x - coords.x, y - coords.y};
	}

	void Coords::operator -= (Coords const& coords)
	{
		x -= coords.x;
		y -= coords.y;
	}

	Coords Coords::operator / (Coords const& coords)
	{
		return *new Coords{x / coords.x, y - coords.y};
	}

	template<class T>
	Coords Coords::operator / (T const& t)
	{
		return *new Coords{x / t, y / t};
	}

	Coords Coords::operator * (Coords const& coords)
	{
		return *new Coords{x * coords.x, y*coords.y};
	}

	template<class T>
	Coords Coords::operator * (T const& t)
	{
		return *new Coords{x * t, y * t};
	}

	Lengths::Lengths(int n)
		: width{n}, height{n}
	{ }

	Lengths::Lengths(int width, int height)
		: width{width}, height{height}
	{ }

	Lengths::Lengths(Coords coords)
		: width{coords.x}, height{coords.y}
	{ }

	Lengths Lengths::operator + (Lengths const& lengths)
	{
		return *new Lengths{width + lengths.width, height + lengths.height};
	}

	Lengths Lengths::operator + (Distance const& distance)
	{
		return *new Lengths{width + distance.begin, height + distance.end};
	}

	Lengths Lengths::operator + (Coords const& coords)
	{
		return *new Lengths{width + coords.x, height + coords.y};
	}

	void Lengths::operator += (Lengths const& lengths)
	{
		width += lengths.width;
		height += lengths.height;
	}

	void Lengths::operator += (Distance const& distance)
	{
		width += distance.begin;
		height += distance.end;
	}

	void Lengths::operator += (Coords const& coords)
	{
		width += coords.x;
		height += coords.y;
	}

	Lengths Lengths::operator - (Lengths const& lengths)
	{
		return *new Lengths{width - lengths.width, height - lengths.height};
	}

	template<class T>
	Lengths Lengths::operator * (T t)
	{
		return *new Lengths{width * t, height * t};
	}

	template<class T>
	Lengths Lengths::operator / (T t)
	{
		return *new Lengths{width / t, height / t};
	}

	void Lengths::operator = (Coords const& coords)
	{
		width = coords.x;
		height = coords.y;
	}

	bool Lengths::operator != (Lengths const& lengths)
	{
		if (width != lengths.width) return true;
		if (height != lengths.height) return true;
		return false;
	}

	Distance::Distance(int n)
		: begin{n}, end{n}
	{ }

	Distance::Distance(int begin, int end)
		: begin{begin}, end{end}
	{ }

	Distance Distance::operator + (Lengths const& lengths)
	{
		return *new Distance{begin + lengths.width, end + lengths.height};
	}

	Distance Distance::operator + (Distance const& distance)
	{
		return *new Distance{begin + distance.begin, end + distance.end};
	}

	Distance Distance::operator + (Coords const& coords)
	{
		return *new Distance{begin + coords.x, end + coords.y};
	}

	void Distance::operator += (Lengths const& lengths)
	{
		begin += lengths.width;
		end += lengths.height;
	}

	void Distance::operator += (Distance const& distance)
	{
		begin += distance.begin;
		end += distance.end;
	}

	void Distance::operator += (Coords const& coords)
	{
		begin += coords.x;
		end += coords.y;
	}

	Brush::Brush(Color color)
	{
		color_pairs.push_back({color, Color::white});
	}

	Brush::Brush(Color color_back, Color color_fore)
	{
		color_pairs.push_back({color_back, color_fore});
	}

	Brush::Brush(char ch)
	{
		this->ch = ch;
		color_pairs.push_back({Color::black, Color::white});	
	}

	Brush::Brush(Color_pair color_pair, char ch)
	{
		color_pairs.push_back(color_pair);
		this->ch = ch;
	}

	Brush::Brush(vector<Color_pair> color_pairs, char ch)
		: color_pairs{color_pairs}, ch{ch}
	{ }

	Brush::Brush(Color_pair color_pair)
	{
		color_pairs.push_back(color_pair);
	}

	Brush::Brush(vector<Color_pair> color_pairs)
		: color_pairs{color_pairs}
	{ }

	void Cells::resize(int width, int height)
	{
		cells.resize(width);
		for(vector<Cell>& in_cells_x : cells)
			in_cells_x.resize(height);
	}

	void Cells::resize(Lengths lengths)
	{
		cells.resize(lengths.width);
		for(vector<Cell>& in_cells_x : cells)
			in_cells_x.resize(lengths.height);
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

	void Surface::draw_cell(Coords xy)
	{
		cells[xy.x][xy.y].color_pair.background = Color::white;
		cells[xy.x][xy.y].is_draw = false;
		is_draw = false;
	}

	void Surface::draw_cell(Coords xy, Brush brush)
	{
		if (brush.color_pairs[0].background != Color::transparency)
			cells[xy.x][xy.y].color_pair.background = brush.color_pairs[0].background;
		if (brush.color_pairs[0].foreground != Color::transparency)
			cells[xy.x][xy.y].color_pair.foreground = brush.color_pairs[0].foreground;
		cells[xy.x][xy.y].ch = brush.ch;
		cells[xy.x][xy.y].is_draw = false;
		is_draw = false;
	}

	void Surface::draw_row(int row, Brush brush)
	{
		for(int x = 0; x <= cells.size(0); x++)
			draw_cell({x, row}, brush);
	}

	void Surface::draw_row(int row, Distance distance, Brush brush)
	{
		for(int x = distance.begin; x <= distance.end; x++)
			draw_cell({x, row}, brush);
	}

	void Surface::draw_column(int column, Brush brush)
	{
		for(int y = 0; y <= cells.size(1); y++)
			draw_cell({column, y}, brush);
	}

	void Surface::draw_column(int column, Distance distance, Brush brush)
	{
		for(int y = distance.begin; y <= distance.end; y++)
			draw_cell({column, y}, brush);
	}

	void Surface::draw_line(Coords xy1, Coords xy2, Brush brush)
	{
		Coords xy{0};
		Lengths len = xy2 - xy1;
		int max_len = (len.width > len.height) ? len.width : len.height;
		for(int i = 0; i <= max_len; i++) {
			xy = xy1 + len * i / max_len;
			draw_cell(xy, brush);
		}
	}

	void Surface::draw_area(Coords xy1, Coords xy2, Brush brush)
	{
		for(int x = xy1.x; x <= xy2.x; x++)
		for(int y = xy1.y; y <= xy2.y; y++)
			draw_cell({x, y}, brush);
	}

	void Surface::draw_frame(Coords xy1, Coords xy2, Brush brush)
	{
		draw_row(xy1.y, {xy1.x, xy2.x}, brush);
		draw_row(xy2.y, {xy1.x, xy2.x}, brush);
		draw_column(xy1.x, {xy1.y + 1, xy2.y - 1}, brush);
		draw_column(xy2.x, {xy1.y + 1, xy2.y - 1}, brush);
	}

	template<typename T>
	void Surface::draw_string(Coords xy, T t)
	{
		string str = to_cast<string, T>(t);
		for(int i = 0; i < str.size(); i++)
			draw_cell({i + xy.x, xy.y}, {str[i]});
	}

	void Surface::draw_string(Coords xy, String_builder string_builder)
	{
		for(int i = 0; i < string_builder.str.size(); i++)
			draw_cell({i + xy.x, xy.y}, {string_builder.str[i]});
	}

	void Surface::draw_string(Coords xy, String_builder string_builder, Brush brush)
	{
		for(int i = 0; i < string_builder.str.size(); i++)
			draw_cell({i + xy.x, xy.y},
				{{brush.color_pairs[0]}, string_builder.str[i]});
	}

	template<typename T>
	void Surface::draw_string(Coords xy, Direction dir, T t)
	{
		string str = to_cast<string, T>(t);
		for(int i = 0; i < str.size(); i++)
		switch(dir) {
			case Direction::up:
				draw_cell({xy.x, xy.y - i}, {str[i]}); break;
			case Direction::right:
				draw_cell({xy.x + i, xy.y}, {str[i]}); break;
			case Direction::down:
				draw_cell({xy.x, xy.y + i}, {str[i]}); break;
			case Direction::left:
				draw_cell({xy.x - i, xy.y}, {str[i]}); break;
		}
	}

	void Surface::draw_string(Coords xy, Direction dir, String_builder string_builder)
	{
		for(int i = 0; i < string_builder.str.size(); i++)
		switch(dir) {
			case Direction::up:
				draw_cell({xy.x, xy.y - i}, {string_builder.str[i]}); break;
			case Direction::right:
				draw_cell({xy.x + i, xy.y}, {string_builder.str[i]}); break;
			case Direction::down:
				draw_cell({xy.x, xy.y + i}, {string_builder.str[i]}); break;
			case Direction::left:
				draw_cell({xy.x - i, xy.y}, {string_builder.str[i]}); break;
		}
	}

	void Surface::draw_string(Coords xy, Direction dir, String_builder string_builder, Brush brush)
	{
		for(int i = 0; i < string_builder.str.size(); i++)
		switch(dir) {
			case Direction::up:
				draw_cell({xy.x, xy.y - i}, {{brush.color_pairs[0]}, string_builder.str[i]}); break;
			case Direction::right:
				draw_cell({xy.x + i, xy.y}, {{brush.color_pairs[0]}, string_builder.str[i]}); break;
			case Direction::down:
				draw_cell({xy.x, xy.y + i}, {{brush.color_pairs[0]}, string_builder.str[i]}); break;
			case Direction::left:
				draw_cell({xy.x - i, xy.y}, {{brush.color_pairs[0]}, string_builder.str[i]}); break;
		}
	}

	void Surface::fill(Brush brush)
	{
		for(int x = 0; x < cells.size(0); x++)
		for(int y = 0; y < cells.size(1); y++)
			draw_cell({x, y}, brush);
	}

	void Surface::clear()
	{
		fill({{{Color::black, Color::white}}, ' '});
	}

	void Surface::draw(Coords xy1, Coords xy2)
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

	void Surface::draw(Coords xy)
	{
		draw(xy, {cells.size(0) - xy.x, cells.size(1) - xy.y});
	}

	void Surface::draw()
	{
		draw({0, 0}, {cells.size(0), cells.size(1)});
	}

	int Standart_screen::width()
	{
		using namespace ncurses;
		int width, oops;
		getmaxyx(stdscr, oops, width);
		return width;
	}

	int Standart_screen::height()
	{
		using namespace ncurses;
		int height, oops;
		getmaxyx(stdscr, height, oops);
		return height;
	}

	Lengths Standart_screen::lengths()
	{
		using namespace ncurses;
		Lengths lengths;
		getmaxyx(stdscr, lengths.height, lengths.width);
		return lengths;
	}

	void Standart_screen::clear()
	{
		cells.fill({{Color::black, Color::white}, ' ', false});
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

	void View_::resize(Lengths lengths)
	{
		this->lengths = lengths;
		surface.cells.resize(lengths);
	}

	void View_::move(Coords xy)
	{
		this->xy = xy;
	}

	void View_::update()
	{
		log_out += "View_::update is not realized";
	}

	void View_::draw()
	{
		if (parent == nullptr)
			surface.draw(xy, xy + lengths);
		else
		{
			Coords parent_xy{0};
			View_* view_ = this;
			while(view_->parent != nullptr) {
				parent_xy += view_->parent->xy;
				view_ = view_->parent;
			}
			surface.draw(xy + parent_xy,
												 xy + parent_xy + lengths);
		}
		items.draw();
	}

	View Views::create_view(Coords xy, Lengths lengths)
	{
		views.push_back(*new View_);
		View_* view = &views[views.size() - 1];
		view->xy = xy;
		view->lengths = lengths;
		view->is_viewed = true;
		view->surface.cells.resize(view->lengths);

		return *new View(views.size() - 1);	
	}

	View Views::create_view(Lengths lengths)
	{
		return create_view({0, 0}, lengths);
	}
	
	View Views::create_view()
	{
		Lengths lengths{0};
		{ using namespace ncurses; getmaxyx(stdscr, lengths.height, lengths.width); }
		return create_view({0, 0}, lengths);
	}

	View Views::create_subview(View& view, Coords xy, Lengths lengths)
	{
		View sub_view = create_view(xy, lengths);
		sub_view->parent = &view;
		view->children.push_back(&sub_view);

		return sub_view;
	}

	View Views::create_subview(View& view, Lengths lengths)
	{
		return create_subview(view, {0, 0}, lengths);
	}

	View Views::create_subview(View& view)
	{
		return create_subview(view, {0, 0}, view->lengths);
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
