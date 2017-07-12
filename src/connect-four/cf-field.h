#ifndef __CF_FIELD_H__
#define __CF_FIELD_H__

#include "cf-view.h"

#include <gtk/gtk.h>

typedef struct _CfField CfField;

typedef enum {
        CF_OWNER_NONE,
        CF_OWNER_ONE,
        CF_OWNER_TWO,
} CfOwner;

GtkWidget       *cf_field_get_button    (CfField *field);

CfView          *cf_field_get_view      (CfField *field);

void             cf_field_set_view      (CfField *field,
                                         CfView *view);

CfOwner          cf_field_get_owner     (CfField *field);

void             cf_field_set_owner     (CfField *field,
                                         CfOwner owner);

void             cf_field_set_preowner  (CfField *field,
                                         CfOwner owner);

gint             cf_field_get_col       (CfField *field);

gint             cf_field_get_row       (CfField *field);

CfField         *cf_field_new_at        (gint col,
                                         gint row);

#endif /* __CF_FIELD_H__ */

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
