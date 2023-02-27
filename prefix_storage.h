#ifndef _PREFIX_STORAGE_H_
#define _PREFIX_STORAGE_H_

/* Represents a prefix table.*/
typedef struct prefixStorage prefixStorage;

/* Creates a prefix table of capacity initialCapacity
and returns a pointer to the table. Returns NULL upon
failure.
*/
prefixStorage *ps_createStorage(unsigned long int initialCapacity);

/* Frees the memory allocated for the prefix table and its items.
*/
int ps_freeStorage(prefixStorage *table);

/* Adds a prefix constisting of 32-bit number base and char mask
to the prefix table. Returns zero if operation was successful, -1
if not. No action is performed on duplication.
*/
int ps_add(prefixStorage *table, unsigned int base, char mask);

/* Checks if IP address ip in 32-bit number form is present
in hash table. Returns the mask of the smallest prefix (with
the highest mask value) which contains the specified address.
Returns -1 otherwise.
*/
char ps_check(prefixStorage *table, unsigned int ip);

/* Deletes a prefix constisting of 32-bit number base and char mask
from the table. Returns zero if operation was successful, -1 if not.
*/
int ps_del(prefixStorage *table, unsigned int base, char mask);

#endif

