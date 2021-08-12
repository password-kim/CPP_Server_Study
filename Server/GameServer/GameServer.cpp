#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <Windows.h>

#pragma region SpinLock

class SpinLock
{
public:
    void lock()
    {
        // CAS (Compare-And-Swap)

        bool expected = false;
        bool desired = true;

#pragma region CAS 의사코드
        /// <summary>
        /// CAS 의사코드
        /// if (_locked == expected)
        /// {
        ///     expected = _locked;
        ///     _locked = desired;
        ///     return true;
        /// }
        /// else
        /// {
        ///     expected = _locked;
        ///     return false;
        /// }
        /// </summary>
#pragma endregion 

        while (_locked.compare_exchange_strong(expected, desired) == false)
        {
            expected = false;
            
            // Sleep! 
            //this_thread::sleep_for(chrono::milliseconds(10));
            this_thread::sleep_for(10ms);
            //this_thread::yield();
        }

    }

    void unlock()
    {
        //_locked = false;
        _locked.store(false);
    }


private:
    atomic<bool> _locked = false;
};

#pragma endregion

mutex m;
queue<int32> q;
HANDLE handle;

// CV는 User-Level-Object (커널 오브젝트X)
condition_variable cv;

void Producer()
{
    while (true)
    {
        // 1) Lock을 잡고
        // 2) 공유 변수 값을 수정
        // 3) Lock을 풀고
        // 4) 조건변수를 통해 다른 쓰레드에게 통지


        {
            unique_lock<mutex> lock(m);
            q.push(100);
        }

        cv.notify_one(); // wait중인 쓰레드가 있으면 딱 1개를 깨운다.

        //::SetEvent(handle);


        //this_thread::sleep_for(100000ms);
    }
}

void Consumer()
{
    while (true)
    {
        unique_lock<mutex> lock(m);
        cv.wait(lock, []() { return q.empty() == false; });
        // 1) Lock을 잡고
        // 2) 조건 확인
        // -만족O => 빠져 나와서 이어서 코드를 진행
        // -만족X => Lock을 풀어주고 대기 상태

        //::WaitForSingleObject(handle, INFINITE);
        // Non-Signal 상태가됨.
        // bManualRset을 True로 하였다면
        // ::ResetEvent(handle)을 이용해 다시 Non-Signal상태로 바꿔줘야함.

        //while (q.empty() == false)
        {
            int32 data = q.front();
            q.pop();
            cout << q.size() << endl;
        }
    }
}

int main()
{
#pragma region Kernel Object
    /// <summary>
    /// 커널 오브젝트
    /// Usage Count
    /// Signal (파란불) / Non-Signal (빨간불) << bool
    /// Auto / Manual << bool
    /// </summary>
#pragma endregion
    handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

    thread t1(Producer);
    thread t2(Consumer);

    t1.join();
    t2.join();

    ::CloseHandle(handle);
}

