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
void DrawTables(char* table_name,char** names_columns,char** page_register){



    int num_columns = NumsOfColumnsInTable(table_name);
 
    
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

        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        if (ImGui::ArrowButton("##left", ImGuiDir_Left) && g_actual_page != 1) { 
            g_actual_page--;
            LoadTable(table_name); 
        }
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right) && g_actual_page < g_max_page) { 
            g_actual_page++; 
            LoadTable(table_name);
        }
        ImGui::SameLine();
        ImGui::Text("%d", g_actual_page);



}
int g_selected_table = -1;
char user_query[200];
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
                    LoadTable(g_tables_name[tables]);
                }
                DrawTables(g_tables_name[tables],g_name_columns,g_page_register);

                ImGui::EndTabItem();
            }   

        }

        ImGui::EndTabBar();
    }
    ImGui::Text("Query ");
    ImGui::SameLine();
    ImGui::InputTextWithHint("","Write the query and the result will be displayed in a table" , user_query, 200);    
    
    
    ImGui::End();
}


void DrawClientPage(){
    DrawClientUI();
}
