#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <Windows.h>
#include <future>

// LTS (Local Thread Storage)
// => thread마다 갖고 있는 각각의 storage.

thread_local int32 LThreadID = 0; // 각각의 thread가 데이터를 가져가서 사용가능.

void ThreadMain(int32 threadid)
{
	LThreadID = threadid;

	while (true)
	{
		cout << "Hi! I'm Thread " << LThreadID << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; i++)
	{
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain, threadId));
	}
	
	for (thread& T : threads)
	{
		T.join();
	}

}

