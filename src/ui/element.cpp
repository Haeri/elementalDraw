#include "elemd/ui/element.hpp"

#include <algorithm>

#include <elemd/ui/document.hpp>

namespace elemd
{

    void Element::paint(elemd::Context* ctx)
    {
        elemd::color bg = style.background_color.is_set ? style.background_color.value
                                                        : elemd::color(255, 255, 255, 0);
        if (hover_style.background_color.is_set &&
            (_state == HOVER || _state == INITIAL_HOVER || _state == HOVER_INITIAL) &&
            style.background_color.value != hover_style.background_color.value)
        {
            float percent = _transition_progress / style.transition_time;
            percent = std::min(std::max(percent, 0.0f), 1.0f);

            if (_state == INITIAL_HOVER)
            {
                bg = elemd::color::color_lerp(style.background_color.value,
                                              hover_style.background_color.value, percent);
                _document->request_high_frequency();
            }
            else if (_state == HOVER_INITIAL)
            {
                bg = elemd::color::color_lerp(hover_style.background_color.value,
                                              style.background_color.value, percent);
                _document->request_high_frequency();
            }
            else if (_state == HOVER)
            {
                bg = hover_style.background_color.value;
            }

            _transition_progress += _document->delta_time;
        }

        if (_transition_progress >= style.transition_time)
        {
            if (_state == INITIAL_HOVER)
                _state = HOVER;
            else if (_state == HOVER_INITIAL)
                _state = INITIAL;
        }

        // Background image
        if (style.background_image.is_set)
        {
            ctx->draw_rounded_image(
                _position.get_x() + style.margin[3], _position.get_y() + style.margin[0],
                _width - (style.margin[3] + style.margin[1]),
                _height - (style.margin[0] + style.margin[2]), style.background_image.value,
                style.border_radius[0], style.border_radius[1], style.border_radius[2],
                style.border_radius[3]);
        }

        // Background Color
        if (bg.a() > 0.0f)
        {
            ctx->set_fill_color(bg);
            ctx->fill_rounded_rect(
                _position.get_x() + style.margin[3], _position.get_y() + style.margin[0],
                _width - (style.margin[3] + style.margin[1]),
                _height - (style.margin[0] + style.margin[2]), style.border_radius[0],
                style.border_radius[1], style.border_radius[2], style.border_radius[3]);
        }

        // Border
        if (style.border_color.a() > 0.0f && style.border_width[0] > 0.0f)
        {
            ctx->set_stroke_color(style.border_color);
            ctx->set_line_width(style.border_width[0]);
            ctx->stroke_rounded_rect(_position.get_x() + style.margin[3] + style.padding[3],
                                     _position.get_y() + style.margin[0] + style.padding[0],
                                     _width - (style.margin[3] + style.margin[1]) -
                                         (style.padding[3] + style.padding[1]),
                                     _height - (style.margin[0] + style.margin[2]) -
                                         (style.padding[0] + style.padding[2]),
                                     style.border_radius[0], style.border_radius[1],
                                     style.border_radius[2], style.border_radius[3]);
        }

        if (_state == HOVER || _state == INITIAL_HOVER)
            debug_paint(ctx);

        if (_scrollable_x || _scrollable_y)
        {
            ctx->set_rect_mask(_position.get_x() + style.margin[3],
                               _position.get_y() + style.margin[0],
                               _width - (style.margin[3] + style.margin[1]),
                               _height - (style.margin[0] + style.margin[2]));
        }
        for (Node* el : _children)
        {
            el->paint(ctx);
        }

        if (_scrollable_x || _scrollable_y)
        {
            ctx->remove_rect_mask();
        }

        // Scrollbars
        if ((_scrollable_y || _scrollable_x) &&
            (_state == HOVER || _state == INITIAL_HOVER || _state == HOVER_INITIAL))
        {
            float percent = _transition_progress / style.transition_time;
            percent = std::min(std::max(percent, 0.0f), 1.0f);

            elemd::color handle_color = style.scroll_bar_color;
            elemd::color handle_color_0 =
                elemd::color(style.scroll_bar_color.r(), style.scroll_bar_color.g(),
                             style.scroll_bar_color.b(), 0);

            if (_state == INITIAL_HOVER)
            {
                handle_color = elemd::color::color_lerp(handle_color_0, handle_color, percent);
                _document->request_high_frequency();
            }
            else if (_state == HOVER_INITIAL)
            {
                handle_color = elemd::color::color_lerp(handle_color, handle_color_0, percent);
                _document->request_high_frequency();
            }

            ctx->set_fill_color(handle_color);

            if (_scrollable_y)
            {
                float bar_length = _height * ((_height - (style.margin[0] + style.padding[0] +
                                                          style.margin[2] + style.padding[2])) /
                                              _min_dims.get_y());
                bar_length = std::max(bar_length, 10.0f);
                ctx->fill_rounded_rect(_position.get_x() + _width - 6,
                                       _position.get_y() + 1 +
                                           ((_height - (bar_length + 2)) * _scroll_percent.get_y()),
                                       5, bar_length, 2);
            }
            if (_scrollable_x)
            {
                float bar_length = _width * ((_width - (style.margin[1] + style.padding[1] +
                                                        style.margin[3] + style.padding[3])) /
                                             _min_dims.get_x());
                bar_length = std::max(bar_length, 10.0f);
                ctx->fill_rounded_rect(_position.get_x() + 1 +
                                           ((_width - (bar_length + 2)) * _scroll_percent.get_x()),
                                       _position.get_y() + _height - 6, bar_length, 5, 2);
            }
            _transition_progress += _document->delta_time;
        }
    }
} // namespace elemd