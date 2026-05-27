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
    std::string razon;

    std::string quitarPuerto(const std::string& ipPuerto) {
        size_t pos = ipPuerto.find(':');

        if (pos != std::string::npos) {
            return ipPuerto.substr(0, pos);
        }

        return ipPuerto;
    }

public:
    Registro() {
        mes = "";
        dia = "";
        hora = "";
        ipOrigen = "";
        ipDestino = "";
        peso = 0;
        razon = "";
    }

    Registro(const std::string& linea) {
        std::stringstream ss(linea);
        std::string origenConPuerto;
        std::string destinoConPuerto;

        ss >> mes >> dia >> hora >> origenConPuerto >> destinoConPuerto >> peso;

        getline(ss, razon);

        ipOrigen = quitarPuerto(origenConPuerto);
        ipDestino = quitarPuerto(destinoConPuerto);
    }

    std::string getFechaCompleta() const {
        return mes + " " + dia + " " + hora;
    }

    std::string getIpOrigen() const {
        return ipOrigen;
    }

    std::string getIpDestino() const {
        return ipDestino;
    }

    int getPeso() const {
        return peso;
    }
};

#endif