#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <array>

#include <elemd/vec2.hpp>

struct rect
{
    elemd::vec2 pos = {};
    elemd::vec2 size = {};
    elemd::vec2 vel = {};

    std::array<rect*, 4> contact = {};
};

namespace physics
{
    static bool aabb(const rect& a, const rect& b);

    static bool point_to_rect(const elemd::vec2& p, const rect* r);

    static bool rect_to_rect(const rect* r1, const rect* r2);

    static bool ray_to_rect(const elemd::vec2& ray_origin, const elemd::vec2& ray_dir,
                            const rect* target, elemd::vec2& contact_point,
                            elemd::vec2& contact_normal, float& t_hit_near);

    bool dynamic_rect_to_rect(const rect* r_dynamic, const float fTimeStep, const rect& r_static,
                              elemd::vec2& contact_point, elemd::vec2& contact_normal,
                              float& contact_time);

    bool resolve_dynamic_rect_to_rect(rect* r_dynamic, const float fTimeStep, rect* r_static);

} // namespace physics

#endif // PHYSICS_HPP