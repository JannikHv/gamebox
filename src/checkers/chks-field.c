#include "chks-field.h"
#include "chks-view.h"

#include <gtk/gtk.h>

#define CHKS_CLASS_FIELD "chks-field"

#define CHKS_CLASS_ONE "chks-player-one"
#define CHKS_CLASS_TWO "chks-player-two"

#define CHKS_CLASS_ONE_PRE "chks-player-one-pre"
#define CHKS_CLASS_TWO_PRE "chks-player-two-pre"

struct _ChksField {
        /* Widget */
        GtkWidget *button;

        /* Connection to view */
        ChksView *view;

        /* States */
        ChksOwner owner;
        gboolean is_queen;
        gint col;
        gint row;
};

static void chks_field_init(ChksField *field)
{
        field->button   = gtk_button_new();
        field->view     = NULL;
        field->owner    = chks_field_get_initial_owner(field);
        field->is_queen = FALSE;

        /* Add style to button */
        gtk_container_set_border_width(GTK_CONTAINER(field->button), 8);
        GtkStyleContext *context = gtk_widget_get_style_context(field->button);
        gtk_style_context_add_class(context, CHKS_CLASS_FIELD);

        switch (field->owner) {
        case CHKS_OWNER_NONE:
                break;
        case CHKS_OWNER_ONE:
                gtk_style_context_add_class(context, CHKS_CLASS_ONE);
                break;
        case CHKS_OWNER_TWO:
                gtk_style_context_add_class(context, CHKS_CLASS_TWO);
                break;
        }
}

/**
 * Accessors
 */
GtkWidget *chks_field_get_button(ChksField *field)
{
        return field->button;
}

ChksView *chks_field_get_view(ChksField *field)
{
        return field->view;
}

void chks_field_set_view(ChksField *field,
                         ChksView *view)
{
        field->view = view;
}

ChksOwner chks_field_get_owner(ChksField *field)
{
        return field->owner;
}

void chks_field_set_owner(ChksField *field,
                          ChksOwner owner)
{
        field->owner = owner;

        GtkStyleContext *context;

        context = gtk_widget_get_style_context(field->button);

        switch (owner) {
        case CHKS_OWNER_NONE:
                gtk_style_context_remove_class(context, CHKS_CLASS_ONE);
                gtk_style_context_remove_class(context, CHKS_CLASS_TWO);
                break;
        case CHKS_OWNER_ONE:
                gtk_style_context_remove_class(context, CHKS_CLASS_ONE_PRE);
                gtk_style_context_add_class(context, CHKS_CLASS_ONE);
                break;
        case CHKS_OWNER_TWO:
                gtk_style_context_remove_class(context, CHKS_CLASS_TWO_PRE);
                gtk_style_context_add_class(context, CHKS_CLASS_TWO);
                break;
        }
}

ChksOwner chks_field_get_initial_owner(ChksField *field)
{
        gint col, row;
        gint i, k;

        col = field->col;
        row = field->row;

        static gint white_x[3][4] = {
                {0, 2, 4, 6}, {1, 3, 5, 7}, {0, 2, 4, 6},
        };

        static gint white_y[3][4] = {
                {7, 7, 7, 7}, {6, 6, 6, 6}, {5, 5, 5, 5},
        };

        static gint black_x[3][4] = {
                {1, 3, 5, 7}, {0, 2, 4, 6}, {1, 3, 5, 7},
        };

        static gint black_y[3][4] = {
                {0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2},
        };

        for (i = 0; i < 3; i++) {
                for (k = 0; k < 4; k++) {
                        if (col == white_x[i][k] &&
                            row == white_y[i][k])
                                return CHKS_OWNER_ONE;

                        if (col == black_x[i][k] &&
                            row == black_y[i][k])
                                return CHKS_OWNER_TWO;
                }
        }

        return CHKS_OWNER_NONE;
}

gboolean chks_field_get_is_queen(ChksField *field)
{
        return field->is_queen;
}

void chks_field_set_preowner(ChksField *field,
                             ChksOwner owner)
{
        GtkStyleContext *context;

        context = gtk_widget_get_style_context(field->button);

        switch (owner) {
        case CHKS_OWNER_NONE:
                gtk_style_context_remove_class(context, CHKS_CLASS_ONE_PRE);
                gtk_style_context_remove_class(context, CHKS_CLASS_TWO_PRE);
                break;
        case CHKS_OWNER_ONE:
                gtk_style_context_add_class(context, CHKS_CLASS_ONE_PRE);
                break;
        case CHKS_OWNER_TWO:
                gtk_style_context_add_class(context, CHKS_CLASS_TWO_PRE);
                break;
        }
}

void chks_field_set_is_queen(ChksField *field,
                             gboolean is_queen)
{
        field->is_queen = is_queen;
}

gint chks_field_get_col(ChksField *field)
{
        return field->col;
}

gint chks_field_get_row(ChksField *field)
{
        return field->row;
}

ChksField *chks_field_new_at(gint col,
                             gint row)
{
        ChksField *field;

        field = g_malloc(sizeof(ChksField));

        field->col = col;
        field->row = row;

        chks_field_init(field);

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
