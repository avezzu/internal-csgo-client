#pragma once
#include <Windows.h>
#include "centity.h"
#include "datatable.h"


class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual void* GetClientNetworkable(int entnum) = 0;
    virtual void* GetClientNetworkableFromHandle(DWORD hEnt) = 0;
    virtual void* GetClientUnknownFromHandle(DWORD hEnt) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual CEntity* GetClientEntity(int entnum) = 0;
    virtual CEntity* GetClientEntityFromHandle(DWORD hEnt) = 0;

    // Returns number of entities currently in use
    virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns highest index actually used
    virtual int					GetHighestEntityIndex(void) = 0;

    // Sizes entity list to specified size
    virtual void				SetMaxEntities(int maxents) = 0;
    virtual int					GetMaxEntities() = 0;
};


class IBaseClientDLL
{
public:
    virtual int				Connect() = 0;
    virtual void            Disconnect() = 0;
    virtual int				Init() = 0;
    virtual void			PostInit() = 0;
    virtual void			Shutdown(void) = 0;
    virtual void			LevelInitPreEntity(char const* pMapName) = 0;
    virtual void			LevelInitPostEntity() = 0;
    virtual void			LevelShutdown(void) = 0;


    virtual ClientClass* GetAllClasses(void) = 0;
};
