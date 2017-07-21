
#ifdef _WIN32
#include <stdio.h>
#include <Windows.h>
#include <string>

void StartProcess(char *FullFilePath, char *Args)
{
	int lenA = lstrlenA(FullFilePath);
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, FullFilePath, lenA, NULL, 0);
	wchar_t *Parameters = 0;
	wchar_t *File = 0;
	wchar_t result[MAX_PATH];
	if (lenW>0)
	{
		File = new wchar_t[lenW];
		::MultiByteToWideChar(CP_ACP, 0, FullFilePath, lenA+1, File, lenW);
	}
	File[lenA] = '\0';
	lenA = lstrlenA(Args);
	lenW = ::MultiByteToWideChar(CP_ACP, 0, Args, lenA, NULL, 0);
	if (lenW>0)
	{
		Parameters = new wchar_t[lenW];
		::MultiByteToWideChar(CP_ACP, 0, Args, lenA, Parameters, lenW);
	}
	Parameters[lenA] = '\0';
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = File;
	ShExecInfo.lpParameters = Parameters;
	ShExecInfo.lpDirectory = 0;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
}

char *GetMainProcessDirectory()
{
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	char *Bufferout = (char*)malloc((1+lstrlen(buffer)) * sizeof(char));
	wcstombs(Bufferout, buffer, lstrlen(buffer));
	Bufferout[lstrlen(buffer)] = '\0';
	return Bufferout;
}

#else
do your own StartProcess
#endif