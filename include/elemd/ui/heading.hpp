#ifndef ELEMD_HEADING_H
#define ELEMD_HEADING_H

#include <elemd/elemental_draw.hpp>
#include <limits>
#include <vector>

#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/vec2.hpp>

#include "elemd/ui/node.hpp"

namespace elemd
{
    class ELEMD_API Heading : public Node
    {
    public:
        Heading();

        void add_child(Node* child) = delete;

        // float layout(elemd::vec2 position, float width, float height) override;
        elemd::vec2 get_minimum_dimensions(float width, float height) override;
        void paint(elemd::Context* ctx) override;

        void set_text(std::string text);
        std::string get_text();

    private:
        std::string _content;
        std::string _formated_content;
    };
} // namespace elemd

#endif // ELEMD_HEADING_H