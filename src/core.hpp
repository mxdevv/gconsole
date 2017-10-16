#ifndef GCONSOLE_CORE_HPP
	#define GCONSOLE_CORE_HPP

#include "core.h"

namespace gcons {	
	enum struct Color {
		transparency = -1,
		black,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		white,

		end, begin = 0
	};

	struct Allign {
		Allign();

		int right : 1;
		int left : 1;
		int up : 1;
		int down : 1;
	};

	struct Color_pair {
		Color background = Color::transparency;
		Color foreground = Color::transparency;
	};

	struct Coords {
		Coords() { }
		Coords(int n);
		Coords(int x, int y);

		int x = nullable, y = nullable;

		Coords operator + (Lengths const& lengths);
		Coords operator + (Distance const& distance);
		Coords operator + (Coords const& coords);
		void operator += (Lengths const& lengths);
		void operator += (Distance const& distance);
		void operator += (Coords const& coords);
		
		Coords operator - (Coords const& coords);
		void operator -= (Coords const& coords);
		Coords operator / (Coords const& coords);
		template<class T>
			Coords operator / (T const& t);
		Coords operator * (Coords const& coords);
		template<class T>
			Coords operator * (T const& t);
	};

	struct Lengths {
		Lengths() { }
		Lengths(int n);
		Lengths(int width, int height);
		Lengths(Coords coords);

		int width = nullable, height = nullable;

		Lengths operator + (Lengths const& lengths);
		Lengths operator + (Distance const& distance);
		Lengths operator + (Coords const& coords);
		void operator += (Lengths const& lengths);
		void operator += (Distance const& distance);
		void operator += (Coords const& coords);
		
		template<class T>
			Lengths operator * (T t);
		template<class T>
			Lengths operator / (T t);
		void operator = (Coords const& coords);
	};

	struct Distance {
		Distance() { }
		Distance(int n);
		Distance(int begin, int end);

		int begin = nullable, end = nullable;

		Distance operator + (Lengths const& lengths);
		Distance operator + (Distance const& distance);
		Distance operator + (Coords const& coords);
		void operator += (Lengths const& lengths);
		void operator += (Distance const& distance);
		void operator += (Coords const& coords);
	};

	struct Brush {
		vector<Color_pair> color_pairs;
		char ch = nullable;
		int size = 1;
	};

	struct Cell {
		Color_pair color_pair;
		char ch = nullable;
		bool is_draw = false;
	};

	struct Cells {
		vector<vector<Cell>> cells;

		void resize(int x, int y);
		void fill(Cell cell);
		int size();
		int size(int deep);

		vector<Cell>& operator [] (int n);
	};

	struct Screen_buffer_base {
		Cells cells;
		bool is_draw = false;
	};

	struct Screen_buffer : Screen_buffer_base {
		void draw_cell(Coords xy); // Test_realized
		void draw_cell(Coords xy, Brush brush); // Realized
		void draw_point(Coords xy, Brush brush);
		void draw_row(int row, Brush brush); // Realized, not size, not multicolors
		void draw_row(int row, Distance distance, Brush brush); // Realized, not size, not multicolors
		void draw_column(int column, Brush brush); // Realized, not size, not multicolors
		void draw_column(int column, Distance distance, Brush brush); // Realized, not size, not multicolors
		void draw_line(Coords xy1, Coords xy2, Brush brush); // Realized, not size, not multicolors
		void draw_triangle(...);
		void draw_polygon(...);
		void draw_area(Coords xy1, Coords xy2, Brush brush); // Realized, not size, not multicolors
		void draw_frame(Coords xy1, Coords xy2, Brush brush); // Realized, not size, not multicolors
		void draw_style_frame(...);
		void draw_string(Coords xy, string str); // Realized
		template<typename T>
			void draw_string(Coords xy, T t); // Realized
		void draw_string(Coords xy, String_builder string_builder); // Realized
		void draw_string(Coords xy, String_builder string_builder, Brush brush); // Realized, not multicolors
		void draw_window(...);
		void fill(Brush brush);
		void clear();

		void draw();
		void draw(Coords xy);
		void draw(Coords xy1, Coords xy2);
	};

	struct Standart_screen : Screen_buffer_base {
		int width();
		int height();
		Coords width_height();

		void clear();

		void draw();

		int x = 0, y = 0;
	}
	standart_screen;

	struct Item {
		bool is_draw = false;

		void draw();
	};

	struct Items {
		vector<Item> items;

		void draw();
	};

	struct View_ {
		Screen_buffer screen_buffer;
		Items items;

		void hide();
		void show();
		void resize(Lengths lengths);
		void cut(Lengths cut_lengths);
		void move(Coords xy);
		
		void draw();
	//private:
		View_* parent = nullptr;
		vector<View_*> children;
		Coords xy;	
		Lengths lengths;
		Lengths cut_lengths;
		Allign allign;
		int depth = 0;
		bool is_viewed = false;
	};

	struct View {
		friend Views;

		View_* operator -> ();
		View_* operator & ();
	private:
		View(int n);

		int i;
	};

	struct Views {
		vector<View_> views;

		View create_view();
		View create_view(Lengths lengths);
		View create_view(Coords xy, Lengths lengths);
		View create_subview(View& view);
		View create_subview(View& view, Lengths lengths);
		View create_subview(View& view, Coords xy, Lengths lengths);
		void draw();

		int size();
		View_& operator [] (int n);
	}
	views;

	struct String_builder {
		template<typename... Args>
			String_builder(Args... arg);

		char& operator [] (int n);

		string str;
	};

	struct Log_out {
		void operator () ();
		template<typename T>
			void operator += (T t);
		void operator += (String_builder string_builder);
	private:
		vector<string> strs;
	}
	log_out;
}

#endif
