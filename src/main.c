#include "gbox-window.h"

#include <gtk/gtk.h>

gint main(gint argc,
          gchar *argv[])
{
        gtk_init(&argc, &argv);

        GtkWidget *win;

        win = gbox_window_new();

        gtk_widget_show_all(win);

        gtk_main();
}

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
