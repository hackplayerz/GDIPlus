#include "pch.h"
#include <iostream>
#include <thread>
#include "FileDownThread.h"

using namespace std;

void TestFunc(int Num)
{
	for (int i = 0; i < 1000; i++)
	{
		cout << Num;
	}
	cout << endl << "Thread" << Num << "End" << endl;
}

int main()
{
	//thread thread1(&TestFunc, 1);
	//thread thread2(&TestFunc, 2);

	//for (int i = 0; i < 1000; i++)
	//{
	//	cout << "타다";
	//}

	//thread1.join(); /** Wait while thread is end */
	//thread2.join();
	//cout << endl <<"Main Thread End" << endl;

	FileDownThread downloader;
	downloader.Run();

	system("pause");
	return 0;
}