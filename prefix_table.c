#include "prefix_table.h"

#include <stdio.h>
#include <stdlib.h>

struct ipPrefix {
	unsigned int base;
	char mask;
};


struct tableItem {
	unsigned int key;
	struct ipPrefix *prefix;
};


struct prefixTable {
	struct tableItem **items;
	unsigned long int capacity;
	unsigned long int length;
};


prefixTable *pt_createTable(unsigned long int initialCapacity)
{
	prefixTable *table = malloc(sizeof(prefixTable));
	
	if (table == NULL) {
		return NULL;
	}

	table->length = 0;
	table->capacity = initialCapacity;
	table->items = calloc(table->capacity, sizeof(struct tableItem*));

	if (table->items == NULL) {
		free(table);
		return NULL;
	}

	return table;
}


int pt_freeTable(prefixTable *table)
{
	if (table == NULL)
		return -1;

	for (unsigned long int i = 0; i < table->capacity; ++i) {
		if (table->items[i] != NULL) {
			free(table->items[i]->prefix);
			free(table->items[i]);
		}
	}
	
	free(table->items);
	free(table);

	return 0;
}


int pt_addItem(struct tableItem **tableItems, unsigned long int tableCapacity, struct ipPrefix *prefix)
{
	unsigned int key = prefix->base - (unsigned int)prefix->mask;
	unsigned long int index = (unsigned long int)key % (tableCapacity - 1);

	struct tableItem *item = malloc(sizeof(struct tableItem));

	if (item == NULL)
		return -1;

	item->key = key;
	item->prefix = prefix;

	while (tableItems[index] != NULL) {
		if (tableItems[index]->key == key) {
			free(item->prefix);
			free(item);
			return -1;
		}
		else {
			++index;
			if (index >= tableCapacity)
				index = 0;
		}
	}

	tableItems[index] = item;

	return 0;
}


int pt_realloc(prefixTable *table, int mode) 
{
	unsigned long int newCapacity;

	if (mode == 1) {
		newCapacity = table->capacity * 2;
		if (newCapacity < table->capacity)
			return -1; /* overflow */
	}
	else
		newCapacity = table->capacity / 2;
	
	struct tableItem **newItems = calloc(newCapacity, sizeof(struct tableItem));

	if (newItems == NULL)
		return -1;
	
	for (unsigned long int i = 0; i < table->capacity; ++i) {
		struct tableItem *item = table->items[i];

		if (item != NULL) {
			pt_addItem(newItems, newCapacity, item->prefix);
			free(table->items[i]);
		}
	}

	free(table->items);
	
	table->capacity = newCapacity;
	table->items = newItems;

	return 0;
}


int pt_add(prefixTable *table, unsigned int base, char mask) 
{
	int ret = 0;

	if (table == NULL || (unsigned int)mask > 32)
		return -1;

	struct ipPrefix *prefix = malloc(sizeof(struct ipPrefix));

	if (prefix == NULL)
		return -1;

	prefix->base = base;
	prefix->mask = mask;

	if (table->length >= table->capacity / 2) {
		if (pt_realloc(table, 1) == -1)
			return -1;
	}

	int result = pt_addItem(table->items, table->capacity, prefix);

	if (result == 0) {
		++table->length;
		ret = 0;
	}
	else
		ret = -1;

	return ret;
}


char pt_check(prefixTable *table, unsigned int ip) 
{
	unsigned long int index;
	unsigned int key, mask32Bit, netAddr32Bit;

	for (unsigned int cidr = 32; cidr != -1; --cidr) {
		mask32Bit = ((1 << cidr) - 1) << (32-cidr);
		netAddr32Bit = (ip & mask32Bit);
		key = netAddr32Bit - cidr;
		index = (unsigned long int)key % (table->capacity - 1);

		while (table->items[index] != NULL) {
			if (table->items[index]->key == key)
				return table->items[index]->prefix->mask;

			++index;

			if (index >= table->capacity)
				index = 0;
		}
	}

	return -1;
}


int pt_delItem(struct tableItem **tableItems, unsigned int tableCapacity, unsigned int base, char mask) 
{
	int ret;
	unsigned int key = base - (unsigned int)mask;
	unsigned long int index, stopIndex = 0;
	index = (unsigned long int)key % (tableCapacity - 1);

	if (index > 0) 
		stopIndex = index - 1;
	else
		stopIndex = tableCapacity - 1;
	

	while (index != stopIndex) {
		if (tableItems[index] != NULL && tableItems[index]->key == key) {
			ret = 0;
			break;
		}
		else {

			++index;
			if (index >= tableCapacity)
				index = 0;
		}
	}

	if (tableItems[index] != NULL && tableItems[index]->key == key) {
		free(tableItems[index]->prefix);
		tableItems[index]->prefix = NULL;
		free(tableItems[index]);
		tableItems[index] = NULL;
	}
	else
		ret = -1;

	return ret;
}


int pt_del(prefixTable *table, unsigned int base, char mask)
{
	int ret = 0;

	if (table == NULL || (unsigned int)mask > 32)
		return -1;

	int result = pt_delItem(table->items, table->capacity, base, mask);
	
	if (result == 0) {
		--table->length;
		ret = 0;
	}
	else
		ret = -1;

	if (table->length <= table->capacity / 6 && table->length >= 6) {
		if (pt_realloc(table, 0) == -1)
			return -1;
	}
	
	return ret;
}
