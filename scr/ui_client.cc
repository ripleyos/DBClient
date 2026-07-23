#include <esat_extra/imgui.h>
#include "../includes/sqlite_client.h"

char db_name[200];

void DrawSelecctionDB(){

    ImGui::Begin("Selecction");
    
    ImGui::InputTextWithHint("DB Name","Write the name of the database" , db_name, 50);

    if (ImGui::Button("Button")){

        InitDatabase(db_name);
    }

    ImGui::End();
}

