#pragma once
#include "../utils/hash.h"
#include "../Interface/datatable.h"

#include <unordered_map>

void SetupNetvars();

void Dump(const char* BaseClass, RecvTable* table, std::uint32_t offset);

inline std::unordered_map<std::uint32_t, std::uint32_t> netvars;

#define NETVAR(func_name, netvar, type)  type& func_name()   \
{														     \
	static auto offset = netvars[fnv::HashConst(netvar)];	 \
	return *(type*)(std::uint32_t(this) + offset);			 \
}
