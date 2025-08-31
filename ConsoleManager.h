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
		//��ȡ���ھ��
		HANDLE getConsoleHandle();
	public:
		//��ȡ������Ϣ
		void loadConsoleInfo();
	public:
	public:
		//��ȡ���ڿ��
		int getConsoleWidth();
		//��ȡ���ڸ߶�
		int getConsoleHeight();
		//���ô��ڱ���
		bool setCConsoleWindowTitle(const char* title);
		//���ô��ڴ�С
		bool setConsoleWindowSize(short width, short height);
		//���û�������С
		bool setConsoleBufferSize(short width, short height);

	public: //cursor 
		//��ȡ�����Ϣ
		void loadCursorInfo();
		//��ȡ����Ƿ�ɼ�
		bool isConsoleCursorVisible();
		//��ȡ����С
		int getConsoleCursorSize();
		//���ù��ɼ�
		bool setConsoleCursorVisible(bool visible);
		//���ù���С
		bool setConsoleCursorSize(unsigned int size);

		//��ȡ���λ��
		COORD getConsoleCursorPosition();
		//�ƶ����λ��
		bool moveConsoleCursorPosition(short x, short y);
		//�����Ļ
		bool clearConsole();



	public: //color
		//��ȡ��ɫ
		WORD getConsoleColor();
		//������ɫ
		bool setConsoleColor(WORD color);
		//������ɫ
		bool resetConsoleColor();

	public:
		bool write(std::string str, WORD color = COLOR_WHITE, WORD bgColor = BG_COLOR_BLACK);

		bool writeOutput(SMALL_RECT rect, std::string str, COORD size, COORD pos, WORD Attributes = COLOR_WHITE | BG_COLOR_BLACK);
		bool writeOutput(RECT rect, const CHAR_INFO* buffer, COORD size, COORD start);
		DWORD writeAttribute(WORD* attribute, int size, COORD pos);
		DWORD writeCharacter(std::string str, COORD pos);

		//��ȡ���
	public:
		std::string read();
		std::string read(int size);
		CHAR_INFO* readOutput(COORD size, COORD pos, SMALL_RECT rect);
		WORD* readAttribute(COORD size, COORD pos);
		std::string readCharacter(COORD size, COORD pos);

	public: //input&output mode
		//�����������ģʽ
		bool setConsoleMode(DWORD mode);
		//��ȡ�������ģʽ
		DWORD getConsoleMode();


		//��ȡ������
		bool readConsoleInput(INPUT_RECORD* buffer, DWORD& count);
#ifdef DETAILED_INPUT_OUTPUT_INFO
		//��ȡ��������
		bool readConsoleKeyboardInput(INPUT_RECORD* buffer, DWORD& count);
		//��ȡ�������
		bool readConsoleMouseInput(INPUT_RECORD* buffer, DWORD& count);
		//��ȡ������Ϣ
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
		//����������Ƿ���
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
		//��ⰴť�Ƿ���
		bool isSelected();
		//���ư�ť
		void draw();
	};

	static ConsoleManager ConsoleM;
}