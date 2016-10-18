#include "MyThread.h"


MyThread::MyThread()
{
	flagFree = TRUE;
}


MyThread::~MyThread()
{
}

void MyThread::checkThread()
{
	DWORD dw;
	GetExitCodeThread(hThread, &dw);
	if (dw == STILL_ACTIVE)
	{
		flagFree = FALSE;
	}
	else
	{
		flagFree = TRUE;
		TerminateThread(hThread, 0);
	}
}

void MyThread::createNewThread(LPTHREAD_START_ROUTINE lpStartAddress)
{
	hThread = CreateThread(NULL, 0, lpStartAddress, NULL, 0, NULL);
	flagFree = FALSE;
	//checkThread();
}

bool MyThread::isFree()
{
	return flagFree;
}
