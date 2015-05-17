#include "voronoi_cell.h"

#include "../glm/gtc/matrix_transform.hpp"

VoronoiCell::VoronoiCell(mpvec3 p)
{
    position = p;
}

void VoronoiCell::addCorner(glm::dvec3 c)
{
    corners.push_back(glm::normalize(c));
}

void VoronoiCell::sortCorners()
{
    corners = quicksort(corners);
}

std::vector<glm::dvec3> VoronoiCell::quicksort(std::vector<glm::dvec3> points)
{
    if (points.size() <= 1)
        return points;

    glm::dvec3 pivot = points[0];
    std::vector<glm::dvec3> prev;
    std::vector<glm::dvec3> next;

    glm::dvec3 pos = position.to_dvec3();

    glm::dvec3 pivnorm = glm::normalize(pivot - pos);
    for (unsigned int i = 1; i < points.size(); i++)
    {
        glm::dvec3 pnorm = glm::normalize(points[i] - pos);
        float side = glm::dot(glm::cross(pivnorm,pnorm), pos);

        if (side < 0)
            prev.push_back(points[i]);
        else
            next.push_back(points[i]);
    }

    prev = quicksort(prev);
    next = quicksort(next);

    prev.push_back(pivot);
    prev.insert(prev.end(),next.begin(),next.end());

    return prev;
}

void VoronoiCell::computeCentroid()
{
    // construct transformation and inverse
    glm::dmat4x4 transform = glm::lookAt(glm::dvec3(0.0),
                                         position.to_dvec3(),
                                         glm::dvec3(0.0,1.0,0.0));
    glm::dmat4x4 inverse = glm::inverse(transform);

    long double cx, cy, cz, area;
    cx = cy = area = 0.0;

    glm::dvec4 pc4 = glm::dvec4(corners[corners.size()-1], 1.0);

    glm::dvec4 prev_t_corner = transform * pc4;
    cz = prev_t_corner.z;

    for (auto it = corners.begin(); it != corners.end(); it++)
    {
        glm::dvec3 corner = *it;
        glm::dvec4 c4 = glm::dvec4(corner, 1.0);
        glm::dvec4 t_corner = transform * c4;

        long double a = (prev_t_corner.x * t_corner.y) 
                 - (t_corner.x * prev_t_corner.y);
        cx += (prev_t_corner.x + t_corner.x) * a;
        cy += (prev_t_corner.y + t_corner.y) * a;
        area += a;

        prev_t_corner = t_corner;
    }

    long double f = 1.0/(3.0 * area);
    centroid = glm::vec3(inverse * glm::dvec4(f*cx, f*cy, cz, 1.0));
}