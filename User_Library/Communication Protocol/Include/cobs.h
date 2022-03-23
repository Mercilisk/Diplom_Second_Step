#ifndef COBS_H
#define COBS_H

#include <stdint.h>
#include <stddef.h>

size_t cobs_encode(const uint8_t * restrict input, size_t length,
	uint8_t * restrict output);
	
size_t cobs_decode(const uint8_t * restrict input, size_t length,
	uint8_t * restrict output);

#endif /* COBS_H */
