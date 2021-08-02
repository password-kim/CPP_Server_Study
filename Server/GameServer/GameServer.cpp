#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>

vector<int32> v;

// Mutual Exclusive (상호배타적)
mutex m; // 자물쇠 역할

// RAII (Resource Acquisition Is Initialization)
template<typename T> 
class LockGuard // 표준->lock_guard
{
public:
    LockGuard(T& m)
    {
        _mutex = &m;
        _mutex->lock();
    }

    ~LockGuard()
    {
        _mutex->unlock();
    }

public:
    T* _mutex;
};

void Push()
{
    for (int32 i = 0; i < 10000; i++)
    {
        // 자물쇠를 자동으로 잠그고 풀어주는것
        //LockGuard<mutex> lockGuard(m); 
        lock_guard<mutex> lockGuard(m);

        
        //m.lock(); // 자물쇠 잠그기

        v.push_back(i);

        if (i == 5000)
            break;


        //m.unlock(); // 자물쇠 풀기

    }
}

int main()
{
    thread t1(Push);
    thread t2(Push);

    t1.join();
    t2.join();

    cout << v.size() << endl;

    return 0;
}

