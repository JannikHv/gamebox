#ifndef __GBOX_PREVIEW_H__
#define __GBOX_PREVIEW_H__

#include <gtk/gtk.h>

typedef struct _GBoxPreview GBoxPreview;

GtkWidget       *gbox_preview_get_viewport              (GBoxPreview *preview);

void             gbox_preview_set_title                 (GBoxPreview *preview,
                                                         const gchar *title);

void             gbox_preview_set_image_name            (GBoxPreview *preview,
                                                         const gchar *image_name);

GtkWidget       *gbox_preview_get_start_button          (GBoxPreview *preview);

GBoxPreview     *gbox_preview_new                       (void);

#endif /* __GBOX_PREVIEW_H__ */

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
