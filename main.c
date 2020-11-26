#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Backend variables:
char dgn_name[16] = "dgn.bin"; //Default name for Dungeon, is changeable
FILE *dgn; //File for storing built dungeon (Created if not found already)

//Game-Based variables:
int gold = 0; //Player's gold.

struct entity {
    char typ; //Type of Entity, used for determining sprites and AI (Player (0x01), Monster(0x02), Chest(0x03), Wall(0x04))
    char hit; //Hitpoints (Used to determine Status)
    char atk; //Attack (Used for combat)
    char def; //Defense (Used for combat)
    char sts; //Status, used for determined interactability (Alive (0x01), Dead (0x02), or Static (0x03))
    char inv[4]; //Inventory, (Armor, Weapon, Potion, Gold)
};
struct entity ldr[8][8]; //Loaded Room

int roll(int u, int l) //Generate a random number
{
    int r = (rand() % (u - l + 1)) + l;
    return r;
}

int genr(char d) //Generate a room based off a difficulty number and store it into ldr
{
    //Add walls:
                //West Wall:
            for (int x = 0; x <= 7; x++) {
                ldr[x][0].typ = 0x04;
                ldr[x][0].sts = 0x03;
            }

                //North Wall:
            for (int y = 0; y <= 7; y++) {
                ldr[0][y].typ = 0x04;
                ldr[0][y].sts = 0x03;
            }

                //South Wall:
            for (int x = 0; x <= 7; x++) {
                ldr[x][7].typ = 0x04;
                ldr[x][7].sts = 0x03;
            }
                //East Wall:
            for (int y = 0; y <= 7; y++) {
                ldr[7][y].typ = 0x04;
                ldr[7][y].sts = 0x03;
            }

            int x = 0;
            int y = 0;

            //Add monsters:
            for (int i = roll(0,3); i >= 0; i--) {
                    x = roll(1,6);
                    y = roll(1,6);

                ldr[x][y].typ = 0x02;
                ldr[x][y].hit = roll(1,d);
                ldr[x][y].atk = roll(1,d);
                ldr[x][y].def = roll(1,d);
                ldr[x][y].sts = 0x01;
                ldr[x][y].inv[0] = roll(0,d/2);
                ldr[x][y].inv[0] = roll(0,d/2);
                ldr[x][y].inv[0] = roll(0,d/2);
                ldr[x][y].inv[0] = roll(0,d/2);
            }

            //Add chests:
            for (int i = roll(0,3); i >= 0; i--) {
                    x = roll(1,6);
                    y = roll(1,6);

                ldr[x][y].typ = 0x03;
                ldr[x][y].sts = 0x02;
                ldr[x][y].inv[0] = roll(0,d/2);
                ldr[x][y].inv[0] = roll(0,d/2);
                ldr[x][y].inv[0] = roll(0,d);
                ldr[x][y].inv[0] = roll(0,d);
            }

    return 0;
}

int gend(int x_SIZE, int y_SIZE) //Generate a dungeon
{
    dgn = fopen("dgn.bin", "wb"); //Create dungeon file and open it for binary reading and writing

    if (dgn == NULL) {
        printf("\nError generating file.\n");
    }

    fseek(dgn, SEEK_SET, 0); //Set file pointer to beginning, just in case something within the file itself is broken.

    //Generated a room, then properly store it into the created file
    for (int x = 0; x <= x_SIZE; x++) {
        for (int y = 0; y <= y_SIZE; y++) {
                //fputc(x, dgn); (let's hope this isn't needed!)
                //fputc(y, dgn);
            genr(roll(1,16));
            fwrite(ldr, 9, 8*8, dgn);
        }
    }

    fclose(dgn);
    return 0;
}

int ldd(int x, int y) //Load any given room into ldr from file.
{
    dgn = fopen("dgn.bin", "rb");
    fseek(dgn, SEEK_SET, 0);
    fseek(dgn, SEEK_SET, (x * y) * sizeof(ldr));

    for (int x = 0; x <= 8; x++) {
        for (int x = 0; x <= 8; x++) {
            fread(&ldr[x][y], 9, 1, dgn);
        }
    }

    fclose(dgn);
    return 0;
}

int main()
{
    srand(time(0)); //Initalize random number gen

    printf("GOOF-CRL Loaded.\n");
    printf("Speed test: \n");
    gend(256, 256);


    return 0;
}
