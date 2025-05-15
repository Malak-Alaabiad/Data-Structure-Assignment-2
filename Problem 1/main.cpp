#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Node{
    string data;
    Node* next;
    Node(string& value){
        data = value;
        next = nullptr;
    }
};

class Stack{
private:
    Node* top;
    int Size;

public:
    Stack(): top(nullptr), Size(0) {}
    ~Stack(){
        while (!isEmpty()){
            pop();
        }
    }

    void push(string& value){
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        Size++;
    }

    string pop(){
        if (isEmpty()){
            return "";
        }
        Node* temp = top;
        string value = top->data;
        top = top->next;
        delete temp;
        Size--;
        return value;
    }

    string peek(){
        if (isEmpty()){
            return "";
        }
        return top->data;
    }

    int size(){
        return Size;
    }

    bool isEmpty(){
        return top == nullptr;
    }

};

class History{
private:
    Stack undoStack;
    Stack redoStack;
public:

    History(){}
    void visit(string& url){
        while (! redoStack.isEmpty()){
            redoStack.pop();
        }
        undoStack.push(url);
    }
    string redo(){
        if (redoStack.isEmpty()){
            return "";
        }
        string url = redoStack.pop();
        undoStack.push(url);
        return url;
    }
    string undo(){
        if (undoStack.size()<=1){
            return "";
        }
        string url = undoStack.pop();
        redoStack.push(url);
        return undoStack.peek();
    }
    string getCurrent(){
        return undoStack.peek();
    }
};

int main(){
    History browserHistory;
    ifstream inputFile("test.txt");
    if(!inputFile.is_open()){
        cout << "Error opening file." << endl;
        return 1;}
    string line;
    int step =1;
    while (getline(inputFile, line)){
        if(line.empty()){
            continue;
        }
        istringstream iss(line);
        string command;
        iss>>command;
        cout<< "Step " << step << ": "<<command;
        if (command == "visit"){
            string url;
            getline(iss, url);
            size_t first = url.find_first_not_of(" ");
            size_t last = url.find_last_not_of(" ");
            if(first != string::npos && last != string::npos){
                url = url.substr(first, last - first + 1);
            }
            browserHistory.visit(url);
            cout<<" "<<url<<endl;
            cout<< "current URL: " << browserHistory.getCurrent() << endl;
        }
        else if (command == "redo") {
            string result = browserHistory.redo();
            if (result.empty()) {
                cout << "cannot go forward"<<endl;
                cout<<"current URL: " << browserHistory.getCurrent() << endl;
            } else {
                cout << "success"<<endl;
                cout<<"current URL: " << result << endl;
            }
        }
        else if (command == "undo") {
            string result = browserHistory.undo();
            if (result.empty()) {
                cout << "cannot go back"<<endl;
                cout<<"current URL: " << browserHistory.getCurrent() << endl;
            } else {
                cout << "success"<<endl;
                cout<<"current URL: " << result << endl;
            }
        }
        else {
            cout << "unknown command" << endl;
        }
        cout << endl;
        step++;
    }
    inputFile.close();
    return 0;
}