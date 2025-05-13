#include "clru.h"
#include <stdio.h>
int main(void) { 
   struct HashMap new_map = HashMap_new();

   char* key = "Hello key!";

   char* val = "I'm a value!";

   struct String key_str = String_from_raw(key);
   struct String val_str = String_from_raw(val);

   HashMap_insert(&new_map, &key_str, &val_str);
   struct String* val_out = HashMap_get(&new_map, &key_str);
   if (val_out != NULL) {
       printf("Value: %s\n", val_out->data);
   } else {
       printf("Key not found!\n");
   }
   HashMap_remove(&new_map, &key_str);
   val_out = HashMap_get(&new_map, &key_str);
   if (val_out != NULL) {
       printf("Value: %s\n", val_out->data);
   } else {
       printf("Key not found!\n");
   }
   HashMap_destruct(&new_map);

   return 0;



}
