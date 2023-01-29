#include <elemd/ui/document.hpp>
#include <elemd/ui/element.hpp>
#include <elemd/ui/heading.hpp>

int main(void)
{
    int WIDTH = 300;
    int HEIGHT = 330;

    elemd::WindowConfig winc = elemd::WindowConfig{"Parser", WIDTH, HEIGHT};
    elemd::Window* window = elemd::Window::create(winc);

    elemd::Document doc(window);
    elemd::Font* font_urbanist = doc.load_font("./res/font/Urbanist-Regular.ttf");

    elemd::Heading h1;
    // h1.set_text("Hello there\nline2");
    h1.set_text(
        std::string(
            "Cras ultricies ligula sed magna dictum porta. Mauris blandit aliquet elit, eget "
            "tincidunt nibh pulvinar a. Vivamus suscipit tortor eget felis porttitor volutpat. "
            "Curabitur aliquet quam id dui posuere blandit. Vestibulum ante ipsum primis in "
            "faucibus orci luctus et ultrices posuere cubilia Curae; Donec velit neque, auctor sit "
            "amet aliquam vel, ullamcorper sit amet ligula. Donec rutrum congue leo eget "
            "malesuada. Pellentesque in ipsum id orci porta dapibus. Pellentesque in ipsum id orci "
            "porta dapibus. Curabitur arcu erat, accumsan id imperdiet et, porttitor at sem. "
            "Pellentesque in ipsum id orci porta dapibus. Lorem ipsum dolor sit amet, consectetur "
            "adipiscing elit. Curabitur aliquet quam id dui posuere blandit. Lorem ipsum dolor sit "
            "amet, consectetur adipiscing elit. Donec sollicitudin molestie malesuada. Vestibulum "
            "ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Donec "
            "velit neque, auctor sit amet aliquam vel, ullamcorper sit amet ligula. Donec rutrum "
            "congue leo eget malesuada. Pellentesque in ipsum id orci porta dapibus. Pellentesque "
            "in ipsum id orci porta dapibus. Curabitur arcu erat, accumsan id imperdiet et, "
            "porttitor at sem. Pellentesque in ipsum id orci porta dapibus. Lorem ipsum dolor sit "
            "amet, consectetur adipiscing elit. Curabitur aliquet quam id dui posuere blandit. "
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sollicitudin molestie "
            "malesuada. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere "
            "cubilia Curae; Donec velit neque, auctor sit amet aliquam vel, ullamcorper sit amet "
            "ligula. Donec rutrum congue leo eget malesuada. Pellentesque in ipsum id orci porta "
            "dapibus. Pellentesque in ipsum id orci porta dapibus. Curabitur arcu erat, accumsan "
            "id imperdiet et, porttitor at sem. Pellentesque in ipsum id orci porta dapibus. Lorem "
            "ipsum dolor sit amet, consectetur adipiscing elit. Curabitur aliquet quam id dui "
            "posuere blandit. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec "
            "sollicitudin molestie malesuada.") +
        std::string(
            "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia "
            "Curae; Donec velit neque, auctor sit amet aliquam vel, ullamcorper sit amet "
            "ligula.Donec sollicitudin molestie malesuada.Pellentesque in ipsum id orci porta "
            "dapibus.Pellentesque in ipsum id orci porta dapibus.Curabitur arcu erat, accumsan id "
            "imperdiet et, porttitor at sem.Donec rutrum congue leo eget malesuada.Curabitur "
            "aliquet quam id dui posuere blandit.Curabitur aliquet quam id dui posuere "
            "blandit.Vestibulum ac diam sit amet quam vehicula elementum sed sit amet "
            "dui.Curabitur non nulla sit amet nisl tempus convallis quis ac lectus.") +
        std::string(
            "Praesent sapien massa, convallis a pellentesque nec, egestas non nisi.Praesent sapien "
            "massa, convallis a pellentesque nec, egestas non nisi.Donec sollicitudin molestie "
            "malesuada.Nulla quis lorem ut libero malesuada feugiat.Quisque velit nisi, pretium ut "
            "lacinia in, elementum id enim.Proin eget tortor risus.Quisque velit nisi, pretium ut "
            "lacinia in, elementum id enim.Cras ultricies ligula sed magna dictum porta.Donec "
            "rutrum congue leo eget malesuada.Vivamus magna justo, lacinia eget consectetur sed, "
            "convallis at tellus.") +
        std::string("Praesent sapien massa, convallis a pellentesque nec, egestas non nisi.Cras "
                    "ultricies ligula sed magna dictum porta.Quisque velit nisi, pretium ut "
                    "lacinia in, elementum id enim.Vivamus magna justo, lacinia eget consectetur "
                    "sed, convallis at tellus.Pellentesque in ipsum id orci porta dapibus.Donec "
                    "sollicitudin molestie malesuada.Vivamus magna justo, lacinia eget consectetur "
                    "sed, convallis at tellus.Nulla porttitor accumsan tincidunt.Donec "
                    "sollicitudin molestie malesuada.Donec sollicitudin molestie malesuada.") +
        std::string(
            "Sed porttitor lectus nibh.Quisque velit nisi, pretium ut lacinia in, elementum id "
            "enim.Donec sollicitudin molestie malesuada.Mauris blandit aliquet elit, eget "
            "tincidunt nibh pulvinar a.Praesent sapien massa, convallis a pellentesque nec, "
            "egestas non nisi.Cras ultricies ligula sed magna dictum porta.Cras ultricies ligula "
            "sed magna dictum porta.Curabitur aliquet quam id dui posuere blandit.Curabitur arcu "
            "erat, accumsan id imperdiet et, porttitor at sem.Vestibulum ac diam sit amet quam "
            "vehicula elementum sed sit amet dui."));

    h1.style.margin[0] = 10;
    h1.style.margin[1] = 10;
    h1.style.margin[2] = 10;
    h1.style.margin[3] = 10;
    h1.style.padding[0] = 10;
    h1.style.padding[1] = 10;
    h1.style.padding[2] = 10;
    h1.style.padding[3] = 10;
    h1.style.font_family = font_urbanist;
    h1.style.font_size = 16;
    // h1.style.color = elemd::color("#fffff1");

    elemd::Element wrapper;
    wrapper.style.background_color = elemd::color(255, 255, 255);
    wrapper.add_child(&h1);

    doc.add_child(&wrapper);

    doc.main_loop();

    return 0;
}