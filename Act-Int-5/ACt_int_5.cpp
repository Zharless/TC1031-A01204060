/**
 * Actividad Integradora 5
 *
 * Descripcion:
 * Este programa lee una bitacora de accesos, construye un grafo dirigido
 * organizado por IP y genera una tabla hash con direccion abierta y prueba
 * cuadratica. Cada valor de la tabla hash almacena un resumen de la informacion
 * de cada IP.
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
 * Alumo:
 * Carlos Alberto Perez Diaz
 *
 * Matricula:
 * A01204060
 *
 * Fuentes consultadas:
 *    Material visto en clase: HashTable con direccion abierta y prueba cuadratica.
 *    Material visto en clase: Graph y listas de adyacencia.
 *    cppreference.com: std::vector, std::map, std::ifstream, std::ofstream.
 *    ChatGPT: apoyo para documentacion, organizacion y revision de warnings.
 *    https://chatgpt.com/share/6a1fa659-cd30-83e8-a666-fed923c52504
 */

#include <iostream>
#include <string>
#include "Graph.h"

int main() {
    try {
        Graph graph;

        graph.loadGraph("bitacoraGrafos.txt");

        graph.buildHashTable(20011);

        std::cout << "Total de colisiones generadas: "
            << graph.getColisiones() << std::endl;

        std::string ip;

        std::cout << "Ingresa la IP a consultar: ";
        std::cin >> ip;

        while (!graph.ipExists(ip)) {
            std::cout << "La IP no existe en la bitacora. Ingresa otra IP: ";
            std::cin >> ip;
        }

        graph.getIPSummary(ip);
    }
    catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}