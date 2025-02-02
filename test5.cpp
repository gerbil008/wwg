#include <std_inc.hpp>

std::string replace_word(std::string str, const std::string from, const std::string to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); 
    }
    return str;
}

int main(void){
    std::cout<<replaceWord("moin", "moin", "hallo");
    return 0;
}