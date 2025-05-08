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

int String_hash(struct String* to_hash){
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

struct Node null_node(){

	return (struct Node){
		.hash = 0,
		.key = "",
		.value = "",
		.is_null = true,
		.next = (struct Node*) NULL

	};


}

struct Node Node_from_kv(struct String* key, struct String* val, struct Node* next, int hash){
	struct Node retval = (struct Node) {
		.hash = hash,
		.key = *key,
		.value = *val,
		.next = next
		
	};
	return retval;

}

struct HashMap{
	size_t capacity;
	int load_factor;
	size_t size;

	struct Node* nodes;

};

struct HashMap HashMap_new(){

	size_t capacity = 16;
	int load_factor = 75;
	size_t size = 0;

	struct Node* nodes = (struct Node*) malloc(sizeof(struct Node) * capacity);

	for(int i = 0; i < capacity; i++){
		nodes[i] = null_node();
	}

	struct HashMap retval = (struct HashMap) {
		.capacity = capacity,
		.load_factor = load_factor,
		.nodes = nodes,
		.size = size,
	};

	return retval;
}

void HashMap_insert(struct HashMap* map, struct String key, struct String val){

	int hash = String_hash(&key);
	struct Node new_node = Node_from_kv(&key, &val, (struct Node*) NULL, hash);
	
	size_t index = hash % (map -> capacity);

	if(map -> nodes[index].is_null)
		map -> nodes[index] = new_node;
	else{
		struct Node* current = &map -> nodes[index];
		while(current -> next != (struct Node*) NULL){
			current = current -> next;
		}
		current -> next = (struct Node*) malloc(sizeof(struct Node));
		*(current -> next) = new_node;

	}

}

void HashMap_remove(struct HashMap* map, struct String &key){

	int hash = String_hash(key);
	size_t index = hash % (map -> capacity);
	struct Node* this_node = &(map -> nodes[index]);
	if(map -> this_node -> is_null)
		return;

	else{
		

	}

}





#endif
