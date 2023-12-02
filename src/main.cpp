#include <stdio.h>
#include <string.h>

typedef void* Pointer;

typedef struct {
    enum { 
        NULL_ITEM           = 0, 
        DEFAULT_CAPACITY    = 128, 
        MAX_CAPACITY        = 136 
    };

    size_t nextFreeIndex    = 0;
    size_t commonSize       = 0;

    char mem[MAX_CAPACITY];
} Arena;

typedef struct {
    Pointer objects[Arena::MAX_CAPACITY];
    size_t nbAllocs = 0;
} AllocatedObjects;

AllocatedObjects allocatedObjects;

#define ANALIZE_ARENA_MEM(ARENA, OBJ_SIZE)                                                        \
    for (size_t idx = 0; idx < allocatedObjects.nbAllocs * OBJ_SIZE; idx += OBJ_SIZE)             \
    {                                                                                             \
        printf("arena.mem[idx: %zu]: %p with value: %d\n", idx, &ARENA.mem[idx], ARENA.mem[idx]); \
    }

#define PRINT_ALL_ARENA_MEM(ARENA, OBJ_SIZE)                                                      \
    for (size_t idx = 0; idx <= ARENA.DEFAULT_CAPACITY; idx += OBJ_SIZE)                          \
    {                                                                                             \
        printf("arena.mem[idx: %zu]: %p with value: %d\n", idx, &ARENA.mem[idx], ARENA.mem[idx]); \
    }

void* allocate(size_t size, Arena& arena) {
    void* ptr = &arena.mem[arena.nextFreeIndex];
    arena.nextFreeIndex += size;
    allocatedObjects.objects[allocatedObjects.nbAllocs] = ptr;
    allocatedObjects.nbAllocs++;
    return ptr;
}

void deallocate(Pointer ptr, Arena& arena) {
    for (size_t idx = 0; idx < allocatedObjects.nbAllocs * arena.commonSize; idx += arena.commonSize) {
        if (&arena.mem[idx] == ptr) {
            arena.mem[idx] = Arena::NULL_ITEM;
        }
    }
    for (size_t idx = 0; idx < allocatedObjects.nbAllocs; idx++) {
        if (allocatedObjects.objects[idx] == ptr) {
            printf("WAS DEALLOCATED: [%p]\n", ptr);
            allocatedObjects.objects[idx] = NULL;
        } 
    }
}

void initArrayAsNULL(void* array, size_t size) {
    memset(array, Arena::NULL_ITEM, size);
}

void initArena(Arena& arena, size_t commonSizeObjects) {
    arena.commonSize = commonSizeObjects;
    initArrayAsNULL(arena.mem, arena.MAX_CAPACITY);
}

void initObjectsOfAllocatedTraceAsNULL(AllocatedObjects& obj, size_t size) {
    initArrayAsNULL(obj.objects, Arena::MAX_CAPACITY);
}

int getIndex(void* ptr, Arena& arena) {
    int idx =  (int) arena.commonSize * (-1);
    for (idx; idx < arena.DEFAULT_CAPACITY;) {
        if (&arena.mem[idx += sizeof(int)] == ptr) {
            return idx;
        }
    }
    return -1;
}

int main() {

    Arena arena;

    initArena(arena, sizeof(int));

    int* i =  (int*) allocate(sizeof(int), arena);
    *i = 9;
    int initIndexOfI = getIndex(i, arena);

    printf("Get index of i of address(%p): %i\n", i, initIndexOfI);

    printf("Value of allocations: %zu\n", allocatedObjects.nbAllocs);
    printf("Value of i: %d in addr of mem: %p\n", *i, i);

    printf("Now value of 'i': %d\n", *i);    
    printf("Printing the arena.mem in the index(%i): %p: with a size of: %zu\n", initIndexOfI, &arena.mem[0], arena.commonSize);

    printf("---------------------------------------------------------------------------\n");

    int* i2 = (int*) allocate(sizeof(int), arena);
    *i2 = 10;
    int initIndexOfI2 = getIndex(i2, arena);

    printf("Value of allocations: %zu\n", allocatedObjects.nbAllocs);
    printf("Value of i2: %d in addr of mem: %p\n", *i2, i2);

    printf("Get index of i2 of address(%p): %i\n", i2, initIndexOfI2);


    printf("Now value of 'i2': %d\n", *i2);
    printf("Printing the arena.mem in the index(%i): %p: with a size of: %zu\n", initIndexOfI2, &arena.mem[4], arena.commonSize);

    printf("---------------------------------------------------------------------------\n");
    printf("printing all the arena memory adresses...\n");
    ANALIZE_ARENA_MEM(arena, sizeof(int));
    printf("---------------------------------------------------------------------------\n");

    PRINT_ALL_ARENA_MEM(arena, sizeof(int));

    printf("---------------------------------------------------------------------------\n");
    printf("Deallocating I...\n");
    deallocate(i, arena);
    PRINT_ALL_ARENA_MEM(arena, sizeof(int));

    return 0;
}