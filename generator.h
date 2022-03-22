#ifndef _GENERATOR_H_
#define _GENERATOR_H_

/* Uses IP addresses in numbers-and-dots notation from <dataFilename>.txt
file as well as randomly generated masks to generate IPv4 subnet addresses
in 32-bit integer form. Stores generated masks and subnet addresses in
<maskFilename>.txt and <netAddrFilename>.txt files, accordingly.
*/
void test_generateData(const char* dataFilename, const char* maskFilename, const char* netAddrFilename);

#endif