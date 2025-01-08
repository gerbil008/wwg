#ifndef RAYLIB_FUNCS
#define RAYLIB_FUNCS

#include "includer.hpp"
#include "parser.hpp"

class button {
    private:
            std::string text;
    public:
        int id;
        int x;
        int y;
        int height;
        int width;
        std::string color;
        std::function<void()> func;
        button(int x, int y, int width, int height, std::string color, std::function<void()> func, int id) : x(x), y(y), width(width), height(height), color(color), func(func), id(id){}

        void check_button(){
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                Vector2 mouse = GetMousePosition();
                if (mouse.x >=  x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
                   func();
            }
        }
        }

        void draw(){
            DrawRectangleLines(x, y, width, height, string_to_color(color));
            DrawText(text.c_str(), x + 5, y + 5, height - height * 0.4, BLACK);
            if(!text.empty()){
            DrawText(text.c_str(), x + 5, y + 5, height - height * 0.4, BLACK);}
        }

        void change_text(std::string msg){
            text = msg;
        }
};

class input_field {
public:
    std::string content;
    int x;
    int y;
    int width;
    int height;
    int id;
    bool clicked = false;

    input_field(int x, int y, int width, int height, int id) : x(x), y(y), width(width), height(height), id(id) {}

    std::string get_content() {
        log(content);
        return content;
    }

    void set_content(std::string con) {
        content = con;
    }

    void add_content(std::string msg) {
        content += msg;
    }

    void sub_con() {
        if (!content.empty()) {
            content.pop_back();
        }
    }

    void delete_con(){
        content = "";
    }

    void draw_me() {
        Color color;
        if(clicked){
            color = ORANGE;
        }
        else{color = BLACK;}
        Rectangle rect = {x, y, width, height};
        DrawRectangleRoundedLines(rect, 20.0f, 16, color);
        DrawText(content.c_str(), x + 5, y + 5, height - height * 0.4, BLACK);
    }
};

#endif
