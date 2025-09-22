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

/*===============hashmap's function=============*/
unsigned long hash(char *str, int capacity){
  unsigned long index = 5381;
  int c;
  while((c = *str++)){
    index = ((index << 5) + index) + c;
  }
  return index % capacity;
}

void insertNode(HashMap *map, char *str, int data){
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
  insertNode(myMap, "chanh", 10);
  //deleteNode(myMap, "chanh");

  int data = lookUpData(myMap, "chanh");

  printf("%d", data);
  
  deleteMap(myMap);
  return EXIT_SUCCESS;
}
