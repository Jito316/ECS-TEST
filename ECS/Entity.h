#pragma once
#include <iostream>
#include <vector>
#include <bitset>

#define MAXCOMPONENTS 100
#define MAXENTITIES 1000

using ComponentMask = std::bitset<MAXCOMPONENTS>;
using Entity = std::uint32_t;	// インデックス
