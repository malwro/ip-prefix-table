#include "prefix_storage.h"

#include <stdio.h>
#include <stdlib.h>

struct ipPrefix {
	unsigned int base;
	char mask;
};


struct storageItem {
	unsigned int key;
	struct ipPrefix *prefix;
};


struct prefixStorage {
	struct storageItem **items;
	unsigned long int capacity;
	unsigned long int length;
};


prefixStorage *ps_createStorage(unsigned long int initialCapacity)
{
	prefixStorage *storage = malloc(sizeof(prefixStorage));
	
	if (storage == NULL) {
		return NULL;
	}

	storage->length = 0;
	storage->capacity = initialCapacity;
	storage->items = calloc(storage->capacity, sizeof(struct storageItem*));

	if (storage->items == NULL) {
		free(storage);
		return NULL;
	}

	return storage;
}


int ps_freeStorage(prefixStorage *storage)
{
	if (storage == NULL)
		return -1;

	for (unsigned long int i = 0; i < storage->capacity; ++i) {
		if (storage->items[i] != NULL) {
			free(storage->items[i]->prefix);
			free(storage->items[i]);
		}
	}
	
	free(storage->items);
	free(storage);

	return 0;
}


int ps_addItem(struct storageItem **storageItems, unsigned long int tableCapacity, struct ipPrefix *prefix)
{
	unsigned int key = prefix->base - (unsigned int)prefix->mask;
	unsigned long int index = (unsigned long int)key % (tableCapacity - 1);

	struct storageItem *item = malloc(sizeof(struct storageItem));

	if (item == NULL)
		return -1;

	item->key = key;
	item->prefix = prefix;

	while (storageItems[index] != NULL) {
		if (storageItems[index]->key == key) {
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

	storageItems[index] = item;

	return 0;
}


int ps_realloc(prefixStorage *storage, int mode) 
{
	unsigned long int newCapacity;

	if (mode == 1) {
		newCapacity = storage->capacity * 2;
		if (newCapacity < storage->capacity)
			return -1; /* overflow */
	}
	else
		newCapacity = storage->capacity / 2;
	
	struct storageItem **newItems = calloc(newCapacity, sizeof(struct storageItem));

	if (newItems == NULL)
		return -1;
	
	for (unsigned long int i = 0; i < storage->capacity; ++i) {
		struct storageItem *item = storage->items[i];

		if (item != NULL) {
			ps_addItem(newItems, newCapacity, item->prefix);
			free(storage->items[i]);
		}
	}

	free(storage->items);
	
	storage->capacity = newCapacity;
	storage->items = newItems;

	return 0;
}


int ps_add(prefixStorage *storage, unsigned int base, char mask) 
{
	int ret = 0;

	if (storage == NULL || (unsigned int)mask > 32)
		return -1;

	struct ipPrefix *prefix = malloc(sizeof(struct ipPrefix));

	if (prefix == NULL)
		return -1;

	prefix->base = base;
	prefix->mask = mask;

	if (storage->length >= storage->capacity / 2) {
		if (ps_realloc(storage, 1) == -1)
			return -1;
	}

	int result = ps_addItem(storage->items, storage->capacity, prefix);

	if (result == 0) {
		++storage->length;
		ret = 0;
	}
	else
		ret = -1;

	return ret;
}


char ps_check(prefixStorage *storage, unsigned int ip) 
{
	unsigned long int index;
	unsigned int key, mask32Bit, netAddr32Bit;

	for (unsigned int cidr = 32; cidr != -1; --cidr) {
		mask32Bit = ((1 << cidr) - 1) << (32-cidr);
		netAddr32Bit = (ip & mask32Bit);
		key = netAddr32Bit - cidr;
		index = (unsigned long int)key % (storage->capacity - 1);

		while (storage->items[index] != NULL) {
			if (storage->items[index]->key == key)
				return storage->items[index]->prefix->mask;

			++index;

			if (index >= storage->capacity)
				index = 0;
		}
	}

	return -1;
}


int ps_delItem(struct storageItem **storageItems, unsigned int tableCapacity, unsigned int base, char mask) 
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
		if (storageItems[index] != NULL && storageItems[index]->key == key) {
			ret = 0;
			break;
		}
		else {

			++index;
			if (index >= tableCapacity)
				index = 0;
		}
	}

	if (storageItems[index] != NULL && storageItems[index]->key == key) {
		free(storageItems[index]->prefix);
		storageItems[index]->prefix = NULL;
		free(storageItems[index]);
		storageItems[index] = NULL;
	}
	else
		ret = -1;

	return ret;
}


int ps_del(prefixStorage *storage, unsigned int base, char mask)
{
	int ret = 0;

	if (storage == NULL || (unsigned int)mask > 32)
		return -1;

	int result = ps_delItem(storage->items, storage->capacity, base, mask);
	
	if (result == 0) {
		--storage->length;
		ret = 0;
	}
	else
		ret = -1;

	if (storage->length <= storage->capacity / 6 && storage->length >= 6) {
		if (ps_realloc(storage, 0) == -1)
			return -1;
	}
	
	return ret;
}
