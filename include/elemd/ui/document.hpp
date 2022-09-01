#ifndef ELEMD_DOCUMENT_HPP
#define ELEMD_DOCUMENT_HPP

#include <chrono>
#include <vector>

#include "elemd/context.hpp"
#include "elemd/elemental_draw.hpp"
#include "elemd/ui/element.hpp"
#include "elemd/window.hpp"

namespace elemd
{
    class ELEMD_API Document
    {
    public:
        float delta_time;

        Document(elemd::Window* window);
        ~Document();

        void add_child(Node* child);
        Node* get_root();
        void main_loop();

        void request_high_frequency();

        elemd::Font* load_font(const std::string& font_file);
        elemd::Image* load_image(const std::string& image);

        int get_width();
        int get_height();

        elemd::Window* get_window();

    private:
        int _width = 0;
        int _height = 0;
        int _document_height = 0;
        float _mouse_x = -100;
        float _mouse_y = -100;

        bool _highFrequencyNext = false;

        Node* _root = nullptr;

        elemd::Window* _window = nullptr;
        elemd::Context* _context = nullptr;

        Node* _focused_node = nullptr;

        std::vector<elemd::Font*> _fonts;
        std::vector<elemd::Image*> _images;

        void paint();
    };
} // namespace elemd

#endif // ELEMD_DOCUMENT_HPP