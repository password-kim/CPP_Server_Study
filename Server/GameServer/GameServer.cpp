#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>

// atomic : All or Nothing
atomic<int32> sum = 0; // 공유데이터를 atomic으로 만든다.

void Add()
{
    for (int32 i = 0; i < 1000000; ++i)
    {
        sum.fetch_add(1);
    }
}

void Sub()
{
    for (int32 i = 0; i < 1000000; ++i)
    {
        sum.fetch_add(-1);
    }
}

int main()
{
    Add();
    Sub();
    cout << sum << endl; // 1번 결과

    thread t1(Add);
    thread t2(Sub);
    t1.join();
    t2.join();
    cout << sum << endl; // 2번 결과

    // 1번 결과와 2번 결과가 같다면 atomic이 제대로 이뤄진것.
    /* 
    atomic이 제대로 이뤄지지 않으면 
    공유데이터를 두 쓰레드에서 꼬이게 사용되므로 
    제대로된 결과값이 안나옴.
    하지만 atomic은 동작이 매우 느리기 때문에
    모든 데이터를 atomic화 하는건 매우 비효율적.
    */

    return 0;
}

