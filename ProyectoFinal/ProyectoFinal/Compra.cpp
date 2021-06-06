#include <mysql.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Compra
{
private:
    int q_estado;
    int n_orden = 1;
    int id_compra = 1;
    MYSQL* conectar;
public:
    void add_connector(MYSQL* _conectar) {
        conectar = _conectar;
    }

    void crear(string proveedores, string productos) {
        int id_proveedor, id_producto, cantidad;
        double precio_unitario, total = 0;
        string fecha_orden = get_fecha_orden(), nit_proveedor, consulta, ingreso;
        bool bandera;
        ;
        cout << "\nOrden de compra: " + to_string(n_orden) + "\nFecha de orden : " + fecha_orden + "\n" + proveedores;

        cout << "\nNit del proveedor: ";
        getline(cin, nit_proveedor);
        id_proveedor = get_id_proveedor(nit_proveedor);
        cout << "\nProveedor: " + get_proveedor(id_proveedor) << endl;

        consulta = "INSERT INTO compras(no_orden_compra, fecha_orden, fecha_ingreso, id_proveedor) VALUES('" + to_string(n_orden) + "', '" + fecha_orden + "', CURRENT_TIMESTAMP(), '" + to_string(id_proveedor) + "');";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {

            do {
                bandera = false;

                cout << productos;
                cout << "\nId del producto: ";
                cin >> id_producto;

                precio_unitario = get_precio_producto(id_producto);
                cout << "\nPrecio unitario: " + to_string(precio_unitario);

                cout << "\nCantidad: ";
                cin >> cantidad;

                for (int i = 0; i < cantidad; i++) {
                    total += precio_unitario;
                }

                cout << "\nIngresar más productos (s/n): ";
                cin >> ingreso;

                consulta = "INSERT INTO compras_detalle(id_compra, id_producto, cantidad, precio_costo_unitario) VALUES('" + to_string(id_compra) + "', '" + to_string(id_producto) + "', '" + to_string(cantidad) + "', '" + to_string(precio_unitario) + "')";
                const char* i = consulta.c_str();
                q_estado = mysql_query(conectar, i);
                if (!q_estado) {
                    cout << "\nTotal: " + to_string(total) << endl;
                }
                else {
                    cout << "Error al ingresar detalle" << endl;
                }

                if (ingreso == "S" || ingreso == "s") {
                    bandera = true;
                }

            } while (bandera);

            generar_factura(fecha_orden, nit_proveedor, get_proveedor(id_proveedor), total);
            id_compra++;
            n_orden++;

        }
        else {
            cout << "Error al ingresar compra" << endl;
        }
    }

    void leer() {
        cout << imprimir();
    }

    void actualizar(string proveedores, string productos) {
        int _id_compra, id_proveedor, id_producto, cantidad;
        double precio_unitario;
        string nit_proveedor, consulta;

        leer();

        cout << "\nId de la compra: ";
        cin >> _id_compra;
        cin.ignore();

        cout << proveedores + "\nNit del proveedor : ";
        getline(cin, nit_proveedor);
        id_proveedor = get_id_proveedor(nit_proveedor);
        cout << "\nNombre del proveedor: " + get_proveedor(id_proveedor) << endl;

        cout << productos;
        cout << "\nId del producto: ";
        cin >> id_producto;

        precio_unitario = get_precio_producto(id_producto);
        cout << "\nPrecio unitario: " + to_string(precio_unitario);

        cout << "\nCantidad: ";
        cin >> cantidad;

        consulta = "UPDATE compras SET fecha_ingreso = CURRENT_TIMESTAMP(), id_proveedor = '" + to_string(id_proveedor) + "' WHERE id = " + to_string(_id_compra);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            consulta = "UPDATE compras_detalle SET cantidad = '" + to_string(cantidad) + "', precio_costo_unitario = '" + to_string(precio_unitario) + "', id_producto = '" + to_string(id_producto) + "' WHERE id = " + to_string(_id_compra);
            const char* i = consulta.c_str();
            q_estado = mysql_query(conectar, i);
            if (!q_estado) {
                cout << "Compra actualizada con éxito" << endl;
                leer();
            }
            else {
                cout << "Error al actualizar detalle" << endl;
            }
        }
        else {
            cout << "Error al actualizar compra" << endl;
        }
    }

    void eliminar() {
        int id;
        string consulta;

        leer();

        cout << "\nId de la compra: ";
        cin >> id;

        consulta = "DELETE FROM compras WHERE compras.id = " + to_string(id);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            cout << "Compra eliminada con éxito" << endl;
            leer();
        }
        else {
            cout << "Error al eliminar compra" << endl;
        }
    }

    string imprimir() {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string consulta, imprimir;
        consulta = "SELECT * FROM compras RIGHT JOIN compras_detalle ON  compras.id = compras_detalle.id_compra;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            imprimir = "\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            imprimir += "id\t| no_orden_compra\t| fecha_orden\t| fecha_ingreso\t| id_proveedor\t| id_detalle\t| cantidad\t| precio_costo_unitario\t| id_compra\t| id_producto\n";
            imprimir += "-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
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
                imprimir += "\t| ";
                imprimir += fila[5];
                imprimir += "\t| ";
                imprimir += fila[6];
                imprimir += "\t| ";
                imprimir += fila[7];
                imprimir += "\t| ";
                imprimir += fila[8];
                imprimir += "\t| ";
                imprimir += fila[9];
                imprimir += "\n";
                imprimir += "-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            }
        }
        else {
            imprimir = "Error al leer venta\n";
        }
        return imprimir;
    }

    string get_fecha_orden() {
        char x[32]{};
        time_t a = time(nullptr);
        struct tm d;
        string s;
        if (localtime_s(&d, &a) == 0) {
            strftime(x, sizeof(x), "%F", &d);
            s = x;
            replace(s.begin(), s.end(), '-', '/');
        }
        return s;
    }

    int get_id_proveedor(string nit) {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string consulta;
        bool bandera = false;
        int id_proveedor;

        consulta = "SELECT * FROM proyecto_final.proveedores;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            while (fila = mysql_fetch_row(resultado)) {
                if (fila[2] == nit) {
                    id_proveedor = stoi(fila[0]);
                    bandera = true;
                    break;
                }
            }
            if (!bandera) {
                cout << "\nEl nit no existe, agrege los datos del proveedor" << endl;
                id_proveedor = crear_proveedor(nit);
            }
        }
        else {
            cout << "Error al leer proveedores" << endl;
        }
        return id_proveedor;
    }

    string get_proveedor(int id) {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string proveedor, consulta;

        consulta = "SELECT * FROM proyecto_final.proveedores;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            while (fila = mysql_fetch_row(resultado)) {
                if (stoi(fila[0]) == id) {
                    proveedor = fila[1];
                    break;
                }
            }
        }
        else {
            cout << "Error al leer proveedores" << endl;
        }
        return proveedor;
    }

    int crear_proveedor(string nit) {
        int id;
        string proveedor, direccion, telefono, consulta;

        cout << "\nProveedor: ";
        getline(cin, proveedor);

        cout << "\nDirección: ";
        getline(cin, direccion);

        cout << "\nTeléfono: ";
        getline(cin, telefono);

        consulta = "INSERT INTO proveedores(proveedor, nit, direccion, telefono) VALUES('" + proveedor + "', '" + nit + "', '" + direccion + "', '" + telefono + "');";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            id = get_id_proveedor(nit);
        }
        else {
            cout << "Error al ingresar cliente" << endl;
        }
        return id;
    }

    double get_precio_producto(int id) {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string consulta;
        double precio;

        consulta = "SELECT * FROM proyecto_final.productos;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            while (fila = mysql_fetch_row(resultado)) {
                if (stoi(fila[0]) == id) {
                    precio = stoi(fila[5]);
                    break;
                }
            }
        }
        else {
            cout << "Error al leer productos" << endl;
        }
        return precio;
    }

    void generar_factura(string fecha, string nit, string proveedor, double total) {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        MYSQL_RES* res;
        string consulta;

        cout << "\n|-----------------------------------------------------------------" << endl;
        cout << "|No Factura\t| " + to_string(n_orden) << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Fecha\t| " + fecha << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Nit\t| " + nit << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Proveedor\t| " + proveedor << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|                             PRODUCTO                            " << endl;
        cout << "|-----------------------------------------------------------------" << endl;

        consulta = "SELECT * FROM compras RIGHT JOIN compras_detalle ON compras.id = compras_detalle.id_compra WHERE compras_detalle.id_compra = " + to_string(id_compra);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            while (fila = mysql_fetch_row(resultado)) {
                int id_producto = stoi(fila[9]);
                int cantidad = stoi(fila[6]);
                for (int j = 0; j < cantidad; j++) {
                    consulta = "SELECT * FROM proyecto_final.productos;";
                    const char* k = consulta.c_str();
                    q_estado = mysql_query(conectar, k);
                    if (!q_estado) {
                        res = mysql_store_result(conectar);
                        while (fila = mysql_fetch_row(res)) {
                            if (stoi(fila[0]) == id_producto) {
                                string producto = fila[1];
                                cout << "|" + producto + "\tQ" + fila[5] << endl;
                                cout << "|-----------------------------------------------------------------" << endl;
                                break;
                            }
                        }
                    }
                    else {
                        cout << "Error al leer productos" << endl;
                    }
                }
            }
        }
        else {
            cout << "Error al leer compras_detalle" << endl;
        }

        cout << "|Total:\tQ" + to_string(total) << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Gracias por su compra." << endl;
        cout << "|-----------------------------------------------------------------" << endl;
    }
};
