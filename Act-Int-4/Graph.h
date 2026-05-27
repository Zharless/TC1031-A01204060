#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Registro.h"
#include "MaxHeap.h"

class IpGrado {
private:
    std::string ip;
    int grado;

public:
    IpGrado() {
        ip = "";
        grado = 0;
    }

    IpGrado(const std::string& _ip, int _grado) {
        ip = _ip;
        grado = _grado;
    }

    std::string getIp() const {
        return ip;
    }

    int getGrado() const {
        return grado;
    }

    bool operator>(const IpGrado& otro) const {
        if (grado == otro.grado) {
            return ip > otro.ip;
        }

        return grado > otro.grado;
    }
};

class Graph {
private:
    int numIps;
    int numIncidencias;

    std::vector<std::string> ips;
    std::vector<std::vector<std::pair<int, int>>> adjList;
    std::vector<Registro> registros;
    std::map<std::string, int> ipIndex;

    std::vector<int> distancias;
    std::vector<int> predecesores;

    const int INF = 0x3f3f3f3f;

    int buscarIndiceIP(const std::string& ip) const {
        std::map<std::string, int>::const_iterator it = ipIndex.find(ip);

        if (it == ipIndex.end()) {
            return -1;
        }

        return it->second;
    }

public:
    Graph() {
        numIps = 0;
        numIncidencias = 0;
    }

    // O(n + m)
    void loadBitacora(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo de entrada");
        }

        archivo >> numIps >> numIncidencias;
        archivo.ignore();

        ips.resize(static_cast<size_t>(numIps));
        adjList.resize(static_cast<size_t>(numIps));

        for (int i = 0; i < numIps; i++) {
            getline(archivo, ips[static_cast<size_t>(i)]);
            ipIndex[ips[static_cast<size_t>(i)]] = i;
        }

        std::string linea;

        for (int i = 0; i < numIncidencias; i++) {
            getline(archivo, linea);

            if (linea.empty()) {
                i--;
                continue;
            }

            Registro registro(linea);
            registros.push_back(registro);

            int origen = buscarIndiceIP(registro.getIpOrigen());
            int destino = buscarIndiceIP(registro.getIpDestino());

            if (origen != -1 && destino != -1) {
                adjList[static_cast<size_t>(origen)].push_back(
                    std::make_pair(destino, registro.getPeso())
                );
            }
        }

        archivo.close();
    }

    // O(n)
    void guardarGrados(const std::string& nombreArchivo) {
        std::ofstream salida(nombreArchivo);

        if (!salida.is_open()) {
            throw std::runtime_error("No se pudo crear grados_ips.txt");
        }

        for (int i = 0; i < numIps; i++) {
            int grado = static_cast<int>(adjList[static_cast<size_t>(i)].size());
            salida << ips[static_cast<size_t>(i)] << " " << grado << std::endl;
        }

        salida.close();
    }

    // O(n log n)
    std::string guardarTop5Grados(const std::string& nombreArchivo) {
        MaxHeap<IpGrado> heap;

        for (int i = 0; i < numIps; i++) {
            int grado = static_cast<int>(adjList[static_cast<size_t>(i)].size());
            heap.push(IpGrado(ips[static_cast<size_t>(i)], grado));
        }

        std::ofstream salida(nombreArchivo);

        if (!salida.is_open()) {
            throw std::runtime_error("No se pudo crear mayores_grados_ips.txt");
        }

        std::string botMaster = "";

        for (int i = 0; i < 5 && !heap.isEmpty(); i++) {
            IpGrado actual = heap.getTop();
            heap.pop();

            if (i == 0) {
                botMaster = actual.getIp();
            }

            salida << actual.getIp() << " " << actual.getGrado() << std::endl;
        }

        salida.close();

        return botMaster;
    }

    // O(m)
    std::string primeraConexionBotMaster(const std::string& botMaster) const {
        for (size_t i = 0; i < registros.size(); i++) {
            if (registros[i].getIpOrigen() == botMaster) {
                return registros[i].getFechaCompleta();
            }
        }

        return "No encontrada";
    }

    // O((n + m) log n)
    void dijkstra(const std::string& ipOrigen) {
        int origen = buscarIndiceIP(ipOrigen);

        if (origen == -1) {
            throw std::runtime_error("La IP origen no existe");
        }

        distancias.assign(static_cast<size_t>(numIps), INF);
        predecesores.assign(static_cast<size_t>(numIps), -1);

        std::priority_queue<
            std::pair<int, int>,
            std::vector<std::pair<int, int>>,
            std::greater<std::pair<int, int>>
        > pq;

        distancias[static_cast<size_t>(origen)] = 0;
        pq.push(std::make_pair(0, origen));

        while (!pq.empty()) {
            int nodoActual = pq.top().second;
            int distanciaActual = pq.top().first;
            pq.pop();

            size_t nodoIndex = static_cast<size_t>(nodoActual);

            if (distanciaActual > distancias[nodoIndex]) {
                continue;
            }

            for (size_t i = 0; i < adjList[nodoIndex].size(); i++) {
                int vecino = adjList[nodoIndex][i].first;
                int peso = adjList[nodoIndex][i].second;

                size_t vecinoIndex = static_cast<size_t>(vecino);

                if (distancias[vecinoIndex] > distancias[nodoIndex] + peso) {
                    distancias[vecinoIndex] = distancias[nodoIndex] + peso;
                    predecesores[vecinoIndex] = nodoActual;
                    pq.push(std::make_pair(distancias[vecinoIndex], vecino));
                }
            }
        }
    }

    // O(n)
    void guardarDistancias(const std::string& nombreArchivo) const {
        std::ofstream salida(nombreArchivo);

        if (!salida.is_open()) {
            throw std::runtime_error("No se pudo crear distancia_botmaster.txt");
        }

        for (int i = 0; i < numIps; i++) {
            size_t index = static_cast<size_t>(i);

            if (distancias[index] == INF) {
                salida << ips[index] << " INF" << std::endl;
            }
            else {
                salida << ips[index] << " " << distancias[index] << std::endl;
            }
        }

        salida.close();
    }

    // O(n)
    std::string ipMayorEsfuerzo() const {
        int mayorDistancia = -1;
        int indiceMayor = -1;

        for (int i = 0; i < numIps; i++) {
            size_t index = static_cast<size_t>(i);

            if (distancias[index] != INF && distancias[index] > mayorDistancia) {
                mayorDistancia = distancias[index];
                indiceMayor = i;
            }
        }

        if (indiceMayor == -1) {
            return "";
        }

        return ips[static_cast<size_t>(indiceMayor)];
    }

    // O(n)
    void guardarCaminoAtaque(const std::string& ipDestino, const std::string& nombreArchivo) const {
        int destino = buscarIndiceIP(ipDestino);

        if (destino == -1) {
            throw std::runtime_error("La IP destino no existe");
        }

        std::vector<int> camino;
        int actual = destino;

        while (actual != -1) {
            camino.push_back(actual);
            actual = predecesores[static_cast<size_t>(actual)];
        }

        std::reverse(camino.begin(), camino.end());

        std::ofstream salida(nombreArchivo);

        if (!salida.is_open()) {
            throw std::runtime_error("No se pudo crear ataque_botmaster.txt");
        }

        for (size_t i = 0; i < camino.size(); i++) {
            salida << ips[static_cast<size_t>(camino[i])] << std::endl;
        }

        salida.close();
    }
};

#endif
