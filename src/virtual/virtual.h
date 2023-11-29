#pragma once
#include "../pointer/pointer.h"

namespace vt
{
    template <typename Return, typename... Arguments>
    constexpr Return Call(void *vmt, const std::uint32_t index, Arguments... args) noexcept
    {
        using Function = Return(__thiscall *)(void *, decltype(args)...);
        return (*static_cast<Function **>(vmt))[index](vmt, args...);
    }
}