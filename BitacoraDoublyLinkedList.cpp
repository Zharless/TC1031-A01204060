/**
 * Bitacora con Doubly Linked List
 *
 * Nombre: Carlos Alberto Pérez Díaz
 * Matricula: A01204060
 *
 * Compilacion para debug:
 *    g++ -std=c++17 -Wall -g -o main *.cpp
 *
 * Compilacion para ejecucion:
 *    g++ -std=c++17 -Wall -O3 -o main *.cpp
 *
 * Ejecucion:
 *    ./main
 *
 * Fuentes de consulta:
 * - GeeksforGeeks (implementaciones de listas doblemente ligadas y Merge Sort)
 *   https://www.geeksforgeeks.org/doubly-linked-list/
 *   https://www.geeksforgeeks.org/merge-sort-for-linked-list/
 *
 * - cppreference (documentación de C++)
 *   https://en.cppreference.com/
 **/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "RegistroBitacora.h"
#include "DoublyLinkedList.h"

RegistroBitacora leerFechaComoRegistro() {
    std::string mes;
    int dia;
    std::string hora;

    std::cin >> mes >> dia >> hora;

    return RegistroBitacora(mes, dia, hora, "", "");
}

int main() {
    DoublyLinkedList<RegistroBitacora> bitacora;
    std::ifstream archivo("bitacoraData.txt");

    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo abrir el archivo bitacoraData.txt" << std::endl;
        return 1;
    }

    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty()) {
            continue;
        }

        std::istringstream iss(linea);
        std::string mes;
        int dia;
        std::string hora;
        std::string ipPuerto;
        std::string razon;

        iss >> mes >> dia >> hora >> ipPuerto;
        std::getline(iss, razon);

        if (!razon.empty() && razon[0] == ' ') {
            razon = razon.substr(1);
        }

        RegistroBitacora registro(mes, dia, hora, ipPuerto, razon);
        bitacora.addLast(registro);
    }

    archivo.close();

    std::cout << "Merge Sort" << std::endl;
    bitacora.mergeSort();
    std::cout << "Comparaciones: " << bitacora.getComparaciones() << std::endl;

    try {
        bitacora.saveToFile("bitacora_ordenada.txt");
    }
    catch (const std::out_of_range& e) {
        std::cout << "Error al guardar bitacora_ordenada.txt: " << e.what() << std::endl;
        return 1;
    }

    RegistroBitacora fechaInicio;
    RegistroBitacora fechaFin;

    std::cout << "Ingrese la fecha de inicio de la busqueda (Jun 01 00:22:36): " << std::endl;
    fechaInicio = leerFechaComoRegistro();

    std::cout << "Ingrese la fecha de fin de la busqueda (Jun 01 08:23:57): " << std::endl;
    fechaFin = leerFechaComoRegistro();

    if (fechaInicio > fechaFin) {
        std::cout << "Error: la fecha de inicio debe ser menor o igual que la fecha de fin." << std::endl;
        return 1;
    }

    try {
        int posInicio = bitacora.binarySearch(fechaInicio);
        int posFin = bitacora.binarySearch(fechaFin);

        if (posInicio == -1 || posFin == -1) {
            throw std::out_of_range("Una o ambas fechas no existen en la bitacora");
        }

        DoublyLinkedList<RegistroBitacora> resultados = bitacora.getRange(posInicio, posFin);

        std::cout << "\nRegistros entre las fechas: " << std::endl;
        resultados.printList();
        resultados.saveToFile("resultado_busqueda.txt");

        DoublyLinkedList<RegistroBitacora> ataquesPeligrosos = resultados.getEveryFifth();

        std::cout << "\nRegistros de ataques mas peligrosos: " << std::endl;
        ataquesPeligrosos.printList();
        ataquesPeligrosos.saveToFile("ataques_mas_peligrosos.txt");
    }
    catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
