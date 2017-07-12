#ifndef __TTT_FIELD_H__
#define __TTT_FIELD_H__

#include "ttt-view.h"

#include <gtk/gtk.h>

typedef struct _TttField TttField;

typedef enum {
        TTT_OWNER_NONE,
        TTT_OWNER_ONE,
        TTT_OWNER_TWO,
} TttOwner;

GtkWidget       *ttt_field_get_button   (TttField *field);

TttView         *ttt_field_get_view     (TttField *field);

void             ttt_field_set_view     (TttField *field,
                                         TttView *view);

TttOwner         ttt_field_get_owner    (TttField *field);

void             ttt_field_set_owner    (TttField *field,
                                         TttOwner owner);

void             ttt_field_set_preowner (TttField *field,
                                         TttOwner owner);

TttField        *ttt_field_new          (void);

#endif /* __TTT_FIELD_H__ */

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
