#ifndef REGISTRO_H
#define REGISTRO_H

#include <sstream>
#include <string>

class Registro {
private:
    std::string lineaOriginal;
    std::string mes;
    std::string dia;
    std::string hora;
    std::string ipCompleta;
    std::string ipSinPuerto;
    unsigned int ipNumerica;

    unsigned int convertirIP(const std::string& ip) {
        std::stringstream ss(ip);
        std::string parte;
        unsigned int resultado = 0;

        for (int i = 0; i < 4; i++) {
            getline(ss, parte, '.');
            resultado = resultado * 256 + static_cast<unsigned int>(stoi(parte));
        }

        return resultado;
    }

public:
    Registro() {
        lineaOriginal = "";
        ipSinPuerto = "";
        ipNumerica = 0;
    }

    Registro(const std::string& linea) {
        lineaOriginal = linea;

        std::stringstream ss(linea);
        ss >> mes >> dia >> hora >> ipCompleta;

        size_t pos = ipCompleta.find(':');

        if (pos != std::string::npos) {
            ipSinPuerto = ipCompleta.substr(0, pos);
        }
        else {
            ipSinPuerto = ipCompleta;
        }

        ipNumerica = convertirIP(ipSinPuerto);
    }

    std::string getLineaOriginal() const {
        return lineaOriginal;
    }

    std::string getIP() const {
        return ipSinPuerto;
    }

    unsigned int getIPNumerica() const {
        return ipNumerica;
    }

    bool operator<(const Registro& otro) const {
        return ipNumerica < otro.ipNumerica;
    }

    bool operator>(const Registro& otro) const {
        return ipNumerica > otro.ipNumerica;
    }
};

#endif