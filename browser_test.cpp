#include "includer.hpp"
#include "raylib_funcs.hpp"

#define web_port 6969
#define dns_port 6968
#define buffer_size 4098
#define dns_buffer_size 512

const char* dns_adress =  "127.0.0.1";

std::mutex buffer_mutex;
std::string full_buffer = "";

std::string ask_dns(std::string website){
    std::string return_ip = "";
    log("made dns_worker");
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[dns_buffer_size] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(dns_port);
    inet_pton(AF_INET, dns_adress, &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, website.c_str(), strlen(website.c_str()), 0);
    read(sock, buffer, dns_buffer_size);
    close(sock);
    return_ip = buffer;
    return return_ip;
}

void client_worker(std::string website){
    log("made web_worker");
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[buffer_size] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(web_port);
    inet_pton(AF_INET, ask_dns(website).c_str(), &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, website.c_str(), strlen(website.c_str()), 0);
    read(sock, buffer, 1024);
    std::lock_guard<std::mutex> lock(buffer_mutex);
    full_buffer = buffer;
    log("This is buffer");
    log(full_buffer);
    close(sock);
}
    
const int distance = 50;
const int window_x = 500;
const int window_y = 500;
int input_field_id = 1;
int field_active = 0;
const int button_x = 10;
const int button_y = 30;
int button_id = 1;

std::vector<input_field*> input_fields;
std::vector<button*> buttons;
input_field main_input(60, 10, 800, 30, input_field_id);

int reg_input(input_field &field){
    input_fields.push_back(&field);
    input_field_id++;
    return 0;
}

int reg_button(button &new_button){
    buttons.push_back(&new_button);
    button_id++;
    return 0;
}

int setup() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(window_x, window_y, "browser_test");
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(30);
    return 0;
}

int input_logic(){
        for (input_field* field : input_fields) {
            field->draw_me();
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            bool field_click = false;
            for (input_field* field : input_fields) {
                if (mouse.x >= field->x && mouse.x <= field->x + field->width && mouse.y >= field->y && mouse.y <= field->y + field->height) {
                    field_active = field->id;
                    field_click = true;
                    field->clicked = true;
                }
            }
            if(!field_click){
                field_active = 0;
                for(input_field *field : input_fields){
                    field->clicked = false;
                }
            }
        }

        if (field_active != 0) {
            for (input_field* field : input_fields) {
                if (field->id == field_active) {
                    
                    int key = GetCharPressed();
                    if (key > 0) {
                        std::string str(1, (char)key);
                        field->add_content(str);
                    }

                    if (IsKeyDown(KEY_BACKSPACE)) {
                        field->sub_con();
                    }
                }
                else{field->clicked = false;}
            }
        }
        return 0;
}

int button_logic(){
    for(button *button1 : buttons){
        button1->draw();
        button1->check_button();
    }
    return 0;
}


int default_stuff(){
    if(full_buffer != ""){
        std::lock_guard<std::mutex> lock(buffer_mutex);
        parse(full_buffer);
        log(full_buffer);}
    return 0;    
}

void test(){
        log("buttomn pressed");
        std::string inpip = main_input.get_content();
        log("moin"+main_input.get_content());
        log(main_input.get_content());
        std::thread t(client_worker, main_input.get_content());
        t.detach();
    }

int main(void) {
    setup();
    reg_input(main_input);
    button button1(880, 10, 50, 30, "blue", test, button_id);
    reg_button(button1);
    button1.change_text("sub");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        input_logic();
        button_logic();
        //log(main_input.content);
        //main_input.get_content();
        default_stuff();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

