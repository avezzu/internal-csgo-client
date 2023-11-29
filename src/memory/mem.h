#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "../module/module.h"
#include "../pointer/pointer.h"


class CSigScan;
using ModulePtr_t = std::unique_ptr<CModule>;

namespace mem
{
    
    void Initialize();
    void ScheduleScan(CSigScan* sigScan);

    ModulePtr_t& GetModule(const char* moduleName);
    CPointer GetInterface(const char* moduleName, const char* interfaceName);
    CPointer GetProcAddress(const char* moduleName, const char* procName);
    
    extern std::unordered_map<uint32_t, ModulePtr_t> m_CachedModules;
    extern std::vector<CSigScan*> m_ScheduledScans; 
    
}