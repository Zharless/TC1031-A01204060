/**
 * Actividad Integradora 3
 * Estructuras de Datos
 *
 * Descripcion:
 * Este programa lee una bitacora de accesos desde un archivo de texto,
 * ordena los registros por direccion IP utilizando Heap Sort,
 * contabiliza los accesos por IP y utiliza un Binary Heap
 * para obtener las IPs con mayor numero de accesos.
 *
 * Autor:
 * Carlos Alberto Perez Diaz
 *
 * Matricula:
 * A01204060
 *
 * Fecha:
 * Mayo 2026
 *
 * Compilacion para debug:
 *    g++ -std=c++17 -Wall -g -o main *.cpp
 *
 * Ejecucion con valgrind:
 *    valgrind --leak-check=full ./main
 *
 * Compilacion para ejecucion:
 *    g++ -std=c++17 -Wall -O3 -o main *.cpp
 *
 * Ejecucion:
 *    ./main
 *
 *
 * Fuentes consultadas:
 *
 * - Material y ejemplos vistos en clase:
 *   QueueLL, StackLL, BST, SplayTree y MaxHeap.
 *
 * - GeeksforGeeks. Binary Heap.
 *   https://www.geeksforgeeks.org/dsa/binary-heap/
 *
 * - GeeksforGeeks. Heap Sort.
 *   https://www.geeksforgeeks.org/dsa/heap-sort/
 *
 * - cppreference. std::vector.
 *   https://en.cppreference.com/w/cpp/container/vector
 *
 * - cppreference. std::fstream.
 *   https://en.cppreference.com/w/cpp/io/basic_fstream
 *
 * - cppreference. size_t.
 *   https://en.cppreference.com/w/cpp/types/size_t
 *
 * - C++ STL notes sobre size_type y size_t:
 *   https://www.dre.vanderbilt.edu/~schmidt/qualcomm/slides/STL
 *
 * - Weiss, Mark Allen.
 *   Data Structures and Algorithm Analysis in C++.
 *
 * - Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
 *   Introduction to Algorithms (3rd Edition).
 *
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstddef>
#include "Registro.h"
#include "MaxHeap.h"

struct AccesoIP {
    std::string ip;
    int accesos;

    bool operator>(const AccesoIP& otro) const {
        return accesos > otro.accesos;
    }

    bool operator<(const AccesoIP& otro) const {
        return accesos < otro.accesos;
    }
};

void heapify(std::vector<Registro>& registros, size_t n, size_t i) {
    size_t mayor = i;
    size_t izq = 2 * i + 1;
    size_t der = 2 * i + 2;

    if (izq < n && registros[izq] > registros[mayor]) {
        mayor = izq;
    }

    if (der < n && registros[der] > registros[mayor]) {
        mayor = der;
    }

    if (mayor != i) {
        Registro temp = registros[i];
        registros[i] = registros[mayor];
        registros[mayor] = temp;

        heapify(registros, n, mayor);
    }
}

void heapSort(std::vector<Registro>& registros) {
    size_t n = registros.size();

    if (n <= 1) {
        return;
    }

    for (size_t i = n / 2; i > 0; i--) {
        heapify(registros, n, i - 1);
    }

    for (size_t i = n - 1; i > 0; i--) {
        Registro temp = registros[0];
        registros[0] = registros[i];
        registros[i] = temp;

        heapify(registros, i, 0);
    }
}

void leerBitacora(std::vector<Registro>& registros, const std::string& nombreArchivo) {
    std::ifstream archivoEntrada(nombreArchivo);

    if (!archivoEntrada.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo de entrada");
    }

    std::string linea;

    while (getline(archivoEntrada, linea)) {
        if (!linea.empty()) {
            registros.push_back(Registro(linea));
        }
    }

    archivoEntrada.close();
}

void guardarBitacoraOrdenada(const std::vector<Registro>& registros, const std::string& nombreArchivo) {
    std::ofstream archivoSalida(nombreArchivo);

    if (!archivoSalida.is_open()) {
        throw std::runtime_error("No se pudo crear el archivo de salida");
    }

    for (size_t i = 0; i < registros.size(); i++) {
        archivoSalida << registros[i].getLineaOriginal() << std::endl;
    }

    archivoSalida.close();
}

std::vector<AccesoIP> contarAccesosPorIP(const std::vector<Registro>& registros) {
    std::vector<AccesoIP> accesos;

    if (registros.empty()) {
        return accesos;
    }

    std::string ipActual = registros[0].getIP();
    int contador = 1;

    for (size_t i = 1; i < registros.size(); i++) {
        if (registros[i].getIP() == ipActual) {
            contador++;
        }
        else {
            accesos.push_back({ ipActual, contador });
            ipActual = registros[i].getIP();
            contador = 1;
        }
    }

    accesos.push_back({ ipActual, contador });

    return accesos;
}

void llenarHeapAccesos(MaxHeap<AccesoIP>& heapAccesos, const std::vector<AccesoIP>& accesos) {
    for (size_t i = 0; i < accesos.size(); i++) {
        heapAccesos.push(accesos[i]);
    }
}

void obtenerTop10(MaxHeap<AccesoIP>& heapAccesos, const std::string& nombreArchivo) {
    std::ofstream archivoSalida(nombreArchivo);

    if (!archivoSalida.is_open()) {
        throw std::runtime_error("No se pudo crear el archivo de IPs con mayor acceso");
    }

    std::cout << std::endl;
    std::cout << "Las 10 IPs con mayor acceso son:" << std::endl;

    for (int i = 0; i < 10 && !heapAccesos.isEmpty(); i++) {
        AccesoIP actual = heapAccesos.getTop();
        heapAccesos.pop();

        std::cout << actual.ip << " " << actual.accesos << std::endl;
        archivoSalida << actual.ip << " " << actual.accesos << std::endl;
    }

    archivoSalida.close();
}

bool buscarMenorConMinimoTres(const std::vector<AccesoIP>& accesos, AccesoIP& menorValido) {
    bool encontrado = false;

    for (size_t i = 0; i < accesos.size(); i++) {
        if (accesos[i].accesos >= 3) {
            if (!encontrado || accesos[i].accesos < menorValido.accesos) {
                menorValido = accesos[i];
                encontrado = true;
            }
        }
    }

    return encontrado;
}

int main() {
    std::vector<Registro> registros;

    try {
        leerBitacora(registros, "bitacoraHeap.txt");

        std::cout << "Registros leidos: " << registros.size() << std::endl;

        heapSort(registros);

        guardarBitacoraOrdenada(registros, "bitacora_ordenada.txt");
        std::cout << "Archivo bitacora_ordenada.txt generado." << std::endl;

        std::vector<AccesoIP> accesos = contarAccesosPorIP(registros);

        MaxHeap<AccesoIP> heapAccesos;
        llenarHeapAccesos(heapAccesos, accesos);

        obtenerTop10(heapAccesos, "ips_con_mayor_acceso.txt");

        std::cout << std::endl;
        std::cout << "Archivo ips_con_mayor_acceso.txt generado." << std::endl;

        AccesoIP menorValido;
        bool encontrado = buscarMenorConMinimoTres(accesos, menorValido);

        std::cout << std::endl;

        if (encontrado) {
            std::cout << "IP con menor numero de accesos mayor o igual a 3:" << std::endl;
            std::cout << menorValido.ip << " " << menorValido.accesos << std::endl;
        }
        else {
            std::cout << "No se encontro una IP con 3 o mas accesos." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}