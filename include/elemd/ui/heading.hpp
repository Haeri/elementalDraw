#ifndef ELEMD_HEADING_HPP
#define ELEMD_HEADING_HPP

#include <limits>
#include <vector>

#include "elemd/color.hpp"
#include "elemd/context.hpp"
#include "elemd/elemental_draw.hpp"
#include "elemd/ui/node.hpp"
#include "elemd/vec2.hpp"

namespace elemd
{
    class ELEMD_API Heading : public Node
    {
    public:
        Heading();

        void add_child(Node* child) = delete;

        // float layout(elemd::vec2 position, float width, float height) override;
        elemd::vec2 get_minimum_dimensions(elemd::Context* ctx, float width, float height) override;
        void paint(elemd::Context* ctx) override;

        void set_text(std::string text);
        void set_text(std::u32string text);
        std::u32string get_text();

    private:
        std::u32string _content;
        std::u32string _formated_content;
    };
} // namespace elemd

#endif // ELEMD_HEADING_HPP