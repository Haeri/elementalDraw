#include "elemd/ui/document.hpp"

#include <thread>

namespace elemd
{
    using ms = std::chrono::duration<double, std::milli>;

    Document::Document(elemd::Window* window) : Document(window, window->create_context())
    {
    }

    Document::Document(elemd::Window* window, elemd::Context* context)
    {
        _root = new Element();
        _root->set_document(this);

        _width = window->get_width() / window->get_dpi_scale();
        _height = window->get_height() / window->get_dpi_scale();

        _window = window;
        _context = context;

        _window->add_resize_listener([&](elemd::resize_event event) {
            _width = event.width / _window->get_scale().get_x();
            _height = event.height / _window->get_scale().get_y();

            _root->style.width.set_pixels(_width);
            _root->style.height.set_pixels(_height);
        });

        _window->add_mouse_move_listener([&](elemd::mouse_move_event event) {
            _mouse_x = event.x;
            _mouse_y = event.y;

            _root->bounds_check(elemd::vec2((float)event.x, (float)event.y));
            Node::finish_hover_event();
        });

        _window->add_mouse_click_listener([&](elemd::mouse_button_event event) {
            if (event.action == elemd::input_action::ACTION_PRESS &&
                event.button == elemd::mouse_button::MOUSE_BUTTON_LEFT)
            {
                Node* node = _root->bounds_check(elemd::vec2((float)event.x, (float)event.y));
                Node::finish_hover_event();

                if (_focused_node != nullptr)
                {
                    _focused_node->set_focus(false);
                }
                _focused_node = node;

                if (node != nullptr)
                {
                    _focused_node->set_focus(true);

                    node->emit_click_event(event);
                }
            }
        });

        _window->add_scroll_listener([&](elemd::scroll_event event) {
            Node* node = _root->bounds_check(elemd::vec2(_mouse_x, _mouse_y));
            Node::finish_hover_event();

            elemd::scroll_event event_override = {event.xoffset * 10.f, event.yoffset * 10.f};

            if (node != nullptr)
            {
                node->emit_scroll_event(event_override);
            }
        });

        _window->add_key_listener([&](elemd::key_event event) {            
            if (event.key == KEY_I && event.action == ACTION_PRESS && event.mods == (KEY_MOD_SHIFT | KEY_MOD_CONTROL))
            {
                show_debug = !show_debug;
            }

            if (_focused_node != nullptr)
            {
                _focused_node->emit_key_event(event);
            }
        });

        _window->add_char_listener([&](elemd::char_event event) {
            if (_focused_node != nullptr)
            {
                _focused_node->emit_char_event(event);
            }
        });

        _context->set_clear_color({0, 0, 0, 0});
    }

    Document::~Document()
    {
        for (auto& font : _fonts)
        {
            font->destroy();
        }
        for (auto& img : _images)
        {
            img->destroy();
        }

        delete _root;

        _window->destroy();
    }

    void Document::add_child(Node* child)
    {
        _root->add_child(child);
        _root->set_document(this);
    }

    Node* Document::get_root()
    {
        return _root;
    }

    void Document::main_loop()
    {
        _tmp_prepare();

        std::chrono::steady_clock::time_point current_time;
        std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();

        while (_window->is_running())
        {
            current_time = std::chrono::steady_clock::now();   
            delta_time = std::chrono::duration_cast<ms>(current_time - last_time).count() / 1000.0f;
            last_time = current_time;

            if (_highFrequencyNext)
            {
                _highFrequencyNext = false;
                _window->poll_events();
            }
            else
            {
                _window->wait_events();
                // helps smooth out the high dt after a interaction pause
                last_time = std::chrono::steady_clock::now();
            }

            // TODO: Check dirty
            paint();
        }
    }

    void Document::request_high_frequency()
    {
        _highFrequencyNext = true;
    }

    elemd::Font* Document::load_font(const std::string& font_file)
    {
        elemd::Font* font = elemd::Font::create(font_file);
        _fonts.push_back(font);

        return font;
    }

    elemd::Image* Document::load_image(const std::string& image)
    {
        elemd::Image* img = elemd::Image::create(image);
        _images.push_back(img);

        return img;
    }

    int Document::get_width()
    {
        return _width;
    }

    int Document::get_height()
    {
        return _height;
    }

    elemd::Window* Document::get_window()
    {
        return _window;
    }

    void Document::_tmp_prepare()
    {
        for (auto& font : _fonts)
        {
            _context->_tmp_register_font(font);
        }
        for (auto& image : _images)
        {
            _context->_tmp_register_image(image);
        }

        _context->_tmp_prepare();
    }

    void Document::paint(double dt)
    {
        delta_time = dt;
        paint();
    }

    void Document::paint()
    {
        _document_height = _root->layout(_context, elemd::vec2(0, 0), _width, _height);
        _root->paint(_context);

        // Draw perf info
        if (show_debug)
        {
            // We do our own dt calculation since the document delta_time fakes the dt when there is inactivty
            static std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            float dt = std::chrono::duration_cast<ms>(now - last_time).count() / 1000.0f;
            last_time = now;

            _context->set_font(nullptr);
            _context->set_font_size(10);
            _context->set_fill_color({30, 30, 30, 255});
            std::string info =
                "dt: " + std::to_string((int)(dt * 1000)) +
                "ms // highfq: " + std::to_string(_highFrequencyNext);
            _context->draw_text(5, _height - 13, info);
            _context->set_fill_color({230, 230, 230, 255});
            _context->draw_text(4, _height - 14, info);
        }

        _context->draw_frame();
        _context->present_frame();
    }
} // namespace elemd