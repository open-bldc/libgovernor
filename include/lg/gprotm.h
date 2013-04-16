/*
 * libgovernor - Open-BLDC configuration and debug protocol library
 * Copyright (C) 2009-2010 by Piotr Esden-Tempski <piotr@esden.net>
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

/*
 * Function definitions of the governor master protocol library
 */

#ifndef GPROTM_H
#define GPROTM_H

#include <stdint.h>

#include "lg/gpdef.h"

int gpm_init(gp_simple_hook_t trigger_output, void *trigger_output_data,
	     gp_with_addr_hook_t register_changed, void *register_changed_data);

int gpm_set_log(gp_simple_hook_t cb, void *data);
int gpm_set_string_received_callback(gp_with_string_hook_t string_received, void *string_received_data);

int32_t gpm_get_register_map_val(uint8_t addr);
int32_t gpm_pickup_byte(void);

int gpm_send_set(uint8_t addr, uint16_t val);
int gpm_send_get(uint8_t addr);
int gpm_send_get_cont(uint8_t addr);
int gpm_send_get_version(void);

int gpm_handle_byte(uint8_t byte);

#endif /* GPROTM_H */
