#include "module.h"
#include "../pointer/pointer.h"
#include "../utils/fnv.h"


CModule::CModule(const char* name) {
    m_Name = name;
    m_Handle = nullptr;
    m_Begin = m_Size = 0;
}

CModule::~CModule() {
    if (!m_Handle) {
        return;
    }
}

bool CModule::Retrieve() {
    if (m_Handle) {
        return true;
    }

    InitializeHandle();
    InitializeBounds();

    return m_Handle != nullptr;
}

uintptr_t CModule::GetInterface(const char* version) {
    uintptr_t rv = 0;
    if (m_Handle) {
        CPointer pCreateInterface = GetProcAddress("CreateInterface");
        if (!pCreateInterface.IsValid()) {
            return rv;
        }
    
        using FnType = uintptr_t (*)(const char*, int*);
        auto CreateInterface = (FnType)pCreateInterface.Get<FARPROC>();

        return CreateInterface(version, nullptr);
    }

    return rv;
}

uintptr_t CModule::GetProcAddress(const char* procName) {
    uintptr_t rv = 0;
    if (m_Handle) {
        rv = reinterpret_cast<uintptr_t>(::GetProcAddress(static_cast<HMODULE>(m_Handle), procName));
    }
    return rv;
}

uintptr_t CModule::FindPattern(const std::span<const int>& pattern) const {
    uintptr_t rv = 0;
    if (m_Handle) {
        uint8_t* bytes = reinterpret_cast<uint8_t*>(m_Begin);

        const int* patternData = pattern.data();
        const size_t patternSize = pattern.size();

        for (size_t i = 0; i < m_Size - patternSize; ++i) {
            bool found = true;
            for (size_t j = 0; j < patternSize; ++j) {
                if (bytes[i + j] != patternData[j] && patternData[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found) {
                rv = reinterpret_cast<uintptr_t>(&bytes[i]);
                break;
            }
        }
    }

    return rv;
}

void CModule::InitializeHandle() {
    m_Handle = GetModuleHandle(GetName());
}

void CModule::InitializeBounds() {
    if (!m_Handle) {
        return;
    }

    const PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(m_Handle);
    const PIMAGE_NT_HEADERS ntHeaders =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(m_Handle) + dosHeader->e_lfanew);

    SetBounds(reinterpret_cast<uintptr_t>(m_Handle), ntHeaders->OptionalHeader.SizeOfImage);
}

void CModule::SetBounds(uintptr_t begin, uintptr_t size) {
    m_Begin = begin;
    m_Size = size;
}