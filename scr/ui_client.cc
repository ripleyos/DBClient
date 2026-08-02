#include <esat_extra/imgui.h>
#include "../includes/sqlite_client.h"

char db_name[200];

enum ApplicationState{
    kSelectDataBase,
    kClientPage,
};



void DrawSelectionDB(){

    ImGui::Begin("Selecction");
    
    ImGui::InputTextWithHint("DB Name","Write the name of the database" , db_name, 50);

    if (ImGui::Button("Button")){

        InitDatabase(db_name);
        
    }

    ImGui::End();
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
                                       
    hacer la tabla de la query                 
                                       

*/
void DrawTables(char* table_name,char** names_columns,char** page_register,int num_cells,int* actual_page,int* max_page){



    int num_columns = NumsOfColumnsInQuery(table_name);
 
    
    if(ImGui::BeginTable("table1",num_columns)){

        ImGui::TableNextRow();
        for (int column = 0; column < num_columns; column++)
        {
            ImGui::TableSetColumnIndex(column);

            ImGui::Text("%s",names_columns[column]);

        }   

        for (int row = 0; row < 10; row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < num_columns; column++)
            {
                ImGui::TableSetColumnIndex(column);
                ImGui::Text("%s",page_register[row * num_columns + column]);
            }
            
        }
        
        ImGui::EndTable();
    }

        // float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        // if (ImGui::ArrowButton("##left", ImGuiDir_Left) && *actual_page != 1) { 
        //     (*actual_page)--;
        //     LoadTable(table_name,&names_columns,&page_register,&num_columns,&num_cells,*actual_page,max_page); 
        // }
        // ImGui::SameLine(0.0f, spacing);
        // if (ImGui::ArrowButton("##right", ImGuiDir_Right) && *actual_page < *max_page) { 
        //     (*actual_page)++; 
        //     LoadTable(table_name,&names_columns,&page_register,&num_columns,&num_cells,*actual_page,max_page);
        // }
        // ImGui::SameLine();
        // ImGui::Text("%d", *actual_page);



}
int g_selected_table = -1;
char user_query[200];
char execute_query[200];
bool loaded = false;

void DrawPageArrows(char* table_name,char*** names_columns,char*** page_register,int num_cells,int* actual_page,int* max_page){
    int num_columns = NumsOfColumnsInQuery(table_name);
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    if (ImGui::ArrowButton("##left", ImGuiDir_Left) && *actual_page != 1) { 
        (*actual_page)--;
        LoadTable(table_name,names_columns,page_register,&num_columns,&num_cells,*actual_page,max_page); 
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right) && *actual_page < *max_page) { 
        (*actual_page)++; 
        LoadTable(table_name,names_columns,page_register,&num_columns,&num_cells,*actual_page,max_page);
    }
    ImGui::SameLine();
    ImGui::Text("%d", *actual_page);
}

void DrawClientUI(){
    ImGui::Begin("Tables");

    if (ImGui::BeginTabBar("example"))
    {
        for (int tables = 0; tables < total_tables; tables++)
        {

            
            if (ImGui::BeginTabItem(g_tables_name[tables]))
            {
                if (g_selected_table != tables)
                {
                    g_selected_table = tables;
                    g_actual_page = 1;
                    LoadTable(g_tables_name[tables],&g_name_columns,&g_page_register,&g_num_columns,&g_num_cells,g_actual_page,&g_max_page);
                }
                DrawTables(g_tables_name[tables],g_name_columns,g_page_register,g_num_cells,&g_actual_page,&g_max_page);
                DrawPageArrows(g_tables_name[tables],&g_name_columns,&g_page_register,g_num_cells,&g_actual_page,&g_max_page);

                ImGui::EndTabItem();
            }   

        }

        ImGui::EndTabBar();
    }


    ImGui::Text("Query ");
    ImGui::SameLine();
    ImGui::InputTextWithHint("","Write the query and the result will be displayed in a table" , user_query, 200);    
    if (ImGui::Button("Button")){
        if (IsQueryValid(user_query))
        {
            loaded = LoadTable(user_query,&g_user_name_columns,&g_user_page_register,&g_user_num_columns,&g_user_num_cells,g_user_actual_page,&g_user_max_page);
            strcpy(execute_query,user_query);
        }
        
        
    }
    if (loaded){
        DrawTables(execute_query,g_user_name_columns,g_user_page_register,g_user_num_cells,&g_user_actual_page,&g_user_max_page);
        DrawPageArrows(execute_query,&g_user_name_columns,&g_user_page_register,g_user_num_cells,&g_user_actual_page,&g_user_max_page);
    }

    
    ImGui::End();
}


void DrawClientPage(){
    DrawClientUI();
}
