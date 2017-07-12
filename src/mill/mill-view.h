#ifndef __MILL_VIEW_H__
#define __MILL_VIEW_H__

#include <gtk/gtk.h>

typedef struct _MillView MillView;

void             mill_view_set_active           (MillView *view,
                                                 gboolean active);

GtkWidget       *mill_view_get_viewport         (MillView *view);

void             mill_view_set_header_bar       (MillView *view,
                                                 GtkWidget *hbar);

MillView        *mill_view_new                  (void);

#endif /* __MILL_VIEW_H__ */

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
