#ifndef GS_BOUNDING_BOX_H
#define GS_BOUNDING_BOX_H

namespace gs
{
    template<typename T>
    class BoundingBox
    {
    public:
        T minCoord;
        T maxCoord;

    private:
        static T CalculateMinCoord(vector<T>& points)
        {
            T minCoord = T::Max();
            for (auto& point : points)
            {
                for (int i = 0; i < T::dimensions; ++i)
                {
                    if (point[i] < minCoord[i])
                    {
                        minCoord[i] = point[i];
                    }
                }
            }
            return minCoord;
        }

        static T CalculateMaxCoord(vector<T>& points)
        {
            T maxCoord = T::Min();
            for (auto& point : points)
            {
                for (int i = 0; i < T::dimensions; ++i)
                {
                    if (point[i] > maxCoord[i])
                    {
                        maxCoord[i] = point[i];
                    }
                }
            }
            return maxCoord;
        }

    public:
        BoundingBox(vector<T>& points)
            : minCoord(CalculateMinCoord(points)),
              maxCoord(CalculateMaxCoord(points))
        {
        }
    };

}

#endif // GS_BOUNDING_BOX_H
