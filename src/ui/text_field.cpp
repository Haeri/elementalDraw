#include "elemd/ui/text_field.hpp"

#include <algorithm>
#include <math.h>

#include <elemd/ui/document.hpp>

namespace elemd
{
    TextField::TextField()
    {
    }
    void TextField::emit_key_event(elemd::key_event event)
    {
        Node::emit_key_event(event);

        if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
        {
            switch (event.key)
            {

            case elemd::keyboard_key::KEY_LEFT:
                if (_cursor_pos > 0)
                {
                    --_cursor_pos;
                }
                break;
            case elemd::keyboard_key::KEY_RIGHT:
                if (_cursor_pos <= _content.length())
                {
                    ++_cursor_pos;
                }
                break;
            case elemd::keyboard_key::KEY_BACKSPACE:
                if (_cursor_pos > 0)
                {
                    _content.erase(_cursor_pos - 1, 1);
                    --_cursor_pos;
                }
                break;
            case elemd::keyboard_key::KEY_DELETE:
                if (_cursor_pos < _content.length())
                {
                    _content.erase(_cursor_pos, 1);
                }
                break;
            case elemd::keyboard_key::KEY_ENTER:
                _content.insert(_cursor_pos, "\n");
                ++_cursor_pos;
                break;
            }
        }
    }

    void TextField::emit_char_event(elemd::char_event event)
    {
        Node::emit_char_event(event);

        _content.insert(_cursor_pos, event.utf8);
        ++_cursor_pos;
    }

    void TextField::set_state(State state)
    {
        Node::set_state(state);
        if (_state == HOVER || _state == INITIAL_HOVER)
        {
            _document->get_window()->set_cursor(elemd::CURSOR_TYPE::IBEAM_CURSOR);
        }
        else
        {
            _document->get_window()->set_cursor(nullptr);
        }
    }

    /*
    float text_field::layout(elemd::vec2 position, float width, float height)
    {
        _position = position;
        _width = width;
        float w_style = (style.margin[4] + style.padding[4]) + (style.margin[1] + style.padding[1]);
        float h_style = (style.margin[0] + style.padding[0]) + (style.margin[2] + style.padding[2]);
        float w_space = width - w_style;


        _height = dim.get_y() + h_style;
        if (style.display == INLINE) {
            _width = dim.get_x() + w_style;
        }

        return _height;
    }*/

    elemd::vec2 TextField::get_minimum_dimensions(float width, float height)
    {
        _formated_content = style.font_family->fit_substring(get_text(), width, style.font_size);
        return style.font_family->measure_dimensions(_formated_content, style.font_size);
    }

    void TextField::paint(elemd::Context* ctx)
    {
        // Background
        if (style.background_color.is_set && style.background_color.value.a() != 0)
        {
            ctx->set_fill_color(style.background_color.value);
            ctx->fill_rounded_rect(
                _position.get_x() + style.margin[3], _position.get_y() + style.margin[0],
                _width - (style.margin[3] + style.margin[1]),
                _height - (style.margin[0] + style.margin[2]), style.border_radius[0],
                style.border_radius[1], style.border_radius[2], style.border_radius[3]);
        }

        if (_focused)
        {
            elemd::vec2 dim = style.font_family->measure_dimensions(_content.substr(0, _cursor_pos),
                                                                    style.font_size);

            _blink_timer += _document->delta_time;
            float alpha = (sin(_blink_timer * 10) + 1) / 2.0f;

            elemd::color col(style.color.rf(), style.color.gf(), style.color.bf(), alpha);

            ctx->set_fill_color(col);
            ctx->fill_rect(_position.get_x() + style.margin[3] + style.padding[3] + dim.x(),
                           _position.get_y() + style.margin[0] + style.padding[0], 1,
                           style.font_size);

            _document->request_high_frequency();
        }

        ctx->set_font(style.font_family);
        ctx->set_font_size(style.font_size);
        ctx->set_fill_color(style.color);
        ctx->draw_text(_position.get_x() + style.margin[3] + style.padding[3],
                       _position.get_y() + style.margin[0] + style.padding[0], _formated_content);

        if (_state == HOVER || _state == HOVER_INITIAL)
            debug_paint(ctx);
    }

    void TextField::set_text(std::string text)
    {
        _content = text;
        _cursor_pos =
            std::max(std::min(_cursor_pos, (unsigned int)_content.size()), (unsigned int)0);
    }

    std::string TextField::get_text()
    {
        return _content;
    }
} // namespace elemd