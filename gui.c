#include <SDL.h>
#include "goof-crl.h"

SDL_Window* gui_window = NULL;
SDL_Surface* gui_screen = NULL;
SDL_Renderer* gui_renderer = NULL;
SDL_Event e;

//Keyboard stuff
char keykeep = 0;

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
            SDL_FillRect(gui_screen, NULL, SDL_MapRGB(gui_screen->format, 0x00, 0x00, 0x00));
            SDL_UpdateWindowSurface(gui_window);

            gui_renderer = SDL_CreateRenderer(gui_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(gui_renderer, 0, 0, 0, 255);
            //SDL_RenderSetScale(gui_renderer, 2, 2);

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

                    board[i][j].w = SCALE;
                    board[i][j].h = SCALE;
                    //printf("%d, %d ", i, j);
                    }
                }

             //fpsTimer.start();
            }
        }
        return 0;
}

int refreshGUI()
{
    //SDL_FillRect(gui_screen, NULL, SDL_MapRGB(gui_screen->format, 0x00, 0xFF, 0x00));
    SDL_RenderClear(gui_renderer);

    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++) {
            if (ldr[i][j].typ == PLR) {SDL_RenderCopy(gui_renderer, PLAYERTEX, NULL, &board[i][j]);}
            if (ldr[i][j].typ == MNR) {SDL_RenderCopy(gui_renderer, MONSTERTEX, NULL, &board[i][j]);}
            if (ldr[i][j].typ == CST) {SDL_RenderCopy(gui_renderer, CHESTTEX, NULL, &board[i][j]);}
            if (ldr[i][j].typ == WLL) {SDL_RenderCopy(gui_renderer, WALLTEX, NULL, &board[i][j]);}
            //printf("%d, %d ", i, j);
            //SDL_RenderDrawRect(gui_renderer, &board[i][j]);
        }
    }

    //SDL_UpdateWindowSurface(gui_window);
    SDL_RenderPresent(gui_renderer);

    Uint8 *keyste = SDL_GetKeyboardState(NULL);

    if (keyste[SDL_SCANCODE_LEFT]) {
        printf("Left.\n");
        keyste[SDL_SCANCODE_LEFT] = 0;
        if (keykeep == 0) {keykeep = 1; return 1;}
        keykeep = 1;
    }

    if (keyste[SDL_SCANCODE_RIGHT]) {
        printf("Right.\n");
        keyste[SDL_SCANCODE_RIGHT] = 0;
        if (keykeep == 0) {keykeep = 1; return 2;}
        keykeep = 1;
    }

    if (keyste[SDL_SCANCODE_UP]) {
        printf("Up.\n");
        keyste[SDL_SCANCODE_UP] = 0;
        if (keykeep == 0) {keykeep = 1; return 3;}
    }

    if (keyste[SDL_SCANCODE_DOWN]) {
        printf("Down.\n");
        keyste[SDL_SCANCODE_DOWN] = 0;
        if (keykeep == 0) {keykeep = 1; return 4;}
        keykeep = 1;
    }

    SDL_PollEvent(&e); //This is terrible
        switch(e.type)
        {
            case SDL_KEYDOWN:
                keykeep = 0;
                //printf("%d", keykeep);
        }

    //printf("GUI refreshed.\r");
    return 0;
}

int debugGUI() //This is literally a copied example so I can compare code
{
        SDL_Window* window;
        SDL_Renderer* renderer;

        /* Initialize SDL. */
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                return 1;

        /* Create the window where we will draw. */
        window = SDL_CreateWindow("SDL_RenderClear",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512,
                        0);

        /* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
        renderer = SDL_CreateRenderer(window, -1, 0);

        /* Select the color for drawing. It is set to red here. */
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        /* Clear the entire screen to our selected color. */
        SDL_RenderClear(renderer);

        /* Up until now everything was drawn behind the scenes.
           This will show the new, red contents of the window. */
        SDL_RenderPresent(renderer);

        /* Give us time to see the window. */
        SDL_Delay(5000);

        /* Always be sure to clean up */
        SDL_Quit();
        return 0;
}

