#include "../includes/sqlite_client.h"
/**
* Opens the selected database
*
* Parameters:
* char *db_name: name of the database file to open
*
* Returns:
* sqlite3*: database connection if it was initialized correctly, NULL otherwise
*/

sqlite3 *db = nullptr;

int const kRowsPerPage  = 10;
int g_num_columns  = 1; 
char** page_register;

char** name_columns;

bool g_is_table_loaded = false;

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

    LoadTable();
    printf("Se abrio correctamente");
    
    return db;
}


/*
 /$$$$$$$$ /$$$$$$  /$$$$$$$   /$$$$$$ 
|__  $$__//$$__  $$| $$__  $$ /$$__  $$
   | $$  | $$  \ $$| $$  \ $$| $$  \ $$
   | $$  | $$  | $$| $$  | $$| $$  | $$
   | $$  | $$  | $$| $$  | $$| $$  | $$
   | $$  | $$  | $$| $$  | $$| $$  | $$
   | $$  |  $$$$$$/| $$$$$$$/|  $$$$$$/
   |__/   \______/ |_______/  \______/ 
                                       

    Cargar datos de pagina en array
    pensar como guardar los datos de las tablas                       
                                       

*/
int NumsOfColumnsInTable(char* table_name){
    int total_columns = 0;

    char query[200] = "SELECT * FROM usuario LIMIT 10";
    char *error_msg;
    sqlite3_stmt* statement = nullptr;
    int result = sqlite3_prepare_v2(db,query,-1,&statement,NULL);

    if (result != SQLITE_OK)
    {
        printf("Error: %s\n",sqlite3_errmsg(db));
        return total_columns;
    }
    

    total_columns = sqlite3_column_count(statement);
    
    // for (int i = 0; i < total_columns; i++)
    // {
        
    // }
    

    return total_columns;

}

void ColumnsNameInTable(){
    sqlite3_stmt* statement = NULL;
    const char* sql = "SELECT * FROM usuario;";

    int result = sqlite3_prepare_v2(db,sql,-1,&statement,NULL);

    if (result == SQLITE_OK) {

    for (int i = 0; i < NumsOfColumnsInTable("usuario"); ++i) {
        strcpy(name_columns[i],sqlite3_column_name(statement, i));

        printf("Column %d: %s\n", i, name_columns[i]);
    }
    }

    sqlite3_finalize(statement);
}


void LoadTable(){
    int const kCellSize = 50;
    int num_columns = NumsOfColumnsInTable("usuario");
    int num_cells = kRowsPerPage * num_columns;

    page_register = (char**)calloc(num_cells,sizeof(*page_register));

    for (int i = 0; i < num_cells; i++) {
        page_register[i] = (char*)calloc(kCellSize,sizeof(char));
    }

    name_columns = (char**)calloc(num_columns,sizeof(*name_columns));

    for (int i = 0; i < num_columns; i++)
    {
        name_columns[i] = (char*)calloc(kCellSize,sizeof(char));
    }
    
    ColumnsNameInTable();

}