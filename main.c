#include "goof-crl.h"

//Game-Based variables:
int pyr_x = 0; //X location of the room that the player is in
int pyr_y = 0; //Y location of the room that the player is in
int gold = 0; //Player's gold.

int roll(int l, int u) //Generate a random number
{
    int r = (rand() % (u - l + 1)) + l;
    return r;
}

int genr(char d, char e) //Generate a room based off a difficulty number, and add an empty wall, and store it into ldr
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
    for (int x = 0; x <= d_SIZE; x++) {
        printf("Generated %d of %d rooms\r", c, max);
        for (int y = 0; y <= d_SIZE; y++) {
            genr(roll(1,4)+((x + 1) / (y + 1)), roll(1,4));
            fwrite(ldr, sizeof(struct entity), 64, dgn);
            c++;
        }
    }
    printf("\n");

    fclose(dgn);
    return 0;
}

int ldd(int x, int y) //Load any given room into ldr from file.
{
    memset(ldr, 0, sizeof(ldr));
    dgn = fopen("dgn.bin", "rb");
    fseek(dgn, SEEK_SET, 0);
    fseek(dgn, SEEK_SET, (x * y) * sizeof(ldr));

    for (int x = 0; x <= 7; x++) {
        for (int y = 0; y <= 7; y++) {
            fread(&ldr[x][y], 9, 1, dgn);
        }
    }

    printf("Loaded room %d, %d\n", x, y);

    fclose(dgn);
    return 0;
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

    ldd(0,0);

    while(run == 0x01)
    {
        refreshGUI();
    }
    return 0;
}
