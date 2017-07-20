#include "gbox-preview.h"

#include <gtk/gtk.h>
#include <string.h>

struct _GBoxPreview {
        /* Container */
        GtkWidget *grid;

        /* Widgets */
        GtkWidget *title;
        GtkWidget *image;
        GtkWidget *btn_start;
};

static const gchar *get_markup_from_title(const gchar *title)
{
        gint length;
        gchar *markup;

        static const gchar *markup_p1 = "<span size='25000' color='#21242C'><u>";
        static const gchar *markup_p2 = "</u>\n</span>";

        length = 52 + strlen(title);
        markup = g_malloc(sizeof(char) * length);

        strcpy(markup, markup_p1);
        strcat(markup, title);
        strcat(markup, markup_p2);

        return markup;
}

static const gchar *get_image_path_from_name(const gchar *name)
{
        gint length;
        gchar *path;

        static const gchar *path_default = "/usr/share/gamebox/images/";

        length = 27 + strlen(name);
        path   = g_malloc(sizeof(char) * length);

        strcpy(path, path_default);
        strcat(path, name);

        return path;
}

/**
 * Accessors
 */
GtkWidget *gbox_preview_get_viewport(GBoxPreview *preview)
{
        return preview->grid;
}

void gbox_preview_set_title(GBoxPreview *preview,
                            const gchar *title)
{
        GtkLabel *label;
        const gchar *markup;

        label  = GTK_LABEL(preview->title);
        markup = get_markup_from_title(title);

        gtk_label_set_markup(label, markup);
}

void gbox_preview_set_image_name(GBoxPreview *preview,
                                 const gchar *image_name)
{
        GtkImage *image;
        const gchar *path;

        image = GTK_IMAGE(preview->image);
        path  = get_image_path_from_name(image_name);

        gtk_image_set_from_file(image, path);
}

GtkWidget *gbox_preview_get_start_button(GBoxPreview *preview)
{
        return preview->btn_start;
}

static void gbox_preview_add_interface(GBoxPreview *preview)
{
        GtkGrid *grid;
        GtkWidget *placeholder;

        grid        = GTK_GRID(preview->grid);
        placeholder = gtk_label_new(NULL);

        gtk_grid_attach(grid, preview->title,     0, 0, 4, 3);
        gtk_grid_attach(grid, preview->image,     0, 3, 4, 3);
        gtk_grid_attach(grid, placeholder,        0, 6, 4, 1);
        gtk_grid_attach(grid, preview->btn_start, 1, 7, 2, 2);
}

static void gbox_preview_init(GBoxPreview *preview)
{
        preview->grid      = gtk_grid_new();
        preview->title     = gtk_label_new(NULL);
        preview->image     = gtk_image_new();
        preview->btn_start = gtk_button_new_with_label("Start Game");

        /* Grid */
        gtk_container_set_border_width(GTK_CONTAINER(preview->grid), 20);
        gtk_grid_set_column_homogeneous(GTK_GRID(preview->grid), TRUE);

        /* Add style to button */
        GtkStyleContext *context = gtk_widget_get_style_context(preview->btn_start);
        gtk_style_context_add_class(context, "suggested-action");

        gbox_preview_add_interface(preview);
}

GBoxPreview *gbox_preview_new(void)
{
        GBoxPreview *preview;

        preview = g_malloc(sizeof(GBoxPreview));

        gbox_preview_init(preview);

        return preview;
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
