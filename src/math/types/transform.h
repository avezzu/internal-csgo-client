#pragma once
#include "quaternion.h"
#include "vector.h"

class alignas(16) CTransform {
   public:
    alignas(16) Vector m_Position;
    alignas(16) Quaternion m_Orientation;

    matrix3x4_t ToMatrix() const;
};
