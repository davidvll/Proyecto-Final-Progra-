#include <mysql.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Venta
{
private:
    int q_estado;
    int n_factura = 1;
    int id_venta = 1;
    MYSQL* conectar;
public:
    void add_connector(MYSQL* _conectar) {
        conectar = _conectar;
    }

    void crear(string clientes, string empleados, string productos) {
        int id_cliente, id_empleado, id_producto, cantidad;
        char serie = 'A';
        double precio_unitario, total = 0;
        string fecha_factura = get_fecha_factura(), nit_cliente, consulta, ingreso;
        bool bandera;
;
        cout << "\nNo factura: " + to_string(n_factura) + "\nSerie: " + serie +"\nFecha de factura : " + fecha_factura + "\n" + clientes;
        
        cout << "\nNit del cliente: ";
        getline(cin, nit_cliente);
        id_cliente = get_id_cliente(nit_cliente);
        cout << "\nNombre del cliente: " + get_nombre_cliente(id_cliente) << endl;

        cout << empleados;
        cout << "\nId del empleado: ";
        cin >> id_empleado;

        consulta = "INSERT INTO ventas(no_factura, serie, fecha_factura, fecha_ingreso, id_cliente, id_empleado) VALUES('" + to_string(n_factura) + "', '" + serie + "', '" + fecha_factura + "', CURRENT_TIMESTAMP(), '" + to_string(id_cliente) + "', '" + to_string(id_empleado) + "');";
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

                consulta = "INSERT INTO ventas_detalle(id_venta, id_producto, cantidad, precio_unitario) VALUES('" + to_string(id_venta) + "', '" + to_string(id_producto) + "', '" + to_string(cantidad) + "', '" + to_string(precio_unitario) + "')";
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

            generar_factura(fecha_factura, nit_cliente, get_nombre_cliente(id_cliente), total);
            id_venta++;
            n_factura++;

        }
        else {
            cout << "Error al ingresar venta" << endl;
        }
    }

    void leer() {
        cout << imprimir();
    }

    void actualizar(string clientes, string empleados, string productos) {
        int _id_venta, id_cliente, id_empleado, id_producto, cantidad;
        double precio_unitario;
        string nit_cliente, consulta;

        leer();

        cout << "\nId de la venta: ";
        cin >> _id_venta;
        cin.ignore();

        cout << clientes + "\nNit del cliente : ";
        getline(cin, nit_cliente);
        id_cliente = get_id_cliente(nit_cliente);
        cout << "\nNombre del cliente: " + get_nombre_cliente(id_cliente) << endl;

        cout << empleados;
        cout << "\nId del empleado: ";
        cin >> id_empleado;

        cout << productos;
        cout << "\nId del producto: ";
        cin >> id_producto;

        precio_unitario = get_precio_producto(id_producto);
        cout << "\nPrecio unitario: " + to_string(precio_unitario);

        cout << "\nCantidad: ";
        cin >> cantidad;

        consulta = "UPDATE ventas SET fecha_ingreso = CURRENT_TIMESTAMP(), id_cliente = '" + to_string(id_cliente) + "', id_empleado = '" + to_string(id_empleado) + "' WHERE id = " + to_string(_id_venta);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            consulta = "UPDATE ventas_detalle SET cantidad = '" + to_string(cantidad) + "', precio_unitario = '" + to_string(precio_unitario) + "', id_producto = '" + to_string(id_producto) + "' WHERE id = " + to_string(_id_venta);
            const char* i = consulta.c_str();
            q_estado = mysql_query(conectar, i);
            if (!q_estado) {
                cout << "Venta actualizada con éxito" << endl;
                leer();
            }
            else {
                cout << "Error al actualizar detalle" << endl;
            }
        }
        else {
            cout << "Error al actualizar venta" << endl;
        }
    }

    void eliminar() {
        int id;
        string consulta;

        leer();

        cout << "\nId de la venta: ";
        cin >> id;

        consulta = "DELETE FROM ventas WHERE ventas.id = " + to_string(id);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            cout << "Venta eliminada con éxito" << endl;
            leer();
        }
        else {
            cout << "Error al eliminar venta" << endl;
        }
    }

    string imprimir() {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string consulta, imprimir;
        consulta = "SELECT * FROM ventas RIGHT JOIN ventas_detalle ON  ventas.id = ventas_detalle.id_venta;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            imprimir = "\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            imprimir += "id\t| no_factura\t| serie\t| fecha_factura\t| fecha_ingreso\t| id_cliente\t| id_empleado\t| id_detalle\t| cantidad\t| precio_unitario\t| id_venta\t| id_producto\n";
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
                imprimir += "\t| ";
                imprimir += fila[10];
                imprimir += "\t| ";
                imprimir += fila[11];
                imprimir += "\n";
                imprimir += "-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            }
        }
        else {
            imprimir = "Error al leer venta\n";
        }
        return imprimir;
    }

    string get_fecha_factura() {
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

    int get_id_cliente(string nit) {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string consulta;
        bool bandera = false;
        int id_cliente;

        consulta = "SELECT * FROM proyecto_final.clientes;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            while (fila = mysql_fetch_row(resultado)) {
                if (fila[3] == nit) {
                    id_cliente = stoi(fila[0]);
                    bandera = true;
                    break;
                }
            }
            if (!bandera) {
                cout << "\nEl nit no existe, agrege los datos del cliente" << endl;
                id_cliente = crear_cliente(nit);
            }
        }
        else {
            cout << "Error al leer clientes" << endl;
        }
        return id_cliente;
    }

    string get_nombre_cliente(int id) {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        string nombre, consulta;

        consulta = "SELECT * FROM proyecto_final.clientes;";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            while (fila = mysql_fetch_row(resultado)) {
                if (stoi(fila[0]) == id) {
                    nombre = fila[1];
                    nombre += " ";
                    nombre += fila[2];
                    break;
                }
            }
        }
        else {
            cout << "Error al leer clientes" << endl;
        }
        return nombre;
    }

    int crear_cliente(string nit) {
        int id;
        string nombres, apellidos, telefono, correo_electronico, consulta;

        cout << "\nNombres: ";
        getline(cin, nombres);

        cout << "\nApellidos: ";
        getline(cin, apellidos);

        cout << "\nTeléfono: ";
        getline(cin, telefono);

        cout << "\nCorreo electrónio: ";
        getline(cin, correo_electronico);

        consulta = "INSERT INTO clientes(nombres, apellidos, nit, telefono, correo_electronico, fecha_ingreso) VALUES('" + nombres + "', '" + apellidos + "', '" + nit + "', '" + telefono + "', '" + correo_electronico + "', CURRENT_TIMESTAMP());";
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            id = get_id_cliente(nit);
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

    void generar_factura(string fecha, string nit, string cliente, double total) {
        MYSQL_ROW fila;
        MYSQL_RES* resultado;
        MYSQL_RES* res;
        string consulta;

        cout << "\n|-----------------------------------------------------------------" << endl;
        cout << "|No Factura\t| " + to_string(n_factura) << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Fecha\t| " + fecha << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Nit\t| " + nit << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Cliente\t| " + cliente << endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|                             PRODUCTO                            " << endl;
        cout << "|-----------------------------------------------------------------" << endl;

        consulta = "SELECT * FROM ventas RIGHT JOIN ventas_detalle ON ventas.id = ventas_detalle.id_venta WHERE ventas_detalle.id_venta = " + to_string(id_venta);
        const char* i = consulta.c_str();
        q_estado = mysql_query(conectar, i);
        if (!q_estado) {
            resultado = mysql_store_result(conectar);
            while (fila = mysql_fetch_row(resultado)) {
                int id_producto = stoi(fila[11]);
                int cantidad = stoi(fila[8]);
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
            cout << "Error al leer ventas_detalle" << endl;
        }

        cout << "|Total:\tQ" + to_string(total)<< endl;
        cout << "|-----------------------------------------------------------------" << endl;
        cout << "|Gracias por su compra." << endl;
        cout << "|-----------------------------------------------------------------" << endl;
    }
};
