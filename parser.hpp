#ifndef PARSER_HPP
#define PARSER_HPP

#include "includer.hpp"
#include <stack>
#include <sstream>


extern const int distance;
extern const int window_x;
extern const int window_y;

const char _var = '$';

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

std::vector<std::string> infixToPostfix(const std::string& expression) {
    std::stack<char> operators;
    std::vector<std::string> postfix;
    std::stringstream number;

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (std::isdigit(c) || c == '.') {
            number << c; 
        } else {
            if (!number.str().empty()) {
                postfix.push_back(number.str());
                number.str("");
                number.clear();
            }

            if (isOperator(c)) {
                while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(c)) {
                    postfix.push_back(std::string(1, operators.top()));
                    operators.pop();
                }
                operators.push(c);
            } else if (c == '(') {
                operators.push(c);
            } else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix.push_back(std::string(1, operators.top()));
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == '(') {
                    operators.pop();
                }
            }
        }
    }

    if (!number.str().empty()) {
        postfix.push_back(number.str());
    }

    while (!operators.empty()) {
        postfix.push_back(std::string(1, operators.top()));
        operators.pop();
    }

    return postfix;
}

double evaluatePostfix(const std::vector<std::string>& postfix) {
    std::stack<double> values;

    for (const std::string& token : postfix) {
        if (isOperator(token[0]) && token.length() == 1) {
            if (values.size() < 2) {
                throw std::runtime_error("Ungültiger Ausdruck.");
            }

            double b = values.top();
            values.pop();
            double a = values.top();
            values.pop();

            switch (token[0]) {
                case '+': values.push(a + b); break;
                case '-': values.push(a - b); break;
                case '*': values.push(a * b); break;
                case '/':
                    if (b == 0) throw std::runtime_error("Division durch null.");
                    values.push(a / b);
                    break;
            }
        } else {
            values.push(std::stod(token));
        }
    }

    if (values.size() != 1) {
        throw std::runtime_error("Ungültiger Ausdruck.");
    }

    return values.top();
}

double evaluateExpression(const std::string& expression) {
    auto postfix = infixToPostfix(expression);
    return evaluatePostfix(postfix);
}


std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return ""; 
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void log(std::string msg){
    std::cout<<msg<<std::endl;
}

Color string_to_color(std::string color) {
    const char* colorName = color.c_str();
    if (strcmp(colorName, "white") == 0) return WHITE;
    if (strcmp(colorName, "black") == 0) return BLACK;
    if (strcmp(colorName, "black") == 0) return RED;
    if (strcmp(colorName, "green") == 0) return GREEN;
    if (strcmp(colorName, "blue") == 0) return BLUE;
    if (strcmp(colorName, "yellow") == 0) return YELLOW;
    if (strcmp(colorName, "orange") == 0) return ORANGE;
    if (strcmp(colorName, "purple") == 0) return PURPLE;
    if (strcmp(colorName, "pink") == 0) return PINK;
    if (strcmp(colorName, "gray") == 0) return GRAY;
    if (strcmp(colorName, "ligthgrey") == 0) return LIGHTGRAY;
    if (strcmp(colorName, "darkgray") == 0) return DARKGRAY;
    return WHITE;
}

std::vector<std::string> get_arguments(std::string command){
    std::vector<std::string> returner = split_str(read_from_char(read_until_char(command, ')'), '('), ',');
    for(std::string & hj : returner){
            hj = Trim(hj.c_str());
            log(hj);
    }
    return returner;
}

std::string preproc1(std::string code){
    std::string returner = replace_word(code, "window_x", std::to_string(GetMonitorWidth(0)));
    returner = replace_word(returner, "window_y", std::to_string(GetMonitorHeight(0)-distance));
    std::vector<std::string> commands = split_str(returner, ';');
    std::string re = "";
    for(std::string command : commands){
        std::string real_command = read_until_char(command, '(');
        real_command = to_lower(Trim(real_command));
        log(real_command);
        if(real_command == "define"){
            for(std::string &command1 : commands){
                if(read_until_char(command1, '(') != "define"){
                        std::string _char = read_until_char(read_from_char(command, _var), _var);
                        if(_char != ""){
                        log("char:"+_char);log("char end");}
                        if(_char == get_arguments(command)[0]){
                                command1 = replace_word(command1, _var+_char+_var, get_arguments(command)[1]);
                        }
                        re += command1+";";
                }
            }
        }
    }
    return re;
}


std::string preprocessor1(std::string code){
    std::string returner = replace_word(code, "window_x", std::to_string(GetMonitorWidth(0)));
    returner = replace_word(returner, "window_y", std::to_string(GetMonitorHeight(0)-distance));
    std::vector<std::string> commands = split_str(returner, ';');
    for(std::string &command : commands){
        std::string real_command = read_until_char(command, '(');
        real_command = to_lower(Trim(real_command));
        log(real_command);
        if(real_command == "define"){
                std::vector<std::string> commands1 = split_str(code, ';');
                for(std::string command1 : commands1){
                    for(std::string &argument : get_arguments(command1)){
                        if(argument == get_arguments(command)[0]){
                            argument = replace_word(argument, get_arguments(command)[0], get_arguments(command)[1]);
                        }
                    }
                }
        }
        }
    return returner;    
}

float stois(std::string num){
    return evaluateExpression(num);
}

int parse(std::string codes){
    std::string code = preproc1(codes);
    log("final code: "+code);
    std::vector<std::string> commands = split_str(code, ';');
    for(std::string command : commands){
        std::string real_command = read_until_char(command, '(');
        real_command = to_lower(Trim(real_command));
        if(real_command == "background"){
                log("background color");
                DrawRectangle(0, distance, GetMonitorWidth(0), GetMonitorHeight(0), string_to_color(get_arguments(command)[0]));
        }
        else if(real_command == "rectangle"){
                 DrawRectangle(stois(get_arguments(command)[0]), stois(get_arguments(command)[1])+20, stois(get_arguments(command)[2]), stois(get_arguments(command)[3]),string_to_color(get_arguments(command)[4]));
        }
        else if(real_command == "text"){
                DrawText(get_arguments(command)[0].c_str(), stois(get_arguments(command)[1]), stois(get_arguments(command)[2])+20, stois(get_arguments(command)[3]), string_to_color(get_arguments(command)[4]));
        }
        else if(real_command == "circle"){
                DrawCircle(stois(get_arguments(command)[0]), stois(get_arguments(command)[1])+20, std::stof(get_arguments(command)[2]), string_to_color(get_arguments(command)[3]));
        }
        else if(real_command == "rectangle_lines"){
            DrawRectangleLines(stois(get_arguments(command)[0]), stois(get_arguments(command)[1])+20, stois(get_arguments(command)[2]), stois(get_arguments(command)[3]),string_to_color(get_arguments(command)[4]));
        }
        else if(real_command == "circle_lines"){
            DrawCircleLines(stois(get_arguments(command)[0]), stois(get_arguments(command)[1])+20, std::stof(get_arguments(command)[2]), string_to_color(get_arguments(command)[3]));
        }
        }
    return 0;
}

#endif
