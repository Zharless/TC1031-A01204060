#ifndef RESUMEN_IP_H
#define RESUMEN_IP_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ResumenIP {
private:
    std::string ip;
    int salientes;
    int entrantes;
    double relacion;
    std::vector<std::string> accesadas;

    // O(1)
    unsigned long long ipToNumber(const std::string& ipValue) const {
        std::stringstream ss(ipValue);
        std::string parte;
        unsigned long long resultado = 0;

        for (int i = 0; i < 4; i++) {
            getline(ss, parte, '.');
            resultado = resultado * 256 + static_cast<unsigned long long>(stoul(parte));
        }

        return resultado;
    }

public:
    // O(1)
    ResumenIP() {
        ip = "";
        salientes = 0;
        entrantes = 0;
        relacion = 0.0;
    }

    // O(k log k), k = cantidad de direcciones accesadas
    ResumenIP(const std::string& ipValue, int outValue, int inValue,
        const std::vector<std::string>& listaAccesadas) {
        ip = ipValue;
        salientes = outValue;
        entrantes = inValue;
        accesadas = listaAccesadas;

        if (entrantes == 0) {
            relacion = static_cast<double>(salientes);
        }
        else {
            relacion = static_cast<double>(salientes) / static_cast<double>(entrantes);
        }

        std::sort(accesadas.begin(), accesadas.end(),
            [this](const std::string& a, const std::string& b) {
                return ipToNumber(a) > ipToNumber(b);
            }
        );
    }

    // O(1)
    std::string getIp() const {
        return ip;
    }

    // O(k), k = cantidad de direcciones accesadas
    void print() const {
        std::cout << "IP: " << ip << std::endl;
        std::cout << "Total de direcciones accesadas desde la IP: "
            << salientes << std::endl;
        std::cout << "Total de direcciones que intentaron acceder a la IP: "
            << entrantes << std::endl;
        std::cout << "Relacion entre el numero de direcciones accesadas y que intentaron acceder a la IP: "
            << std::fixed << std::setprecision(2) << relacion << std::endl;
        std::cout << "Lista de direcciones accesadas:" << std::endl;

        for (size_t i = 0; i < accesadas.size(); i++) {
            std::cout << accesadas[i] << std::endl;
        }
    }
};

#endif
