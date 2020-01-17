#include "pch.h"
#include "FileDownThread.h"

void FileDownThread::ThreadMain()
{
	for (int i = 0; i <= 10; i++)
	{
		system("cls");
		cout << i * 10 << "% downloaded" << endl;

		Sleep(700);
	}
}
