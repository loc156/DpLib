#include "ConsoleManager.h"

using namespace ConsoleFactory;

HANDLE ConsoleManager::getConsoleHandle() {
	return hConsole;
}

void ConsoleManager::loadConsoleInfo() {
	if (hConsole == INVALID_HANDLE_VALUE) {
		throw std::exception("Invalid console handle");
	}
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		throw std::exception("Failed to get console info");
	}
}

int ConsoleManager::getConsoleWidth() {
	loadConsoleInfo();
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}
int ConsoleManager::getConsoleHeight() {
	loadConsoleInfo();
	return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

bool ConsoleManager::setCConsoleWindowTitle(const char* title) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	return SetConsoleTitle(title);
}
bool ConsoleManager::setConsoleWindowSize(short width, short height) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	SMALL_RECT sr = { 0, 0, width - 1, height - 1 };
	return SetConsoleWindowInfo(hConsole, TRUE, &sr);
}
bool ConsoleManager::setConsoleBufferSize(short width, short height) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	COORD coord = { width, height };
	return SetConsoleScreenBufferSize(hConsole, coord);
}


void ConsoleManager::loadCursorInfo() {
	if (hConsole == INVALID_HANDLE_VALUE) {
		throw std::exception("Invalid console handle");
	}
	if (!GetConsoleCursorInfo(hConsole, &cci)) {
		throw std::exception("Failed to get cursor info");
	}
}

bool ConsoleManager::isConsoleCursorVisible() {
	loadCursorInfo();
	return cci.bVisible;
}
int ConsoleManager::getConsoleCursorSize() {
	loadCursorInfo();
	return cci.dwSize;
}

bool ConsoleManager::setConsoleCursorVisible(bool visible) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	cci = { cci.dwSize, visible };
	return SetConsoleCursorInfo(hConsole, &cci);
}
bool ConsoleManager::setConsoleCursorSize(unsigned int size) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	cci = { size, cci.bVisible };
	return SetConsoleCursorInfo(hConsole, &cci);
}

COORD ConsoleManager::getConsoleCursorPosition() {
	loadConsoleInfo();
	return csbi.dwCursorPosition;
}
bool ConsoleManager::moveConsoleCursorPosition(short x, short y) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	COORD coord = { x, y };
	return SetConsoleCursorPosition(hConsole, coord);
}
bool ConsoleManager::clearConsole() {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	const COORD coord = { 0, 0 };
	DWORD count;
	return WriteConsoleOutputCharacterA(hConsole, " ", 1, coord, &count);
}


ConsoleManager::ConsoleManager() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		throw std::exception("Failed to get console handle");
	}
	loadConsoleInfo();
	loadCursorInfo();
}
ConsoleManager::~ConsoleManager() {
	if (hConsole != INVALID_HANDLE_VALUE) {
		CloseHandle(hConsole);
	}
}





bool ConsoleManager::setConsoleMode(DWORD mode) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	return SetConsoleMode(hConsole, mode);
}
DWORD ConsoleManager::getConsoleMode() {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	DWORD mode;
	GetConsoleMode(hConsole, &mode);
	return mode;
}



bool ConsoleManager::readConsoleInput(INPUT_RECORD* buffer, DWORD& count) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	return ReadConsoleInput(hConsole, buffer, count, &count);

}


bool ConsoleManager::setConsoleColor(WORD color) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	loadConsoleInfo();
	return SetConsoleTextAttribute(hConsole, color);
}
WORD ConsoleManager::getConsoleColor() {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	loadConsoleInfo();
	return csbi.wAttributes;
}
bool ConsoleManager::resetConsoleColor() {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	return SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


bool ConsoleManager::write(std::string str, WORD color, WORD bgColor) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	DWORD count;
	setConsoleColor(color|bgColor);
	if (!WriteConsoleA(hConsole, str.c_str(), str.size(), &count, NULL)) {
		resetConsoleColor();
		return false;
	}
	resetConsoleColor();
	return true;
}


bool ConsoleManager::writeOutput(RECT rect, const CHAR_INFO* buffer, COORD size, COORD start) {
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	return WriteConsoleOutput(hConsole, buffer, size, start, &csbi.srWindow);
}
bool ConsoleManager::writeOutput(SMALL_RECT rect, std::string str, COORD size, COORD pos, WORD Attributes){
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	CHAR_INFO* buffer = new CHAR_INFO[str.size()];
	for (int i = 0; i < str.size(); i++) {
		buffer[i].Char.UnicodeChar = str[i];
		buffer[i].Attributes = Attributes;
	}
	WriteConsoleOutput(hConsole, buffer, size, pos, &rect);
	delete[] buffer;
	return true;
}
DWORD ConsoleManager::writeAttribute(WORD* attribute, int size, COORD pos){
	DWORD numWritten;
	WriteConsoleOutputAttribute(hConsole, attribute, size, pos, &numWritten);
	return numWritten;
}
DWORD ConsoleManager::writeCharacter(std::string str, COORD pos) {
	DWORD numWritten;
	WriteConsoleOutputCharacter(hConsole, str.c_str(), str.size(), pos, &numWritten);
	return numWritten;
}


std::string ConsoleManager::read() {
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
std::string ConsoleManager::read(int size) {
	std::string str;
	char c;
	for (int i = 0; i < size; i++) {
		ReadConsole(hConsole, &c, 1, NULL, NULL);
		str += c;
	}
	return str;
}
CHAR_INFO* ConsoleManager::readOutput(COORD size, COORD pos, SMALL_RECT rect) {
	CHAR_INFO* buffer = new CHAR_INFO[size.X * size.Y];
	ReadConsoleOutput(hConsole, buffer, size, pos, &rect);
	return buffer;
}
WORD* ConsoleManager::readAttribute(COORD size, COORD pos) {
	WORD* attribute = new WORD[size.X * size.Y];
	ReadConsoleOutputAttribute(hConsole, attribute, size.X * size.Y, pos, NULL);
	return attribute;
}
std::string ConsoleManager::readCharacter(COORD size, COORD pos) {
	std::string str;
	char* buffer = new char[size.X * size.Y];
	ReadConsoleOutputCharacter(hConsole, buffer, size.X * size.Y, pos, NULL);
	for (int i = 0; i < size.X * size.Y; i++) {
		str += buffer[i];
	}
	delete[] buffer;
	return str;
}



ConsoleButton::ConsoleButton() {}
ConsoleButton::ConsoleButton(COORD pos, WORD width, WORD height) {
	this->pos = pos;
	this->width = width;
	this->height = height;
}
void ConsoleButton::setPos(COORD pos) {
	this->pos = pos;
}
void ConsoleButton::setWidth(WORD width) {
	this->width = width;
}
void ConsoleButton::setHeight(WORD height) {
	this->height = height;
}
COORD ConsoleButton::getPos() {
	return pos;
}
WORD ConsoleButton::getWidth() {
	return width;
}
WORD ConsoleButton::getHeight() {
	return height;
}
bool ConsoleButton::isSelected() {
	INPUT_RECORD buffer;
	DWORD count;
	if (!ConsoleM.readConsoleInput(&buffer, count)) {
		return false;
	}
	if (buffer.EventType == MOUSE_EVENT) {
		MOUSE_EVENT_RECORD mouseEvent = buffer.Event.MouseEvent;
		if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			if (mouseEvent.dwMousePosition.X >= pos.X
				&& mouseEvent.dwMousePosition.X <= pos.X + width
				&& mouseEvent.dwMousePosition.Y >= pos.Y
				&& mouseEvent.dwMousePosition.Y <= pos.Y + height) {
				return true;
			}
		}
	}
	return false;
}



ConsoleButtonText::ConsoleButtonText(std::string text, COORD pos, WORD color, WORD bgColor) {
	this->text = text;
	this->pos = pos;
	this->color = color;
	this->bgColor = bgColor;
	this->selected = false;
	button = ConsoleButton(pos, short(text.size()), 1);
}
void ConsoleButtonText::setPos(COORD pos) {
	this->pos = pos;
}
void ConsoleButtonText::setColor(WORD color) {
	this->color = color;
}
void ConsoleButtonText::setBgColor(WORD bgColor) {
	this->bgColor = bgColor;
}
void ConsoleButtonText::setText(std::string text) {
	this->text = text;
}
COORD ConsoleButtonText::getPos() {
	return pos;
}
WORD ConsoleButtonText::getColor() {
	return color;
}
WORD ConsoleButtonText::getBgColor() {
	return bgColor;
}
bool ConsoleButtonText::isSelected() {
	if (button.isSelected()) {
		selected = true;
		return true;
	}
	selected = false;
	return false;
}
void ConsoleButtonText::draw(){
	if (selected) {
		ConsoleM.setConsoleColor(color | 0x0080);
	}
	else {
		ConsoleM.setConsoleColor(color|bgColor);
	}
	ConsoleM.writeOutput(SMALL_RECT{ pos.X, pos.Y, short(pos.X + text.size() - 1), pos.Y }, text, { short(text.size()), 1 }, COORD{ 0, 0 }, color | bgColor);
}