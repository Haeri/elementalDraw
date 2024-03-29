#ifndef ELEMD_TEXT_FIELD_HPP
#define ELEMD_TEXT_FIELD_HPP

#include <limits>
#include <vector>

#include "elemd/color.hpp"
#include "elemd/context.hpp"
#include "elemd/elemental_draw.hpp"
#include "elemd/ui/node.hpp"
#include "elemd/vec2.hpp"

namespace elemd
{
    class ELEMD_API TextField : public Node
    {
    public:
        TextField();

        void add_child(Node* child) = delete;

        void emit_key_event(elemd::key_event event) override;
        void emit_char_event(elemd::char_event event) override;
        void set_state(State state) override;
        // float layout(elemd::vec2 position, float width, float height) override;
        elemd::vec2 get_minimum_dimensions(elemd::Context* ctx, float width, float height) override;
        void paint(elemd::Context* ctx) override;

        void set_text(std::string text);
        void set_text(std::u32string text);
        std::u32string get_text();

    private:
        std::u32string _content;
        std::u32string _formated_content;

        unsigned int _cursor_pos = 0;
        float _blink_timer = 0;
    };
} // namespace elemd

#endif // ELEMD_TEXT_FIELD_HPP