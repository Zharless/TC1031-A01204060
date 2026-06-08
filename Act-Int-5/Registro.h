#ifndef REGISTRO_H
#define REGISTRO_H

#include <sstream>
#include <string>

class Registro {
private:
    std::string mes;
    std::string dia;
    std::string hora;
    std::string ipOrigen;
    std::string ipDestino;
    int peso;

    // O(p), p = longitud de la IP con puerto
    std::string quitarPuerto(const std::string& ipPuerto) {
        size_t pos = ipPuerto.find(':');

        if (pos != std::string::npos) {
            return ipPuerto.substr(0, pos);
        }

        return ipPuerto;
    }

public:
    // O(1)
    Registro() {
        mes = "";
        dia = "";
        hora = "";
        ipOrigen = "";
        ipDestino = "";
        peso = 0;
    }

    // O(l), l = longitud de la linea
    Registro(const std::string& linea) {
        std::stringstream ss(linea);
        std::string origenConPuerto;
        std::string destinoConPuerto;

        ss >> mes >> dia >> hora >> origenConPuerto >> destinoConPuerto >> peso;

        ipOrigen = quitarPuerto(origenConPuerto);
        ipDestino = quitarPuerto(destinoConPuerto);
    }

    // O(1)
    std::string getIpOrigen() const {
        return ipOrigen;
    }

    // O(1)
    std::string getIpDestino() const {
        return ipDestino;
    }

    // O(1)
    int getPeso() const {
        return peso;
    }
};

#endif
