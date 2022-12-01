#ifndef JSONDATA_H
#define JSONDATA_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>

#include "groupitem.h"
#include "errorcontroller.h"
using namespace std;

class JsonData{
private:
    /* Поля */
    ErrorController error;
    string jsonText;
    vector<GroupItem> rootGroupItems;


    /* Методы */
    string getInnerText(string text){
        /* Передается текст, внутри {}. Обрабатывается от лишних знаков. */
        string reqText = "";
        int count = 1, index = 1;
        while (count > 0){
            char symbol = text[index++];
            if (symbol == ' ' || symbol == '\0' || symbol == '\n'){
                continue;
            }
            if (symbol == '{'){
                count++;
            } else if (symbol == '}'){
                count--;
                if (count == 0){
                    continue;
                }
            }
            reqText += symbol;
        }
        return reqText;
    }

    int bracketPosition(string reqText, int index){
        int count = 1;
        while (count > 0){
            char c = reqText[index++];
            if (c == '{'){
                count++;
            } else if (c == '}'){
                count--;
            }
        }
        return index;
    }

    vector<GroupItem> getChildren(string text){
        /* Сначала получаем нужный текст */
        string reqText = getInnerText(text);
        /* Затем заполняем вектор */
        vector<GroupItem> groupRoot;

        GroupItem item; // временный элемент

        string word = "";
        bool flag_arr = false;
        int i = 0;
        while (i < (int)reqText.length()){
            char symbol = reqText[i++]; // Внимание! Сразу увеличиваем индекс.
            if (symbol == ':'){
                item = GroupItem(word);
                word = "";
                continue;
            }
            if (symbol == ',' || symbol == ']'){ // должны добавить в корневой вектор, если это обычный элемент
                if (item.isEmpty())
                    continue;
                GroupItem child = GroupItem(word);
                item.addChild(child);
                word = "";
                if (symbol == ']'){
                    flag_arr = false;
                }
                if (!flag_arr){
                    groupRoot.push_back(item);
                    item.clear();
                }
                continue;
            }
            if (symbol == '{'){
                int end_i = bracketPosition(reqText, i);
                string childText = reqText.substr(i - 1, end_i - i + 2);
                vector<GroupItem> children = getChildren(childText); // рекурсия

                item.addChild(children);
                if (!flag_arr){
                    groupRoot.push_back(item);
                    item.clear();
                }
                i = end_i; // Переносимся под конец пройденного текста
                continue;
            }
            if (symbol == '}'){
                continue;
            }
            if (symbol == '['){
                flag_arr = true;
                continue;
            }
            word += symbol;
        }
        /* В случае, если нет запятой в конце текста */
        if (word != ""){
            item.addChild(word);
            groupRoot.push_back(item);
        }
        return groupRoot;
    }

    bool isJSONFile(string path){
        bool flag = true;
        if ((int)path.find(".json") == -1)
            flag = false;
        return flag;
    }

public:
    JsonData(string path){
        /* Конструктор. Считывает данные из файла и записывает */
        ifstream myFile(path);
        if (!myFile.is_open() && isJSONFile(path)){
            error = ErrorController();
            return;
        }
        string line = "";
        jsonText = "";
        while (getline(myFile, line)){
            jsonText += line + '\n';
        }
        error = ErrorController(jsonText);
        if (error.isOk())
            rootGroupItems = getChildren(jsonText);
    }

    vector<GroupItem> getData(){
        vector<GroupItem> empty_data;
        if (!error.isOk())
            return empty_data;
        return rootGroupItems;
    }

    string getJsonText(){
        return jsonText;
    }

    int rootSize(){
        return rootGroupItems.size();
    }

    GroupItem at(int row){
        GroupItem empty_data;
        if (!error.isOk() || (row < 0 || row >= (int)rootGroupItems.size()))
            return empty_data;
        return rootGroupItems.at(row);
    }

    string getCodText(){
        return error.getCodText();
    }

    bool isError(){
        return error.getCod() != 200;
    }

    int getLineError(){
        return error.getErrorLine();
    }

    int getColError(){
        return error.getErrorCol();
    }
};

#endif // JSONDATA_H
