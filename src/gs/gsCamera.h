#pragma once

#include "gsInputState.h"
#include "gsMatrixStack.h"
#include "gsVec3.h"
#include "../matrix/Matrices.h"

namespace gs
{
    class Camera
    {
        private:
            constexpr static float maxZoom = 2.0f;
            constexpr static float minZoom = 0.1f;

            float       aspectRatio;
            float       latitude;
            float       longitude;
            float       zoom;     //between 0.0 and 1.0
            gs::Vec3d   position;

            Matrix4     viewMatrix;
            Matrix4     projectionMatrix;

        private:
            void NormalizeLatitude();
            void NormalizeLongitude();

        public:
            //void ApplyTransformation(gs::MatrixStack<Matrix4>& matrix) const;
            Matrix4 GetViewMatrix() const;
            Matrix4 GetProjectionMatrix() const;
            void    Move(const float latitudeChange, const float longitudeChange);
            void    SetOrthographic(const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal);
            void    Update(InputState& input);
            void    UpdateViewMatrix();

        public:
            explicit Camera(const float aspectRatio);
            //Camera(const float aspectRatio, const double latitude, const double longitude); //TODO: implement
    };
}