#ifndef ELEM_ELEMENT_H
#define ELEM_ELEMENT_H

#include <elemd/elemental_draw.hpp>
#include <limits>
#include <vector>

#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/vec2.hpp>

#include "elemd/ui/node.hpp"

namespace elemd
{
    class ELEMD_API Element : public Node
    {
    public:
        // float layout(elemd::vec2 position, float width, float height) override;
        void paint(elemd::Context* ctx) override;
    };
} // namespace elemd

#endif // ELEM_ELEMENT_H