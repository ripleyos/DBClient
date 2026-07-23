#include "../includes/sqlite_client.h"
/**
* Opens or creates the database and initializes the required tables
*
* Parameters:
* char *db_name: name of the database file to open or create
*
* Returns:
* sqlite3*: database connection if it was initialized correctly, NULL otherwise
*/

sqlite3 *db = nullptr;
sqlite3* InitDatabase(char *db_name){
    
    char str_db[10] = ".db";
    char cpy_name[200];

    strcpy(cpy_name,db_name);
    strcat(cpy_name,str_db);
    char *err_msg = NULL;

    int rc = sqlite3_open_v2(cpy_name,&db,SQLITE_OPEN_READWRITE,NULL);

    if (rc != SQLITE_OK){
        printf("Error al abrir o crear la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        printf("%s",cpy_name);
        return NULL;
    }

    printf("Se abrio correctamente");
    return db;
}