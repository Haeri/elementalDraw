#include <chrono>
#include <thread>
#include <vector>
#include <codecvt> // for std::codecvt_utf8
#include <locale>  // for std::wstring_convert

#include <elemd/font.hpp>
#include <elemd/ui/document.hpp>
#include <elemd/ui/element.hpp>
#include <elemd/ui/heading.hpp>
#include <elemd/ui/node.hpp>

#include "solver.hpp"

void add_operation(elemd::Heading* display, std::u32string op)
{
    if (display->get_text() == U"0")
    {
        display->set_text(op);
    }
    else
    {
        display->set_text(display->get_text() + (std::u32string)op);
    }
}

int main(void)
{
    bool maximized = false;

    int WIDTH = 300;
    int HEIGHT = 330;

    bool should_move = false;
    float mouse_to_window_delta_x = 0;
    float mouse_to_window_delta_y = 0;

    float mouse_x = 0;
    float mouse_y = 0;

    bool parenthesis_flip = false;

    elemd::WindowConfig winc = elemd::WindowConfig{"Calculator", WIDTH, HEIGHT};
    // winc.decorated = false;
    // winc.transparent = true;
    winc.icon_file = "./res/icon.png";
    elemd::Window* window = elemd::Window::create(winc);

    window->add_mouse_move_listener([&](elemd::mouse_move_event event) {
        if (should_move)
        {
            elemd::vec2 pos = window->get_position();
            window->set_position(pos.get_x() + event.x - mouse_to_window_delta_x,
                                 pos.get_y() + event.y - mouse_to_window_delta_y);
        }
    });

    window->add_mouse_click_listener([&](elemd::mouse_button_event event) {
        if (should_move && event.action == elemd::ACTION_RELEASE)
        {
            should_move = false;
        }
    });

    elemd::Document doc(window);
    elemd::Font* font_urbanist = doc.load_font("./res/font/Urbanist-Regular.ttf");

    elemd::color button_color_num = elemd::color("#333333");
    elemd::color button_color_num_hover = elemd::color("#6b6b6b");

    elemd::color button_color_op = elemd::color("#7d7d7d");
    elemd::color button_color_op_hover = elemd::color("#dedede");

    elemd::color button_color_accent = elemd::color("#fe9505");
    elemd::color button_color_accent_hover = elemd::color("#ffb959");

    elemd::color font_color_num = elemd::color("#ffffff");
    elemd::color font_color_op = elemd::color("#151515");
    elemd::color font_color_accent = elemd::color("#ffffff");

    elemd::Element body;
    body.id = "body";
    body.style.background_color = elemd::color("#1d1d1d");
    body.style.border_radius[0] = 5;
    body.style.border_radius[1] = 5;
    body.style.border_radius[2] = 5;
    body.style.border_radius[3] = 5;
    body.style.height.set_percent(100);

    elemd::Node::Style button_num_style;
    button_num_style.background_color = button_color_num;
    button_num_style.margin[0] = 1;
    button_num_style.margin[1] = 1;
    button_num_style.margin[2] = 1;
    button_num_style.margin[3] = 1;
    button_num_style.border_radius[0] = 6;
    button_num_style.border_radius[1] = 6;
    button_num_style.border_radius[2] = 6;
    button_num_style.border_radius[3] = 6;
    button_num_style.width.set_percent(25);
    button_num_style.transition_time = 0.3f;

    elemd::Node::Style button_num_style_hover = button_num_style;
    button_num_style_hover.background_color = button_color_num_hover;

    elemd::Node::Style button_op_style = button_num_style;
    button_op_style.background_color = button_color_op;

    elemd::Node::Style button_op_style_hover = button_op_style;
    button_op_style_hover.background_color = button_color_op_hover;

    elemd::Node::Style button_accent_style = button_num_style;
    button_accent_style.background_color = button_color_accent;

    elemd::Node::Style button_accent_style_hover = button_accent_style;
    button_accent_style_hover.background_color = button_color_accent_hover;

    elemd::Node::Style key_heading_style;
    key_heading_style.padding[0] = 6;
    key_heading_style.padding[1] = 10;
    key_heading_style.padding[2] = 6;
    key_heading_style.padding[3] = 10;
    key_heading_style.color = font_color_num;
    key_heading_style.font_family = font_urbanist;
    key_heading_style.font_size = 25;

    elemd::Element title_bar;
    title_bar.id = "title_bar";
    title_bar.style.background_color = elemd::color(30, 30, 30);
    title_bar.hover_style.background_color = elemd::color(36, 36, 36);
    title_bar.style.transition_time = 0.2f;
    title_bar.style.padding[0] = 14;
    title_bar.style.padding[1] = 14;
    title_bar.style.padding[2] = 14;
    title_bar.style.padding[3] = 14;
    title_bar.style.border_radius[0] = 5;
    title_bar.style.border_radius[1] = 5;
    title_bar.add_click_listener([&](elemd::Node::node_click_event event) {
        mouse_to_window_delta_x = event.event.x;
        mouse_to_window_delta_y = event.event.y;
        should_move = true;
        return true;
    });

    elemd::Element min_btn;
    min_btn.style.display = elemd::Node::INLINE;
    min_btn.style.background_color = elemd::color(50, 215, 75);
    min_btn.style.padding[0] = 6;
    min_btn.style.padding[1] = 6;
    min_btn.style.padding[2] = 6;
    min_btn.style.padding[3] = 6;
    min_btn.style.border_radius[0] = 6;
    min_btn.style.border_radius[1] = 6;
    min_btn.style.border_radius[2] = 6;
    min_btn.style.border_radius[3] = 6;
    min_btn.style.margin[1] = 8;
    min_btn.hover_style = min_btn.style;
    min_btn.hover_style.background_color = elemd::color("#5ddf71");
    min_btn.style.transition_time = 0.2f;
    min_btn.add_click_listener([&](elemd::Node::node_click_event event) {
        should_move = false;
        window->minimize();
        return false;
    });

    elemd::Element max_btn;
    max_btn.style.display = elemd::Node::INLINE;
    max_btn.style.background_color = elemd::color(255, 214, 10);
    max_btn.style.padding[0] = 6;
    max_btn.style.padding[1] = 6;
    max_btn.style.padding[2] = 6;
    max_btn.style.padding[3] = 6;
    max_btn.style.border_radius[0] = 6;
    max_btn.style.border_radius[1] = 6;
    max_btn.style.border_radius[2] = 6;
    max_btn.style.border_radius[3] = 6;
    max_btn.style.margin[1] = 8;
    max_btn.hover_style = max_btn.style;
    max_btn.hover_style.background_color = elemd::color("#ffdf3d");
    max_btn.style.transition_time = 0.2f;
    max_btn.add_click_listener([&](elemd::Node::node_click_event event) {
        if (!maximized)
        {
            window->maximize();
        }
        else
        {
            window->restore();
        }
        maximized = !maximized;
        return false;
    });

    elemd::Element close_btn;
    close_btn.style.display = elemd::Node::INLINE;
    close_btn.style.background_color = elemd::color(255, 69, 58);
    close_btn.style.padding[0] = 6;
    close_btn.style.padding[1] = 6;
    close_btn.style.padding[2] = 6;
    close_btn.style.padding[3] = 6;
    close_btn.style.border_radius[0] = 6;
    close_btn.style.border_radius[1] = 6;
    close_btn.style.border_radius[2] = 6;
    close_btn.style.border_radius[3] = 6;
    close_btn.style.margin[1] = 8;
    close_btn.hover_style = close_btn.style;
    close_btn.hover_style.background_color = elemd::color("#ff726b");
    close_btn.style.transition_time = 0.2f;
    close_btn.add_click_listener([&](elemd::Node::node_click_event event) {
        window->close();
        return false;
    });

    title_bar.add_child(&close_btn);
    title_bar.add_child(&max_btn);
    title_bar.add_child(&min_btn);

    elemd::Element display;
    display.id = "display";
    display.style.padding[0] = 10;
    display.style.padding[1] = 10;
    display.style.padding[2] = 10;
    display.style.padding[3] = 10;
    display.style.scroll_bar_color = elemd::color(120, 120, 120, 100);
    display.style.transition_time = 0.2;

    elemd::Element keypad;
    keypad.style.padding[0] = 5;
    keypad.style.padding[1] = 5;
    keypad.style.padding[2] = 20;
    keypad.style.padding[3] = 5;

    elemd::Heading solution;
    solution.set_text("0");
    solution.style.color = font_color_num;
    solution.style.font_family = font_urbanist;
    solution.style.font_size = 30;
    solution.style.padding[0] = 10;
    solution.style.padding[2] = 10;

    elemd::Heading equation;
    equation.set_text("");
    equation.style.display = elemd::Node::BLOCK;
    equation.style.color = button_color_op;
    equation.style.font_family = font_urbanist;
    equation.style.font_size = 16;

    display.add_child(&equation);
    display.add_child(&solution);

    // C    +/-     %       /
    elemd::Element button_c;
    button_c.style = button_op_style;
    button_c.hover_style = button_op_style_hover;

    elemd::Element button_plus_minus;
    button_plus_minus.style = button_op_style;
    button_plus_minus.hover_style = button_op_style_hover;

    elemd::Element button_percent;
    button_percent.style = button_op_style;
    button_percent.hover_style = button_op_style_hover;

    elemd::Element button_div;
    button_div.style = button_op_style;
    button_div.hover_style = button_op_style_hover;

    // 7    8       9       x
    elemd::Element button_7;
    button_7.style = button_num_style;
    button_7.hover_style = button_num_style_hover;

    elemd::Element button_8;
    button_8.style = button_num_style;
    button_8.hover_style = button_num_style_hover;

    elemd::Element button_9;
    button_9.style = button_num_style;
    button_9.hover_style = button_num_style_hover;

    elemd::Element button_mul;
    button_mul.style = button_op_style;
    button_mul.hover_style = button_op_style_hover;

    // 4    5       6       -
    elemd::Element button_4;
    button_4.style = button_num_style;
    button_4.hover_style = button_num_style_hover;

    elemd::Element button_5;
    button_5.style = button_num_style;
    button_5.hover_style = button_num_style_hover;

    elemd::Element button_6;
    button_6.style = button_num_style;
    button_6.hover_style = button_num_style_hover;

    elemd::Element button_sub;
    button_sub.style = button_op_style;
    button_sub.hover_style = button_op_style_hover;

    // 1    2       3       +
    elemd::Element button_1;
    button_1.style = button_num_style;
    button_1.hover_style = button_num_style_hover;

    elemd::Element button_2;
    button_2.style = button_num_style;
    button_2.hover_style = button_num_style_hover;

    elemd::Element button_3;
    button_3.style = button_num_style;
    button_3.hover_style = button_num_style_hover;

    elemd::Element button_add;
    button_add.style = button_op_style;
    button_add.hover_style = button_op_style_hover;

    // 0    ,      <       =
    elemd::Element button_0;
    button_0.style = button_num_style;
    button_0.style.width.set_percent(50);
    button_0.hover_style = button_num_style_hover;

    elemd::Element button_comma;
    button_comma.style = button_num_style;
    button_comma.hover_style = button_num_style_hover;

    elemd::Element button_parenthesis;
    button_parenthesis.style = button_op_style;
    button_parenthesis.hover_style = button_op_style_hover;

    elemd::Element button_equ;
    button_equ.style = button_accent_style;
    button_equ.hover_style = button_accent_style_hover;

    elemd::Heading h_c;
    h_c.set_text("c");
    h_c.style = key_heading_style;
    h_c.style.color = font_color_op;
    h_c.add_click_listener([&](elemd::Node::node_click_event event) {
        solution.set_text("0");
        equation.set_text("");
        return true;
    });
    button_c.add_child(&h_c);

    elemd::Heading h_back;
    h_back.set_text("<");
    h_back.style = key_heading_style;
    h_back.style.color = font_color_op;
    h_back.add_click_listener([&](elemd::Node::node_click_event event) {
        std::u32string s = solution.get_text();
        if (s.length() > 1)
        {
            s.pop_back();
            solution.set_text(s);
        }
        else
        {
            solution.set_text("0");
        }
        equation.set_text("");
        return true;
    });
    button_plus_minus.add_child(&h_back);

    elemd::Heading h_div;
    h_div.set_text("/");
    h_div.style = key_heading_style;
    h_div.style.color = font_color_op;
    h_div.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"/");
        return true;
    });
    button_div.add_child(&h_div);

    elemd::Heading h_7;
    h_7.set_text("7");
    h_7.style = key_heading_style;
    h_7.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"7");
        return true;
    });
    button_7.add_child(&h_7);

    elemd::Heading h_8;
    h_8.set_text("8");
    h_8.style = key_heading_style;
    h_8.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"8");
        return true;
    });
    button_8.add_child(&h_8);

    elemd::Heading h_9;
    h_9.set_text("9");
    h_9.style = key_heading_style;
    h_9.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"9");
        return true;
    });
    button_9.add_child(&h_9);

    elemd::Heading h_mul;
    h_mul.set_text("x");
    h_mul.style = key_heading_style;
    h_mul.style.color = font_color_op;
    h_mul.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"*");
        return true;
    });
    button_mul.add_child(&h_mul);

    // 4    5       6       -
    elemd::Heading h_4;
    h_4.set_text("4");
    h_4.style = key_heading_style;
    h_4.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"4");
        return true;
    });
    button_4.add_child(&h_4);

    elemd::Heading h_5;
    h_5.set_text("5");
    h_5.style = key_heading_style;
    h_5.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"5");
        return true;
    });
    button_5.add_child(&h_5);

    elemd::Heading h_6;
    h_6.set_text("6");
    h_6.style = key_heading_style;
    h_6.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"6");
        return true;
    });
    button_6.add_child(&h_6);

    elemd::Heading h_sub;
    h_sub.set_text("-");
    h_sub.style = key_heading_style;
    h_sub.style.color = font_color_op;
    h_sub.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"-");
        return true;
    });
    button_sub.add_child(&h_sub);

    // 1    2       3       +
    elemd::Heading h_1;
    h_1.set_text("1");
    h_1.style = key_heading_style;
    h_1.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"1");
        return true;
    });
    button_1.add_child(&h_1);

    elemd::Heading h_2;
    h_2.set_text("2");
    h_2.style = key_heading_style;
    h_2.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"2");
        return true;
    });
    button_2.add_child(&h_2);

    elemd::Heading h_3;
    h_3.set_text("3");
    h_3.style = key_heading_style;
    h_3.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"3");
        return true;
    });
    button_3.add_child(&h_3);

    elemd::Heading h_add;
    h_add.set_text("+");
    h_add.style = key_heading_style;
    h_add.style.color = font_color_op;
    h_add.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"+");
        return true;
    });
    button_add.add_child(&h_add);

    // 0    ,      <       =
    elemd::Heading h_0;
    h_0.set_text("0");
    h_0.style = key_heading_style;
    h_0.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U"0");
        return true;
    });
    button_0.add_child(&h_0);

    elemd::Heading h_comma;
    h_comma.set_text(".");
    h_comma.style = key_heading_style;
    h_comma.add_click_listener([&](elemd::Node::node_click_event event) {
        add_operation(&solution, U".");
        return true;
    });
    button_comma.add_child(&h_comma);

    elemd::Heading h_parenthesis;
    h_parenthesis.set_text("()");
    h_parenthesis.style = key_heading_style;
    h_parenthesis.style.color = font_color_op;
    h_parenthesis.add_click_listener([&](elemd::Node::node_click_event event) {
        if (parenthesis_flip)
        {
            add_operation(&solution, U")");
        }
        else
        {
            add_operation(&solution, U"(");
        }
        parenthesis_flip = !parenthesis_flip;
        return true;
    });
    button_parenthesis.add_child(&h_parenthesis);

    elemd::Heading h_equ;
    h_equ.set_text("=");
    h_equ.style = key_heading_style;
    h_equ.add_click_listener([&](elemd::Node::node_click_event event) {
        equation.set_text(solution.get_text());

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
        

        std::string val = std::to_string(Solver::eval(conv_utf8_utf32.to_bytes(solution.get_text())));
        val.erase(val.find_last_not_of('0') + 1, std::u32string::npos);

        if (val.back() == '.')
        {
            val.pop_back();
        }

        solution.set_text(val);
        return true;
    });
    button_equ.add_child(&h_equ);

    elemd::Element row_1;
    row_1.add_child(&button_c);
    row_1.add_child(&button_plus_minus);
    row_1.add_child(&button_parenthesis);
    row_1.add_child(&button_div);

    elemd::Element row_2;
    row_2.add_child(&button_7);
    row_2.add_child(&button_8);
    row_2.add_child(&button_9);
    row_2.add_child(&button_mul);

    elemd::Element row_3;
    row_3.add_child(&button_4);
    row_3.add_child(&button_5);
    row_3.add_child(&button_6);
    row_3.add_child(&button_sub);

    elemd::Element row_4;
    row_4.add_child(&button_1);
    row_4.add_child(&button_2);
    row_4.add_child(&button_3);
    row_4.add_child(&button_add);

    elemd::Element row_5;
    row_5.add_child(&button_0);
    row_5.add_child(&button_comma);
    row_5.add_child(&button_equ);

    keypad.add_child(&row_1);
    keypad.add_child(&row_2);
    keypad.add_child(&row_3);
    keypad.add_child(&row_4);
    keypad.add_child(&row_5);

    // body.add_child(&title_bar);
    body.add_child(&display);
    body.add_child(&keypad);

    window->add_char_listener([&](elemd::char_event event) {
        std::u32string in = {static_cast<char32_t>(event.unnicode)};
        if (in.find_first_not_of(U"0123456789.+-*/()=") == std::u32string::npos)
        {
            add_operation(&solution, in);
        }
    });

    window->add_key_listener([&](elemd::key_event event) {
        if (event.action == elemd::ACTION_PRESS)
        {
            switch (event.key)
            {
            case elemd::keyboard_key::KEY_BACKSPACE:
                h_back.emit_click_event({});
                break;
            case elemd::keyboard_key::KEY_ENTER:
                h_equ.emit_click_event({});
                break;
            }
        }
    });

    doc.add_child(&body);
    doc.main_loop();

    return 0;
}