#include "goof-crl.h"

//Game-Based variables:
int ldr_x = 0; //Currently loaded room's X location within dungeon
int ldr_y = 0; //Currently loaded room's Y location within dungeon

int plr_rx = 1; //X location of the tile that the player is in
int plr_ry = 1; //Y location of the tile that the player is in
int plr_dx = 0; //X location of the room that the player is in
int plr_dy = 0; //Y location of the room that the player is in
int gld = 0; //Player's gold.

int roll(int l, int u) //Generate a random number
{
    int r = (rand() % (u - l + 1)) + l;
    return r;
}

int genr(int d, char e) //Generate a room based off a difficulty number, and add an empty wall, and store it into ldr
{

    //Add walls (made better thanks to a friend):
       for (int i = 0; i <= 7; i++) {

        //West Wall:
        if (e != 1) {
            ldr[i][0].typ = WLL;
            ldr[i][0].sts = STC;
        }

        //North Wall:
        if (e != 2) {
            ldr[0][i].typ = WLL;
            ldr[0][i].sts = STC;
        }

        //South Wall:
        if (e != 3) {
            ldr[i][7].typ = WLL;
            ldr[i][7].sts = STC;
        }

        //East Wall:
        if (e != 4) {
            ldr[7][i].typ = WLL;
            ldr[7][i].sts = STC;
        }

        }


        int x = 0;
        int y = 0;

        //Add monsters:
        for (int i = 0; i < roll(1, d); i++) {
            x = roll(1,6);
            y = roll(1,6);

            //printf("%d, %d ", x, y);

            ldr[x][y].typ = MNR;
            ldr[x][y].hit = roll(1,d);
            ldr[x][y].atk = roll(1,d);
            ldr[x][y].def = roll(1,d);
            ldr[x][y].sts = ALV;
            ldr[x][y].inv[0] = roll(0,d/2);
            ldr[x][y].inv[1] = roll(0,d/2);
            ldr[x][y].inv[2] = roll(0,d/2);
            ldr[x][y].inv[3] = roll(0,d/2);
        }

        //Add chests:
        for (int i = 0; i < roll(1, d); i++) {
            x = roll(1,6);
            y = roll(1,6);

            //printf("%d, %d ", x, y);

            ldr[x][y].typ = CST;
            ldr[x][y].sts = DED;
            ldr[x][y].inv[0] = roll(0,d/2);
            ldr[x][y].inv[1] = roll(0,d/2);
            ldr[x][y].inv[2] = roll(0,d);
            ldr[x][y].inv[3] = roll(0,d);
        }

        //printf("\n");

    return 0;
}

int gend(int d_SIZE) //Generate a dungeon
{
    dgn = fopen("dgn.bin", "wb"); //Create dungeon file and open it for binary reading and writing

    if (dgn == NULL) {
        printf("\nError generating file.\n");
    }
    fseek(dgn, SEEK_SET, 0); //Set file pointer to beginning, just in case something within the file itself is broken.

    int max = d_SIZE * d_SIZE;
    int c = 0;
    //Generated a room, then properly store it into the created file
    for (int x = 0; x < d_SIZE; x++) {
        printf("Generated %d of %d rooms\r", c, max);
        for (int y = 0; y < d_SIZE; y++) {
            genr(roll(1,4)+((x + 1) / (y + 1)), roll(1, 4));
            fwrite(&ldr, 9, 64, dgn);
            clrr(); //I would never have forgotten to clear the room before making a new one, how could I be so stupid
            c++;
        }
    }
    printf("\n");

    //This is terrible.
    ldd(d_SIZE/2,d_SIZE/2);
    plr_dx = d_SIZE/2;
    plr_dy = d_SIZE/2;
    ldr[1][1].typ = PLR;
    ldr[1][1].hit = 10;
    ldr[1][1].atk = 1;
    ldr[1][1].def = 1;

    printf("\n");

    fclose(dgn);
    return 0;
}

int clrr() //Clear the entirety of ldr
{
    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
            ldr[i][j].typ = 0;
            ldr[i][j].hit = 0;
            ldr[i][j].atk = 0;
            ldr[i][j].def = 0;
            ldr[i][j].inv[0] = 0;
            ldr[i][j].inv[1] = 0;
            ldr[i][j].inv[2] = 0;
            ldr[i][j].inv[3] = 0;
        }
    }
    return 0;
}

//This will soon be unused
int sdd(int x, int y) //Save ldr into any given spot within the file.
{
    dgn = fopen("dgn.bin", "rb");
    fseek(dgn, ((x + y) * 576), SEEK_SET);
    fwrite(ldr, 9, 64, dgn);

    printf("\nSaved room %d, %d\n", x, y);
    //debug_prm(x,y);

    fclose(dgn);
    return 0;
}

//This will soon be unused
int ldd(int x, int y) //Load any given room into ldr from file.
{
    clrr();
    dgn = fopen("dgn.bin", "rb");
    fseek(dgn, ((x + y) * 576), SEEK_SET);
    fread(ldr, 9, 64, dgn);

    ldr_x = x;
    ldr_y = y;

    printf("\nLoaded room %d, %d\n", x, y);
    //debug_prm(x,y);

    fclose(dgn);
    return 0;
}

int clre(int x, int y) //Clear any spot on ldr
{
    ldr[x][y].typ = 0;
    ldr[x][y].hit = 0;
    ldr[x][y].atk = 0;
    ldr[x][y].def = 0;
    ldr[x][y].inv[0] = 0;
    ldr[x][y].inv[1] = 0;
    ldr[x][y].inv[2] = 0;
    ldr[x][y].inv[3] = 0;
    return 0;
}

int move(int ox, int oy, int dx, int dy , char arg) //Move any entity into a new room, ARGS: 1: flip X position, 2: flip Y position
{
    struct entity h = ldr[ox][oy];//Store entity
    clre(ox, oy); //Set original place to 0
    sdd(ldr_x, ldr_y); //Save original room after removing entity
    ldd(dx, dy); //Load new room

    //printf("%d, %d", ox, oy);

    //Change the entity's placement within the room
    if(arg == 1) {
        if(ox == 0) {ldr[7][oy] = h;} else {ldr[0][oy] = h;}
    }
    if(arg == 2) {
        if(oy == 0) {ldr[ox][7] = h;} else {ldr[ox][0] = h;}
    }

    //Sitting here reviewing this code and I wish I documented what it was supposed to do, remind me to document more processes.
    //RE: This code is not to change the placement of the player entity, but to update the player's saved positions within the dungeon.
    if(h.typ == PLR) {
        plr_dx = dx;
        plr_dy = dy;

        if(arg == 1) {
            if (ox == 0) {plr_rx = 7;} else {plr_rx = 0;}
        }
        if(arg == 2) {
            if (oy == 0) {plr_ry = 7;} else {plr_ry = 0;}
        }
        return 0;
    }

    //ldd(ldr_x, ldr_y); - What was I thinking when I put this here

    return 0;
}

int pute(int ox, int oy, int nx, int ny) //Copy an entity into a spot on ldr and clear original spot
{
    struct entity h = ldr[ox][oy]; //Store entity
    clre(ox, oy); //Set original place to 0
    ldr[nx][ny] = h; //Put entity in new postion

    //If the entity being moved is the player, update player's position
    if (ldr[nx][ny].typ == PLR) {
        plr_rx = nx;
        plr_ry = ny;
    }

    //printf("Moved entity at [%d, %d] to [%d, %d]\n", ox, oy, nx, ny);

    return 0;
}

int plra(int a) //All of a player's actions
{
    //1 - Left
    //2 - Right
    //3 - Up
    //4 - Down

    if (a != 0){
        if (a == 1) {
            //Move player into new room on the left if the player is at the edge
            if (plr_rx-1 == -1) {
                move(plr_rx, plr_ry, plr_dx-1, plr_dy, 1);
            } else if (ldr[plr_rx-1][plr_ry].typ == 0 && plr_rx-1 > -1) { //Move player left if empty
                pute(plr_rx, plr_ry, plr_rx-1, plr_ry);
                //printf("Moved left\n");
            }
        }

        if (a == 2) {
            //Move player into new room on the right if the player is at the edge
            if (plr_rx+1 == 8) {
                move(plr_rx, plr_ry, plr_dx+1, plr_dy, 1);
            } else if (ldr[plr_rx+1][plr_ry].typ == 0 && plr_rx+1 < 8) { //Move player right if empty
                pute(plr_rx, plr_ry, plr_rx+1, plr_ry);
                //printf("Moved right\n");
            }
        }

        if (a == 3) {
            //Move player into new room above if the player is at the edge
            if (plr_ry-1 == -1) {
                move(plr_rx, plr_ry, plr_dx, plr_dy-1, 2);
            } else if (ldr[plr_rx][plr_ry-1].typ == 0 && plr_ry-1 > -1) { //Move player up if empty
                pute(plr_rx, plr_ry, plr_rx, plr_ry-1);
                //printf("Moved up\n");
            }

        }

        if (a == 4) {
            //Move player into new room below if the player is at the edge
            if (plr_ry+1 == 8) {
                move(plr_rx, plr_ry, plr_dx, plr_dy+1, 2);
            } else if (ldr[plr_rx][plr_ry+1].typ == 0 && plr_ry+1 < 8) { //Move player down if empty
                pute(plr_rx, plr_ry, plr_rx, plr_ry+1);
                //printf("Moved down\n");
            }
        }
    }

    //printf("X: %d, Y: %d \n", plr_rx, plr_ry);

    return -1;
}

int debug_prm() //Prints currently loaded room into the console
{
    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++) {
            printf(" %x ", ldr[i][j].typ);
        }
        printf("\n");
    }
    return 0;
}

int main(int argc, char* argv[])
{
    //debugGUI();

    srand(time(0)); //Initalize random number gen
    int input = 0;

    printf("GOOF-CRL Loaded.\n");
    printf("Enter wished size of dungeon\n[WARNING: LARGER DUNGEON SIZES WILL RESULT IN LARGE FILE SIZES]\n");
    scanf("%d",&input);

    gend(input);
    printf("Dungeon generated.\n");

    //printf("Enter wished scale of screen:\n"); - I don't think I need this
    //scanf("%d", &input);
    initGUI(64);

    char run = 0x01;

    input = 0;

    int msec = 0;
    int trigger = 1;
    while(run == 0x01)
    {
        input = refreshGUI(input);
        plra(input);
    }
    return 0;
}
