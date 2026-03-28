/**
 * BitacoraProyecto.cpp
 *
 * Descripcion:
 * Este programa lee el archivo bitacoraData.txt, guarda los registros
 * en un vector, los ordena por fecha, genera bitacora_ordenada.txt,
 * realiza busquedas por fecha y encuentra el primer par de registros
 * con diferencia exacta de D dias.
 * Fuentes:
 * - https://www.geeksforgeeks.org/stringstream-c-applications/
 * - https://www.geeksforgeeks.org/stdstoi-function-in-cpp/
 * - https://www.geeksforgeeks.org/merge-sort/
 * - https://www.geeksforgeeks.org/binary-search/
 * - Apuntes de clase
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class Fecha {
public:
    std::string mes;
    int dia;
    int hora;
    int minuto;
    int segundo;

    Fecha() {
        mes = "";
        dia = 0;
        hora = 0;
        minuto = 0;
        segundo = 0;
    }

    Fecha(std::string m, int d, int h, int min, int s) {
        mes = m;
        dia = d;
        hora = h;
        minuto = min;
        segundo = s;
    }

    int mesNumero() const {
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
        return 0;
    }

    int diasAntesDelMes() const {
        int m = mesNumero();

        if (m == 1) return 0;
        if (m == 2) return 31;
        if (m == 3) return 59;
        if (m == 4) return 90;
        if (m == 5) return 120;
        if (m == 6) return 151;
        if (m == 7) return 181;
        if (m == 8) return 212;
        if (m == 9) return 243;
        if (m == 10) return 273;
        if (m == 11) return 304;
        if (m == 12) return 334;

        return 0;
    }

    long long aSegundos() const {
        long long dias = diasAntesDelMes() + (dia - 1);

        return dias * 24LL * 3600LL
            + hora * 3600LL
            + minuto * 60LL
            + segundo;
    }

    bool operator<(const Fecha& otra) const {
        return aSegundos() < otra.aSegundos();
    }

    bool operator==(const Fecha& otra) const {
        return mes == otra.mes &&
            dia == otra.dia &&
            hora == otra.hora &&
            minuto == otra.minuto &&
            segundo == otra.segundo;
    }

    std::string toString() const {
        std::stringstream ss;

        ss << mes << " ";

        if (dia < 10) {
            ss << "0";
        }
        ss << dia << " ";

        if (hora < 10) {
            ss << "0";
        }
        ss << hora << ":";

        if (minuto < 10) {
            ss << "0";
        }
        ss << minuto << ":";

        if (segundo < 10) {
            ss << "0";
        }
        ss << segundo;

        return ss.str();
    }
};

class Registro {
public:
    Fecha fecha;
    std::string ip;
    std::string mensaje;
    std::string lineaCompleta;

    Registro() {
    }

    Registro(Fecha f, std::string i, std::string m, std::string linea)
        : fecha(f), ip(i), mensaje(m), lineaCompleta(linea) {
    }
};

void merge(std::vector<Registro>& registros, int left, int mid, int right, long long& comparaciones) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Registro> L(n1);
    std::vector<Registro> R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = registros[left + i];
    }

    for (int j = 0; j < n2; j++) {
        R[j] = registros[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        comparaciones++;

        if (L[i].fecha < R[j].fecha) {
            registros[k] = L[i];
            i++;
        }
        else {
            registros[k] = R[j];
            j++;
        }

        k++;
    }

    while (i < n1) {
        registros[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        registros[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<Registro>& registros, int left, int right, long long& comparaciones) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(registros, left, mid, comparaciones);
        mergeSort(registros, mid + 1, right, comparaciones);
        merge(registros, left, mid, right, comparaciones);
    }
}

void guardarBitacoraOrdenada(const std::vector<Registro>& registros, const std::string& nombreArchivo) {
    std::ofstream salida(nombreArchivo);

    for (std::size_t i = 0; i < registros.size(); i++) {
        salida << registros[i].lineaCompleta << std::endl;
    }

    salida.close();
}

int busquedaBinaria(const std::vector<Registro>& registros, const Fecha& objetivo) {
    int left = 0;
    int right = (int)registros.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (registros[mid].fecha == objetivo) {
            return mid;
        }

        if (registros[mid].fecha < objetivo) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

bool buscarPrimerParConD(const std::vector<Registro>& registros, int D, int& idx1, int& idx2) {
    long long diferenciaObjetivo = (long long)D * 24LL * 3600LL;

    int i = 0;
    int j = 1;

    while (i < (int)registros.size() && j < (int)registros.size()) {
        if (i == j) {
            j++;
            continue;
        }

        long long t1 = registros[i].fecha.aSegundos();
        long long t2 = registros[j].fecha.aSegundos();
        long long diferencia = t2 - t1;

        if (diferencia == diferenciaObjetivo) {
            idx1 = i;
            idx2 = j;
            return true;
        }
        else if (diferencia < diferenciaObjetivo) {
            j++;
        }
        else {
            i++;
            if (i == j) {
                j++;
            }
        }
    }

    return false;
}

int main() {
    std::ifstream archivo("bitacoraData.txt");

    if (!archivo) {
        std::cout << "No se pudo abrir el archivo" << std::endl;
        return 1;
    }

    std::vector<Registro> registros;
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea == "") {
            continue;
        }

        std::stringstream ss(linea);

        std::string mes;
        int dia;
        std::string horaStr;
        std::string ip;

        ss >> mes >> dia >> horaStr >> ip;

        int h = std::stoi(horaStr.substr(0, 2));
        int m = std::stoi(horaStr.substr(3, 2));
        int s = std::stoi(horaStr.substr(6, 2));

        std::string mensaje;
        std::getline(ss, mensaje);

        if (!mensaje.empty() && mensaje[0] == ' ') {
            mensaje.erase(0, 1);
        }

        Fecha f(mes, dia, h, m, s);
        Registro r(f, ip, mensaje, linea);

        registros.push_back(r);
    }

    archivo.close();

    long long comparaciones = 0;

    if (!registros.empty()) {
        mergeSort(registros, 0, (int)registros.size() - 1, comparaciones);
    }

    guardarBitacoraOrdenada(registros, "bitacora_ordenada.txt");

    int B;
    std::cin >> B;

    std::ofstream resultados("resultados_busqueda.txt");

    int exitosas = 0;
    int noExitosas = 0;

    for (int i = 0; i < B; i++) {
        std::string mes;
        int dia;
        std::string horaStr;

        std::cin >> mes >> dia >> horaStr;

        int h = std::stoi(horaStr.substr(0, 2));
        int m = std::stoi(horaStr.substr(3, 2));
        int s = std::stoi(horaStr.substr(6, 2));

        Fecha buscada(mes, dia, h, m, s);

        int indice = busquedaBinaria(registros, buscada);

        if (indice != -1) {
            resultados << "[" << indice << "] " << registros[indice].lineaCompleta << std::endl;
            exitosas++;
        }
        else {
            resultados << "Fecha no encontrada: " << buscada.toString() << std::endl;
            noExitosas++;
        }
    }

    int D;
    std::cin >> D;

    resultados << "Busqueda del primer par de registros con " << D << " dias de diferencia: " << std::endl;

    int idx1 = -1;
    int idx2 = -1;

    bool encontrado = buscarPrimerParConD(registros, D, idx1, idx2);

    if (encontrado) {
        resultados << "[" << idx1 << "] " << registros[idx1].lineaCompleta << std::endl;
        resultados << "[" << idx2 << "] " << registros[idx2].lineaCompleta << std::endl;
    }
    else {
        resultados << "No existe dicho par de registros" << std::endl;
    }

    resultados.close();

    std::cout << "X Sort" << std::endl;
    std::cout << "Comparaciones: " << comparaciones << std::endl;
    std::cout << std::endl;
    std::cout << "Numero total de operaciones de busqueda por fecha solicitadas: " << B << ": " << std::endl;
    std::cout << "Numero total de busquedas exitosas: " << exitosas << std::endl;
    std::cout << "Numero total de busquedas no exitosas: " << noExitosas << std::endl;
    std::cout << std::endl;
    std::cout << "Busqueda del primer par de registros con " << D << " dias de diferencia: " << std::endl;

    if (encontrado) {
        std::cout << "[" << idx1 << "] " << registros[idx1].lineaCompleta << std::endl;
        std::cout << "[" << idx2 << "] " << registros[idx2].lineaCompleta << std::endl;
    }
    else {
        std::cout << "No existe dicho par de registros" << std::endl;
    }
    
    return 0;
    
}