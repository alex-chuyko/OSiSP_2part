#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include "ThreadPool.h"

void testMethod2()
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	printf("%02d:%02d:%02d test method run\n", s.wHour, s.wMinute, s.wSecond);
	Sleep(3000);
	GetLocalTime(&s);
	printf("%02d:%02d:%02d test method end\n", s.wHour, s.wMinute, s.wSecond);
}

void testMethod()
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	printf("%02d:%02d:%02d test method run\n", s.wHour, s.wMinute, s.wSecond);
	int testVar1 = 234;
	int testVar2 = 94;
	int result = testVar1 + testVar2;
	GetLocalTime(&s);
	printf("%02d:%02d:%02d test method end\n", s.wHour, s.wMinute, s.wSecond);
}

int main()	
{
	ThreadPool *tp;
	tp = new ThreadPool(2);
	tp->createNewTask((LPTHREAD_START_ROUTINE)testMethod);
	tp->createNewTask((LPTHREAD_START_ROUTINE)testMethod2);
	tp->createNewTask((LPTHREAD_START_ROUTINE)testMethod);
	tp->createNewTask((LPTHREAD_START_ROUTINE)testMethod2);
	tp->createNewTask((LPTHREAD_START_ROUTINE)testMethod2);
	tp->createNewTask((LPTHREAD_START_ROUTINE)testMethod2);
	Sleep(5000);
	tp->createNewTask((LPTHREAD_START_ROUTINE)testMethod);
	tp->~ThreadPool();
	return 0;
}