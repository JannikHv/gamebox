#ifndef __CF_VIEW_H__
#define __CF_VIEW_H__

#include "cf-view.h"

#include <gtk/gtk.h>

typedef struct _CfView CfView;

void             cf_view_set_active             (CfView *view,
                                                 gboolean active);


GtkWidget       *cf_view_get_viewport           (CfView *view);


void             cf_view_set_header_bar         (CfView *view,
                                                 GtkWidget *hbar);

CfView          *cf_view_new                    (void);

#endif /* __CF_VIEW_H__ */

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
