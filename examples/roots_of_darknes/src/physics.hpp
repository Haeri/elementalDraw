#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <elemd/vec2.hpp>

#include <algorithm>
#include <cmath>

struct rect
{
    elemd::vec2 pos;
    elemd::vec2 size;
};

namespace physics
{
    static bool point_to_rect(const elemd::vec2& p, const rect* r)
    {
        return (p.get_x() >= r->pos.get_x() && p.get_y() >= r->pos.get_y() &&
                p.get_x() < r->pos.get_x() + r->size.get_x() &&
                p.get_y() < r->pos.get_y() + r->size.get_y());
    }

    static bool rect_to_rect(const rect* r1, const rect* r2)
    {
        return (r1->pos.get_x() < r2->pos.get_x() + r2->size.get_x() &&
                r1->pos.get_x() + r1->size.get_x() > r2->pos.get_x() &&
                r1->pos.get_y() < r2->pos.get_y() + r2->size.get_y() &&
                r1->pos.get_y() + r1->size.get_y() > r2->pos.get_y());
    }

    static bool ray_to_rect(const elemd::vec2& ray_origin, const elemd::vec2& ray_dir,
                            const rect* target, elemd::vec2& contact_point,
                            elemd::vec2& contact_normal, float& t_hit_near)
    {
        contact_normal = {0, 0};
        contact_point = {0, 0};

        // Cache division
        elemd::vec2 invdir = 1.0f / ray_dir;

        // Calculate intersections with rectangle bounding axes
        elemd::vec2 t_near = (target->pos - ray_origin) * invdir;
        elemd::vec2 t_far = (target->pos + target->size - ray_origin) * invdir;

        if (std::isnan(t_far.get_y()) || std::isnan(t_far.get_x()))
            return false;
        if (std::isnan(t_near.get_y()) || std::isnan(t_near.get_x()))
            return false;

        // Sort distances
        if (t_near.get_x() > t_far.get_x())
            std::swap(t_near.x(), t_far.x());
        if (t_near.get_y() > t_far.get_y())
            std::swap(t_near.y(), t_far.y());

        // Early rejection
        if (t_near.get_x() > t_far.get_y() || t_near.get_y() > t_far.get_x())
            return false;

        // Closest 'time' will be the first contact
        t_hit_near = std::max(t_near.get_x(), t_near.get_y());

        // Furthest 'time' is contact on opposite side of target
        float t_hit_far = std::min(t_far.get_x(), t_far.get_y());

        // Reject if ray direction is pointing away from object
        if (t_hit_far < 0)
            return false;

        // Contact point of collision from parametric line equation
        contact_point = ray_origin + t_hit_near * ray_dir;

        if (t_near.get_x() > t_near.get_y())
            if (invdir.get_x() < 0)
                contact_normal = {1, 0};
            else
                contact_normal = {-1, 0};
        else if (t_near.get_x() < t_near.get_y())
            if (invdir.get_y() < 0)
                contact_normal = {0, 1};
            else
                contact_normal = {0, -1};

        // Note if t_near == t_far, collision is principly in a diagonal
        // so pointless to resolve. By returning a CN={0,0} even though its
        // considered a hit, the resolver wont change anything.
        return true;
    }

} // namespace physics

#endif // PHYSICS_HPP