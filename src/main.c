#include <stdio.h>

#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"

#define PNTR_TILED_IMPLEMENTATION
#include "pntr_tiled.h"


typedef struct AppData {
    cute_tiled_map_t* map;
    pntr_sound* music;
    float x, y;
} AppData;

bool Init(pntr_app* app) {
    AppData* appData = pntr_load_memory(sizeof(AppData));
    pntr_app_set_userdata(app, appData);

    appData->map = pntr_load_tiled("assets/maps/SuperMarioBrosMap1-1.tmj");
    appData->music = pntr_load_sound("assets/sounds/music-ground.ogg");
    pntr_play_sound(appData->music, true);

    appData->x = 0;
    appData->y = 0;

    return true;
}

bool Update(pntr_app* app, pntr_image* screen) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
    float deltaTime = pntr_app_delta_time(app);
    
    if (appData->x > 0) {
        appData->x = 0;
    }
    if (appData->y > 0) {
        appData->y = 0;
    }
    if (appData->x - screen->width < -appData->map->width * appData->map->tilewidth) {
        appData->x = -appData->map->width * appData->map->tilewidth + screen->width;
    }
    if (appData->y - screen->height < -appData->map->height * appData->map->tileheight) {
        appData->y = -appData->map->height * appData->map->tileheight + screen->height;
    }

    pntr_update_tiled(appData->map, pntr_app_delta_time(app));
    pntr_clear_background(screen, PNTR_BLACK);
    pntr_draw_tiled(screen, appData->map, appData->x, appData->y, PNTR_WHITE);

    return true;
}

void Close(pntr_app* app) {
    AppData* appData = (AppData*)pntr_app_userdata(app);

    pntr_unload_tiled(appData->map);

    pntr_unload_memory(appData);
}

void Event(pntr_app* app, pntr_app_event* event) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
}

pntr_app Main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (pntr_app) {
        .width = 320,
        .height = 240,
        .title = "SMB",
        .init = Init,
        .update = Update,
        .close = Close,
        .event = Event,
        .fps = 0
    };
}