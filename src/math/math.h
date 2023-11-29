#pragma once

#include "types/transform.h"
#include "types/vmatrix.h"

struct ImVec2;

class CMath {
   public:
    static CMath& Get() {
        static CMath inst;
        return inst;
    }

    void UpdateViewMatrix(VMatrix* viewMatrix);

    bool WorldToScreen(const Vector& in, ImVec2& out);
    void TransformAABB(const matrix3x4_t& transform, const Vector& minsIn, const Vector& maxsIn, Vector& minsOut, Vector& maxsOut);

    Vector Origin(const matrix3x4_t &matrix);

   private:
    VMatrix m_ViewMatrix;
};
