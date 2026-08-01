#ifndef SQLITE_CLIENT_H
#define SQLITE_CLIENT_H
#include "esat_extra/sqlite3.h"
#include <string.h>
char** g_name_columns = nullptr;
char** g_page_register = nullptr;

char** g_user_query_name_columns = nullptr;
char** g_user_quey_page_register = nullptr;

char** g_tables_name = nullptr;
int g_actual_page = 1;
int g_max_page = 1;
int total_tables = 0;
sqlite3* InitDatabase(char *db_name);
int NumsOfColumnsInTable(char* table_name);
int NumsOfTablesInDB();
void LoadTable(char* table_name);
#endif  // __INTERFACE_H__