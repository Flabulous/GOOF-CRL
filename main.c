#include <stdio.h>
#include <stdlib.h>

struct entity {
    char typ; //Type of Entity (Player, Monster, Chest, Wall)
    char hit; //Hitpoints (Used to determine Status)
    char atk; //Attack (Used for combat)
    char def; //Defense (Used for combat)
    char sts; //Status (Alive, Dead, or Static)
    char inv[6]; //Inventory (Armor, Weapon, Potion, Gold, X Position, Y Position)
};



struct entity ldr[7][7]; //Loaded Room

int gen() //Generate a dungeon
{

}

int main()
{
    printf("GOOF-CRL Loaded.\n");
    return 0;
}
