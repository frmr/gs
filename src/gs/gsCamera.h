#ifndef GS_CAMERA_H
#define GS_CAMERA_H

#include "gsMatrixStack.h"

namespace gs
{
    class Camera
    {
        private:
            constexpr static float maxZoomDistance = 10.0f;
            constexpr static float minZoomDistance = 2.0f;

            float latitude;
            float longitude;
            float zoom;     //between 0.0 and 1.0
            float x, y, z;



        private:
            void NormalizeLatitude();
            void NormalizeLongitude();

        public:
            void ApplyTransformation( gs::MatrixStack& matrix ) const;
            void Move( const double latitudeChange, const double longitudeChange );

        public:
            Camera();
            Camera( const double latitude, const double longitude );
    };
}

#endif // GS_CAMERA_H
