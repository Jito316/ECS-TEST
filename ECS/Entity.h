#pragma once
#include <iostream>
#include <vector>
#include <bitset>

#define MaxComponents 100

using ComponentMask = std::bitset<MaxComponents>;
using Entity = std::uint32_t;	// インデックス
