#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Node.h"

template <class T>
class DoublyLinkedList {

private:
    Node<T>* head;
    Node<T>* tail;
    int numElements;
    long long comparaciones;
    Node<T>* split(Node<T>* headRef) {
        if (headRef == nullptr || headRef->next == nullptr) {
            return nullptr;
        }

        Node<T>* slow = headRef;
        Node<T>* fast = headRef;

        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        Node<T>* secondHalf = slow->next;
        slow->next = nullptr;

        if (secondHalf != nullptr) {
            secondHalf->prev = nullptr;
        }

        return secondHalf;
    }
    Node<T>* merge(Node<T>* first, Node<T>* second) {
        if (first == nullptr) {
            return second;
        }

        if (second == nullptr) {
            return first;
        }

        Node<T>* newHead = nullptr;
        Node<T>* current = nullptr;

        comparaciones++;
        if (first->data <= second->data) {
            newHead = first;
            first = first->next;
        }
        else {
            newHead = second;
            second = second->next;
        }

        newHead->prev = nullptr;
        current = newHead;

        while (first != nullptr && second != nullptr) {
            comparaciones++;

            if (first->data <= second->data) {
                current->next = first;
                first->prev = current;
                first = first->next;
            }
            else {
                current->next = second;
                second->prev = current;
                second = second->next;
            }

            current = current->next;
        }

        if (first != nullptr) {
            current->next = first;
            first->prev = current;
        }
        else if (second != nullptr) {
            current->next = second;
            second->prev = current;
        }

        return newHead;
    }
    Node<T>* mergeSortRec(Node<T>* headRef) {
        if (headRef == nullptr || headRef->next == nullptr) {
            return headRef;
        }

        Node<T>* secondHalf = split(headRef);

        headRef = mergeSortRec(headRef);
        secondHalf = mergeSortRec(secondHalf);

        return merge(headRef, secondHalf);
    }
    void updateTail() {
        tail = head;

        if (tail == nullptr) {
            return;
        }

        while (tail->next != nullptr) {
            tail = tail->next;
        }
    }

public:
    /*
     * Constructor
     */
    DoublyLinkedList() : head(nullptr), tail(nullptr), numElements(0), comparaciones(0) {}

    /*
     * Destructor
     */
    ~DoublyLinkedList() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
        numElements = 0;
    }

    DoublyLinkedList(const DoublyLinkedList<T>& other)
        : head(nullptr), tail(nullptr), numElements(0), comparaciones(0) {
        Node<T>* current = other.head;
        while (current != nullptr) {
            addLast(current->data);
            current = current->next;
        }
    }

    /*
     * Regresa true si la lista esta vacia
     */
    bool isEmpty() const {
        return numElements == 0;
    }

    /*
     * Regresa el numero de elementos
     */
    int getNumElements() const {
        return numElements;
    }
    long long getComparaciones() const {
        return comparaciones;
    }
    Node<T>* getHead() const {
        return head;
    }

    /*
     * Regresa el apuntador al ultimo nodo
     */
    Node<T>* getTail() const {
        return tail;
    }
    /*
 * Regresa el nodo en la posicion indicada
 */
    Node<T>* getNodeAt(int pos) const {
        if (pos < 0 || pos >= numElements) {
            throw std::out_of_range("Posicion fuera de rango");
        }

        Node<T>* current;

        if (pos < numElements / 2) {
            current = head;
            for (int i = 0; i < pos; i++) {
                current = current->next;
            }
        }
        else {
            current = tail;
            for (int i = numElements - 1; i > pos; i--) {
                current = current->prev;
            }
        }

        return current;
    }

    /*
     * Regresa el dato en la posicion indicada
     */
    T getDataAt(int pos) const {
        return getNodeAt(pos)->data;
    }
    /*
     * Inserta un elemento al final
     */
    void addLast(const T& value) {
        Node<T>* newNode = new Node<T>(value);

        if (isEmpty()) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        numElements++;
    }

    /*
 * Busca un elemento usando busqueda binaria sobre la lista ordenada
 * Regresa la posicion si lo encuentra, o -1 si no existe
 */
    int binarySearch(const T& target) const {
        int left = 0;
        int right = numElements - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            T midValue = getDataAt(mid);

            if (midValue == target) {
                return mid;
            }
            else if (midValue < target) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }

        return -1;
    }

    /*
     * Imprime todos los elementos de la lista
     */
    void printList() const {
        Node<T>* current = head;
        while (current != nullptr) {
            std::cout << current->data.toString() << std::endl;
            current = current->next;
        }
    }
    void printFirstN(int n) const {
        Node<T>* current = head;
        int count = 0;

        while (current != nullptr && count < n) {
            std::cout << current->data.toString() << std::endl;
            current = current->next;
            count++;
        }
    }
    /*
 * Guarda todos los elementos de la lista en un archivo
 */
    void saveToFile(const std::string& fileName) const {
        std::ofstream outFile(fileName);

        if (!outFile.is_open()) {
            throw std::out_of_range("No se pudo crear el archivo de salida");
        }

        Node<T>* current = head;

        while (current != nullptr) {
            outFile << current->data.toString() << std::endl;
            current = current->next;
        }

        outFile.close();
    }
    void mergeSort() {
        comparaciones = 0;
        head = mergeSortRec(head);

        if (head != nullptr) {
            head->prev = nullptr;
        }

        updateTail();
    }
    DoublyLinkedList<T> getRange(int startPos, int endPos) const {
        if (startPos < 0 || endPos < 0 || startPos >= numElements || endPos >= numElements || startPos > endPos) {
            throw std::out_of_range("Rango invalido");
        }

        DoublyLinkedList<T> result;

        for (int i = startPos; i <= endPos; i++) {
            result.addLast(getDataAt(i));
        }

        return result;
    }
    /*
 * Regresa una nueva lista con cada quinto elemento de la lista actual
 * Se conservan las posiciones 4, 9, 14, 19, ...
 */
    DoublyLinkedList<T> getEveryFifth() const {
        DoublyLinkedList<T> result;

        for (int i = 4; i < numElements; i += 5) {
            result.addLast(getDataAt(i));
        }

        return result;
    }
    /*
 * Operador de asignacion
 */
    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other) {
        if (this != &other) {
            Node<T>* current = head;
            while (current != nullptr) {
                Node<T>* temp = current;
                current = current->next;
                delete temp;
            }

            head = nullptr;
            tail = nullptr;
            numElements = 0;
            comparaciones = 0;

            current = other.head;
            while (current != nullptr) {
                addLast(current->data);
                current = current->next;
            }
        }

        return *this;
    }
};

#endif#pragma once
