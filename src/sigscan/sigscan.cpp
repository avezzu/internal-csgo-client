#include <iostream>
#include "sigscan.h"
#include "../memory/mem.h"

CSigScan::CSigScan(const char* name, const char* libraryName, const std::initializer_list<SigData_t>& data) {

    m_Name = name;
    m_LibraryName = libraryName;

    m_Data.reserve(data.size());
    m_Data.insert(m_Data.end(), data.begin(), data.end());

    mem::ScheduleScan(this);
}

void CSigScan::FindSignature() {
    auto& library = mem::GetModule(m_LibraryName);
  
    for (size_t i = 0; i < m_Data.size(); ++i) {
        const auto& data = m_Data[i];

        m_Value = library->FindPattern(data.m_Signature);
        if (m_Value.IsValid()) {
            if (data.m_Procedure) {
                data.m_Procedure(m_Value);
            }
            std::cout << "[signature] {" << m_Name << "}: " << std::hex << m_Value.Get<void*>() << std::endl;
            return;
        }

        std::cout << "[signature] {" << m_Name << "} was not found" << std::endl;
    }
}