#ifndef SQLITE_CLIENT_H
#define SQLITE_CLIENT_H
#include "esat_extra/sqlite3.h"
#include <string.h>
char** g_name_columns = nullptr;
char** g_page_register = nullptr;

int g_num_columns = 0;
int g_num_cells = 0;

int g_user_num_columns = 0;
int g_user_num_cells = 0;

char** g_user_name_columns = nullptr;
char** g_user_page_register = nullptr;

char** g_user_query_name_columns = nullptr;
char** g_user_quey_page_register = nullptr;

char** g_tables_name = nullptr;
int g_actual_page = 1;
int g_max_page = 1;

int g_user_actual_page = 1;
int g_user_max_page = 1;

int total_tables = 0;
sqlite3* InitDatabase(char *db_name);
int NumsOfColumnsInQuery(char* table_name);
int NumsOfTablesInDB();
bool IsQueryValid(char* query);
bool LoadTable(char* base_query,char*** name_columns,char*** page_register,int *num_columns, int *num_cells,int actual_page, int *max_page);
#endif  // __INTERFACE_H__