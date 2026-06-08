#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "HashTable.h"
#include "Registro.h"
#include "ResumenIP.h"

class Graph {
private:
    int numIps;
    int numIncidencias;

    std::vector<std::string> ips;
    std::map<std::string, int> ipIndex;
    std::vector<std::vector<int>> adjList;
    std::vector<int> inDegree;

    HashTable<std::string, ResumenIP> hashTable;

    // O(log n)
    int getIndex(const std::string& ip) const {
        std::map<std::string, int>::const_iterator it = ipIndex.find(ip);

        if (it == ipIndex.end()) {
            return -1;
        }

        return it->second;
    }

public:
    // O(1)
    Graph() {
        numIps = 0;
        numIncidencias = 0;
    }

    // O(n + m)
    void loadGraph(const std::string& fileName) {
        std::ifstream file(fileName);

        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo de entrada");
        }

        file >> numIps >> numIncidencias;
        file.ignore();

        ips.resize(static_cast<size_t>(numIps));
        adjList.resize(static_cast<size_t>(numIps));
        inDegree.resize(static_cast<size_t>(numIps), 0);

        for (int i = 0; i < numIps; i++) {
            getline(file, ips[static_cast<size_t>(i)]);
            ipIndex[ips[static_cast<size_t>(i)]] = i;
        }

        std::string line;

        for (int i = 0; i < numIncidencias; i++) {
            getline(file, line);

            if (line.empty()) {
                i--;
                continue;
            }

            Registro registro(line);

            int origin = getIndex(registro.getIpOrigen());
            int destination = getIndex(registro.getIpDestino());

            if (origin != -1 && destination != -1) {
                adjList[static_cast<size_t>(origin)].push_back(destination);
                inDegree[static_cast<size_t>(destination)]++;
            }
        }

        file.close();
    }

    // O(n * k log k), k = maximo grado de salida
    void buildHashTable(int hashSize) {
        hashTable.setMaxSize(hashSize);

        for (int i = 0; i < numIps; i++) {
            std::vector<std::string> accesadas;
            size_t index = static_cast<size_t>(i);

            for (size_t j = 0; j < adjList[index].size(); j++) {
                int destination = adjList[index][j];
                accesadas.push_back(ips[static_cast<size_t>(destination)]);
            }

            int salientes = static_cast<int>(adjList[index].size());
            int entrantes = inDegree[index];

            ResumenIP resumen(ips[index], salientes, entrantes, accesadas);
            hashTable.add(ips[index], resumen);
        }
    }

    // O(1)
    unsigned int getColisiones() const {
        return hashTable.getColisiones();
    }

    // O(1) promedio, O(n) peor caso
    bool ipExists(const std::string& ip) const {
        return hashTable.findIndex(ip) != -1;
    }

    // O(k), k = cantidad de direcciones accesadas
    void getIPSummary(const std::string& ip) const {
        ResumenIP resumen = hashTable.find(ip);
        resumen.print();
    }
};

#endif
