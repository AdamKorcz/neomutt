/**
 * @file
 * Config used by libpop
 *
 * @authors
 * Copyright (C) 2020 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page pop_config Config used by libpop
 *
 * Config used by libpop
 */

#include "config.h"
#include <stddef.h>
#include <config/lib.h>
#include <stdbool.h>
#include <stdint.h>
#include "private.h"
#include "mutt/lib.h"
#include "conn/lib.h"

/**
 * pop_auth_validator - Validate the "pop_authenticators" config variable - Implements ConfigDef::validator() - @ingroup cfg_def_validator
 */
static int pop_auth_validator(const struct ConfigSet *cs, const struct ConfigDef *cdef,
                              intptr_t value, struct Buffer *err)
{
  const struct Slist *pop_auth_methods = (const struct Slist *) value;
  if (!pop_auth_methods || (pop_auth_methods->count == 0))
    return CSR_SUCCESS;

  struct ListNode *np = NULL;
  STAILQ_FOREACH(np, &pop_auth_methods->head, entries)
  {
    if (pop_auth_is_valid(np->data))
      continue;
#ifdef USE_SASL
    if (sasl_auth_validator(np->data))
      continue;
#endif
    mutt_buffer_printf(err, _("Option %s: %s is not a valid authenticator"),
                       cdef->name, np->data);
    return CSR_ERR_INVALID;
  }

  return CSR_SUCCESS;
}

static struct ConfigDef PopVars[] = {
  // clang-format off
  { "pop_auth_try_all", DT_BOOL, true, 0, NULL,
    "(pop) Try all available authentication methods"
  },
  { "pop_authenticators", DT_SLIST|SLIST_SEP_COLON, 0, 0, pop_auth_validator,
    "(pop) List of allowed authentication methods (colon-separated)"
  },
  { "pop_check_interval", DT_NUMBER|DT_NOT_NEGATIVE, 60, 0, NULL,
    "(pop) Interval between checks for new mail"
  },
  { "pop_delete", DT_QUAD, MUTT_ASKNO, 0, NULL,
    "(pop) After downloading POP messages, delete them on the server"
  },
  { "pop_host", DT_STRING, 0, 0, NULL,
    "(pop) Url of the POP server"
  },
  { "pop_last", DT_BOOL, false, 0, NULL,
    "(pop) Use the 'LAST' command to fetch new mail"
  },
  { "pop_oauth_refresh_command", DT_STRING|DT_COMMAND|DT_SENSITIVE, 0, 0, NULL,
    "(pop) External command to generate OAUTH refresh token"
  },
  { "pop_pass", DT_STRING|DT_SENSITIVE, 0, 0, NULL,
    "(pop) Password of the POP server"
  },
  { "pop_reconnect", DT_QUAD, MUTT_ASKYES, 0, NULL,
    "(pop) Reconnect to the server is the connection is lost"
  },
  { "pop_user", DT_STRING|DT_SENSITIVE, 0, 0, NULL,
    "(pop) Username of the POP server"
  },

  { "pop_checkinterval",         DT_SYNONYM, IP "pop_check_interval",         },

  { NULL },
  // clang-format on
};

/**
 * config_init_pop - Register pop config variables - Implements ::module_init_config_t - @ingroup cfg_module_api
 */
bool config_init_pop(struct ConfigSet *cs)
{
  return cs_register_variables(cs, PopVars, 0);
}
