#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <process.h>
#include <string>
#include <iostream>
#include "MyThread.h"

class ThreadPool
{
public:
	ThreadPool(int ThreadNumber);
	~ThreadPool();
	void createNewTask(LPTHREAD_START_ROUTINE lpStartAddress);
	int threadNumber;
	int getFreeThread();
	void checkThread(MyThread *thread);
private:
	MyThread *threadArr;
	HANDLE *hThreadArr = new HANDLE [3];
	SYSTEMTIME sysTime;
	int freeThreadNumber;
	char *fileName = "log.dat";
	FILE *fileHandler;
};

