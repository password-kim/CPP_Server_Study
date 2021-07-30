#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>

void HelloThread()
{
    cout << "Hello Thread" << endl;
}

void HelloThread_2(int32 num)
{
    cout << num << endl;
}

int main()
{

#pragma region ThreadFunc
    //t.hardware_concurrency(); // CPU 코어 개수
    //t.get_id(); // thread마다 id
    //t.detach(); // std::thread 객체에서 실제 thread를 분리
    //t.joinable(); // thread가 실제 객체를 관리하는 thread인지를 확인
#pragma endregion
    vector<thread> v;

    for (int32 i = 0; i < 10; ++i)
    {
        v.push_back(thread(HelloThread_2, i));
    }

    for (int32 i = 0; i < 10; ++i)
    {
        if (v[i].joinable())
            v[i].join();
    }

    cout << "Hello Main" << endl;


    return 0;
}

