#include <elemd/ui/document.hpp>
#include <elemd/ui/element.hpp>
#include <elemd/ui/heading.hpp>

elemd::Font* lato_regular;
elemd::Font* lato_black;
elemd::Font* slack_icons;

void add_item(elemd::Node*, std::string, elemd::Image*, std::string);

int main(void)
{
    int WIDTH = 1920 / 2;
    int HEIGHT = 1248 / 2;

    bool move_drag = false;
    bool maximized = false;

    float mouse_to_window_delta_x = 0;
    float mouse_to_window_delta_y = 0;

    /* --------------- Colors --------------- */

    elemd::color title_dark = elemd::color("#171b21");
    elemd::color sidebar_blue = elemd::color("#1d2229");
    elemd::color select_green = elemd::color("#4f9588");

    elemd::color title_red = elemd::color("#ff6059");
    elemd::color title_yellow = elemd::color("#ffbe2f");
    elemd::color title_green = elemd::color("#2aca41");

    elemd::color status_green = elemd::color("#2bac76");

    elemd::WindowConfig winc = elemd::WindowConfig{"Slack", WIDTH, HEIGHT};
    winc.decorated = false;
    winc.transparent = true;
    winc.vsync = true;
    elemd::Window* window = elemd::Window::create(winc);

    /* --------------- Events --------------- */

    window->add_mouse_move_listener([&](elemd::mouse_move_event event) {
        if (move_drag)
        {
            elemd::vec2 pos = window->get_position();
            window->set_position(pos.get_x() + event.x - mouse_to_window_delta_x,
                                 pos.get_y() + event.y - mouse_to_window_delta_y);
        }
    });

    window->add_mouse_click_listener([&](elemd::mouse_button_event event) {
        if (move_drag && event.action == elemd::ACTION_RELEASE)
        {
            move_drag = false;
        }
    });

    /* Style */
    elemd::Node::Style sidebar_element_css;
    sidebar_element_css.padding[0] = 6;
    sidebar_element_css.padding[1] = 16;
    sidebar_element_css.padding[2] = 6;
    sidebar_element_css.padding[3] = 16;
    sidebar_element_css.transition_time = 0.1;
    sidebar_element_css.background_color = sidebar_blue;

    elemd::Document doc(window);
    elemd::Image* _template = doc.load_image("./res/template.png");
    elemd::Image* p1 = doc.load_image("./res/p1.jfif");
    elemd::Image* p2 = doc.load_image("./res/p2.jfif");
    elemd::Image* p3 = doc.load_image("./res/p3.jfif");
    lato_regular = doc.load_font("./res/font/Lato-Regular.ttf");
    lato_black = doc.load_font("./res/font/Lato-Black.ttf");
    slack_icons = doc.load_font("./res/font/slack-icons.ttf");
    // elemd::font* slack_icons = doc.load_font("./res/font/slack-icons.woff2");

    elemd::Node* root = doc.get_root();

    // root->style.background_image = _template;
    // root->style.width.set_pixels(WIDTH);
    // root->style.height.set_pixels(HEIGHT);

    elemd::Element title_bar;
    title_bar.id = "title_bar";
    title_bar.style.background_color = title_dark;
    title_bar.style.padding[0] = 8;
    title_bar.style.padding[1] = 6;
    title_bar.style.padding[2] = 8;
    title_bar.style.padding[3] = 6;
    title_bar.style.border_radius[2] = 6;
    title_bar.style.border_radius[3] = 6;
    title_bar.add_click_listener([&](elemd::Node::node_click_event event) {
        mouse_to_window_delta_x = event.event.x;
        mouse_to_window_delta_y = event.event.y;
        move_drag = true;
        return true;
    });

    elemd::Element close_btn;
    close_btn.style.display = elemd::Node::INLINE;
    close_btn.style.background_color = title_red;
    close_btn.style.padding[0] = 6;
    close_btn.style.padding[1] = 6;
    close_btn.style.padding[2] = 6;
    close_btn.style.padding[3] = 6;
    close_btn.style.margin[0] = 6;
    close_btn.style.margin[1] = 4;
    close_btn.style.margin[2] = 6;
    close_btn.style.margin[3] = 4;
    close_btn.style.border_radius[0] = 6;
    close_btn.style.border_radius[1] = 6;
    close_btn.style.border_radius[2] = 6;
    close_btn.style.border_radius[3] = 6;
    close_btn.hover_style = close_btn.style;
    close_btn.hover_style.background_color = elemd::color("#ff726b");
    close_btn.style.transition_time = 0.2f;
    close_btn.add_click_listener([&](elemd::Node::node_click_event event) {
        window->close();
        return false;
    });

    elemd::Element min_btn;
    min_btn.style.display = elemd::Node::INLINE;
    min_btn.style.background_color = title_green;
    min_btn.style.padding[0] = 6;
    min_btn.style.padding[1] = 6;
    min_btn.style.padding[2] = 6;
    min_btn.style.padding[3] = 6;
    min_btn.style.margin[0] = 6;
    min_btn.style.margin[1] = 4;
    min_btn.style.margin[2] = 6;
    min_btn.style.margin[3] = 4;
    min_btn.style.border_radius[0] = 6;
    min_btn.style.border_radius[1] = 6;
    min_btn.style.border_radius[2] = 6;
    min_btn.style.border_radius[3] = 6;
    min_btn.hover_style = min_btn.style;
    min_btn.hover_style.background_color = elemd::color("#5ddf71");
    min_btn.style.transition_time = 0.2f;
    min_btn.add_click_listener([&](elemd::Node::node_click_event event) {
        move_drag = false;
        window->minimize();
        return false;
    });

    elemd::Element max_btn;
    max_btn.style.display = elemd::Node::INLINE;
    max_btn.style.background_color = title_yellow;
    max_btn.style.padding[0] = 6;
    max_btn.style.padding[1] = 6;
    max_btn.style.padding[2] = 6;
    max_btn.style.padding[3] = 6;
    max_btn.style.margin[0] = 6;
    max_btn.style.margin[1] = 4;
    max_btn.style.margin[2] = 6;
    max_btn.style.margin[3] = 4;
    max_btn.style.border_radius[0] = 6;
    max_btn.style.border_radius[1] = 6;
    max_btn.style.border_radius[2] = 6;
    max_btn.style.border_radius[3] = 6;
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

    title_bar.add_child(&close_btn);
    title_bar.add_child(&max_btn);
    title_bar.add_child(&min_btn);

    elemd::Element body;
    // body.style.background_color = elemd::color(255, 30, 30);
    body.style.border_radius[0] = 6;
    body.style.border_radius[1] = 6;
    body.style.border_radius[2] = 6;
    body.style.border_radius[3] = 6;
    body.style.height.set_percent(93.5f);
    body.id = "body";

    elemd::Element sidebar;
    sidebar.id = "sidebar";
    sidebar.style.background_color = sidebar_blue;
    sidebar.style.width.set_percent(23);
    sidebar.style.height.set_percent(100);
    body.add_child(&sidebar);

    elemd::Element workspace;
    workspace.id = "workspace";
    workspace.style.padding[0] = 12;
    workspace.style.padding[1] = 16;
    workspace.style.padding[2] = 12;
    workspace.style.padding[3] = 16;
    sidebar.add_child(&workspace);

    elemd::Element workspace_left;
    workspace_left.id = "workspace_left";
    // workspace_left.style.width.set_percent(80);
    workspace.add_child(&workspace_left);

    elemd::Heading workspace_title;
    workspace_title.id = "workspace_title";
    workspace_title.style.color = elemd::color("#ffffff");
    workspace_title.style.font_size = 15;
    workspace_title.style.font_family = lato_black;
    workspace_title.style.margin[2] = 6;
    workspace_title.set_text("Fiktion GmbH");
    workspace_left.add_child(&workspace_title);

    elemd::Heading workspace_title_arrow;
    workspace_title_arrow.id = "workspace_title_arrow";
    workspace_title_arrow.style.color = elemd::color("#ffffff");
    workspace_title_arrow.style.font_size = 18;
    workspace_title_arrow.style.font_family = slack_icons;
    workspace_title_arrow.style.margin[3] = 2;
    workspace_title_arrow.style.width.set_pixels(80);
    workspace_title_arrow.set_text("\xee\x89\x92");
    workspace_left.add_child(&workspace_title_arrow);

    elemd::Element workspace_user_status;
    workspace_user_status.id = "workspace_user_status";
    workspace_user_status.style.display = elemd::Node::INLINE;
    workspace_user_status.style.background_color = elemd::color("#2aa772");
    workspace_user_status.style.padding[0] = 4;
    workspace_user_status.style.padding[1] = 4;
    workspace_user_status.style.padding[2] = 4;
    workspace_user_status.style.padding[3] = 4;
    workspace_user_status.style.border_radius[0] = 4;
    workspace_user_status.style.border_radius[1] = 4;
    workspace_user_status.style.border_radius[2] = 4;
    workspace_user_status.style.border_radius[3] = 4;
    workspace_user_status.style.margin[0] = 4;
    workspace_user_status.style.margin[1] = 5;
    workspace_left.add_child(&workspace_user_status);

    elemd::Heading workspace_user;
    workspace_user.id = "workspace_user";
    workspace_user.style.color = elemd::color("#ffffff");
    workspace_user.style.font_size = 13;
    workspace_user.style.font_family = lato_regular;
    workspace_user.set_text("Nikki Kroll");
    workspace_left.add_child(&workspace_user);

    elemd::Element separator;
    separator.style.background_color = elemd::color("#34393F");
    separator.style.padding[0] = 1;
    sidebar.add_child(&separator);

    elemd::Element fixed_list;
    fixed_list.id = "fixed_list";
    fixed_list.style.padding[0] = 10;
    fixed_list.style.padding[2] = 10;
    sidebar.add_child(&fixed_list);

    elemd::Element fixed_list_1;
    fixed_list_1.id = "fixed_list_1";
    fixed_list_1.style = sidebar_element_css;
    fixed_list_1.hover_style.background_color = title_dark;
    fixed_list.add_child(&fixed_list_1);

    elemd::Heading fixed_list_1_icon;
    fixed_list_1_icon.id = "fixed_list_1_icon";
    fixed_list_1_icon.style.color = elemd::color("#D2D3D4");
    fixed_list_1_icon.style.font_size = 16;
    fixed_list_1_icon.style.font_family = slack_icons;
    fixed_list_1_icon.style.margin[1] = 26;
    fixed_list_1_icon.set_text("\xee\x84\x83");
    fixed_list_1.add_child(&fixed_list_1_icon);

    elemd::Heading fixed_list_1_title;
    fixed_list_1_title.id = "fixed_list_1_title";
    fixed_list_1_title.style.color = elemd::color("#ffffff");
    fixed_list_1_title.style.font_size = 15;
    fixed_list_1_title.style.font_family = lato_black;
    fixed_list_1_title.set_text("Ungelesenes");
    fixed_list_1.add_child(&fixed_list_1_title);

    elemd::Element fixed_list_2;
    fixed_list_2.id = "fixed_list_2";
    fixed_list_2.style = sidebar_element_css;
    fixed_list_2.hover_style.background_color = title_dark;
    fixed_list.add_child(&fixed_list_2);

    elemd::Heading fixed_list_2_icon;
    fixed_list_2_icon.id = "fixed_list_2_icon";
    fixed_list_2_icon.style.color = elemd::color("#D2D3D4");
    fixed_list_2_icon.style.font_size = 16;
    fixed_list_2_icon.style.font_family = slack_icons;
    fixed_list_2_icon.style.margin[1] = 26;
    fixed_list_2_icon.set_text("\xee\x85\xb1");
    fixed_list_2.add_child(&fixed_list_2_icon);

    elemd::Heading fixed_list_2_title;
    fixed_list_2_title.id = "fixed_list_2_title";
    fixed_list_2_title.style.color = elemd::color("#D2D3D4");
    fixed_list_2_title.style.font_size = 15;
    fixed_list_2_title.style.font_family = lato_regular;
    fixed_list_2_title.set_text("Threads");
    fixed_list_2.add_child(&fixed_list_2_title);

    elemd::Element fixed_list_3;
    fixed_list_2.id = "fixed_list_3";
    fixed_list_3.style = sidebar_element_css;
    fixed_list_3.hover_style.background_color = title_dark;
    fixed_list.add_child(&fixed_list_3);

    elemd::Heading fixed_list_3_icon;
    fixed_list_3_icon.id = "fixed_list_3_icon";
    fixed_list_3_icon.style.color = elemd::color("#D2D3D4");
    fixed_list_3_icon.style.font_size = 16;
    fixed_list_3_icon.style.font_family = slack_icons;
    fixed_list_3_icon.style.margin[1] = 26;
    fixed_list_3_icon.set_text("\xee\x80\x89");
    fixed_list_3.add_child(&fixed_list_3_icon);

    elemd::Heading fixed_list_3_title;
    fixed_list_3_title.id = "fixed_list_3_title";
    fixed_list_3_title.style.color = elemd::color("#D2D3D4");
    fixed_list_3_title.style.font_size = 15;
    fixed_list_3_title.style.font_family = lato_regular;
    fixed_list_3_title.set_text("Erw\xc3\xa4hnungen & Re...");
    fixed_list_3.add_child(&fixed_list_3_title);

    elemd::Element fixed_list_4;
    fixed_list_2.id = "fixed_list_4";
    fixed_list_4.style = sidebar_element_css;
    fixed_list_4.hover_style.background_color = title_dark;
    fixed_list.add_child(&fixed_list_4);

    elemd::Heading fixed_list_4_icon;
    fixed_list_4_icon.id = "fixed_list_4_icon";
    fixed_list_4_icon.style.color = elemd::color("#D2D3D4");
    fixed_list_4_icon.style.font_size = 16;
    fixed_list_4_icon.style.font_family = slack_icons;
    fixed_list_4_icon.style.margin[1] = 26;
    fixed_list_4_icon.set_text("\xee\x81\x99");
    fixed_list_4.add_child(&fixed_list_4_icon);

    elemd::Heading fixed_list_4_title;
    fixed_list_4_title.id = "fixed_list_4_title";
    fixed_list_4_title.style.color = elemd::color("#ffffff");
    fixed_list_4_title.style.font_size = 15;
    fixed_list_4_title.style.font_family = lato_black;
    fixed_list_4_title.set_text("Entw\xc3\xbcrfe");
    fixed_list_4.add_child(&fixed_list_4_title);

    elemd::Element fixed_list_5;
    fixed_list_2.id = "fixed_list_5";
    fixed_list_5.style = sidebar_element_css;
    fixed_list_5.hover_style.background_color = title_dark;
    fixed_list.add_child(&fixed_list_5);

    elemd::Heading fixed_list_5_icon;
    fixed_list_5_icon.id = "fixed_list_5_icon";
    fixed_list_5_icon.style.color = elemd::color("#D2D3D4");
    fixed_list_5_icon.style.font_size = 16;
    fixed_list_5_icon.style.font_family = slack_icons;
    fixed_list_5_icon.style.margin[1] = 26;
    // fixed_list_5_icon.set_text("\xee\x8a\x93");
    fixed_list_5_icon.set_text("\xee\x85\x88");
    fixed_list_5.add_child(&fixed_list_5_icon);

    elemd::Heading fixed_list_5_title;
    fixed_list_5_title.id = "fixed_list_5_title";
    fixed_list_5_title.style.color = elemd::color("#D2D3D4");
    fixed_list_5_title.style.font_size = 15;
    fixed_list_5_title.style.font_family = lato_regular;
    fixed_list_5_title.set_text("Mehr anzeigen");
    fixed_list_5.add_child(&fixed_list_5_title);

    elemd::Element separator2;
    separator2.style.background_color = elemd::color("#34393F");
    separator2.style.padding[0] = 1;
    sidebar.add_child(&separator2);

    elemd::Element body_content;
    body_content.id = "body_content";
    body_content.style.background_color = elemd::color(255, 255, 255);
    body_content.style.width.set_percent(77);
    body_content.style.height.set_percent(100);
    /*body_content.style.padding[0] = 10;
    body_content.style.padding[1] = 10;
    body_content.style.padding[2] = 10;
    body_content.style.padding[3] = 10;
    */
    body_content.style.transition_time = 0.2;
    body.add_child(&body_content);

    add_item(&body_content, "Jim Carpenter", p1,
             "What did the janitor say when he jumped out of the closet? - Supplies! 😂");
    add_item(&body_content, "Ella Bridger", p2,
             "Two kids were beating up a kid in an ally, so I stepped into help. He didn't stand a "
             "chance against the three of us.");
    add_item(&body_content, "Elizabeth Lee", p3,
             "What did the ocean say to the beach? - Nothing, it just waved 🤣🤣");
    add_item(&body_content, "Jim Carpenter", p1,
             "Why do seagulls fly over the ocean? - Because if they flew over the bay, we'd call "
             "them bagels. This one is great! Sed porttitor lectus nibh. Vivamus suscipit tortor "
             "eget felis porttitor volutpat. Nulla quis lorem ut libero malesuada feugiat. "
             "Curabitur aliquet quam id dui posuere blandit. Sed porttitor lectus nibh.");
    add_item(&body_content, "Ella Bridger", p2,
             "How do you get a squirrel to like you? Act like a nut.");
    add_item(&body_content, "Elizabeth Lee", p3,
             "If a child refuses to sleep during nap time, are they guilty of resisting a rest?");
    add_item(&body_content, "Ella Bridger", p2,
             "What did Baby Corn say to Mama Corn? - Where's Pop Corn? 🤔");
    add_item(
        &body_content, "Ella Bridger", p2,
        "What's the best thing about Switzerland? - I don't know, but the flag is a big plus.");
    add_item(&body_content, "Ella Bridger", p2,
             "What did the janitor say when he jumped out of the closet? - Supplies!");
    add_item(&body_content, "Ella Bridger", p2,
             "What did the janitor say when he jumped out of the closet? - Supplies!");
    /*------------------------------------------*/

    /*
    for (size_t i = 0; i < 5; i++)
    {
        elemd::element* item = new elemd::element();
        item->id = "scroll_item_" + std::to_string(i);
        item->style.padding[0] = 20;
        item->style.padding[2] = 20;

        elemd::heading* head = new elemd::heading();
        head->id = "scroll_item_content_" + std::to_string(i);
        head->style.font_size = 15;
        head->style.font_family = lato_regular;
        head->set_text("Sed porttitor lectus nibh. Vivamus suscipit tortor eget felis porttitor
    volutpat. Nulla quis lorem ut libero malesuada feugiat. Curabitur aliquet quam id dui posuere
    blandit. Sed porttitor lectus nibh. Praesent sapien massa, convallis a pellentesque nec, egestas
    non nisi. Praesent sapien massa, convallis a pellentesque nec, egestas non nisi. Nulla quis
    lorem ut libero malesuada feugiat. Cras ultricies ligula sed magna dictum porta. Praesent sapien
    massa, convallis a pellentesque nec, egestas non nisi."); item->add_child(head);

        if (i == 10) {
            for (size_t i = 0; i < 10; i++)
            {
                elemd::element* item_inside = new elemd::element();
                item_inside->id = "scroll_item_" + std::to_string(i);
                item_inside->style.padding[0] = 20;
                item_inside->style.padding[2] = 20;

                elemd::heading* head_inside = new elemd::heading();
                head_inside->id = "scroll_item_content_" + std::to_string(i);
                head_inside->style.font_size = 15;
                head_inside->style.font_family = lato_regular;
                head_inside->set_text("Inside text!");
                item_inside->add_child(head_inside);

                item->add_child(item_inside);

            }
        }
        body_content.add_child(item);
    }
    */

    doc.add_child(&title_bar);
    doc.add_child(&body);

    doc.main_loop();

    return 0;
}

void add_item(elemd::Node* parent, std::string _name, elemd::Image* img, std::string text)
{
    elemd::Element* item = new elemd::Element();
    item->style.padding[0] = 12;
    item->style.padding[1] = 20;
    item->style.padding[2] = 12;
    item->style.padding[3] = 20;
    item->hover_style.background_color = elemd::color(250, 250, 250);

    elemd::Element* pic = new elemd::Element();
    pic->style.background_image = img;
    pic->style.width.set_pixels(50);
    pic->style.height.set_pixels(50);
    pic->style.margin[1] = 14;
    pic->style.border_radius[0] = 5;
    pic->style.border_radius[1] = 5;
    pic->style.border_radius[2] = 5;
    pic->style.border_radius[3] = 5;
    item->add_child(pic);

    elemd::Element* item_body = new elemd::Element();
    item_body->style.width.set_percent(80);
    item->add_child(item_body);

    elemd::Heading* name = new elemd::Heading();
    name->style.display = elemd::Node::Display::BLOCK;
    name->style.font_size = 15;
    name->style.font_family = lato_black;
    name->style.padding[2] = 5;
    name->set_text(_name);
    item_body->add_child(name);

    elemd::Heading* head = new elemd::Heading();
    head->style.font_size = 14;
    head->style.font_family = lato_regular;
    head->style.color = elemd::color(100, 100, 100);
    head->set_text(text);
    item_body->add_child(head);

    parent->add_child(item);
}