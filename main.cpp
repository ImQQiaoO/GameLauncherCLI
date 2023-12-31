#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <thread>

#include "printer.h"

printer p;

/**
 * \brief Check if a process is running by its PID
 * \param processId: The PID of the process to check
 * \return bool: true if the process is running, false otherwise
 */
bool is_process_running(const DWORD processId) {
	bool exists = false;
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(PROCESSENTRY32W);

	const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32FirstW(snapshot, &entry)) {
		do {
			if (entry.th32ProcessID == processId) {
				exists = true;
				break;
			}
		} while (Process32NextW(snapshot, &entry));
	}
	CloseHandle(snapshot);
	return exists;
}

/**
 * \brief Execute a process in a given directory
 * \param exePath
 * \param workingDirectory
 * \return
 */
DWORD start_process_in_directory(const wchar_t *exePath, const wchar_t *workingDirectory) {
	STARTUPINFOW si; // 注意这里使用 STARTUPINFOW
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// 创建进程
	if (!CreateProcessW(  // 直接调用 CreateProcessW
		exePath,          // 可执行文件路径
		nullptr,             // 命令行参数
		nullptr,             // 进程句柄不可继承
		nullptr,             // 线程句柄不可继承
		FALSE,            // 句柄继承选项
		0,                // 没有创建标志
		nullptr,             // 使用父进程的环境块
		workingDirectory, // 设置工作目录
		&si,              // 指向 STARTUPINFOW 结构
		&pi               // 指向 PROCESS_INFORMATION 结构
	)) {
		std::wcerr << L"CreateProcessW failed (" << GetLastError() << L")." << std::endl;
		return false;
	}
	// 获取进程号
	const DWORD processId = pi.dwProcessId;
	// 通告printer
	//p.add_line(processId, )TODO
	// 等待进程结束
	//WaitForSingleObject(pi.hProcess, INFINITE);
	//// 关闭进程和线程句柄
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return processId;
}

void monitor_process(DWORD process_id) {
	bool isRunning = true;
	int cnt = 0;
	while (isRunning) {
		isRunning = is_process_running(process_id);
		Sleep(1000);
		++cnt;
		//clear_last_line();
		clear_console_except_last_line();
		std::wcout << L"Process " << process_id << L" has been running for " << cnt << L"s." << std::endl;
		std::wstring line = L"Process " + std::to_wstring(process_id) + L" has been running for " + std::to_wstring(cnt) + L"s.";
	}
	clear_last_line();
	std::wcout << L"Process " << process_id << L" stopped and ran for " << cnt << L"s." << std::endl;
}

bool kill_process(DWORD processId) {
	HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
	if (processHandle == NULL) {
		return false;
	}
	BOOL result = TerminateProcess(processHandle, 1);
	CloseHandle(processHandle);

	return result != 0;
}

void input_func(DWORD process_id) {
	std::wstring input;
	std::wcout << "Enter exit/e to exit:";
	while (true) {
		std::wcin >> input;
		if (input == L"exit" || input == L"e") {
			kill_process(process_id);
			break;
		}
	}
}

int main() {
	//const wchar_t *processName = L"TiWorker.exe"; // 进程名
	const wchar_t *process_dir = L"D:\\Pics\\Games\\SeaBed\\SeaBed\\SeaBed.exe"; // 进程名
	auto process_id = start_process_in_directory(process_dir, L"D:\\Pics\\Games\\SeaBed\\SeaBed");

	std::wcout << L"Process id: " << process_id << std::endl;
	//p.add_and_print(L"Process id: " + std::to_wstring(process_id));

	std::wcout << "\n";
	std::thread monitor_thread(monitor_process, process_id);
	std::thread input_thread(input_func, process_id);
	if (monitor_thread.joinable()) {
		monitor_thread.join();
	}
	if (input_thread.joinable()) {
		input_thread.join();
	}

	//monitor_process(process_id);
	return 0;
}
