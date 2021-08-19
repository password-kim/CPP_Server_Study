#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <Windows.h>
#include <future>

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100000; i++)
	{
		sum += i;
	}

	return sum;
}

class Knight
{
public:
	int64 GetHP() { return 100; }
};

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Promise Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}


int main()
{
	// 동기(Synchronous) 실행
	// int64 sum = Calculate();
	// cout << sum << endl;

	// 비동기(Non-Synchronous) 실행
	// std::future
	{
		/// <summary>
		/// 1) deferred -> lazy evaluation 지연해서 실행.
		/// 2) async -> 별도의 thread를 만들어서 실행.
		/// 3) deferred | async -> 둘 중 알아서 실행.
		/// </summary>
		/// <returns></returns>
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// TODO
		// std::future_status status = future.wait_for(100ms);

		

		int64 sum = future.get(); // 결과물이 이제서야 필요하다.
	}


	// 멤버함수도 활용가능
	{
		Knight knight;

		std::future<int64> future = std::async(std::launch::async, &Knight::GetHP, knight);

	}


	// std::promise
	{
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}


	// std::pakaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	// 결론)
	// mutex, condition_variable를 쓰지 않고 단순한 것들을 처리할 수 있는 방법.
	// 특히나, 한 번 발생하는 이벤트에 유용하다!
	
	// 1) async
	// -> 원하는 함수를 비동기적으로 실행.
	// 2) promise
	// -> 결과물을 promise를 통해 future로 받아줌.
	// 3) packaged_task
	// -> 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌.

}

