#pragma once
#include "Library.h"

#define COLOR_RED (FOREGROUND_RED | FOREGROUND_INTENSITY)                                     
#define COLOR_GREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)                                 
#define COLOR_BLUE (FOREGROUND_BLUE | FOREGROUND_INTENSITY)                                         
#define COLOR_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)                      
#define COLOR_MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)                        
#define COLOR_CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)                       
#define COLOR_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define COLOR_BLACK (0)                                                                                                        

#define BG_COLOR_RED (BACKGROUND_RED | BACKGROUND_INTENSITY)                                  
#define BG_COLOR_GREEN (BACKGROUND_GREEN | BACKGROUND_INTENSITY)                               
#define BG_COLOR_BLUE (BACKGROUND_BLUE | BACKGROUND_INTENSITY)	                                    
#define BG_COLOR_YELLOW (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY)                
#define BG_COLOR_MAGENTA (BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY)                
#define BG_COLOR_CYAN (BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)                                         
#define BG_COLOR_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BG_COLOR_BLACK (0)                                                                              

namespace ConsoleFactory {
	class ConsoleManager
	{
	public:
#ifdef NEW_CHAR_INFO
		class CharInfo
		{
		private:
			int x;
			int y;
			CHAR_INFO** ci;
		public:
			CharInfo();
			CharInfo(int x, int y);
		public:
			bool writeFill(std::string str, WORD attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			CHAR_INFO* getCharInfo();
		};
#endif
	private:
		HANDLE hConsole;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		CONSOLE_CURSOR_INFO cci;
	public:
		//获取窗口句柄
		HANDLE getConsoleHandle();
	public:
		//读取窗口信息
		void loadConsoleInfo();
	public:
	public:
		//获取窗口宽度
		int getConsoleWidth();
		//获取窗口高度
		int getConsoleHeight();
		//设置窗口标题
		bool setCConsoleWindowTitle(const char* title);
		//设置窗口大小
		bool setConsoleWindowSize(short width, short height);
		//设置缓冲区大小
		bool setConsoleBufferSize(short width, short height);

	public: //cursor 
		//读取光标信息
		void loadCursorInfo();
		//获取光标是否可见
		bool isConsoleCursorVisible();
		//获取光标大小
		int getConsoleCursorSize();
		//设置光标可见
		bool setConsoleCursorVisible(bool visible);
		//设置光标大小
		bool setConsoleCursorSize(unsigned int size);

		//获取光标位置
		COORD getConsoleCursorPosition();
		//移动光标位置
		bool moveConsoleCursorPosition(short x, short y);
		//清除屏幕
		bool clearConsole();



	public: //color
		//获取颜色
		WORD getConsoleColor();
		//设置颜色
		bool setConsoleColor(WORD color);
		//重置颜色
		bool resetConsoleColor();

	public:
		bool write(std::string str, WORD color = COLOR_WHITE, WORD bgColor = BG_COLOR_BLACK);

		bool writeOutput(SMALL_RECT rect, std::string str, COORD size, COORD pos, WORD Attributes = COLOR_WHITE | BG_COLOR_BLACK);
		bool writeOutput(RECT rect, const CHAR_INFO* buffer, COORD size, COORD start);
		DWORD writeAttribute(WORD* attribute, int size, COORD pos);
		DWORD writeCharacter(std::string str, COORD pos);

		//读取相关
	public:
		std::string read();
		std::string read(int size);
		CHAR_INFO* readOutput(COORD size, COORD pos, SMALL_RECT rect);
		WORD* readAttribute(COORD size, COORD pos);
		std::string readCharacter(COORD size, COORD pos);

	public: //input&output mode
		//设置输入输出模式
		bool setConsoleMode(DWORD mode);
		//获取输入输出模式
		DWORD getConsoleMode();


		//读取总输入
		bool readConsoleInput(INPUT_RECORD* buffer, DWORD& count);
#ifdef DETAILED_INPUT_OUTPUT_INFO
		//读取键盘输入
		bool readConsoleKeyboardInput(INPUT_RECORD* buffer, DWORD& count);
		//读取鼠标输入
		bool readConsoleMouseInput(INPUT_RECORD* buffer, DWORD& count);
		//读取窗口消息
		bool readConsoleWindowMessage(LPMSG buffer, DWORD& count);
#endif

	public:
		ConsoleManager();
		~ConsoleManager();
	};
	class ConsoleButton {
	private:
		COORD pos;
		WORD width;
		WORD height;
		WORD color;
		WORD bgColor;
	public:
		ConsoleButton();
		ConsoleButton(COORD pos, WORD width, WORD height);
		void setPos(COORD pos);
		void setWidth(WORD width);
		void setHeight(WORD height);
		COORD getPos();
		WORD getWidth();
		WORD getHeight();
		//检测区域内是否按下
		bool isSelected();
	};
	class ConsoleButtonText {
	private:
		std::string text;
		COORD pos;
		WORD color;
		WORD bgColor;
		bool selected;
		ConsoleButton button;
	public:
		ConsoleButtonText(std::string text, COORD pos, WORD color = COLOR_WHITE, WORD bgColor = BG_COLOR_BLACK);
		void setPos(COORD pos);
		void setColor(WORD color);
		void setBgColor(WORD bgColor);
		void setText(std::string text);
		COORD getPos();
		WORD getColor();
		WORD getBgColor();
		//检测按钮是否按下
		bool isSelected();
		//绘制按钮
		void draw();
	};

	static ConsoleManager ConsoleM;
}