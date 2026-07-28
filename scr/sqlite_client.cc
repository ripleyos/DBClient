#include "../includes/sqlite_client.h"
#include "../includes/ui_client.h"
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

bool g_is_table_loaded = false;

void InitTableNamesMem(){
    int total_tables = NumsOfTablesInDB();
    g_tables_name = (char**)calloc(total_tables,sizeof(*g_tables_name));
    
    for (int i = 0; i < total_tables; i++)
    {
        g_tables_name[i] = (char*)calloc(50,sizeof(char));
    }
}

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
    InitTableNamesMem();
    printf("Se abrio correctamente");
    g_aplication_state = kClientPage;
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
                                       
    Hacer que las querys no se repitan todo el rato, solo una vez y ya.                       
    Devolver errores en caso de error                                         

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
    sqlite3_finalize(statement);
    return total_columns;

}

int NumsOfTablesInDB(){
    char *query = "SELECT COUNT(*) FROM  sqlite_schema WHERE  type ='table' AND name NOT LIKE 'sqlite_%'";

    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(db,query,-1,&statement,NULL);
    int total_tables = 0;
    if (result != SQLITE_OK)
    {
        return total_tables;
    }
    sqlite3_step(statement);

    total_tables = sqlite3_column_int(statement,0); 
    sqlite3_finalize(statement);
    return total_tables;
}

void ColumnsNameInTable(){
    sqlite3_stmt* statement = NULL;
    const char* sql = "SELECT * FROM usuario;";

    int result = sqlite3_prepare_v2(db,sql,-1,&statement,NULL);

    if (result == SQLITE_OK) {

    for (int i = 0; i < NumsOfColumnsInTable("usuario"); ++i) {
        strcpy(g_name_columns[i],sqlite3_column_name(statement, i));

        printf("Column %d: %s\n", i, g_name_columns[i]);
    }
    }

    sqlite3_finalize(statement);
}

void GetActualPage(int page){
    int const krows = 10;
    int offset = (page - 1) * krows;
    
    char* query = "SELECT * FROM usuario LIMIT 10 OFFSET ?";
    sqlite3_stmt *statement = nullptr;
    
    int result = sqlite3_prepare_v2(db,query,-1,&statement,nullptr);
    
    sqlite3_bind_int(statement,1,offset);
    
    int num_columns = sqlite3_column_count(statement);
    if (result == SQLITE_OK)
    {
        int actual_row = 0;

        
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            for (int column = 0; column < num_columns; column++)
            {
                char* const buffer = (char* const)sqlite3_column_text(statement,column);
                // printf("%s \n",buffer);
                strcpy(g_page_register[actual_row * num_columns + column],buffer);
            }
            actual_row++;
        }
        
    }
    sqlite3_finalize(statement);
    for (int row = 0; row < kRowsPerPage; ++row) {
    for (int column = 0; column < num_columns; ++column) {
        printf("%s\t",
            g_page_register[row * num_columns + column]);
    }

    printf("\n");
    }


}

void FreePageMem(int num_cells){
    for (int i = 0; i < num_cells; ++i) {
        free(g_page_register[i]);
        g_page_register[i] = nullptr;
    }

    free(g_page_register);
    g_page_register = nullptr;
}

void FreeNameColumns(int num_columns){
    for (int i = 0; i < num_columns; ++i) {
        free(g_name_columns[i]);
        g_name_columns[i] = nullptr;
    }

    free(g_name_columns);
    g_name_columns = nullptr;    
}

int TotalRegisters(){
    char* query = "SELECT COUNT(*) FROM usuario";
    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db,query,-1,&statement,nullptr);
    int total = 0;
    if (result == SQLITE_OK)
    {
        sqlite3_step(statement);
        total = sqlite3_column_int(statement,0);
    }

    sqlite3_finalize(statement);
    return total;
}

void LoadTable(){
    
    //Reserves for mem

    int const kCellSize = 50;
    int num_columns = NumsOfColumnsInTable("usuario");
    
    if (g_name_columns != nullptr)
    {
        FreeNameColumns(num_columns);
    }
    
    //Reserve for the names
    g_name_columns = (char**)calloc(num_columns,sizeof(*g_name_columns));
    
    for (int i = 0; i < num_columns; i++)
    {
        g_name_columns[i] = (char*)calloc(kCellSize,sizeof(char));
    }

    
    int num_cells = kRowsPerPage * num_columns;
    if (g_page_register != nullptr)
    {
        FreePageMem(num_cells);
    }
    
    //Reserve for the page
    g_page_register = (char**)calloc(num_cells,sizeof(*g_page_register));

    for (int i = 0; i < num_cells; i++) {
        g_page_register[i] = (char*)calloc(kCellSize,sizeof(char));
    }

    
    //Save Columns names
    ColumnsNameInTable();
    GetActualPage(g_actual_page);
    g_max_page = TotalRegisters() / 10;
    printf("max = %d total = %d",g_max_page,TotalRegisters());


}
