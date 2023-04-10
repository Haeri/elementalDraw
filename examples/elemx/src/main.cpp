#include <elemd/ui/document.hpp>
#include <elemd/ui/element.hpp>
#include <elemd/ui/heading.hpp>
#include <elemd/ui/text_field.hpp>

int main(void)
{
    int WIDTH = 400;
    int HEIGHT = 450;

    elemd::WindowConfig winc = elemd::WindowConfig{"Editor", WIDTH, HEIGHT};    
    elemd::Window* window = elemd::Window::create(winc);

    elemd::Document doc(window);

    elemd::Font* emj_font = doc.load_font("C:/Windows/Fonts/seguiemj.ttf");


    elemd::Node* root = doc.get_root();
    root->style.height.set_percent(100.0f);
    root->style.background_color = {30, 30, 30};
    root->style.border_width[0] = 1.0f;
    root->style.border_width[0] = 1.0f;
    root->style.border_width[0] = 1.0f;
    root->style.border_width[0] = 1.0f;
    root->style.border_color = {60, 60, 60};

    elemd::TextField tf;
    tf.set_text(U"This is an editable text field! 📝🥰");
    tf.style.font_family = emj_font;
    tf.style.font_size = 12;
    tf.style.color = {248, 248, 248}; 
    tf.style.padding[0] = 20;
    tf.style.padding[1] = 30;
    tf.style.padding[2] = 20;
    tf.style.padding[3] = 30;

    elemd::Element wrapper;
    
    
    wrapper.add_child(&tf);
    
    doc.add_child(&wrapper);

    doc.main_loop();

    return 0;
}