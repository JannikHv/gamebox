#ifndef __MILL_CHECK_H__
#define __MILL_CHECK_H__

#include "mill-field.h"

#include <gtk/gtk.h>

gint            mill_check_get_free_fields      (MillField *field[7][7],
                                                 MillOwner owner);

gboolean        mill_check_get_field_in_range   (MillField *field[7][7],
                                                 MillField *piece,
                                                 MillField *dest);

gint            mill_check_get_field_in_mill    (MillField *field[7][7],
                                                 MillField *piece);

#endif /* __MILL_CHECK_H__ */

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
