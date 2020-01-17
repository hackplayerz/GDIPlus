#pragma disable(warning:4996)
#include <process.h>
#include <stdio.h>

void CharProcess(void* P)
{
	char ch = (char)P;

	for (int i = 0; i < 100; i++)
	{
		printf("%c", ch);
	}
	printf("\n");
}

int main()
{
	for (int i = 0; i < 10; i++)
	{
		//_beginthread(CharProcess, 0, (void*)(rand() % 26 + 'A'));
		_beginthread(CharProcess, 0, (void*)('A' + i));
	}

	system("pause");
	return 0;
}