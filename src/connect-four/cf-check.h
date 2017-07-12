#ifndef __CF_CHECK_H__
#define __CF_CHECK_H__

#include "cf-field.h"

#include <gtk/gtk.h>

gboolean         cf_check_get_won               (CfField *field[7][6]);

CfField         *cf_check_get_lowest_field      (CfField *field[7][6],
                                                 CfField *start);

#endif /* __CF_CHECK_H__ */

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
