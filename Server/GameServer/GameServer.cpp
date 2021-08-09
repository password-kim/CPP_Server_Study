#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>

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

int32 sum = 0;

mutex m;

SpinLock spinLock;

void Add()
{
    for (int32 i = 0; i < 100000; i++)
    {
        lock_guard<SpinLock> gaurd(spinLock);
        sum++;
    }
}

void Sub()
{
    for (int32 i = 0; i < 100000; i++)
    {
        lock_guard<SpinLock> gaurd(spinLock);
        sum--;
    }
}

int main()
{
    thread t1(Add);
    thread t2(Sub);

    t1.join();
    t2.join();

    cout << sum << endl;
}

