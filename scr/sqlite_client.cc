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


bool g_is_table_loaded = false;

void InitTableNamesMem(){
    total_tables = NumsOfTablesInDB();
    g_tables_name = (char**)calloc(total_tables,sizeof(*g_tables_name));
    
    for (int i = 0; i < total_tables; i++)
    {
        g_tables_name[i] = (char*)calloc(50,sizeof(char));
    }

    
}

void NameOfTables(){
    char* query = "SELECT name FROM sqlite_master WHERE type ='table' AND name NOT LIKE 'sqlite_%'";
    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(db,query,-1,&statement,nullptr);

    if (result == SQLITE_OK)
    {
        int actual_row = 0;

        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            char* const buffer = 
                (char* const)sqlite3_column_text(statement,0);


            strcpy(g_tables_name[actual_row],buffer);

            actual_row++;
        }
    }
    else
    {
        printf("Error: %s\n",sqlite3_errmsg(db));
    }

    sqlite3_finalize(statement);
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

    InitTableNamesMem();
    NameOfTables();
    LoadTable(g_tables_name[0]);
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

    char query[200] = "SELECT * FROM ? LIMIT 10";
    sprintf(query,"SELECT * FROM \"%s\" LIMIT 0",table_name);
    char *error_msg;

    sqlite3_stmt* statement = nullptr;
    int result = sqlite3_prepare_v2(db,query,-1,&statement,NULL);
    if (result != SQLITE_OK)
    {

        printf("Error333: %s\n",sqlite3_errmsg(db));
        return total_columns;
    }
    

    total_columns = sqlite3_column_count(statement);

    sqlite3_finalize(statement);
    return total_columns;

}

int NumsOfTablesInDB(){
    char *query = "SELECT COUNT(*) FROM  sqlite_master  WHERE  type ='table' AND name NOT LIKE 'sqlite_%'";

    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(db,query,-1,&statement,NULL);
    int total_tables = 0;
    if (result != SQLITE_OK)
    {

            printf("Error: %s\n",sqlite3_errmsg(db));
        return total_tables;
    }
    sqlite3_step(statement);

    total_tables = sqlite3_column_int(statement,0); 
    sqlite3_finalize(statement);
    printf("total:%d",total_tables);
    return total_tables;
}

void ColumnsNameInTable(char* table_name){
    sqlite3_stmt* statement = NULL;
    char query[200];
    sprintf(query,"SELECT * FROM  \"%s\"",table_name);
    int result = sqlite3_prepare_v2(db,query,-1,&statement,NULL);

    if (result == SQLITE_OK) {


        for (int i = 0; i < NumsOfColumnsInTable(table_name); ++i) {
            strcpy(g_name_columns[i],sqlite3_column_name(statement, i));
        }
    }

    sqlite3_finalize(statement);
}

void GetActualPage(int page,char *table_name){
    int const krows = 10;
    int offset = (page - 1) * krows;
    
    char query[200];
    sprintf(query,"SELECT * FROM  \"%s\" LIMIT 10 OFFSET ?",table_name);
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
                printf("%s \n",buffer);
                strcpy(g_page_register[actual_row * num_columns + column],buffer);
            }
            actual_row++;
        }
        
    }
    sqlite3_finalize(statement);


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

int TotalRegisters(char* table_name){
    char query[200];
    sprintf(query,"SELECT COUNT(*) FROM  \"%s\"",table_name);
    
    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db,query,-1,&statement,nullptr);
    sqlite3_bind_text(statement,1,table_name,-1,SQLITE_TRANSIENT);
    int total = 0;
    if (result == SQLITE_OK)
    {
        sqlite3_step(statement);
        total = sqlite3_column_int(statement,0);
    }

    sqlite3_finalize(statement);
    return total;
}
int g_num_columns = 0;
int g_num_cells = 0;
/*
 /$$$$$$$$ /$$$$$$  /$$$$$$$   /$$$$$$ 
|__  $$__//$$__  $$| $$__  $$ /$$__  $$
   | $$  | $$  \ $$| $$  \ $$| $$  \ $$
   | $$  | $$  | $$| $$  | $$| $$  | $$
   | $$  | $$  | $$| $$  | $$| $$  | $$
   | $$  | $$  | $$| $$  | $$| $$  | $$
   | $$  |  $$$$$$/| $$$$$$$/|  $$$$$$/
   |__/   \______/ |_______/  \______/ 
                                       
    modificar LoadTable() para que carge cualquier pagina, y obtener de esa manera la tabla despues del from y lo que venga despues del where
    de esa manera el count para obtener bien la paginacion se puede hacer bien si pone limits               
                                       

*/
void LoadTable(char* table_name){
    
    //Reserves for mem

    int const kCellSize = 50;
    int num_columns = NumsOfColumnsInTable(table_name);
    
    if (g_name_columns != nullptr)
    {
        FreeNameColumns(g_num_columns);
    }
    
    if (g_page_register != nullptr)
    {
        FreePageMem(g_num_cells);
    }

    //Reserve for the names
    g_name_columns = (char**)calloc(num_columns,sizeof(*g_name_columns));
    
    for (int i = 0; i < num_columns; i++)
    {
        g_name_columns[i] = (char*)calloc(kCellSize,sizeof(char));
    }

    
    int num_cells = kRowsPerPage * num_columns;

    
    //Reserve for the page
    g_page_register = (char**)calloc(num_cells,sizeof(*g_page_register));

    for (int i = 0; i < num_cells; i++) {
        g_page_register[i] = (char*)calloc(kCellSize,sizeof(char));
    }

    g_num_columns = num_columns;
    g_num_cells = num_cells;
    //Save Columns names
    ColumnsNameInTable(table_name);
    GetActualPage(g_actual_page,table_name);
    g_max_page = TotalRegisters(table_name) / 10;
    printf("\nHOLA ESTOY DENTRO\n");



}
