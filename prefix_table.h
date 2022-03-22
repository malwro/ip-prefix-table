#ifndef _PREFIX_TABLE_H_
#define _PREFIX_TABLE_H_

/* Represents a prefix table.*/
typedef struct prefixTable prefixTable;

/* Creates a prefix table of capacity initialCapacity
and returns a pointer to the table. Returns NULL upon
failure.
*/
prefixTable *pt_createTable(unsigned long int initialCapacity);

/* Frees the memory allocated for the prefix table and its items.
*/
int pt_freeTable(prefixTable *table);

/* Adds a prefix constisting of 32-bit number base and char mask
to the prefix table. Returns zero if operation was successful, -1
if not. No action is performed on duplication.
*/
int pt_add(prefixTable *table, unsigned int base, char mask);

/* Checks if IP address ip in 32-bit number form is present
in hash table. Returns the mask of the smallest prefix (with
the highest mask value) which contains the specified address.
Returns -1 otherwise.
*/
char pt_check(prefixTable *table, unsigned int ip);

/* Deletes a prefix constisting of 32-bit number base and char mask
from the table. Returns zero if operation was successful, -1 if not.
*/
int pt_del(prefixTable *table, unsigned int base, char mask);

#endif

