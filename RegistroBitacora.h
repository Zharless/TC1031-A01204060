#ifndef REGISTROBITACORA_H
#define REGISTROBITACORA_H

#include <string>
#include <sstream>
#include <stdexcept>

class RegistroBitacora {
private:
    std::string mes;
    int dia;
    std::string hora;
    std::string ipPuerto;
    std::string razon;

    int convertirMesANumero() const {
        if (mes == "Jan") return 1;
        if (mes == "Feb") return 2;
        if (mes == "Mar") return 3;
        if (mes == "Apr") return 4;
        if (mes == "May") return 5;
        if (mes == "Jun") return 6;
        if (mes == "Jul") return 7;
        if (mes == "Aug") return 8;
        if (mes == "Sep") return 9;
        if (mes == "Oct") return 10;
        if (mes == "Nov") return 11;
        if (mes == "Dec") return 12;

        throw std::out_of_range("Mes invalido");
    }

    void obtenerHoraMinutoSegundo(int& h, int& m, int& s) const {
        h = std::stoi(hora.substr(0, 2));
        m = std::stoi(hora.substr(3, 2));
        s = std::stoi(hora.substr(6, 2));
    }

public:
    RegistroBitacora()
        : mes(""), dia(0), hora(""), ipPuerto(""), razon("") {
    }

    RegistroBitacora(const std::string& mes, int dia, const std::string& hora,
        const std::string& ipPuerto, const std::string& razon)
        : mes(mes), dia(dia), hora(hora), ipPuerto(ipPuerto), razon(razon) {
    }

    std::string getMes() const { return mes; }
    int getDia() const { return dia; }
    std::string getHora() const { return hora; }
    std::string getIpPuerto() const { return ipPuerto; }
    std::string getRazon() const { return razon; }

    std::string toString() const {
        std::ostringstream oss;
        oss << mes << " " << dia << " " << hora << " " << ipPuerto << " " << razon;
        return oss.str();
    }

    long long getClaveFecha() const {
        int numeroMes = convertirMesANumero();
        int h, m, s;
        obtenerHoraMinutoSegundo(h, m, s);

        return static_cast<long long>(numeroMes) * 100000000LL +
            static_cast<long long>(dia) * 1000000LL +
            static_cast<long long>(h) * 10000LL +
            static_cast<long long>(m) * 100LL +
            static_cast<long long>(s);
    }

    bool operator<(const RegistroBitacora& other) const {
        return getClaveFecha() < other.getClaveFecha();
    }

    bool operator>(const RegistroBitacora& other) const {
        return getClaveFecha() > other.getClaveFecha();
    }

    bool operator==(const RegistroBitacora& other) const {
        return getClaveFecha() == other.getClaveFecha();
    }

    bool operator<=(const RegistroBitacora& other) const {
        return getClaveFecha() <= other.getClaveFecha();
    }

    bool operator>=(const RegistroBitacora& other) const {
        return getClaveFecha() >= other.getClaveFecha();
    }
};

#endif