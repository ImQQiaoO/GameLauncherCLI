#include "printer.h"

void clear_last_line() {

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	// 获取控制台句柄
	const HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// 获取控制台屏幕缓冲区信息
	GetConsoleScreenBufferInfo(h_stdout, &csbi);
	// 计算行和列
	int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	//std::wcout << columns << L" " << rows << std::endl;	// TODO：DEBUG
	// 将光标移动到最后一行的起始位置
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y -= 1;
	// 更新光标位置
	SetConsoleCursorPosition(h_stdout, csbi.dwCursorPosition);
	// 清除控制台最后一行
	DWORD written;
	FillConsoleOutputCharacter(h_stdout, ' ', columns, csbi.dwCursorPosition, &written);
	// 将光标重新移动到清除行的开始
	SetConsoleCursorPosition(h_stdout, csbi.dwCursorPosition);
}

void clear_console_except_last_line() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    const HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    // 获取控制台屏幕缓冲区信息
    GetConsoleScreenBufferInfo(h_stdout, &csbi);

    // 计算行和列
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int total_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // 保存当前光标位置
    COORD current_cursor_position = csbi.dwCursorPosition;

    // 将光标移动到控制台第一行
    COORD top_left_coord;
    top_left_coord.X = 0;
    top_left_coord.Y = csbi.srWindow.Top;

    SetConsoleCursorPosition(h_stdout, top_left_coord);

    // 清除控制台除了最后一行以外的所有内容
    DWORD written;
    FillConsoleOutputCharacter(h_stdout, ' ', columns * (total_rows - 1), top_left_coord, &written);
    FillConsoleOutputAttribute(h_stdout, csbi.wAttributes, columns * (total_rows - 1), top_left_coord, &written);

    // 恢复光标到原始位置
    SetConsoleCursorPosition(h_stdout, current_cursor_position);
}