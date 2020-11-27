#include <SDL.h>
#include "goof-crl.h"

SDL_Window* gui_window = NULL;
SDL_Surface* gui_screen = NULL;
SDL_Renderer* gui_renderer = NULL;
SDL_Event e;

SDL_Rect board[8][8];

//Sprite bitmaps for dungeon GUI
SDL_Surface *bitmapSurface = NULL;

SDL_Texture *PLAYERTEX = NULL;
SDL_Texture *MONSTERTEX = NULL;
SDL_Texture *CHESTTEX = NULL;
SDL_Texture *WALLTEX = NULL;

int initGUI(int SCALE)
{
    printf("Initializing GUI...\n");
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("\nSDL Error that I don't know how to print!\n");
    } else {
        gui_window = SDL_CreateWindow("GOOF-GUI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 8*SCALE, 8*SCALE, SDL_WINDOW_SHOWN);
        if(gui_window == NULL ) {
           printf("\nWindow could not be created!\n");
        } else {
            printf("Loading Textures...\n");
            gui_screen = SDL_GetWindowSurface(gui_window);
            //SDL_FillRect(gui_screen, NULL, SDL_MapRGB(gui_screen->format, 0x00, 0x25, 0x00));
            SDL_UpdateWindowSurface(gui_window);

            SDL_CreateRenderer(gui_window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(gui_renderer, 0, 255, 0, 255);
            SDL_RenderSetScale(gui_renderer, SCALE, SCALE);

            bitmapSurface = SDL_LoadBMP("img/player.bmp");
            PLAYERTEX = SDL_CreateTextureFromSurface(gui_renderer, bitmapSurface);
            SDL_FreeSurface(bitmapSurface);

            bitmapSurface = SDL_LoadBMP("img/monster.bmp");
            MONSTERTEX = SDL_CreateTextureFromSurface(gui_renderer, bitmapSurface);
            SDL_FreeSurface(bitmapSurface);

            bitmapSurface = SDL_LoadBMP("img/chest.bmp");
            CHESTTEX = SDL_CreateTextureFromSurface(gui_renderer, bitmapSurface);
            SDL_FreeSurface(bitmapSurface);

            bitmapSurface = SDL_LoadBMP("img/wall.bmp");
            WALLTEX = SDL_CreateTextureFromSurface(gui_renderer, bitmapSurface);
            SDL_FreeSurface(bitmapSurface);

            printf("Setting up Rects...\n");
            for (int i = 0; i <= 7; i++) {
                for (int j = 0; j <= 7; j++) {
                    board[i][j].x = i*SCALE;
                    board[i][j].y = j*SCALE;
                    board[i][j].w = i+SCALE;
                    board[i][j].h = j+SCALE;
                    }
                }
            }
        }
    printf("GUI Initialized.\n");
    return 0;
}

int refreshGUI()
{
    //SDL_FillRect(gui_screen, NULL, SDL_MapRGB(gui_screen->format, 0x00, 0xFF, 0x00));
    SDL_PollEvent(&e); //This is terrible.
    SDL_RenderClear(gui_renderer);

    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++) {
            if (ldr[i][j].typ == PLR) {SDL_RenderCopy(gui_renderer, PLAYERTEX, NULL, &board[i][j]);}
            if (ldr[i][j].typ == MNR) {SDL_RenderCopy(gui_renderer, MONSTERTEX, NULL, &board[i][j]);}
            if (ldr[i][j].typ == CST) {SDL_RenderCopy(gui_renderer, CHESTTEX, NULL, &board[i][j]);}
            if (ldr[i][j].typ == WLL) {SDL_RenderCopy(gui_renderer, WALLTEX, NULL, &board[i][j]);}
            SDL_UpdateTexture()
            SDL_RenderDrawRect(gui_renderer, &board[i][j]);
        }
    }

    SDL_RenderPresent(gui_renderer);
    SDL_UpdateWindowSurface(gui_window);
    //printf("GUI refreshed.\r");
    return 0;
}


