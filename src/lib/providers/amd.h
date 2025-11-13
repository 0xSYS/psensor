/*
 * Copyright (C) 2010-2011 thgreasi@gmail.com, jeanfi@gmail.com
 * Copyright (C) 2012-2016 jeanfi@gmail.com
 * Copyright (C) 2025 kiptunor
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */









#ifndef _PSENSOR_AMD_H_
#define _PSENSOR_AMD_H_

#include "../psensor.h"
#include <log_c/log.h>




// Too old to be used and it's not supported anymore
static inline void amd_psensor_list_update(struct psensor **s)
{
    log_warn("amd_psensor_list_update(): AMD ADL PROVIDER IS DEPRECATED !!!");
}
static inline void amd_psensor_list_append(struct psensor ***s, int n)
{
    log_warn("amd_psensor_list_append(): AMD ADL PROVIDER IS DEPRECATED !!!");
}
static inline void amd_cleanup(void)
{
    log_warn("amd_cleanup(): AMD ADL PROVIDER IS DEPRECATED !!!");
}

#endif
