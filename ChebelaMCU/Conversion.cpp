// 
// 
// 

#include "Conversion.h"

int Conversion::ByteToInt16(unsigned char* b) {
	int f;
	char bt[] = { b[0], b[1] };
	memcpy(&f, &bt, 2);
	return f;
}