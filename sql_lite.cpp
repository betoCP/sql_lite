#include <iostream>
#include <sqlite3.h>

using namespace std;

// Función de callback para mostrar los resultados de las consultas.
// Se llama cada vez que una consulta devuelve una fila de resultados.
// argc: número de columnas en la fila.
// argv: array con los valores de cada columna en la fila actual.
// azColName: array con los nombres de las columnas.
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    // Recorre cada columna de la fila actual y muestra su nombre y valor.
    for (int i = 0; i < argc; i++) {
        // Si el valor de la columna es NULL, muestra "NULL" en su lugar.
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl; // Espacio entre filas para mayor legibilidad.
    return 0;
}

int main() {
    sqlite3* db;          // Puntero a la base de datos.
    char* zErrMsg = 0;    // Variable para almacenar mensajes de error.
    int rc;               // Código de retorno para las funciones de SQLite.
    const char* sql;      // Cadena para almacenar las consultas SQL.

    // Abre o crea la base de datos "test.db".
    rc = sqlite3_open("test.db", &db);
    if (rc) {
        // Si la base de datos no se puede abrir, muestra el error y termina el programa.
        cerr << "No se puede abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return 0;
    }
    else {
        // Si la base de datos se abre correctamente, muestra un mensaje de éxito.
        cout << "Base de datos abierta correctamente." << endl;
    }

    // Crear la tabla "COMPANY" si no existe.
    sql = "CREATE TABLE IF NOT EXISTS COMPANY(" \
        "ID INT PRIMARY KEY NOT NULL," \
        "NAME TEXT NOT NULL," \
        "AGE INT NOT NULL," \
        "ADDRESS CHAR(50)," \
        "SALARY REAL);";

    // Ejecuta la consulta SQL para crear la tabla.
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        // Si hay un error al crear la tabla, muestra el mensaje de error.
        cerr << "Error al crear la tabla: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);  // Libera el mensaje de error.
    }
    else {
        cout << "Tabla creada correctamente." << endl;
    }

    // Insertar un valor en la tabla "COMPANY".
    sql = "INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY) " \
        "VALUES (1, 'Paul', 32, 'California', 20000.00);";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        // Si hay un error al insertar, muestra el mensaje de error.
        cerr << "Error al insertar en la tabla: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else {
        cout << "Valor insertado correctamente." << endl;
    }

    // Mostrar los valores insertados en la tabla.
    sql = "SELECT * FROM COMPANY;";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        // Si hay un error al mostrar los datos, muestra el mensaje de error.
        cerr << "Error al mostrar la tabla: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    // Modificar el salario del empleado con ID=1.
    sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1;";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        // Si hay un error al actualizar, muestra el mensaje de error.
        cerr << "Error al actualizar la tabla: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else {
        cout << "Valor actualizado correctamente." << endl;
    }

    // Mostrar los valores actualizados de la tabla.
    sql = "SELECT * FROM COMPANY;";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        // Si hay un error al mostrar los datos, muestra el mensaje de error.
        cerr << "Error al mostrar la tabla: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    // Cierra la base de datos.
    sqlite3_close(db);
    return 0;
}
