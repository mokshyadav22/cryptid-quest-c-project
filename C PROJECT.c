#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for Player
typedef struct {
    char name[50];
    int health;
    int maxHealth;
    int attack;
    int gold;
    int potions;
} Player;

// Structure for Monster
typedef struct {
    char name[30];
    int health;
    int attack;
    int rewardGold;
} Monster;

// Function Prototypes
void clearScreen();
void displayMenu();
void startAdventure(Player *p);
void saveGame(Player p);
int loadGame(Player *p);

int main() {
    Player player;
    int choice;

    do {
        clearScreen();
        printf("========================================\n");
        printf("    CRYPTID QUEST: THE TERMINAL LABYRINTH\n");
        printf("========================================\n");
        printf("1. New Game\n");
        printf("2. Load Saved Game\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("\nEnter your hero's name: ");
                scanf("%s", player.name);
                player.health = 100;
                player.maxHealth = 100;
                player.attack = 15;
                player.gold = 10;
                player.potions = 2;
                startAdventure(&player);
                break;
            case 2:
                if (loadGame(&player)) {
                    printf("\nGame Loaded Successfully!\n");
                    printf("Welcome back, %s!\n", player.name);
                    system("pause"); // Use getchar() if on Linux/macOS
                    startAdventure(&player);
                } else {
                    printf("\nNo saved game found!\n");
                    system("pause");
                }
                break;
            case 3:
                printf("\nExiting game. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while(choice != 3);

    return 0;
}

void clearScreen() {
    // Simple screen clear technique for terminal
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void startAdventure(Player *p) {
    int choice;
    Monster dungeonMonsters[3] = {
        {"Goblin Thief", 40, 10, 15},
        {"Cave Troll", 70, 18, 30},
        {"Labyrinth Guardian", 120, 25, 60}
    };

    int currentRoom = 1;

    while (currentRoom <= 3 && p->health > 0) {
        clearScreen();
        printf("--- ROOM %d OF 3 ---\n", currentRoom);
        printf("Hero: %s | HP: %d/%d | Attack: %d | Gold: %d | Potions: %d\n", 
               p->name, p->health, p->maxHealth, p->attack, p->gold, p->potions);
        printf("----------------------------------------\n");
        printf("A wild %s appears!\n", dungeonMonsters[currentRoom - 1].name);
        printf("1. Attack Monster\n");
        printf("2. Drink Health Potion (+30 HP)\n");
        printf("3. Save Game and Quit to Menu\n");
        printf("Choose action: ");
        scanf("%d", &choice);

        Monster *m = &dungeonMonsters[currentRoom - 1];

        switch(choice) {
            case 1:
                // Combat turn
                m->health -= p->attack;
                printf("\nYou strike the %s for %d damage!\n", m->name, p->attack);
                if (m->health > 0) {
                    p->health -= m->attack;
                    printf("The %s strikes back for %d damage!\n", m->name, m->attack);
                } else {
                    printf("You defeated the %s!\n", m->name);
                    p->gold += m->rewardGold;
                    printf("You looted %d gold.\n", m->rewardGold);
                    currentRoom++;
                }
                system("pause");
                break;
            case 2:
                if (p->potions > 0) {
                    p->health += 30;
                    if (p->health > p->maxHealth) p->health = p->maxHealth;
                    p->potions--;
                    printf("\nYou drank a potion. Current HP: %d\n", p->health);
                } else {
                    printf("\nYou have no potions left!\n");
                }
                system("pause");
                break;
            case 3:
                saveGame(*p);
                printf("\nGame Saved! Returning to main menu...\n");
                system("pause");
                return;
            default:
                printf("\nInvalid action!\n");
                system("pause");
        }
    }

    if (p->health <= 0) {
        clearScreen();
        printf("\nYOU DIED! Game Over.\n");
        system("pause");
    } else if (currentRoom > 3) {
        clearScreen();
        printf("\nCONGRATULATIONS! You conquered the Labyrinth and escaped with %d gold!\n", p->gold);
        system("pause");
    }
}

void saveGame(Player p) {
    FILE *file = fopen("savegame.txt", "w");
    if (file == NULL) {
        printf("Error saving game!\n");
        return;
    }
    fprintf(file, "%s %d %d %d %d %d", p.name, p.health, p.maxHealth, p.attack, p.gold, p.potions);
    fclose(file);
}

int loadGame(Player *p) {
    FILE *file = fopen("savegame.txt", "r");
    if (file == NULL) {
        return 0; // File doesn't exist
    }
    fscanf(file, "%s %d %d %d %d %d", p->name, &(p->health), &(p->maxHealth), &(p->attack), &(p->gold), &(p->potions));
    fclose(file);
    return 1;
}