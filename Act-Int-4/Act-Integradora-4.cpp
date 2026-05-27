/**
 * Actividad Integradora 4
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
 * Autor:
 * Carlos Alberto Perez Diaz
 *
 * Matricula:
 * A01204060
 *
 * Fuentes consultadas:
 *    - Codigos vistos en clase por el profesor:
 *      Graph, Heap, DFS, BFS y Dijkstra.
 *    - cppreference. std::vector, std::map, std::priority_queue,
 *      std::ifstream y std::ofstream.
 *      https://en.cppreference.com/
 *    - ChatGPT. Apoyo para organizar, documentar y adaptar el codigo.
 *      https://chatgpt.com/share/6a1656ec-dfe8-83e8-ac4a-226ef215e5eb
 */

#include <iostream>
#include "Graph.h"

int main() {
    try {
        Graph grafo;

        grafo.loadBitacora("bitacoraGrafos.txt");

        grafo.guardarGrados("grados_ips.txt");

        std::string botMaster = grafo.guardarTop5Grados("mayores_grados_ips.txt");

        std::cout << "Bot master presumible: " << botMaster << std::endl;

        std::string primeraConexion = grafo.primeraConexionBotMaster(botMaster);

        std::cout << "Primera conexion del bot master: "
            << primeraConexion << std::endl;

        grafo.dijkstra(botMaster);

        grafo.guardarDistancias("distancia_botmaster.txt");

        std::string ipMayorEsfuerzo = grafo.ipMayorEsfuerzo();

        std::cout << "IP que requiere mayor esfuerzo para ser atacada: "
            << ipMayorEsfuerzo << std::endl;

        grafo.guardarCaminoAtaque(ipMayorEsfuerzo, "ataque_botmaster.txt");

        std::cout << "Archivos generados correctamente." << std::endl;
    }
    catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}