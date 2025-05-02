#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>

#include "DpKey.hpp"

namespace DpLib
{
	class Dpiost {
	public:
		struct print_state {
			bool Dpause = false;
			bool Dnewline = false;
		};
		print_state state;

	public:
		DpKey key;
		void pause() {
			while (true) {
				key.updateFSM(VK_RETURN, std::chrono::steady_clock::now());
				if (key.current->state == DpKey::State::PRESS)
					break;
			}
		}

	public:
		Dpiost CColor(WORD color) {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, color);
			return *this;
		}

	public:
		struct ColorCode {
			int code;
		};
		void set_color(ColorCode color) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color.code);
		}

	public:
		template <typename T>
		void base_print(T const& t) {
			std::cout << t;
			if (state.Dnewline) {
				std::cout << std::endl;
			}
			if (state.Dpause) {
				pause();
			}
		}
	public:
		Dpiost& print(std::string const& t) {
			base_print(t);
			return *this;
		}
		Dpiost& print(int const& t) {
			base_print(t);
			return *this;
		}
		Dpiost& print(double const& t) {
			base_print(t);
			return *this;
		}
		Dpiost& print(ColorCode const& t) {
			set_color(t);
			return *this;
		}


		template<typename A, typename... Args>
		Dpiost& operator()(A const& a, Args const&... args) {
			print(a);
			operator()(args...);
			return *this;
		}
		Dpiost& operator()() {
			return *this;
		}

		Dpiost& boolpt(bool const& t) {
			base_print(t ? "true" : "false");
			return *this;
		}


	public:

		Dpiost& input(std::string &t) {
			std::cin >> t;
			return *this;
		}
		Dpiost& input(int &t) {
			std::string s;
			std::cin >> s;
			t = std::stoi(s);
			return *this;
		}
	};


	using COLOR = Dpiost::ColorCode;
	COLOR RED = { FOREGROUND_RED };                                           //红色
	COLOR GREEN = { FOREGROUND_GREEN };                                       //绿色
	COLOR BLUE = { FOREGROUND_BLUE };                                         //蓝色
	COLOR YELLOW = { FOREGROUND_RED | FOREGROUND_GREEN };                     //黄色
	COLOR CYAN = { FOREGROUND_BLUE | FOREGROUND_GREEN };                      //青色
	COLOR MAGENTA = { FOREGROUND_RED | FOREGROUND_BLUE };                     //品红色
	COLOR WHITE = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };    //白色
	COLOR BLACK = { 0 };                                                      //黑色
}