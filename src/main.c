#include <stdio.h>

#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"

#define PNTR_TILED_IMPLEMENTATION
#include "pntr_tiled.h"

typedef enum {
    GAME_START,
    GAME_PLAYING,
    GAME_OVER
} GameState;


typedef struct AppData {
    cute_tiled_map_t* map;
    pntr_sound* music;
    float x, y;
    cute_tiled_layer_t* layer;
    GameState mode;
    pntr_image* logo;
    pntr_font* font;
} AppData;

void load_map(AppData* appData, char* name) {
    char n[100];
    sprintf(n, "assets/maps/%s.tmj", name);
    appData->map = pntr_load_tiled(n);
    appData->layer = pntr_tiled_layer(appData->map, name);
    char* musicFile = NULL;
    if (appData->map->property_count > 0) {
        for (int i =0;i<appData->map->property_count;i++) {
            if (strcmp("music", appData->map->properties[i].name.ptr) == 0) {
                sprintf(n, "assets/sounds/music-%s.ogg", (char*)appData->map->properties[i].data.string.ptr);
                appData->music = pntr_load_sound(n);
                pntr_play_sound(appData->music, true);
                break;
            }
        }
    }
    appData->mode = GAME_PLAYING;
    appData->x = 0;
    appData->y = 0;
}


bool Init(pntr_app* app) {
    AppData* appData = pntr_load_memory(sizeof(AppData));
    pntr_app_set_userdata(app, appData);
    load_map(appData, "SuperMarioBrosMap1-1");
    appData->mode = GAME_START;
    appData->logo = pntr_load_image("assets/logo.png");
    appData->font = pntr_load_font_tty("assets/font.png", 16, 15, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-\",.!: ");
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

    if (appData->mode == GAME_START) {
        pntr_draw_image(app->screen, appData->logo, 64, 48);
        pntr_draw_text(app->screen,appData->font, "PRESS START\n\n TO BEGIN!", 65, 145, PNTR_WHITE);
    }

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