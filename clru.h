#ifndef CLRU_H
#define CLRU_H
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define HASH_PRIME_CONST 31
struct String{

	char* data;
	size_t len;

};

struct String String_from_raw(char* raw){
	size_t len = strlen(raw);
	struct String retstr = (struct String) {
		.data = raw,
		.len = len,
	};
	return retstr;
}

int String_hash(const struct String* to_hash){
	int hash = 0;
	
	for(size_t i = 0; i < to_hash->len; i++){
		hash = HASH_PRIME_CONST * hash + (int) (to_hash -> data[i]);
	}
	return hash;
}

struct Node{
	int hash;
	struct String key;
	struct String value;
	bool is_null;
	//In case of collisions
	struct Node* next;

};

struct Node _null_node(){

	return (struct Node){
		.hash = 0,
		.key = "",
		.value = "",
		.is_null = true,
		.next = (struct Node*) NULL

	};


}

struct Node Node_from_kv(const struct String* key, const struct String* val, struct Node* next, int hash){
	struct Node retval = (struct Node) {
		.hash = hash,
		.key = *key,
		.value = *val,
		.next = next
		
	};
	return retval;

}
//frees all nodes except for head of list (should ONLY BE DONE WHEN DESTRUCTING A HASHMAP)
void Node_clean_list(struct Node* head) {
    if (head == NULL || head->is_null) {
		return;
	}
	
	struct Node* current = head->next;
	struct Node* temp;

	while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }

	head->next = NULL;
}

void Node_remove_linkedlist(struct Node** head, const struct String* key){
	struct Node* current = *head;
	struct Node* prev = NULL;

	if (!current->is_null && strcmp(current->value.data, key->data)){
		*head = current -> next;
		free(current);
		return;
	}

	while (current != NULL && strcmp(current->value.data, key->data) != 0){
		prev = current;
		current = current->next;
	}

	if (current == NULL){
		return;
	}

	prev->next = current->next;
	free(current);
}

struct HashMap{
	size_t capacity;
	double load_factor;
	size_t size;

	struct Node* nodes;

};

struct HashMap HashMap_new(){

	size_t capacity = 16;
	double load_factor = 0.75;
	size_t size = 0;

	struct Node* nodes = (struct Node*) malloc(sizeof(struct Node) * capacity);

	for(int i = 0; i < capacity; i++){
		nodes[i] = _null_node();
	}

	struct HashMap retval = (struct HashMap) {
		.capacity = capacity,
		.load_factor = load_factor,
		.nodes = nodes,
		.size = size,
	};

	return retval;
}

void HashMap_destruct(struct HashMap* map){
	
	for(int i = 0; i < map->capacity; i++){
		if(!map->nodes[i].is_null){
			Node_clean_list(&(map->nodes[i]));
		}
	}
	
	free(map->nodes);

}
void HashMap_resize(struct HashMap* map);
void HashMap_insert(struct HashMap* map, const struct String* key, const struct String* val){

	int hash = String_hash(key);
	struct Node new_node = Node_from_kv(key, val, NULL, hash);
	
	size_t index = hash % (map -> capacity);

	if(map -> nodes[index].is_null){
		map -> nodes[index] = new_node;
		map -> size ++;
	}

	else{
		struct Node* current = (&map -> nodes[index]);
		while(current -> next != NULL){
			if (strcmp(current -> key.data, key->data) == 0){
				current->value = *val;
				return;
			}
			current = current -> next;
		}
		current -> next = (struct Node*) malloc(sizeof(struct Node));
		*(current -> next) = new_node;
		map -> size ++;

	}
	if((double) map -> size >= (double) (map -> capacity * map -> load_factor)){
		HashMap_resize(map);
	}

}

void HashMap_resize(struct HashMap* map){
	size_t new_capacity = map->capacity * 2;
	struct Node* new_nodes = (struct Node*) malloc(sizeof(struct Node) * new_capacity);
	for(int i = 0; i < new_capacity; i++){
		new_nodes[i] = _null_node();
	}
	struct HashMap new_map = (struct HashMap) {
		.capacity = new_capacity,
		.size = 0,
		.load_factor = map->load_factor,
		.nodes = new_nodes,

	};

	for(int i = 0; i < map->capacity; i++){
		if(map->nodes[i].is_null){
			continue;
		}
		else{
			struct Node* current = &(map->nodes[i]);
			while (current != NULL){
				HashMap_insert(&new_map, &current->key, &current->value);
				current = current->next;
			}
			Node_clean_list(&(map->nodes[i]));
		}

	}

	free(map->nodes);
	*map = new_map;
}

bool HashMap_contains(struct HashMap* map, const struct String* key);

void HashMap_remove(struct HashMap* map, const struct String* key){
	if (HashMap_contains(map, key) == false){
		return;
	}
	int hash = String_hash(key);
	size_t index = hash % (map -> capacity);
	struct Node* this_node = &(map -> nodes[index]);
	
	Node_remove_linkedlist(&this_node,  key);
	map->size --;

}

struct String* HashMap_get(struct HashMap* map, const struct String* key){

	int hash = String_hash(key);
	size_t index = hash % map->capacity;

	struct Node* val =  &(map->nodes[index]);
	if (val->is_null){
		return NULL;
	}

	while (val != NULL && strcmp(val->value.data, key->data) != 0){
		val = val->next;
	}
	if (val == NULL){
		return NULL;
	}
	else {
		return &val->value;
	}

}

inline bool HashMap_contains(struct HashMap* map, const struct String* key){
	struct String* val = HashMap_get(map, key);

	if(val == NULL){
		return false;
	}
	else return true;
}



#endif
