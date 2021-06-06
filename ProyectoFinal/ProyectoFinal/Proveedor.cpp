#include <mysql.h>
#include <iostream>
#include <string>
using namespace std;

class Proveedor
{
private:
    int q_estado;
    MYSQL* conectar;
public:
    void add_connector(MYSQL* _conectar) {
        conectar = _conectar;
    }

    void crear() {
        string proveedor, nit, direccion, telefono, consulta;

        cout << "\nProveedor: ";
        getline(cin, proveedor);

        cout << "\nNit: ";
        getline(cin, nit);

        cout << "\nDirección: ";
        getline(cin, direccion);

        cout << "\nTeléfono: ";
        getline(cin, telefono);

        consulta = "INSERT INTO proveedores(proveedor, nit, direccion, telefono) VALUES('" + proveedor + "', '" + nit + "', '" + direccion + "', '" + telefono + "');";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            cout << "Proveedor ingresado con éxito" << endl;
            leer();
        }
        else {
            cout << "Error al ingresar proveedor" << endl;
        }
    }

    void leer() {
        cout << imprimir();
    }

    void actualizar() {
        int id;
        string proveedor, nit, direccion, telefono, consulta;

        leer();

        cout << "\nId del proveedor: ";
        cin >> id;

        cin.ignore();

        cout << "\nProveedor: ";
        getline(cin, proveedor);

        cout << "\nNit: ";
        getline(cin, nit);

        cout << "\nDirección: ";
        getline(cin, direccion);

        cout << "\nTeléfono: ";
        getline(cin, telefono);

        consulta = "UPDATE proveedores SET proveedor = '" + proveedor + "', nit = '" + nit + "', direccion = '" + direccion + "', telefono = '" + telefono + "' WHERE id = " + to_string(id);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            cout << "Proveedor actualizado con éxito" << endl;
            leer();
        }
        else {
            cout << "Error al actualizar proveedor" << endl;
        }
    }

    void eliminar() {
        int id;
        string consulta;

        leer();

        cout << "\nId del proveedor: ";
        cin >> id;

        consulta = "DELETE FROM proveedores WHERE proveedores.id = " + to_string(id);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            cout << "Proveedor eliminado con éxito" << endl;
            leer();
        }
        else {
            cout << "Error al eliminar proveedor" << endl;
        }
    }

    string imprimir() {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string consulta, imprimir;
        consulta = "SELECT * FROM proyecto_final.proveedores;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            imprimir = "\n----------------------------------------------------------\n";
            imprimir += "id\t| proveedor\t| nit\t| direccion\t| telefono\n";
            imprimir += "----------------------------------------------------------\n";
            while (fila = mysql_fetch_row(resultado)) {
                imprimir += fila[0];
                imprimir += "\t| ";
                imprimir += fila[1];
                imprimir += "\t| ";
                imprimir += fila[2];
                imprimir += "\t| ";
                imprimir += fila[3];
                imprimir += "\t| ";
                imprimir += fila[4];
                imprimir += "\n";
                imprimir += "----------------------------------------------------------\n";
            }
        }
        else {
            imprimir = "Error al leer proveedores\n";
        }
        return imprimir;
    }
};
