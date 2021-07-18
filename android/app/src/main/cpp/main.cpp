//
// Created by martines on 13. 1. 21.
//

#include "BattleMapApp.h"
#include "glad/glad.h"
#include "SDL.h"

int main(int argc, char* argv[]){
    arc::Engine engine;
    engine.LaunchDesktopApp<BattleMapApp>({});
    return 0;
}

