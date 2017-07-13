#include "mill-field.h"
#include "mill-view.h"

#include <gtk/gtk.h>

#define MILL_CLASS_FIELD "mill-field"
#define MILL_CLASS_BLOCK "mill-block"

#define MILL_CLASS_ONE "mill-player-one"
#define MILL_CLASS_TWO "mill-player-two"

struct _MillField {
        /* Widget */
        GtkWidget *button;

        /* Connection to view */
        MillView *view;

        /* States */
        MillOwner owner;
        gint col;
        gint row;
};

/**
 * Accessors
 */
GtkWidget *mill_field_get_button(MillField *field)
{
        return field->button;
}

MillView *mill_field_get_view(MillField *field)
{
        return field->view;
}

void mill_field_set_view(MillField *field,
                         MillView *view)
{
        field->view = view;
}

MillOwner mill_field_get_owner(MillField *field)
{
        return field->owner;
}

void mill_field_set_owner(MillField *field,
                          MillOwner owner)
{
        field->owner = owner;

        GtkStyleContext *context;

        context = gtk_widget_get_style_context(field->button);

        switch (owner) {
        case MILL_OWNER_NONE:
                gtk_style_context_remove_class(context, MILL_CLASS_ONE);
                gtk_style_context_remove_class(context, MILL_CLASS_TWO);
                break;
        case MILL_OWNER_ONE:
                gtk_style_context_add_class(context, MILL_CLASS_ONE);
                break;
        case MILL_OWNER_TWO:
                gtk_style_context_add_class(context, MILL_CLASS_TWO);
                break;

        default:
                break;
        }
}

void mill_field_set_preowner(MillField *field,
                             MillOwner owner)
{
        GtkStyleContext *context;

        context = gtk_widget_get_style_context(field->button);

        switch (owner) {
        case MILL_OWNER_NONE:
                gtk_style_context_remove_class(context, MILL_CLASS_ONE);
                gtk_style_context_remove_class(context, MILL_CLASS_TWO);
                break;
        case MILL_OWNER_ONE:
                gtk_style_context_add_class(context, MILL_CLASS_ONE);
                break;
        case MILL_OWNER_TWO:
                gtk_style_context_add_class(context, MILL_CLASS_TWO);
                break;

        default:
                break;
        }
}

gint mill_field_get_col(MillField *field)
{
        return field->col;
}

gint mill_field_get_row(MillField *field)
{
        return field->row;
}

static MillOwner mill_field_get_initial_owner(MillField *field)
{
        gint col, row;
        gint i, k;

        col = field->col;
        row = field->row;

        static gint block_x[6][4] = {
                {1, 2, 4, 5}, {0, 2, 4, 6}, {0, 1, 5, 6},
                {0, 1, 5, 6}, {0, 2, 4, 6}, {1, 2, 4, 5},
        };

        static gint block_y[6][4] = {
                {0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2},
                {4, 4, 4, 4}, {5, 5, 5, 5}, {6, 6, 6, 6},
        };

        /* Middle piece exception */
        if (col == 3 && row == 3)
                return MILL_OWNER_BLOCK;

        for (i = 0; i < 6; i++) {
                for (k = 0; k < 4; k++) {
                        if (col == block_x[i][k] &&
                            row == block_y[i][k])
                                return MILL_OWNER_BLOCK;
                }
        }

        return MILL_OWNER_NONE;
}

static void mill_field_init(MillField *field)
{
        field->view  = NULL;
        field->owner = mill_field_get_initial_owner(field);

        GtkStyleContext *context;
        const gchar *class_name;

        if (field->owner == MILL_OWNER_NONE) {
                field->button = gtk_button_new();
                gtk_container_set_border_width(GTK_CONTAINER(field->button), 10);
                class_name = MILL_CLASS_FIELD;
        } else {
                field->button = gtk_label_new(NULL);
                class_name = MILL_CLASS_BLOCK;
        }

        context = gtk_widget_get_style_context(field->button);
        gtk_style_context_add_class(context, class_name);
}

MillField *mill_field_new_at(gint col,
                             gint row)
{
        MillField *field;

        field = g_malloc(sizeof(MillField));

        field->col = col;
        field->row = row;

        mill_field_init(field);

        return field;
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
