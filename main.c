#include <gba.h>
#include <stdio.h>

typedef struct {
    const char *name;
    const char *type;
    int level;
    int evo_level;
    const char *evo;
    const char *mega;
} Creature;

static Creature starters[] = {
    {"Flamcub","FIRE",5,16,"Flamiger","Mega Pyroclaw"},
    {"Aquafin","WATER",5,16,"Aquarush","Mega Tidalion"},
    {"Leafling","GRASS",5,16,"Leafang","Mega Floravine"}
};

static const char *gyms[16] = {
    "1 Meadow","2 Tide","3 Ember","4 Storm","5 Stone","6 Frost",
    "7 Shadow","8 Wind","9 Bloom","10 Steel","11 Desert","12 Ocean",
    "13 Thunder","14 Mountain","15 Eclipse","16 Champion"
};

static const char *costumes[6] = {
    "Trainer Outfit","Forest Ranger","Shadow Suit",
    "Frost Gear","Champion Coat","Explorer Gear"
};

static int starter = -1, mega = 0;

static void wait_a(void) {
    while (1) {
        scanKeys();
        if (keysDown() & KEY_A) return;
        VBlankIntrWait();
    }
}

static void clear_screen(void) {
    consoleDemoInit();
    iprintf("\x1b[2J");
}

static void choose_starter(void) {
    int c = 0;
    while (1) {
        clear_screen();
        iprintf("=== CHOOSE YOUR STARTER ===\n\n");
        iprintf("%c Flamcub  [FIRE]\n",  c==0?'>':' ');
        iprintf("%c Aquafin  [WATER]\n", c==1?'>':' ');
        iprintf("%c Leafling [GRASS]\n\n",c==2?'>':' ');
        iprintf("UP/DOWN = choose   A = select\n");
        scanKeys();
        u16 k = keysDown();
        if (k & KEY_UP) c = (c + 2) % 3;
        if (k & KEY_DOWN) c = (c + 1) % 3;
        if (k & KEY_A) { starter = c; return; }
        VBlankIntrWait();
    }
}

static void screen_map(void) {
    clear_screen();
    iprintf("=== REX REGION MAP ===\n\n");
    iprintf("       [SNOW]---[GYM 6]\n");
    iprintf("          |          |\n");
    iprintf("[FOREST]-[REX CITY]-[DESERT]\n");
    iprintf("    |       |          |\n");
    iprintf(" [GYM7]   [GYM8]    [VOLCANO]\n");
    iprintf("    |       |          |\n");
    iprintf(" [OCEAN]--[ROUTE]--[CHAMPION]\n\n");
    iprintf("16 gyms • towns • houses • NPCs\n");
    iprintf("\nA = back");
    wait_a();
}

static void screen_dex(void) {
    clear_screen();
    iprintf("=== REXDEX: 1024 ===\n\n");
    iprintf("#0001 Flamcub   FIRE\n");
    iprintf("#0002 Aquafin   WATER\n");
    iprintf("#0003 Leafling  GRASS\n");
    iprintf("#0004 Voltwing  ELECTRIC\n");
    iprintf("#0005 Stonepaw  ROCK\n");
    iprintf("#0006 Frostfang ICE\n");
    iprintf("#0007 Shadebit  SHADOW\n");
    iprintf("#0008 Psyra     PSYCHIC\n");
    iprintf("\nSlots #0001-#1024 reserved for original creatures.\n");
    iprintf("A = back");
    wait_a();
}

static void screen_gyms(void) {
    clear_screen();
    iprintf("=== 16 GYM JOURNEY ===\n\n");
    for (int i=0;i<16;i++) iprintf("%s\n", gyms[i]);
    iprintf("\nA = back");
    wait_a();
}

static void screen_costumes(void) {
    clear_screen();
    iprintf("=== COSTUME CLOSET ===\n\n");
    for (int i=0;i<6;i++) iprintf("%d. %s\n", i+1, costumes[i]);
    iprintf("\nA = back");
    wait_a();
}

static void screen_story(void) {
    clear_screen();
    iprintf("=== THE ECLIPSE STORY ===\n\n");
    iprintf("1. New Journey\n");
    iprintf("2. First Eclipse Encounter\n");
    iprintf("3. Shadow Forest Secrets\n");
    iprintf("4. Road of 16 Badges\n");
    iprintf("5. Attack on Rex City\n");
    iprintf("6. Varek's Final Plan\n");
    iprintf("7. Champion Island\n\n");
    iprintf("Villain: Varek / Team Eclipse\n");
    iprintf("Relic: Ancient Champion Relic\n\n");
    iprintf("A = back");
    wait_a();
}

static void screen_battle(void) {
    clear_screen();
    Creature c = starters[starter];
    iprintf("=== WILD BATTLE ===\n\n");
    iprintf("REX: %s Lv.%d\n", c.name, c.level);
    iprintf("HP  [##########]\n\n");
    iprintf("A = use move   B = run\n");
    iprintf("START = Mega Evolution when unlocked\n");
    while (1) {
        scanKeys();
        u16 k = keysDown();
        if (k & KEY_B) return;
        if (k & KEY_A) {
            clear_screen();
            iprintf("%s used a move!\n", c.name);
            iprintf("Battle complete. EXP gained.\n\nA = back");
            wait_a();
            return;
        }
        if ((k & KEY_START) && mega) {
            clear_screen();
            iprintf("MEGA EVOLUTION!\n\n");
            iprintf("%s -> %s\n\nA = back", c.name, c.mega);
            wait_a();
            return;
        }
        VBlankIntrWait();
    }
}

static void menu(void) {
    int c=0;
    const char *items[]={"Map","Battle","RexDex","Gyms","Costumes","Story","Profile"};
    while (1) {
        clear_screen();
        iprintf("=== REX REGION ===\n\n");
        for (int i=0;i<7;i++) iprintf("%c %s\n",i==c?'>':' ',items[i]);
        iprintf("\nUP/DOWN  A=select  B=back\n");
        scanKeys();
        u16 k=keysDown();
        if (k&KEY_UP) c=(c+6)%7;
        if (k&KEY_DOWN) c=(c+1)%7;
        if (k&KEY_B) return;
        if (k&KEY_A) {
            if(c==0) screen_map();
            else if(c==1) screen_battle();
            else if(c==2) screen_dex();
            else if(c==3) screen_gyms();
            else if(c==4) screen_costumes();
            else if(c==5) screen_story();
            else {
                clear_screen();
                Creature x=starters[starter];
                iprintf("=== REX PROFILE ===\n\nWhite-haired trainer\nStarter: %s\nType: %s\n",x.name,x.type);
                iprintf("Evolves: Lv.%d -> %s\nMega: %s\n",x.evo_level,x.evo,x.mega);
                iprintf("\nA = back"); wait_a();
            }
        }
        VBlankIntrWait();
    }
}

int main(void) {
    irqInit();
    irqEnable(IRQ_VBLANK);
    consoleDemoInit();

    while (starter < 0) {
        clear_screen();
        iprintf("================================\n");
        iprintf("          REX REGION GBA        \n");
        iprintf("      Original Monster RPG      \n");
        iprintf("================================\n\n");
        iprintf("A = Start\nB = Menu\nSTART = Story\n\n");
        scanKeys();
        u16 k=keysDown();
        if(k&KEY_A) choose_starter();
        if(k&KEY_B) choose_starter();
        if(k&KEY_START) screen_story();
        VBlankIntrWait();
    }

    clear_screen();
    iprintf("Welcome, Rex!\n\n");
    iprintf("Starter: %s\n",starters[starter].name);
    iprintf("Evolution: Lv.%d\n",starters[starter].evo_level);
    iprintf("\nYour 16-gym journey begins.\n\nA = continue");
    wait_a();

    while (1) {
        clear_screen();
        iprintf("=== REX REGION ===\n\n");
        iprintf("A = Menu\nSTART = Story\n");
        iprintf("B = Battle\n");
        iprintf("\nRex • Team Eclipse • 16 Gyms\n");
        scanKeys();
        u16 k=keysDown();
        if(k&KEY_A) menu();
        if(k&KEY_B) screen_battle();
        if(k&KEY_START) screen_story();
        VBlankIntrWait();
    }
}
