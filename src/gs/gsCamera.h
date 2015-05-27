#ifndef GS_CAMERA_H
#define GS_CAMERA_H

#include "gsInputState.h"
#include "gsMatrixStack.h"
#include "gsVec3.h"
#include "../matrix/Matrices.h"

namespace gs
{
    class Camera
    {
        private:
            constexpr static float maxZoomDistance = 10.0f;
            constexpr static float minZoomDistance = 2.0f;

            float       latitude;
            float       longitude;
            float       zoom;     //between 0.0 and 1.0
            gs::Vec3f   position;

            Matrix4     viewMatrix;
            Matrix4     projectionMatrix;

        private:
            void NormalizeLatitude();
            void NormalizeLongitude();

        public:
            //void ApplyTransformation( gs::MatrixStack<Matrix4>& matrix ) const;
            Matrix4 GetViewMatrix() const;
            Matrix4 GetProjectionMatrix() const;
            void    Move( const double latitudeChange, const double longitudeChange );
            bool    SetOrthographic( const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal );
            void    Update( const InputState& input );

        public:
            Camera();
            Camera( const double latitude, const double longitude );
    };
}

#endif // GS_CAMERA_H
