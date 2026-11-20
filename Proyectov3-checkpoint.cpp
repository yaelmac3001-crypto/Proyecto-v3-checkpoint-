// proyectoV3.cpp : Sistema de Alumnos v3 = v2 + guardar, cargar y reporte
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

struct Alumno { string nombre; string matricula; float calif[3]; int grupo; };
struct Maestro { string nombre; string materia; };
struct Grupo { string nombre; string carrera; };

const int MAX = 100;
const char* ARCH_DATOS = "datos.txt";      
const char* ARCH_REPORTE = "reporte.txt";  

Alumno  al[MAX];  int nA = 0;
Grupo   gr[MAX];  int nG = 0;
Maestro ma[MAX];  int nM = 0;


float pedir(const char* msg, float a, float b) {
    float x;
    while (cout << msg, !(cin >> x) || x < a || x > b) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  Valor invalido (" << a << " a " << b << ")\n";
    }
    cin.ignore(1000, '\n');
    return x;
}

string texto(const char* msg) {
    string s;
    do { cout << msg; getline(cin, s); } while (s.empty());
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] == '|') s[i] = '/';
    return s;
}

bool confirmar(const char* msg) {
    string r = texto(msg);
    return r[0] == 's' || r[0] == 'S';
}

float promedio(const Alumno& a) {
    return (a.calif[0] + a.calif[1] + a.calif[2]) / 3;
}


void guardarDatos() {
    ofstream f(ARCH_DATOS);
    f << "GRUPOS " << nG << '\n';
    for (int i = 0; i < nG; i++)
        f << gr[i].nombre << '|' << gr[i].carrera << '\n';
    f << "MAESTROS " << nM << '\n';
    for (int i = 0; i < nM; i++)
        f << ma[i].nombre << '|' << ma[i].materia << '\n';
    f << "ALUMNOS " << nA << '\n';
    for (int i = 0; i < nA; i++)
        f << al[i].nombre << '|' << al[i].matricula << '|' << al[i].calif[0] << '|'
        << al[i].calif[1] << '|' << al[i].calif[2] << '|' << al[i].grupo << '\n';
    f.close();

    if (f.fail()) cout << "  ERROR: no se pudo escribir " << ARCH_DATOS << "\n";
    else cout << "  Datos guardados en " << ARCH_DATOS << " (" << nA << " alumnos, "
        << nG << " grupos, " << nM << " maestros)\n";
}


bool leerCabecera(ifstream& f, const char* etiqueta, int& n) {
    string t;
    if (!(f >> t >> n) || t != etiqueta || n < 0 || n > MAX) return false;
    f.ignore(1000, '\n');
    return true;
}


bool leerCampos(ifstream& f, string c[], int k) {
    string linea;
    if (!getline(f, linea)) return false;
    istringstream ss(linea);
    for (int i = 0; i < k; i++)
        if (!getline(ss, c[i], '|')) return false;
    return ss.eof();   
}


void cargarDatos(bool alIniciar) {
    ifstream f(ARCH_DATOS);
    if (!f) {
        cout << "  No hay datos guardados todavia"
            << (alIniciar ? " (primer uso). Se inicia vacio.\n"
                : ". Los datos actuales no cambian.\n");
        return;
    }

    Grupo tg[MAX];  Maestro tm[MAX];  Alumno ta[MAX];
    int g = 0, m = 0, a = 0;
    string c[6];

    bool ok = leerCabecera(f, "GRUPOS", g);
    for (int i = 0; ok && i < g; i++) {
        ok = leerCampos(f, c, 2);
        tg[i].nombre = c[0];  tg[i].carrera = c[1];
    }

    ok = ok && leerCabecera(f, "MAESTROS", m);
    for (int i = 0; ok && i < m; i++) {
        ok = leerCampos(f, c, 2);
        tm[i].nombre = c[0];  tm[i].materia = c[1];
    }

    ok = ok && leerCabecera(f, "ALUMNOS", a);
    for (int i = 0; ok && i < a; i++) {
        ok = leerCampos(f, c, 6);
        if (!ok) break;
        try {
            ta[i].nombre = c[0];
            ta[i].matricula = c[1];
            for (int j = 0; j < 3; j++) {
                ta[i].calif[j] = stof(c[2 + j]);
                if (ta[i].calif[j] < 0 || ta[i].calif[j] > 100) ok = false;
            }
            ta[i].grupo = stoi(c[5]);
            if (ta[i].grupo < 0 || ta[i].grupo >= g) ok = false;   
        }
        catch (...) { ok = false; }
    }

    if (!ok) {
        cout << "  El archivo " << ARCH_DATOS << " esta danado, no se cargo nada.\n";
        return;
    }

    for (int i = 0; i < g; i++) gr[i] = tg[i];
    for (int i = 0; i < m; i++) ma[i] = tm[i];
    for (int i = 0; i < a; i++) al[i] = ta[i];
    nG = g;  nM = m;  nA = a;
    cout << "  Datos cargados: " << nA << " alumnos, " << nG << " grupos, "
        << nM << " maestros\n";
}


void generarReporte() {
    if (nA == 0) { cout << "  No hay alumnos para el reporte\n"; return; }

   
    int orden[MAX];
    float prom[MAX];
    float suma = 0;
    for (int i = 0; i < nA; i++) { orden[i] = i; prom[i] = promedio(al[i]); suma += prom[i]; }
    for (int i = 1; i < nA; i++) {
        int clave = orden[i], j = i - 1;
        while (j >= 0 && prom[orden[j]] < prom[clave]) { orden[j + 1] = orden[j]; j--; }
        orden[j + 1] = clave;
    }

    ofstream f(ARCH_REPORTE);
    f << fixed << setprecision(2) << left;
    f << string(80, '=') << "\nREPORTE DE ALUMNOS ORDENADOS POR PROMEDIO\n" << string(80, '=') << "\n"
        << setw(4) << "#" << setw(12) << "Matricula" << setw(24) << "Nombre"
        << setw(12) << "Grupo" << setw(20) << "Carrera" << "Promedio\n"
        << string(80, '-') << "\n";
    for (int k = 0; k < nA; k++) {
        const Alumno& a = al[orden[k]];
        f << setw(4) << k + 1 << setw(12) << a.matricula.substr(0, 11)
            << setw(24) << a.nombre.substr(0, 23)
            << setw(12) << gr[a.grupo].nombre.substr(0, 11)
            << setw(20) << gr[a.grupo].carrera.substr(0, 19) << prom[orden[k]] << "\n";
    }
    f << string(80, '-') << "\n"
        << "Total de alumnos:  " << nA << "\n"
        << "Promedio general:  " << suma / nA << "\n"
        << "Promedio mas alto: " << al[orden[0]].nombre << " (" << prom[orden[0]] << ")\n"
        << "Promedio mas bajo: " << al[orden[nA - 1]].nombre << " (" << prom[orden[nA - 1]] << ")\n";
    f.close();

    if (f.fail()) cout << "  ERROR: no se pudo escribir " << ARCH_REPORTE << "\n";
    else cout << "  Reporte generado en " << ARCH_REPORTE << "\n";
}

int main() {
    int op;

    cout.setf(ios::fixed);
    cout.precision(2);

    cargarDatos(true);   

    do {
        cout << "\n1. Capturar alumnos\n2. Capturar grupos y carreras\n"
            "3. Capturar maestros\n4. Reportes\n"
            "5. Guardar datos\n6. Cargar datos\n7. Generar reporte a archivo\n8. Salir\n";
        op = (int)pedir("Opcion: ", 1, 8);

        if (op == 1) {
            if (nG == 0) { cout << "Primero captura grupos (opcion 2)\n"; continue; }
            if (nA == MAX) { cout << "Limite de alumnos alcanzado\n"; continue; }
            int n = (int)pedir("Cuantos alumnos? ", 1, MAX - nA);
            for (int i = 0; i < n; i++, nA++) {
                cout << "\nAlumno " << nA + 1 << "\n";
                al[nA].nombre = texto("  Nombre: ");
                al[nA].matricula = texto("  Matricula: ");
                for (int j = 0; j < 3; j++) {
                    cout << "  Calificacion " << j + 1 << " (0-100)";
                    al[nA].calif[j] = pedir(": ", 0, 100);
                }
                for (int g = 0; g < nG; g++) cout << "  " << g + 1 << ". " << gr[g].nombre << "\n";
                al[nA].grupo = (int)pedir("  Grupo: ", 1, nG) - 1;
            }
        }
        else if (op == 2) {
            if (nG == MAX) { cout << "Limite de grupos alcanzado\n"; continue; }
            int n = (int)pedir("Cuantos grupos? ", 1, MAX - nG);
            for (int i = 0; i < n; i++, nG++) {
                cout << "\nGrupo " << nG + 1 << "\n";
                gr[nG].nombre = texto("  Nombre del grupo: ");
                gr[nG].carrera = texto("  Carrera: ");
            }
        }
        else if (op == 3) {
            if (nM == MAX) { cout << "Limite de maestros alcanzado\n"; continue; }
            int n = (int)pedir("Cuantos maestros? ", 1, MAX - nM);
            for (int i = 0; i < n; i++, nM++) {
                cout << "\nMaestro " << nM + 1 << "\n";
                ma[nM].nombre = texto("  Nombre: ");
                ma[nM].materia = texto("  Materia: ");
            }
        }
        else if (op == 4) {
            if (nA == 0) { cout << "No hay alumnos\n"; continue; }

            int mejor[MAX], cuenta[MAX];
            float prom[MAX];
            for (int g = 0; g < nG; g++) { mejor[g] = -1; cuenta[g] = 0; prom[g] = -1; }

            cout << "\n-- Promedio por alumno --\n";
            for (int i = 0; i < nA; i++) {
                float p = promedio(al[i]);
                int g = al[i].grupo;
                cout << al[i].matricula << " " << al[i].nombre << ": " << p << "\n";
                cuenta[g]++;
                if (p > prom[g]) { prom[g] = p; mejor[g] = i; }
            }

            cout << "\n-- Mejor promedio por grupo --\n";
            for (int g = 0; g < nG; g++) {
                cout << gr[g].nombre << ": ";
                if (mejor[g] < 0) cout << "sin alumnos\n";
                else cout << al[mejor[g]].nombre << " (" << prom[g] << ")\n";
            }

            cout << "\n-- Alumnos por carrera --\n";
            for (int g = 0; g < nG; g++) {
                bool vista = false;
                int total = 0;
                for (int j = 0; j < nG; j++)
                    if (gr[j].carrera == gr[g].carrera) {
                        if (j < g) vista = true;
                        total += cuenta[j];
                    }
                if (!vista) cout << gr[g].carrera << ": " << total << "\n";
            }

            if (nM > 0) {
                cout << "\n-- Maestros --\n";
                for (int i = 0; i < nM; i++) cout << ma[i].nombre << " - " << ma[i].materia << "\n";
            }
        }
        else if (op == 5) {
            guardarDatos();
        }
        else if (op == 6) {
            if (nA + nG + nM > 0 && !confirmar("Esto reemplaza los datos actuales. Continuar? (s/n): "))
                cout << "  Cancelado\n";
            else
                cargarDatos(false);
        }
        else if (op == 7) {
            generarReporte();
        }
        else if (op == 8) {
            if (confirmar("Guardar antes de salir? (s/n): ")) guardarDatos();
        }
    } while (op != 8);

    return 0;
}