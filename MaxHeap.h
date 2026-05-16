#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <vector>
#include <stdexcept>
#include <cstddef>

template <class T>
class MaxHeap {
private:
    std::vector<T> datos;

    size_t padre(size_t i) const {
        return (i - 1) / 2;
    }

    size_t hijoIzq(size_t i) const {
        return (2 * i) + 1;
    }

    size_t hijoDer(size_t i) const {
        return (2 * i) + 2;
    }

    void intercambia(size_t i, size_t j) {
        T temp = datos[i];
        datos[i] = datos[j];
        datos[j] = temp;
    }

    void acomodaArriba(size_t i) {
        while (i > 0 && datos[i] > datos[padre(i)]) {
            intercambia(i, padre(i));
            i = padre(i);
        }
    }

    void acomodaAbajo(size_t i) {
        size_t mayor = i;
        size_t izq = hijoIzq(i);
        size_t der = hijoDer(i);

        if (izq < datos.size() && datos[izq] > datos[mayor]) {
            mayor = izq;
        }

        if (der < datos.size() && datos[der] > datos[mayor]) {
            mayor = der;
        }

        if (mayor != i) {
            intercambia(i, mayor);
            acomodaAbajo(mayor);
        }
    }

public:
    MaxHeap() {
    }

    void push(const T& valor) {
        datos.push_back(valor);
        acomodaArriba(datos.size() - 1);
    }

    void pop() {
        if (isEmpty()) {
            throw std::out_of_range("El heap esta vacio");
        }

        datos[0] = datos[datos.size() - 1];
        datos.pop_back();

        if (!isEmpty()) {
            acomodaAbajo(0);
        }
    }

    T getTop() const {
        if (isEmpty()) {
            throw std::out_of_range("El heap esta vacio");
        }

        return datos[0];
    }

    bool isEmpty() const {
        return datos.empty();
    }

    size_t getCurrentSize() const {
        return datos.size();
    }
};

#endif