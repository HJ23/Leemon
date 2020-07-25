#include "engine.h"
#include <Windows.h>

LPSTR pathx = (LPSTR)"stockfish_10_x64.exe";


// create pipe connection to specified exe file -> basically runs exe file and gets its output 


void Connector::connect() {
	pipe_in_r = pipe_in_w = pipe_out_r = pipe_out_w = NULL;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CreatePipe(&pipe_out_r, &pipe_out_w, &sa, 0);
	CreatePipe(&pipe_in_r, &pipe_in_w, &sa, 0);

	sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	sti.wShowWindow = SW_HIDE;
	sti.hStdInput = pipe_in_r;
	sti.hStdOutput = pipe_out_w;
	sti.hStdError = pipe_out_w;
	CreateProcess(pathx, pathx, NULL, NULL, true, 0, NULL, NULL, &sti, &pi);

}

std::string Connector::get_result(std::string argument) {
	std::string str;
	argument = "position startpos moves " + argument + "\n" + "go depth 16" + "\n";
	WriteFile(pipe_in_w, argument.c_str(), argument.length(), &write, NULL);
	Sleep(3000);
	PeekNamedPipe(pipe_out_r, by, sizeof(by), &read, &available, NULL);

	do {
		ZeroMemory(by, sizeof(by));
		if (!ReadFile(pipe_out_r, by, sizeof(by), &read, NULL) || !read) break;
		by[read] = 0;
		str += (char*)by;

	} while (read >= sizeof(by));
	try {
		int n = str.find("bestmove");
		if (n != -1) return str.substr(n + 9, 4);
		else {
			throw ENGINE_ERROR_EXCEPTION();
		}
	}
	catch (EXCEPTION exception) {
		std::cout << exception.get_error_message();
		getc(stdin);
		exit(0);
	}


}

void Connector::close() {

	WriteFile(pipe_in_w, "quit\n", 5, &write, NULL);
	if (pipe_in_w != NULL) CloseHandle(pipe_in_w);
	if (pipe_in_r != NULL) CloseHandle(pipe_in_r);
	if (pipe_out_w != NULL) CloseHandle(pipe_out_w);
	if (pipe_out_r != NULL) CloseHandle(pipe_out_r);
	if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
	if (pi.hThread != NULL) CloseHandle(pi.hThread);


}
