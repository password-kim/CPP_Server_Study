﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <Windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"

using TL = TypeList<class Player, class Knight, class Mage, class Archer>;

class Player
{
public:
	Player()
	{
		INIT_TL(Player);
	}
	virtual ~Player() {}

	DECLARE_TL
};

class Knight : public Player
{
public:
	Knight() { INIT_TL(Knight); }
};

class Mage : public Player
{

public:
	Mage() { INIT_TL(Mage); }
};

class Archer : public Player
{

public:
	Archer() { INIT_TL(Archer) }
};

class Dog
{

};

int main()
{
	/*----------------------------------------------------------------
	TypeList<Mage, Knight>::Head whoAMI;
	TypeList<Mage, Knight>::Tail whoAMI2;

	TypeList<Mage, TypeList<Knight, Archer>>::Head whoAMI3;
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4;
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5;

	int32 len1 = Length<TypeList<Mage, Knight>>::value;
	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;


	TypeAt<TL, 0>::Result whoAMI6;
	TypeAt<TL, 1>::Result whoAMI7;
	TypeAt<TL, 2>::Result whoAMI8;

	int32 index1 = IndexOf<TL, Player>::value;
	int32 index2 = IndexOf<TL, Archer>::value;
	int32 index3 = IndexOf<TL, Dog>::value;

	bool canConvert1 = Conversion<Player, Knight>::exists;
	bool canConvert2 = Conversion<Knight, Player>::exists;
	bool canConvert3 = Conversion<Knight, Dog>::exists;
	-------------------------------------------------------------------*/

	{
		Player* player = new Player();

		bool canCast = CanCast<Knight*>(player);
		Knight* knight = TypeCast<Knight*>(player);


		delete player;
	}

	{
		shared_ptr<Player> player = MakeShared<Knight>();

		shared_ptr<Archer> archer = TypeCast<Archer>(player);
		bool canCast = CanCast<Mage>(player);

	}
}

