#include "prefix_table.h"
#include "generator.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>


int main()
{
	int cidr, operationResult;
	char lineNumsDotsIP[15], checkResult = 0;
	unsigned int lineNet, lineMask, netAddr32Bit, mask32Bit, ip32Bit;

	const char *dataFilename = "random_ips.txt",
		*maskFilename = "masks.txt",
		*netAddrFilename = "net_addresses.txt",
		*testFilename = "test_ips.txt";
		
	/* test_generateData(dataFilename, maskFilename, netAddrFilename); */

	FILE *dataFile = fopen(dataFilename, "r");
	FILE *maskFile = fopen(maskFilename, "r");
        FILE *netAddrFile = fopen(netAddrFilename, "r");
	FILE *testFile = fopen(testFilename, "r");

	if (dataFile == NULL || netAddrFile == NULL || netAddrFile == NULL || testFile == NULL) {
                printf("ERROR: Could not open a file. \n");
		perror("Cause:");
                exit(-1);
        }


	/* Create a hash table */
	prefixTable *table = pt_createTable(310);


	/* Add 50 prefixes to the hash table in bulk */
	while (fscanf(netAddrFile, "%u", &lineNet) == 1 && fscanf(maskFile, "%u", &lineMask)) {
		operationResult = pt_add(table, lineNet, (char)lineMask);
	}


	/* Check if IP addresses which were used to create prefixes can be found in the hash table */
	printf("TEST 1: Check for IPs from dataFile\n");
	while (fscanf(dataFile, "%s", lineNumsDotsIP) == 1) {
		ip32Bit = inet_network(lineNumsDotsIP);
		checkResult = pt_check(table, ip32Bit);

		if (checkResult != -1)
			printf("FOUND IP %s, suitable subnet mask: %u\n", lineNumsDotsIP, (unsigned int)checkResult);
		else
			printf("NO IP %s\n", lineNumsDotsIP);
	}


	/* Perform check with test IPs (which were not used to create prefixes) */
	printf("\n\nTEST 2: Check for test IPs\n");
	while (fscanf(testFile, "%s", lineNumsDotsIP) == 1) {
		ip32Bit = inet_network(lineNumsDotsIP);
		checkResult = pt_check(table, ip32Bit);

		if (checkResult != -1)
			printf("FOUND IP %s, suitable subnet mask: %u\n", lineNumsDotsIP, (unsigned int)checkResult);
		else
			printf("NO IP %s\n", lineNumsDotsIP);	
	}


	/* Duplicate addition of the first position from network and mask .txt files */
	fseek(netAddrFile, 0, SEEK_SET);
	fseek(maskFile, 0, SEEK_SET);

	operationResult = fscanf(maskFile, "%u", &lineMask);
	operationResult = fscanf(netAddrFile, "%u", &lineNet);
	operationResult = pt_add(table, lineNet, (char)lineMask);

	printf("\n\nResult of duplicated addition: %d\n\n", operationResult);


	/* Perform bulk deletion */
	fseek(netAddrFile, 0, SEEK_SET);
	fseek(maskFile, 0, SEEK_SET);

	while (fscanf(netAddrFile, "%u", &lineNet) == 1 && fscanf(maskFile, "%u", &lineMask)) {
		operationResult = pt_del(table, lineNet, (char)lineMask);
	}


	/* Perform deletion of previously deleted item */
	fseek(netAddrFile, 0, SEEK_SET);
	fseek(maskFile, 0, SEEK_SET);
	fscanf(netAddrFile, "%u", &lineNet) == 1 && fscanf(maskFile, "%u", &lineMask);
	operationResult = pt_del(table, lineNet, (char)lineMask);
	printf("\n\nResult of deletion of previously deleted item: %d\n\n", operationResult);


	/* Check for deleted prefixes */
	printf("\nTEST 3: Check for deleted prefixes\n");
	fseek(dataFile, 0, SEEK_SET);
	while (fscanf(dataFile, "%s", lineNumsDotsIP) == 1) {
		ip32Bit = inet_network(lineNumsDotsIP);
		checkResult = pt_check(table, ip32Bit);

		if (checkResult != -1)
			printf("FOUND IP %s, suitable subnet mask: %u\n", lineNumsDotsIP, (unsigned int)checkResult);
		else
			printf("NO IP %s\n", lineNumsDotsIP);
	}


	/* Free the memory allocated for the hash table */
	operationResult = pt_freeTable(table);
	printf("\n\nResult of table freeing up: %d\n", operationResult);

	fclose(dataFile);
	fclose(maskFile);
	fclose(netAddrFile);
	fclose(testFile);

	return 0;
}

