#include <esat_extra/imgui.h>
#include "../includes/sqlite_client.h"

char db_name[200];

enum ApplicationState{
    kSelectDataBase,
    kClientPage,
};

ApplicationState g_aplication_state;

void DrawSelectionDB(){

    ImGui::Begin("Selecction");
    
    ImGui::InputTextWithHint("DB Name","Write the name of the database" , db_name, 50);

    if (ImGui::Button("Button")){

        InitDatabase(db_name);
        
    }

    ImGui::End();
}

void DrawClientPage(){

}

void DrawTables(){

    ImGui::Begin("Tables");

    if(ImGui::BeginTable("table1",NumsOfColumnsInTable("usuario"))){

    }



    ImGui::End();

}