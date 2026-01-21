#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10

typedef struct HashNode {
    int key;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *table[TABLE_SIZE];
} HashMap;

int hash(int key)
{
    return key % TABLE_SIZE;
}

void initHashMap(HashMap *map)
{
    for (int i = 0; i < TABLE_SIZE; i++)
        map->table[i] = NULL;
}

void insert(HashMap *map, int key)
{
    int index = hash(key);
    printf("key :%d -index:%d\n",key ,index);

    HashNode *node = malloc(sizeof(HashNode));
    node->key = key;
    node->next = map->table[index];
    map->table[index] = node;
}

int search(HashMap *map, int key)
{
    int index = hash(key);
    HashNode *curr = map->table[index];

    while (curr) {
        if (curr->key == key)
            return 1;
        curr = curr->next;
    }
    return 0;
}

void freeHashMap(HashMap *map)
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *curr = map->table[i];
        while (curr) {
            HashNode *temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
}

// 🔥 MAIN LOGIC
int longestConsecutive(int arr[], int size)
{
    HashMap map;
    initHashMap(&map);

    for (int i = 0; i < size; i++)
        insert(&map, arr[i]);

    int maxCount = 0;

    for (int i = 0; i < size; i++) {

        int value = arr[i];

        // if (search(&map, value - 1))
        //     continue;

        int count = 1;
        int next = value + 1;

        while (search(&map, next)) {
            count++;
            next++;
        }

        if (count > maxCount)
            maxCount = count;
    }

    freeHashMap(&map);
    return maxCount;
}

int main()
{
    int arr[] = {100, 16, 4, 2, 1, 3, 200, 101,1010,1011};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Longest Consecutive Count: %d\n",
           longestConsecutive(arr, size));

    return 0;
}
