/*
 * Copyright (C) 2010-2024 jeanfi@gmail.com, 0xSYS
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



#include "include/psensor/precord.h"
#include "include/psensor/plog.h"
#include "ext_lib/CMessagePack/cmp.h"

#include <stdlib.h>
#include <stdio.h>


//Helper functions used for initialisation, reading and writing PSensor recordings
static bool read_bytes(void *data, size_t sz, FILE *fh)
{
  return fread(data, sizeof(uint8_t), sz, fh) == (sz * sizeof(uint8_t));
}

static bool file_reader(cmp_ctx_t *ctx, void *data, size_t limit)
{
  return read_bytes(data, limit, (FILE *)ctx->buf);
}

static bool file_skipper(cmp_ctx_t *ctx, size_t count)
{
  return fseek((FILE *)ctx->buf, count, SEEK_CUR);
}

static size_t file_writer(cmp_ctx_t *ctx, const void *data, size_t count)
{
  return fwrite(data, sizeof(uint8_t), count, (FILE *)ctx->buf);
}

int psr_init(const char * prf_name)
{
  int fn_stat;
  FILE * prf = NULL;
  cmp_ctx_t prfStruct = {0};

  //Initialise file writing in binary mode
  prf = fopen(prf_name, "w+b");

  if(prf == NULL)
  {
    log_err("Failed to initialise PRF writing!");
    fn_stat = 1;
  }
  else
  {
    fn_stat = 0;
    cmp_init(&prfStruct, prf, file_reader, file_skipper, file_writer);

    //File structure assign

    //Write the first string in "the top of the file". Basicly an important messate for ppl who are trying to edit this file in certain editors.
    if (!cmp_write_str(&prfStruct, "DO NOT EDIT THE FILE, THIS COMPLETELY MAKES IT UNREADBLE!!!", 59))
    {
      fn_stat = 2;
      log_err("CMP String Err: %s", cmp_strerror(&prfStruct));
    }
  }

  fclose(prf);

  return fn_stat;
}
