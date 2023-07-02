#pragma once
#include <Windows.h>
#include <cstdint>
#include <functional>

template<typename T>
T* GetInterface(const char* name, const char* lib)
{
    const auto handle = GetModuleHandle(lib);

    if (!handle)
        return nullptr;

    const auto functionAddress = GetProcAddress(handle, "CreateInterface");

    if (!functionAddress)
        return nullptr;

    using FnType = T * (*)(const char*, int*);
    auto CreateInterface = (FnType)functionAddress;

    return CreateInterface(name, nullptr);
}


template <typename Return, typename ... Arguments>
constexpr Return Call(void* vmt, const std::uint32_t index, Arguments ... args) noexcept
{
    using Function = Return(__thiscall*)(void*, decltype(args)...);
    return (*static_cast<Function**>(vmt))[index](vmt, args...);
}


namespace scanner {
    uintptr_t* PatternScan(const char* moduleName, const char* pattern);
}
