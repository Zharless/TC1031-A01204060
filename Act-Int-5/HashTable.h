#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdexcept>
#include <string>
#include <vector>

template <class K, class T>
class HashNode {
private:
    K key;
    T data;
    int status;

public:
    // O(1)
    HashNode() {
        key = K();
        data = T();
        status = 0;
    }

    // O(1)
    K getKey() const {
        return key;
    }

    // O(1)
    T getData() const {
        return data;
    }

    // O(1)
    int getStatus() const {
        return status;
    }

    // O(1)
    void setData(const K& keyValue, const T& dataValue) {
        key = keyValue;
        data = dataValue;
        status = 1;
    }

    // O(1)
    void clearData() {
        key = K();
        data = T();
        status = 2;
    }
};

template <class K, class T>
class HashTable {
private:
    std::vector<HashNode<K, T>> table;
    int maxSize;
    int numElements;
    unsigned int colisiones;

    // O(s), s = longitud de la llave string
    int hashString(const std::string& keyValue) const {
        unsigned long long hash = 0;

        for (size_t i = 0; i < keyValue.size(); i++) {
            hash = (hash * 131 + static_cast<unsigned long long>(keyValue[i])) %
                static_cast<unsigned long long>(maxSize);
        }

        return static_cast<int>(hash);
    }

public:
    // O(1)
    HashTable() {
        maxSize = 0;
        numElements = 0;
        colisiones = 0;
    }

    // O(n)
    HashTable(int selectedSize) {
        maxSize = selectedSize;
        numElements = 0;
        colisiones = 0;
        table.resize(static_cast<size_t>(maxSize));
    }

    // O(1)
    void setMaxSize(int selectedSize) {
        maxSize = selectedSize;
        numElements = 0;
        colisiones = 0;
        table.clear();
        table.resize(static_cast<size_t>(maxSize));
    }

    // O(s), s = longitud de la llave
    int getHashIndex(const K& keyValue) const {
        return hashString(keyValue);
    }

    // O(1) promedio, O(n) peor caso
    void add(const K& keyValue, const T& dataValue) {
        if (numElements == maxSize) {
            throw std::out_of_range("La tabla hash esta llena");
        }

        if (findIndex(keyValue) != -1) {
            throw std::out_of_range("La llave ya existe en la tabla hash");
        }

        int hashIndex = getHashIndex(keyValue);
        int i = 0;
        int currentIndex = hashIndex;

        while (table[static_cast<size_t>(currentIndex)].getStatus() == 1) {
            colisiones++;
            i++;
            currentIndex = (hashIndex + i * i) % maxSize;
        }

        table[static_cast<size_t>(currentIndex)].setData(keyValue, dataValue);
        numElements++;
    }

    // O(1) promedio, O(n) peor caso
    int findIndex(const K& keyValue) const {
        if (maxSize == 0) {
            return -1;
        }

        int hashIndex = getHashIndex(keyValue);
        int i = 0;
        int currentIndex = hashIndex;

        while (i < maxSize) {
            int status = table[static_cast<size_t>(currentIndex)].getStatus();

            if (status == 0) {
                return -1;
            }

            if (status == 1 &&
                table[static_cast<size_t>(currentIndex)].getKey() == keyValue) {
                return currentIndex;
            }

            i++;
            currentIndex = (hashIndex + i * i) % maxSize;
        }

        return -1;
    }

    // O(1) promedio, O(n) peor caso
    T find(const K& keyValue) const {
        int index = findIndex(keyValue);

        if (index == -1) {
            throw std::out_of_range("La llave no existe en la tabla hash");
        }

        return table[static_cast<size_t>(index)].getData();
    }

    // O(1) promedio, O(n) peor caso
    void remove(const K& keyValue) {
        int index = findIndex(keyValue);

        if (index == -1) {
            throw std::out_of_range("La llave no existe en la tabla hash");
        }

        table[static_cast<size_t>(index)].clearData();
        numElements--;
    }

    // O(1)
    int getNumElements() const {
        return numElements;
    }

    // O(1)
    unsigned int getColisiones() const {
        return colisiones;
    }

    // O(1)
    double getAlfa() const {
        if (maxSize == 0) {
            return 0.0;
        }

        return static_cast<double>(numElements) / static_cast<double>(maxSize);
    }
};

#endif
