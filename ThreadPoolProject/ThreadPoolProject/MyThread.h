#pragma once
#include <Windows.h>
class MyThread
{
public:
	MyThread();
	~MyThread();
	void createNewThread(LPTHREAD_START_ROUTINE lpStartAddress);
	void checkThread();
	HANDLE hThread;
	bool flagFree;
	bool isFree();

private:
};

