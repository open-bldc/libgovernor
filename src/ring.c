/*
 * libgovernor - Open-BLDC configuration and debug protocol library
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>

#include "lg/ring.h"

void ring_init(struct ring *ring, uint8_t * buf, ring_size_t size)
{
	ring->data = buf;
	ring->size = size;
	ring->begin = 0;
	ring->end = 0;
}

int32_t ring_write_ch(struct ring *ring, uint8_t ch)
{
	if (((ring->end + 1) % ring->size) != ring->begin) {
		ring->data[ring->end++] = ch;
		ring->end %= ring->size;
		return (int32_t) ch;
	}

	return -1;
}

int32_t ring_write(struct ring * ring, uint8_t * data, ring_size_t size)
{
	int32_t i;

	for (i = 0; i < size; i++) {
		if (ring_write_ch(ring, data[i]) < 0) {
			return -i;
		}
	}

	return i;
}

int32_t ring_safe_write_ch(struct ring *ring, uint8_t ch)
{
	int ret;
	int retry_count = 100;

	do {
		ret = ring_write_ch(ring, ch);
	} while ((ret < 0) && ((retry_count--) > 0));

	return ret;
}

int32_t ring_safe_write(struct ring * ring, uint8_t * data, ring_size_t size)
{
	int32_t i;

	for (i = 0; i < size; i++) {
		if (0 > ring_safe_write_ch(ring, data[i])) {
			return -i;
		}
	}

	return i;
}

int32_t ring_read_ch(struct ring * ring, uint8_t * ch)
{
	int32_t ret = -1;

	if (ring->begin != ring->end) {
		ret = ring->data[ring->begin++];
		ring->begin %= ring->size;
		if (ch)
			*ch = ret;
	}

	return ret;
}

int32_t ring_read(struct ring * ring, uint8_t * data, ring_size_t size)
{
	int32_t i;

	for (i = 0; i < size; i++) {
		if (ring_read_ch(ring, data + i) < 0) {
			return i;
		}
	}

	return -i;
}
