#ifndef MENU_HPP
#define MENU_HPP
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;
void hidecursor() {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
// 显示鼠标光标
void showcursor() {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	CursorInfo.bVisible = true;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
// 移除控制台属性
void removeAttributes() {
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;	//移除快速编辑模式
	mode &= ~ENABLE_INSERT_MODE;		//移除插入模式
	mode |= ENABLE_MOUSE_INPUT;			//添加鼠标输入
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}
// 添加控制台属性
void addAttributes() {
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode |= ENABLE_QUICK_EDIT_MODE;		//添加快速编辑模式
	mode |= ENABLE_INSERT_MODE;     	//添加插入模式
	mode |= ENABLE_MOUSE_INPUT;			//添加鼠标输入
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

// 与鼠标有关的函数
// 获取鼠标位置
COORD getcursor() {
	CONSOLE_SCREEN_BUFFER_INFO temp;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
	return temp.dwCursorPosition;
}
// 设置鼠标位置
void setcursor(const COORD &temp = {0, 0}) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
// 等待鼠标事件，事件包括鼠标移动、点击、滚轮滚动等一切由鼠标出发的事件
MOUSE_EVENT_RECORD waitMouseEvent(bool move = true) {
	INPUT_RECORD record; //输入事件
	DWORD reg;           //临时寄存
	while (true) {
		Sleep(10);
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &reg);                                  //将输入事件存入record
		if (record.EventType == MOUSE_EVENT and (move | (record.Event.MouseEvent.dwEventFlags != MOUSE_MOVED))) //是鼠标事件 && 移动事件与模式对应
			return record.Event.MouseEvent;
	}
}
// 调试代码，用于输出COORD变量，打印鼠标位置
ostream &operator<<(ostream &pout, const COORD &temp) {
	pout.setf(ios_base::fixed);
	pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
	return pout;
}

// 与控制台文本颜色有关的类
// 一些常用颜色组合的定义
#define BlackWhite 0x07
#define BlackCyan 0x0b
#define YellowBlue 0xe9
struct Color {
	short Default;		// 默认颜色组合
	short Highlight;	// 文本高亮颜色组合
	short lastcolor;	// 用于实现切换颜色组合，属于辅助变量

	// 初始化
	Color() : Default(BlackWhite), Highlight(BlackCyan), lastcolor(BlackWhite) {}
	Color(short Default = BlackWhite, short Highlight = BlackCyan) : Default(Default), Highlight(Highlight), lastcolor(BlackWhite) {}

	// 将文本颜色设置为Default组合
	void setDefault() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Default);
		lastcolor = Default;
	}
	// 将文本颜色设置为Highlight组合
	void setHighlight() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Highlight);
		lastcolor = Highlight;
	}
};

// Menu类的前置声明，Parameter类中使用
class Menu;
// 定义Parameter成员变量状态
// buttonState变量值的定义
#define mouseLeftButton FROM_LEFT_1ST_BUTTON_PRESSED	// 左键
#define mouseMiddleButton FROM_LEFT_2ND_BUTTON_PRESSED	// 中键
#define mouseRightButton RIGHTMOST_BUTTON_PRESSED		// 右键
// eventFlag变量值的定义
#define mouseClick 0x0					// 鼠标按键按下或松开时触发
#define mouseMove MOUSE_MOVED			// 鼠标移动时触发
#define mouseDoubleClick DOUBLE_CLICK	// 鼠标第二次按下时触发，触发此事件前一定会触发 mouseClick 事件
#define mouseWheel MOUSE_WHEELED		// 鼠标滚轮滚动时触发
// *****Parameter类必须作为回调函数参数传入*****
struct Parameter {
	DWORD buttonState;		// 鼠标按键记录
	DWORD controlkeyState;	// 控制按键状态
	DWORD eventFlag;		// 鼠标事件记录
	Menu *menu;				// Menu型指针，传入原菜单

	// 初始化
	Parameter() : buttonState(mouseLeftButton), controlkeyState(0), eventFlag(0), menu(nullptr) {}
	Parameter(MOUSE_EVENT_RECORD mouseEvent, Menu *menu) : buttonState(mouseEvent.dwButtonState), controlkeyState(mouseEvent.dwControlKeyState), eventFlag(mouseEvent.dwEventFlags), menu(menu) {}
};
// 定义回调函数类型callback
typedef bool(*callback)(Parameter);

// 菜单的文本类，每个Text型变量都代表菜单中的一行
struct Text {
	string text;		// 文本内容
	Color color;		// 颜色属性
	COORD position;		// 文本位置
	callback function;	// 回调函数

	// 初始化
	Text() : text(""), color(Color(0, 0)), position({0, 0}), function(nullptr) {}
	Text(string text, Color color, callback function) : text(text), color(color), position({0, 0}), function(function) {}

	// 判断鼠标坐标是否与文本位置重合
	bool operator== (const COORD &mousePosition) const {
		if (position.Y == mousePosition.Y and position.X <= mousePosition.X and mousePosition.X < position.X + (short)text.length()) {
			return true;
		}
		return false;
	}
	// 判断鼠标坐标是否不与文本位置重合
	bool operator!= (const COORD &mousePosition) const {
		return !operator==(mousePosition);
	}
};

// *****菜单类*****
class Menu {
	private:
		long sleepTime;						// 循环中等待时间
		short consoleHeight, consoleWidth;	// 控制台长、宽属性
		vector<Text> linedata;				// 存放菜单每行的信息
	protected:
		// 获取控制台大小
		void getconsoleSize() {
			CONSOLE_SCREEN_BUFFER_INFO temp;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
			consoleHeight = temp.dwSize.Y;	// 赋值
			consoleWidth = temp.dwSize.X;	// 赋值
		}
		// 清屏
		void clearScreen() {
			getconsoleSize();
			setcursor({0, 0});
			for (short i = 0; i < consoleHeight; ++i) {
				for (short j = 0; j < consoleWidth; ++j) {
					cout << " ";
				}
			}
			setcursor({0, 0});
		}
		// 输出
		void write(string text, bool isEndl = false) {
			printf("%s", text.c_str());
			if (isEndl) {
				printf("\n");
			}
		}
		// 整行覆盖输出
		void rewrite(Text data) {
			setcursor({0, data.position.Y});
			// 清空旧数据
			for (short j = 0; j < data.position.X; ++j) {
				write(" ");
			}
			setcursor({0, data.position.Y});
			// 输出新数据
			write(data.text);
			setcursor({0, data.position.Y});
		}
		// 输出菜单，并对linedata内position成员变量初始化
		void initializePosition() {
			clearScreen();
			for (auto &data : linedata) {
				data.position = getcursor();
				data.color.setDefault();
				write(data.text, true);
			}
		}
		// 刷新菜单
		void refresh(COORD hangPosition) {
			for (auto &data : linedata) {
				// 如果鼠标位于%data%按钮上 并且 %data%按钮不处于高亮
				if (data == hangPosition and data.color.lastcolor != data.color.Highlight) {
					data.color.setHighlight();	// 设置为高亮颜色
					rewrite(data);
				}
				// 如果鼠标不位于%data%按钮上 并且 %data%按钮处于高亮
				if (data != hangPosition and data.color.lastcolor != data.color.Default) {
					data.color.setDefault();	// 设置为默认颜色
					rewrite(data);
				}
			}
		}
		// 自动调用回调函数
		bool implement(MOUSE_EVENT_RECORD mouseEvent) {
			bool isExit = false;	// 储存回调函数返回的结果，表示是否要退出菜单
			for (auto &data : linedata) {
				if (data == mouseEvent.dwMousePosition) {
					if (data.function != nullptr) {
						clearScreen();
						data.color.setDefault();
						addAttributes();
						showcursor();
						isExit = data.function(Parameter(mouseEvent, this));
						removeAttributes();
						hidecursor();
						initializePosition();
					}
					break;
				}
			}
			return isExit;
		}
	public:
		// 构造函数
		Menu() : sleepTime(50), consoleHeight(0), consoleWidth(0) {}
		~Menu() {}

		// 向菜单尾部添加一行
		Menu &push_back(string text = "", callback function = nullptr, short colorDefault = BlackWhite, short colorHighlight = BlackCyan) {
			if (function == nullptr) {
				linedata.push_back(Text(text, Color(colorDefault, colorDefault), function));
			} else {
				linedata.push_back(Text(text, Color(colorDefault, colorHighlight), function));
			}
			return *this;
		}
		// 从菜单尾部弹出一行
		Menu &pop_back() {
			linedata.pop_back();
			return *this;
		}
		// 清空菜单
		Menu &clear() {
			linedata.clear();
			return *this;
		}
		void showMenu() {
			removeAttributes();
			hidecursor();
			MOUSE_EVENT_RECORD mouseEvent;
			Sleep(100);
			initializePosition();
			bool isExit = false;
			while (!isExit) {
				Sleep(sleepTime);
				mouseEvent = waitMouseEvent();
				switch (mouseEvent.dwEventFlags) {
					case mouseMove: {
						refresh(mouseEvent.dwMousePosition);
						break;
					}
					case mouseClick: {
						if (mouseEvent.dwButtonState and mouseEvent.dwButtonState != mouseWheel) {
							isExit = implement(mouseEvent);
						}
						break;
					}
				}
			}
			clearScreen();
			Sleep(100);
		}
};
// exitMenu，可以当作回调函数使用，会自动返回true，退出菜单
bool _exitMenu(Parameter) {
	return true;
}
#define exitMenu _exitMenu
#endif
