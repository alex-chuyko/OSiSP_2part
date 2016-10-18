#include "ThreadPool.h"


ThreadPool::ThreadPool(int ThreadNumber)
{
	if (ThreadNumber > 0)
	{
		threadArr = new MyThread[ThreadNumber];
		threadNumber = ThreadNumber;
		freeThreadNumber = threadNumber;
		fileHandler = fopen(fileName, "a");
		GetLocalTime(&sysTime);
		fprintf(fileHandler, "%d-%02d-%02d %02d:%02d:%02d Message: Initialize the object threadpool with %d threads\n",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, threadNumber);
	}
	else
	{
		threadNumber = 0;
		freeThreadNumber = threadNumber;
		fileHandler = fopen(fileName, "a");
		GetLocalTime(&sysTime);
		fprintf(fileHandler, "%d-%02d-%02d %02d:%02d:%02d Warning: The minimum number of threads - 1\n",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, threadNumber);
	}
}


ThreadPool::~ThreadPool()
{
	fprintf(fileHandler, "%d-%02d-%02d %02d:%02d:%02d Message: Threads Pool has been destroyed!\n",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	fclose(fileHandler);
	delete hThreadArr;
}

void ThreadPool::checkThread(MyThread *thread)
{
	DWORD dw;
	GetExitCodeThread(thread->hThread, &dw);
	if (dw == STILL_ACTIVE)
	{
		thread->flagFree = FALSE;
	}
	else
	{
		thread->flagFree = TRUE;
		TerminateThread(thread->hThread, 0);
	}
}

int ThreadPool::getFreeThread()
{
	int i = 0;
	for (i = 0; i <= threadNumber - 1; i++)
	{
		checkThread(&threadArr[i]);
		if (threadArr[i].isFree() == TRUE)
			return i;
	}
	return -1;
}

void ThreadPool::createNewTask(LPTHREAD_START_ROUTINE lpStartAddress)
{
	int currentThreadNumber = getFreeThread(); 
	if (currentThreadNumber != -1)
	{
		threadArr[currentThreadNumber].createNewThread(lpStartAddress); 
		GetLocalTime(&sysTime);
		fprintf(fileHandler, "%d-%02d-%02d %02d:%02d:%02d Thread:%d Message: It created a new task!\n",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, threadArr[currentThreadNumber].hThread);
	}
	else
	{
		GetLocalTime(&sysTime);
		fprintf(fileHandler, "%d-%02d-%02d %02d:%02d:%02d Error: It exceeded the maximum number of working threads!\n",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	}
}
