#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <string>
#include <vector>
using namespace std;

class GroupItem{
private:
    string name;
    vector<GroupItem> children;
    int childrenCount;

public:
    GroupItem(){
        name = "";
        children.clear();
        childrenCount = 0;
    }

    GroupItem(string title){
        name = title;
        children.clear();
        childrenCount = 0;
    }

    void addChild(GroupItem newChild){ // Функция удочерения
        children.push_back(newChild);
        childrenCount++;
    }

    void addChild(vector<GroupItem> newVectorChilds){ // Функция удочерения близняшек ( перегрузка )
        for (GroupItem child : newVectorChilds){
            children.push_back(child);
            childrenCount++;
        }
    }

    void setName(string title){
        name = title;
    }

    string getName(){
        return name;
    }

    int getChildrenCount(){
        return childrenCount;
    }

    GroupItem getChild(int index){
        return children.at(index);
    }

    void clear(){
        name = "";
        children.clear();
        childrenCount = 0;
    }

    bool isEmpty(){
        if (name == "" && childrenCount == 0){
            return true;
        }
            return false;
    }
};

#endif // GROUPITEM_H
