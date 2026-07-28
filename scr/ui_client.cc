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
                                       
    Hacer que la primera tabla sea un hijo. y el apartado de la segunda sea otro hijo                     
                                       

*/
void DrawTables(){



    int num_columns = NumsOfColumnsInTable("usuario");
 
    
    if(ImGui::BeginTable("table1",num_columns)){

        ImGui::TableNextRow();
        for (int column = 0; column < num_columns; column++)
        {
            ImGui::TableSetColumnIndex(column);

            ImGui::Text("%s",g_name_columns[column]);

        }   

        for (int row = 0; row < 10; row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < num_columns; column++)
            {
                ImGui::TableSetColumnIndex(column);
                ImGui::Text("%s",g_page_register[row * num_columns + column]);
            }
            
        }
        
        ImGui::EndTable();
    }

        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        if (ImGui::ArrowButton("##left", ImGuiDir_Left) && g_actual_page != 1) { 
            g_actual_page--;
            LoadTable(); 
        }
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right) && g_actual_page < g_max_page) { 
            g_actual_page++; 
            LoadTable();
        }
        ImGui::SameLine();
        ImGui::Text("%d", g_actual_page);



}

void DrawClientUI(){
    ImGui::Begin("Tables");
    
    for (int tables = 0; tables < 10; tables++)
    {
        /* code */
    }
    
    
    DrawTables();
    ImGui::End();
}


void DrawClientPage(){
    DrawClientUI();
}
