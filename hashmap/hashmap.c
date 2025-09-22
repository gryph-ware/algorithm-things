#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define INITIAL_SIZE 97
#define LOAD_FACTOR 0.85

/*=================define what hashmap must have==========*/
typedef struct Node{
  char *str_key;
  int data;
  int freq;
  struct Node *next;
} Node;

typedef struct HashMap{
  int size;
  int capacity;
  Node **table;
}HashMap;

/*=================djb2 hash function==========*/
unsigned long hash(char *str, int capacity){
  unsigned long index = 5381;
  int c;
  while((c = *str++)){
    index = ((index << 5) + index) + c;
  }
  return index % capacity;
}

/*===============replacement function=============*/
char *customDup(char *str){
  if(str == NULL){
    return NULL;
  }
  char *str_dup = malloc(strlen(str) + 1);
  strcpy(str_dup, str);
  return str_dup;
}

/*===============create things=============*/
HashMap *createMap(){
  HashMap *map = malloc(sizeof(HashMap));
  map->size = 0;
  map->capacity = INITIAL_SIZE;
  map->table = calloc(map->capacity, sizeof(Node*));
  return map;
}

Node *createNode(char *str, int data){
  Node *new = malloc(sizeof(Node));
  new->str_key = customDup(str);
  new->data = data;
  new->freq = 1;
  new->next = NULL;
  return new;
}
/*===============resize entire hashmap=============*/
int primes[] = {199, 431, 1237, 3011, 10007, 20021, 40009, 60013, 80021, 90001, 99991};
int size = sizeof(primes) / sizeof(primes[0]);

int nextSize(int current){
  for(int i = 0; i < size; i++){
    if(current < primes[i]){
      return primes[i];
    }
  }
  return (primes[size - 1] * 2) + 1;
}

void reSize(HashMap *map){
  int old_capacity = map->capacity;
  int newSize = nextSize(old_capacity);

  Node **newTable = calloc(newSize, sizeof(Node*));
  
  for(int i = 0; i < old_capacity; i++){
    Node *entry = map->table[i];
    while(entry){
      Node *next = entry->next;
      unsigned long index = hash(entry->str_key, newSize);
      entry->next = newTable[index];
      newTable[index] = entry;
      entry = next;
    }
  }
  free(map->table);
  map->table = newTable;
  map->capacity = newSize;
}

/*===============hashmap's function=============*/
void insertNode(HashMap *map, char *str, int data){
  if((float) (map->size + 1) / (map->capacity) >= LOAD_FACTOR){
    reSize(map);
  }
  unsigned long index = hash(str, map->capacity);
  Node *entry = map->table[index];
  while(entry){
    if(strcmp(entry->str_key, str) == 0){
      entry->data = data;
      entry->freq ++;
      return;
    }
    entry = entry->next;
  }
  Node *newNode = createNode(str, data);
  newNode->next = map->table[index];
  map->table[index] = newNode;
  map->size++;
}

int lookUpData(HashMap *map, char *str){
  unsigned long index = hash(str, map->capacity);
  Node *entry = map->table[index];
  while(entry){
    if(strcmp(entry->str_key, str) == 0){
      return entry->data;
    }
    entry = entry->next;
  }
  return 0;
}

/* void displayMap(HashMap *map){ */
/*   for(int i = 0; i < map->capacity */
/* } */

void deleteNode(HashMap *map, char *str){
  unsigned long index = hash(str, map->capacity);
  Node *next = map->table[index];
  Node *prev = NULL;
  while(next){
    if(strcmp(next->str_key, str) == 0){
      if(prev){
	prev->next = next->next;
      } else {
	map->table[index] = next->next;
      }
      free(next->str_key);
      free(next);
      map->size--;
      return;
    }
    prev = next;
    next = next->next;
  }
}

void deleteMap(HashMap *map){
  for(int i = 0; i < map->capacity; i++){
    Node *entry = map->table[i];
    while(entry){
      Node *temp = entry;
      entry = entry->next;
      free(temp->str_key);
      free(temp);
    }
  }
  free(map->table);
  free(map);
}

int main(){

  HashMap *myMap = createMap();
  //insertNode(myMap, "chanh", 10);
  //deleteNode(myMap, "chanh");

  //int data = lookUpData(myMap, "chanh");

  //  printf("%d", data);
  
  deleteMap(myMap);
  
  return EXIT_SUCCESS;
}
