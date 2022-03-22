#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>


void test_generateData(const char* dataFilename, const char* maskFilename, const char* netAddrFilename) 
{
        char lineNumsDotsIP[15];
        unsigned int cidr, mask32Bit, netAddr32Bit, ip32Bit;

        FILE *dataFile = fopen(dataFilename, "r");
        FILE *maskFile = fopen(maskFilename, "w");
        FILE *netAddrFile = fopen(netAddrFilename, "w");
        
        if (dataFile == NULL || maskFile == NULL || netAddrFile == NULL) {
                printf("ERROR: Could not open a file. \n");
		perror("Cause:");
                exit(-1);
        }

        while(fscanf(dataFile, "%s", lineNumsDotsIP) == 1) {
                ip32Bit = inet_network(lineNumsDotsIP);

                while(1) {
                        cidr = (rand() % 32);
                        mask32Bit = ((1 << cidr) - 1) << (32-cidr);
                        netAddr32Bit = (ip32Bit & mask32Bit);
                        
                        if (netAddr32Bit != 0)
                                break;
                }

                fprintf(maskFile, "%u\n", cidr);
                fprintf(netAddrFile, "%u\n", netAddr32Bit);
        }

        fseek(dataFile, 0, SEEK_SET);
        fseek(maskFile, 0, SEEK_SET);
	fseek(netAddrFile, 0, SEEK_SET);

        fclose(dataFile);
        fclose(maskFile);
        fclose(netAddrFile);
}
