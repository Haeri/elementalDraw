#include "elemd/ui/node.hpp"

#include <algorithm>
#include <iostream>
#include <map>

#include "elemd/ui/document.hpp"

namespace elemd
{
    std::map<Node*, int> Node::_hover_map;

    void Node::add_child(Node* child)
    {
        _children.push_back(child);
        child->set_parent(this);
        set_document(_document);
    }

    void Node::remove_child(int index)
    {
        Node* elem = _children[index];
        remove_child(elem);
    }

    void Node::remove_child(Node* child)
    {
        _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
        child->set_parent(nullptr);
    }

    void Node::set_state(State state)
    {
        _transition_progress = 0;

        if (style.transition_time <= 0)
        {
            if (state == HOVER_INITIAL)
                state = INITIAL;
            else if (state == INITIAL_HOVER)
                state = HOVER;
        }
        else
        {
        }
        _state = state;
    }

    void Node::set_focus(bool focus)
    {
        _focused = focus;
    }

    void Node::set_document(Document* doc)
    {
        _document = doc;
        for (auto& child : _children)
        {
            child->set_document(doc);
        }
    }

    void Node::add_click_listener(std::function<bool(node_click_event)> callback)
    {
        _click_event_callbacks.push_back(callback);
    }

    void Node::add_scroll_listener(std::function<bool(node_scroll_event)> callback)
    {
        _scroll_event_callbacks.push_back(callback);
    }

    int Node::get_width()
    {
        return _width;
    }

    int Node::get_height()
    {
        return _height;
    }

    elemd::vec2 Node::get_position()
    {
        return _position;
    }

    void Node::add_to_hover_list(Node* node)
    {
        auto find = _hover_map.find(node);
        if (find == _hover_map.end())
        {
            node->set_state(INITIAL_HOVER);
        }
        _hover_map[node] = 2;
    }

    void Node::finish_hover_event()
    {
        for (auto it = _hover_map.begin(); it != _hover_map.end();)
        {
            it->second -= 1;
            if (it->second <= 0)
            {
                it->first->set_state(HOVER_INITIAL);
                _hover_map.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }

    void Node::emit_click_event(elemd::mouse_button_event event)
    {
        bool bubble = true;
        for (auto& var : _click_event_callbacks)
        {
            if (!var({this, event}))
                bubble = false;
        }

        if (bubble && _parent != nullptr)
        {
            _parent->emit_click_event(event);
        }
    }

    void Node::emit_scroll_event(elemd::scroll_event event)
    {
        bool bubble = true;

        // only bubble event if we are not scrollable or hit a scroll stop
        if (_scrollable_x)
        {
            if (event.xoffset < 0 && _scroll_percent.get_x() >= 1)
            {
            }
            else if (event.xoffset > 0 && _scroll_percent.get_x() <= 0)
            {
            }
            else if (event.xoffset)
            {
                bubble = false;
                _scroll_offset = _scroll_offset + elemd::vec2((float)event.xoffset, 0.f);
            }
        }
        if (_scrollable_y)
        {
            if (event.yoffset < 0 && _scroll_percent.get_y() >= 1)
            {
            }
            else if (event.yoffset > 0 && _scroll_percent.get_y() <= 0)
            {
            }
            else if (event.yoffset)
            {
                bubble = false;
                _scroll_offset = _scroll_offset + elemd::vec2(0.f, (float)event.yoffset);
            }
        }

        for (auto& var : _scroll_event_callbacks)
        {
            if (!var({this, event}))
                bubble = false;
        }

        if (bubble && _parent != nullptr)
        {
            _parent->emit_scroll_event(event);
        }
    }

    void Node::emit_key_event(elemd::key_event event)
    {
        bool bubble = true;
        for (auto& var : _key_event_callbacks)
        {
            if (!var({this, event}))
                bubble = false;
        }

        if (bubble && _parent != nullptr)
        {
            _parent->emit_key_event(event);
        }
    }

    void Node::emit_char_event(elemd::char_event event)
    {
        bool bubble = true;
        for (auto& var : _char_event_callbacks)
        {
            if (!var({this, event}))
                bubble = false;
        }

        if (bubble && _parent != nullptr)
        {
            _parent->emit_char_event(event);
        }
    }

    Node* Node::bounds_check(elemd::vec2 pos)
    {

        // std::cout << "Node: p" << "(" << _position.get_x() << ", " << _position.get_y() << ") "
        // << " w" << get_width() << " h" << get_height();
        if ((pos.get_x() > _position.get_x() + style.margin[3] &&
             pos.get_x() < _position.get_x() + (get_width() - style.margin[1])) &&
            (pos.get_y() > _position.get_y() + style.margin[0] &&
             pos.get_y() < _position.get_y() + (get_height() - style.margin[2])))
        {
            add_to_hover_list(this);

            if (_children.size() != 0)
            {
                for (auto& el : _children)
                {
                    Node* ret = el->bounds_check(pos);
                    if (ret != nullptr)
                    {
                        return ret;
                    }
                }
                return this;
            }
            else
            {
                return this;
            }
        }

        // std::cout << std::endl;

        // hover = false;
        return nullptr;
    }

    elemd::vec2 Node::get_minimum_dimensions(elemd::Context* ctx, float available_width, float available_height)
    {
        elemd::vec2 child_pos = _position + elemd::vec2(style.margin[3] + style.padding[3],
                                                        style.margin[0] + style.padding[0]);

        int index = 0;
        float width_accum = 0;
        float max_line_height = 0;

        float calc_width = 0;
        float calc_height = 0;

        for (Node* el : _children)
        {

            // child_pos.y() += height_offset;

            float child_height = el->layout(ctx, child_pos + elemd::vec2(width_accum, calc_height),
                                            available_width, available_height);

            width_accum += el->get_width();
            if (max_line_height < el->get_height())
            {
                max_line_height = el->get_height();
            }

            // Check if line is full
            if (_children.size() == (index + 1) ||
                width_accum + _children[index + 1]->get_width() > available_width)
            {

                if (width_accum > calc_width)
                {
                    calc_width = width_accum;
                }
                calc_height += max_line_height;

                width_accum = 0;
                max_line_height = 0;
            }

            ++index;
        }
        return elemd::vec2(calc_width, calc_height);
    }

    float Node::layout(elemd::Context* ctx, elemd::vec2 position, float width, float height)
    {
        _position = position;

        float available_core_width =
            (width - (style.margin[3] + style.padding[3])) - (style.margin[1] + style.padding[1]);
        float available_core_height =
            (height - (style.margin[0] + style.padding[0])) - (style.margin[2] + style.padding[2]);

        // Handle Width

        switch (style.width.get_type())
        {
        case value::Type::PERCENT:
            _width = width * (style.width.get_value() / 100.0f);
            available_core_width =
                _width - (style.margin[1] + style.padding[1] + style.margin[3] + style.padding[3]);
            break;
        case value::Type::PIXELS:
            available_core_width = style.width.get_value();
            _width = available_core_width + style.margin[1] + style.padding[1] + style.margin[3] +
                     style.padding[3];
            break;
        }

        switch (style.height.get_type())
        {
        case value::Type::PERCENT: // Percent defines the outer dimensions (This makes layouting
                                   // much easier)
            _height = height * (style.height.get_value() / 100.0f);
            available_core_height =
                _height - (style.margin[0] + style.padding[0] + style.margin[2] + style.padding[2]);
            break;
        case value::Type::PIXELS: // Pixels defines the inner dimensions
            available_core_height = style.height.get_value();
            _height = available_core_height + style.margin[0] + style.padding[0] + style.margin[2] +
                      style.padding[2];
            break;
        }

        // Handle minimum dimensions

        _min_dims = get_minimum_dimensions(ctx, available_core_width, available_core_height);
        _scrollable_x = _min_dims.get_x() > available_core_width;
        _scrollable_y = _min_dims.get_y() > available_core_height;

        float clmp_x = 0;
        float clmp_y = 0;

        if (_min_dims.get_x() > available_core_width)
        {
            clmp_x =
                std::clamp(_scroll_offset.get_x(), available_core_width - _min_dims.get_x(), 0.f);
            _scroll_offset = elemd::vec2(clmp_x, clmp_y);
            _scroll_percent = elemd::vec2(clmp_x / (available_core_width - _min_dims.get_x()),
                                          clmp_y / (available_core_height - _min_dims.get_y()));
        }
        if (_min_dims.get_y() > available_core_height)
        {
            clmp_y =
                std::clamp(_scroll_offset.get_y(), available_core_height - _min_dims.get_y(), 0.f);
            _scroll_offset = elemd::vec2(clmp_x, clmp_y);
            _scroll_percent = elemd::vec2(clmp_x / (available_core_width - _min_dims.get_x()),
                                          clmp_y / (available_core_height - _min_dims.get_y()));
        }

        if (clmp_x != 0 || clmp_y != 0)
        {
            // std::cout << _scroll_offset.get_x() << " " << _scroll_offset.get_y() << "\t" <<
            // _scroll_percent.get_x() << " " << _scroll_percent.get_y() << "\n";
            offset(_scroll_offset.get_x(), _scroll_offset.get_y());
        }

        if (style.display == Display::BLOCK && style.width.get_type() == value::Type::AUTO)
        {
            _width = width;
        }
        else if (style.display == Display::INLINE && style.width.get_type() == value::Type::AUTO)
        {
            _width = _min_dims.x() + (style.margin[3] + style.padding[3]) +
                     (style.margin[1] + style.padding[1]);
        }

        // Handle Height

        switch (style.height.get_type())
        {
        case value::Type::AUTO:
            _height = _min_dims.y() + (style.margin[0] + style.padding[0]) +
                      (style.margin[2] + style.padding[2]);
            break;
        }

        return _height;
    }

    void Node::debug_paint(elemd::Context* ctx)
    {
        // DEBUG
        if (_document->show_debug)
        {
            // if (_state != INITIAL) {
            // Margin
            // ctx->set_line_width(1);
            ctx->set_fill_color(elemd::color("#ae815221"));
            ctx->fill_rect(_position.get_x(), _position.get_y(), _width, _height);

            // Padding
            ctx->set_fill_color(elemd::color("#b8c47f21"));
            ctx->fill_rect(_position.get_x() + style.margin[3], _position.get_y() + style.margin[0],
                           _width - (style.margin[3] + style.margin[1]),
                           _height - (style.margin[0] + style.margin[2]));

            // Content
            ctx->set_fill_color(elemd::color("#3e3e4221"));
            ctx->fill_rect(_position.get_x() + style.margin[3] + style.padding[3],
                           _position.get_y() + style.margin[0] + style.padding[0],
                           _width - (style.margin[3] + style.margin[1]) -
                               (style.padding[3] + style.padding[1]),
                           _height - (style.margin[0] + style.margin[2]) -
                               (style.padding[0] + style.padding[2]));

            ctx->set_font(nullptr);
            ctx->set_fill_color(elemd::color(160, 160, 160));
            ctx->set_font_size(10);
            ctx->draw_text(_position.get_x() + 2.0f, _position.get_y(),
                           (id + "\n" + std::to_string((int)(_width + 0.5)) + "x" +
                            std::to_string((int)(_height + 0.5))));
        }
    }

    void Node::destroy()
    {
        _should_destroy = true;
    }

    void Node::destroy_immediately()
    {
        for (auto& child : _children)
        {
            child->destroy();
        }
        _parent = nullptr;

        delete this;
    }

    void Node::offset(float x, float y)
    {
        for (Node* el : _children)
        {
            el->_position = el->_position + elemd::vec2(x, y);
            el->offset(x, y);
        }
    }

    void Node::set_parent(Node* parent)
    {
        _parent = parent;

        if (parent != nullptr)
        {
            _document = parent->_document;
        }
    }

} // namespace elemd