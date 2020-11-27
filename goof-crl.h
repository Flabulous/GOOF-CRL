#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

//Backend variables:
#define dgn_name "dgn.bin" //Default name for Dungeon, is changeable
FILE *dgn; //File for storing built dungeon (Created if not found already)
//Entity types
#define PLR 0x01
#define MNR 0x02
#define CST 0x03
#define WLL 0x04

//Entity statuses;
#define ALV 0x01
#define DED 0x02
#define STC 0x03

struct entity {
    char typ; //Type of Entity, used for determining sprites and AI (Player (0x01), Monster(0x02), Chest(0x03), Wall(0x04))
    char hit; //Hitpoints (Used to determine Status)
    char atk; //Attack (Used for combat)
    char def; //Defense (Used for combat)
    char sts; //Status, used for determined interactability (Alive (0x01), Dead (0x02), or Static (0x03))
    char inv[4]; //Inventory, (Armor, Weapon, Potion, Gold)
};

struct entity ldr[8][8]; //Loaded Room
