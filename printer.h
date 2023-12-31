#ifndef PRINTER_H
#define PRINTER_H

#include <map>
#include <ranges>
#include <string>
#include <iostream>
#include <windows.h>


class printer {
	std::map<DWORD, std::wstring> content_map_;
public:
	printer() = default;
	~printer() = default;
	printer(const printer &) = delete;				// 禁止拷贝构造
	printer &operator=(const printer &) = delete;	// 禁止赋值
	printer(printer &&) = delete;					// 禁止移动构造
	printer &operator=(printer &&) = delete;		// 禁止移动赋值

	void add_and_print(const DWORD processId, const std::wstring &line) {
		add_line(processId, line);
		print();
	}

	void add_line(const DWORD processId, const std::wstring &line) {
		content_map_[processId] = line;
	}

	void print() const {
		//system("cls");	// 清除屏幕
		for (const auto& val : content_map_ | std::views::values) {
			std::wcout << val << '\n';
		}
	}

	void clear() {
		content_map_.clear();
	}

};

void clear_last_line();

void clear_console_except_last_line();

#endif // PRINTER_H
