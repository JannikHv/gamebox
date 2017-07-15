#ifndef __TTT_VIEW_H__
#define __TTT_VIEW_H__

#include <gtk/gtk.h>

typedef struct _TttView TttView;

void             ttt_view_reset                 (TttView *view);

void             ttt_view_set_header_bar        (TttView *view,
                                                 GtkWidget *hbar);

GtkWidget       *ttt_view_get_viewport          (TttView *view);

TttView         *ttt_view_new                   (void);

#endif /* __TTT_VIEW_H__ */

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
