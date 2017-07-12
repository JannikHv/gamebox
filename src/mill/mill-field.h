#ifndef __MILL_FIELD_H__
#define __MILL_FIELD_H__

#include "mill-view.h"

#include <gtk/gtk.h>

typedef struct _MillField MillField;

typedef enum {
        MILL_OWNER_BLOCK,
        MILL_OWNER_NONE,
        MILL_OWNER_ONE,
        MILL_OWNER_TWO,
} MillOwner;

GtkWidget        *mill_field_get_button         (MillField *field);

MillView         *mill_field_get_view           (MillField *field);

void             mill_field_set_view            (MillField *field,
                                                 MillView *view);

MillOwner        mill_field_get_owner           (MillField *field);

void             mill_field_set_owner           (MillField *field,
                                                 MillOwner owner);

void             mill_field_set_preowner        (MillField *field,
                                                 MillOwner owner);

gint             mill_field_get_col             (MillField *field);

gint             mill_field_get_row             (MillField *field);

MillField       *mill_field_new_at              (gint col,
                                                 gint row);

#endif /* __MILL_FIELD_H__ */

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
