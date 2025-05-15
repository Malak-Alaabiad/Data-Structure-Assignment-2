#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define Template template <typename T>

Template
class Queue {
private:
    Vector<T> data;

public:
    Queue() {}

    void push(const T& value) {
        data.push_back(value);
    }

    void pop() {
        if (data.empty()) {
            throw out_of_range("Queue is empty");
        }
        for (size_t i = 1; i < data.size(); ++i) {
            data[i - 1] = data[i];
        }
        data.pop_back();
    }

    T& front() {
        if (data.empty()) {
            throw out_of_range("Queue is empty");
        }
        return data[0];
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};


#endif