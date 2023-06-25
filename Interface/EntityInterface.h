#pragma once
#include <Windows.h>
#include "../csgo.hpp"

class Entity
{
public:

    void AntiFlash()
    {
        int* flash_addr = (int*)(uintptr_t(this) + hazedumper::netvars::m_flFlashDuration);
        if (flash_addr)
        {
            *flash_addr = 0;
        }
    }

    const int& GetHealth()
    {
        return *(int*)(uintptr_t(this) + hazedumper::netvars::m_iHealth);
    }
};


class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual void* GetClientNetworkable(int entnum) = 0;
    virtual void* GetClientNetworkableFromHandle(DWORD hEnt) = 0;
    virtual void* GetClientUnknownFromHandle(DWORD hEnt) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual Entity* GetClientEntity(int entnum) = 0;
    virtual Entity* GetClientEntityFromHandle(DWORD hEnt) = 0;

    // Returns number of entities currently in use
    virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns highest index actually used
    virtual int					GetHighestEntityIndex(void) = 0;

    // Sizes entity list to specified size
    virtual void				SetMaxEntities(int maxents) = 0;
    virtual int					GetMaxEntities() = 0;
};
