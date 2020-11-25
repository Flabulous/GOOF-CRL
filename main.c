#include <stdio.h>
#include <stdlib.h>
struct entity {
    char typ; //Type of Entity (Player, Monster, Chest, Wall)
    char hit; //Hitpoints (Used to determine Status)
    char atk; //Attack (Used for combat)
    char def; //Defense (Used for combat)
    char sts; //Status (Alive, Dead, or Static)
    char inv[]; //Inventory (Armor, Weapon, Potion, Gold)
};

int

int main()
{
    printf("Hello world!\n");
    return 0;
}
