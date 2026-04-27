#ifndef NODE_H
#define NODE_H

template <class T>
class Node {
public:
    T data;
    Node<T>* prev;
    Node<T>* next;

    Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};

#endif
