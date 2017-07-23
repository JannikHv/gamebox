#ifndef __CHKS_FIELD_H__
#define __CHKS_FIELD_H__

#include "chks-view.h"

#include <gtk/gtk.h>

typedef struct _ChksField ChksField;

typedef enum {
        CHKS_OWNER_NONE,
        CHKS_OWNER_ONE,
        CHKS_OWNER_TWO,
} ChksOwner;

GtkWidget       *chks_field_get_button          (ChksField *field);

ChksView        *chks_field_get_view            (ChksField *field);

void             chks_field_set_view            (ChksField *field,
                                                 ChksView *view);

ChksOwner        chks_field_get_owner           (ChksField *field);

void             chks_field_set_owner           (ChksField *field,
                                                 ChksOwner owner);

ChksOwner        chks_field_get_initial_owner   (ChksField *field);

void             chks_field_set_preowner        (ChksField *field,
                                                 ChksOwner owner);

gboolean         chks_field_get_is_queen        (ChksField *field);

void             chks_field_set_is_queen        (ChksField *field,
                                                 gboolean is_queen);

gint             chks_field_get_col             (ChksField *field);

gint             chks_field_get_row             (ChksField *field);

ChksField       *chks_field_new_at              (gint col,
                                                 gint row);

#endif /* __CHKS_FIELD_H__ */

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
