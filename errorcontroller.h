#ifndef ERRORCONTROLLER_H
#define ERRORCONTROLLER_H
#include <string>

using namespace std;

class ErrorController{
private:
    int cod;
    int errorline;
    int errorcol;
    /*
    Коды программы:
    200 - все отлично
    400 - файл не открывается или не json формат
    401 - неверное количество фигурных или квадратных скобок
    402 - json-файл содержит запрещенный символ
    403 - не закрыты кавычки
    */
    string codText;
    string jsonText;

    void setProperty(int code, int line, int col){
        cod = code;
        errorline = line;
        errorcol = col;
    }

    void setCodText(){
        switch (cod) {
        case 200:
            codText = "JSON загружен.";
            break;
        case 400:
            codText = "Файл невозможно открыть";
            break;
        case 401:
            codText = "JSON содержит неверное количество фигурных или квадратных скобок.";
            break;
        case 402:
            codText = "JSON содержит запрещенные символы.";
            break;
        case 403:
            codText = "Внутри JSON открыта кавычка.";
            break;
        case 404:
            codText = "Лишняя запятая или двоеточие";
            break;
        case 405:
            codText = "Лишняя запятая или двоеточие";
            break;
        default:
            codText = "Неизвестная ошибка";
            break;
        }
    }

    void correctData(string text){
        /* Проверяет, что файл не сломан. */
        bool isMark_1 = false, isMark_2 = false; // Одинарные и двойные кавычки
        bool isComma = false, isColon = false;
        bool inSquare = false;
        bool hasItem = false, hasName = false;

        cod = 200;
        int count_figure = 0, count_square = 0;
        string forbidden_symbols = "+-*/|_\()<>;%^&!~";
        int line_count = 1, col_count = 0;
        /* Счетчики открытых символов */
        int line_last_opened_figure = 1, col_last_opened_figure = 1;
        int line_last_opened_square = 1, col_last_opened_square = 1;
        int line_last_opened_mark_1 = 1, line_last_opened_mark_2 = 1;
        int col_last_opened_mark_1 = 1, col_last_opened_mark_2 = 1;
        bool flag_bool = false;
        string word = "";
        for (char symbol : text){
            col_count++;
            if (count_figure < 0 || count_square < 0){
                cod = 401;
                errorline = line_count;
                errorcol = col_count;
                break;
            }
            if (!isMark_1 && !isMark_2 && (int)forbidden_symbols.find(symbol) != -1){
                cod = 402;
                errorline = line_count;
                errorcol = col_count;
                break;
            }

            if ((word == "true") || (word == "false") || (word == "null")){
                flag_bool = true;
            }

            switch (symbol){
            case ',':
                if (!isMark_2 && !isMark_1){
                    if ((isComma && !inSquare) || (!hasItem && !flag_bool)){
                        cod = 404;
                        errorline = line_count;
                        errorcol = col_count;
                    }
                    isColon = false;
                    isComma = true;
                    hasItem = false;
                    hasName = false;
                    flag_bool = false;
                    word = "";
                }
                break;

            case ':':
                if (!isMark_2 && !isMark_1){
                    if ((isColon && !inSquare) || !hasName){
                        cod = 405;
                        errorline = line_count;
                        errorcol = col_count;
                    }
                    hasItem = false;
                    isComma = false;
                    isColon = true;
                    hasName = false;
                    flag_bool = false;
                    word = "";
                }
                break;

            case '{':
                if (!isMark_2 && !isMark_1){
                    isComma = false, isColon = false;
                    inSquare = false;
                    hasItem = false;
                    count_figure++;
                    line_last_opened_figure = line_count;
                    col_last_opened_figure = col_count;
                    word = "";
                }
                break;

            case '}':
                if (!isMark_2 && !isMark_1){
                    count_figure--;
                    isColon = false;
                    isComma = false;
                    if (count_square > 0){
                        inSquare = true;
                    } else {
                        inSquare = false;
                    }
                    word = "";
                }
                break;

            case '[':
                if (!isMark_2 && !isMark_1){
                    isComma = false, isColon = false;
                    inSquare = true;
                    count_square++;
                    line_last_opened_square = line_count;
                    col_last_opened_square = col_count;
                    word = "";
                }
                break;

            case ']':
                if (!isMark_2 && !isMark_1){
                    count_square--;
                    word = "";
                }
                if (!hasItem && !flag_bool){
                    cod = 404;
                    errorline = line_count;
                    errorcol = col_count;
                }
                flag_bool = false;
                break;

            case '"':
                if (!isMark_1){
                    if (!isMark_2){
                        line_last_opened_mark_2 = line_count;
                        col_last_opened_mark_2 = col_count;
                    }
                    isMark_2 = !isMark_2;
                }
                break;

            case '\'':
                if (!isMark_2){
                    if (!isMark_1){
                        line_last_opened_mark_1 = line_count;
                        col_last_opened_mark_1 = col_count;
                    }
                    isMark_1 = !isMark_1;
                }
                break;

            case '\n':
                line_count++;
                col_count = 0;
                break;

            default:
                if (!isspace(symbol)){
                    word += symbol;
                }
                if ((isdigit(symbol) || isMark_1 || isMark_2)){
                    hasItem = true;
                }
                if (((isalpha(symbol) || isdigit(symbol) || isspace(symbol)) && (isMark_1 || isMark_2))){
                    hasName = true;
                }
                break;
            }
            /* Конец кейса */

            if (!isOk()){
                break;
            }
        }
        if (isOk()){
            if (isColon)
                setProperty(405, line_last_opened_figure, col_last_opened_figure);
            if (count_figure > 0)
                setProperty(401, line_last_opened_figure, col_last_opened_figure);
            if (count_square > 0)
                setProperty(401, line_last_opened_square, col_last_opened_square);
            if (isMark_1)
                setProperty(403, line_last_opened_mark_1, col_last_opened_mark_1);
            if (isMark_2)
                setProperty(403, line_last_opened_mark_2, col_last_opened_mark_2);
        }
    }


public:
    ErrorController(){
        cod = 400;
        errorline = -1;
        errorcol = -1;
        setCodText();
    }

    ErrorController(string text){
        jsonText = text;
        errorline = -1;
        errorcol = -1;
        correctData(text);
        setCodText();
    }

    void setCod(int newCod){
        cod = newCod;
        setCodText();
    }

    string getCodText(){
        return codText;
    }

    int getCod(){
        return cod;
    }

    bool isOk(){
        return cod == 200;
    }

    int getErrorLine(){
        return errorline;
    }

    int getErrorCol(){
        return errorcol;
    }
};

#endif // ERRORCONTROLLER_H
