#pragma once

#include "Library.h"

//颜色定义
#define COLOR_RED (FOREGROUND_RED | FOREGROUND_INTENSITY)                                     
#define COLOR_GREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)                                 
#define COLOR_BLUE (FOREGROUND_BLUE | FOREGROUND_INTENSITY)                                         
#define COLOR_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)                      
#define COLOR_MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)                        
#define COLOR_CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)                       
#define COLOR_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)                             
#define COLOR_GRAY (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)       
#define COLOR_BLACK (0)                                                                               
#define COLOR_DEFAULT (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)                          

#define BG_COLOR_RED (BACKGROUND_RED | BACKGROUND_INTENSITY)                                  
#define BG_COLOR_GREEN (BACKGROUND_GREEN | BACKGROUND_INTENSITY)                               
#define BG_COLOR_BLUE (BACKGROUND_BLUE | BACKGROUND_INTENSITY)	                                    
#define BG_COLOR_YELLOW (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY)                
#define BG_COLOR_MAGENTA (BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY)                
#define BG_COLOR_CYAN (BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)                   
#define BG_COLOR_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)                          
#define BG_COLOR_GRAY (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)      
#define BG_COLOR_BLACK (0)                                                                            
#define BG_COLOR_DEFAULT (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)                    


namespace DpLib
{
	class DpConsole {

		//基本信息
	public:
		HANDLE hConsole;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
	public:
		HANDLE GetConsoleHandle() {
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			return hConsole;
		}
		void GetBufferInfo() {
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		}

		//窗口相关
	public:
		void setTitle(std::string title) {
			SetConsoleTitle(title.c_str());
		}
		void setSize(int width, int height) {
			COORD size = { width, height };
			SetConsoleScreenBufferSize(hConsole, size);
		}
		void setPosition(int x, int y) {
			int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			SMALL_RECT rect = { 0, 0, width - 1, height - 1 };
			SetConsoleWindowInfo(hConsole, TRUE, &rect);
		}

		// 光标相关
	public:
		CONSOLE_CURSOR_INFO cursorInfo;
	public:
		void getCursorInfo() {
			GetConsoleCursorInfo(hConsole, &cursorInfo);
		}
		void setCursorPosition(int x, int y) {
			COORD coord = { x, y };
			SetConsoleCursorPosition(hConsole, coord);
		}
		void setCursorVisible(bool visible) {
			cursorInfo.bVisible = visible;
			SetConsoleCursorInfo(hConsole, &cursorInfo);
		}
		void setCursorSize(int size) {
			cursorInfo.dwSize = size;
			SetConsoleCursorInfo(hConsole, &cursorInfo);
		}

		//颜色相关
	public:
		void setColor(int color) {
			SetConsoleTextAttribute(hConsole, color);
		}
		void setColor(int color, int bgColor) {
			SetConsoleTextAttribute(hConsole, color | bgColor);
		}
		void resetColor() {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

		//写入缓冲区相关
	public:
		void write(std::string str) {
			WriteConsole(hConsole, str.c_str(), str.size(), NULL, NULL);
		}
		void write(std::string str, int color) {
			setColor(color);
			write(str);
			resetColor();
		}
		void write(std::string str, int color, int bgColor) {
			setColor(color, bgColor);
			write(str);
			resetColor();
		}
		void writeOutput(COORD size, COORD pos, SMALL_RECT rect, std::string str) {
			CHAR_INFO* buffer = new CHAR_INFO[str.size()];
			for (int i = 0; i < str.size(); i++) {
				buffer[i].Char.UnicodeChar = str[i];
				buffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			}
			WriteConsoleOutput(hConsole, buffer, size, pos, &rect);
			delete[] buffer;
		}
		void writeOutput(COORD size, COORD pos, SMALL_RECT rect, CHAR_INFO* buffer) {
			WriteConsoleOutput(hConsole, buffer, size, pos, &rect);
		}
		void writeOutput(COORD size, COORD pos, SMALL_RECT rect, std::string str, int color) {
			CHAR_INFO* buffer = new CHAR_INFO[str.size()];
			for (int i = 0; i < str.size(); i++) {
				buffer[i].Char.UnicodeChar = str[i];
				buffer[i].Attributes = color;
			}
			WriteConsoleOutput(hConsole, buffer, size, pos, &rect);
			delete[] buffer;
		}
		DWORD writeAttribute(WORD* attribute, int size, COORD pos) {
			DWORD numWritten;
			WriteConsoleOutputAttribute(hConsole, attribute, size, pos, &numWritten);
			return numWritten;
		}
		DWORD writeCharacter(std::string str, COORD pos) {
			DWORD numWritten;
			WriteConsoleOutputCharacter(hConsole, str.c_str(), str.size(), pos, &numWritten);
			return numWritten;
		}

		//读取相关
	public:
		std::string read() {
			std::string str;
			char c;
			while (ReadConsole(hConsole, &c, 1, NULL, NULL)) {
				if (c == '\r') {
					continue;
				}
				if (c == '\n') {
					break;
				}
				str += c;
			}
			return str;
		}
		std::string read(int size) {
			std::string str;
			char c;
			for (int i = 0; i < size; i++) {
				ReadConsole(hConsole, &c, 1, NULL, NULL);
				str += c;
			}
			return str;
		}
		CHAR_INFO* readOutput(COORD size, COORD pos, SMALL_RECT rect) {
			CHAR_INFO* buffer = new CHAR_INFO[size.X * size.Y];
			ReadConsoleOutput(hConsole, buffer, size, pos, &rect);
			return buffer;
		}
		WORD* readAttribute(COORD size, COORD pos) {
			WORD* attribute = new WORD[size.X * size.Y];
			ReadConsoleOutputAttribute(hConsole, attribute, size.X * size.Y, pos, NULL);
			return attribute;
		}
		std::string readCharacter(COORD size, COORD pos) {
			std::string str;
			char* buffer = new char[size.X * size.Y];
			ReadConsoleOutputCharacter(hConsole, buffer, size.X * size.Y, pos, NULL);
			for (int i = 0; i < size.X * size.Y; i++) {
				str += buffer[i];
			}
			delete[] buffer;
			return str;
		}
	};
}
