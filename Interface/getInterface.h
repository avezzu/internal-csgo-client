#pragma once
#include <Windows.h>

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
