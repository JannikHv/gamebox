#include "ttt-field.h"
#include "ttt-view.h"

#include <gtk/gtk.h>

#define TTT_MARKUP_ONE "<span size='47500' color='#000000'>X</span>"
#define TTT_MARKUP_TWO "<span size='47500' color='#000000'>O</span>"

#define TTT_PRE_MARKUP_ONE "<span size='47500' color='#858585'>X</span>"
#define TTT_PRE_MARKUP_TWO "<span size='47500' color='#858585'>O</span>"

#define TTT_CLASS_FIELD "ttt-field"

struct _TttField {
        /* Widgets */
        GtkWidget *button;
        GtkWidget *label;

        /* Connection to view */
        TttView *view;

        /* States */
        TttOwner owner;
};

/**
 * Accessors
 */
GtkWidget *ttt_field_get_button(TttField *field)
{
        return field->button;
}

TttView *ttt_field_get_view(TttField *field)
{
        return field->view;
}

void ttt_field_set_view(TttField *field,
                        TttView *view)
{
        field->view = view;
}

TttOwner ttt_field_get_owner(TttField *field)
{
        return field->owner;
}

void ttt_field_set_owner(TttField *field,
                         TttOwner owner)
{
        field->owner = owner;

        GtkLabel *label = GTK_LABEL(field->label);

        switch (owner) {
        case TTT_OWNER_NONE:
                gtk_label_set_markup(label, "");
                break;
        case TTT_OWNER_ONE:
                gtk_label_set_markup(label, TTT_MARKUP_ONE);
                break;
        case TTT_OWNER_TWO:
                gtk_label_set_markup(label, TTT_MARKUP_TWO);
                break;
        }
}

void ttt_field_set_preowner(TttField *field,
                            TttOwner owner)
{
        GtkLabel *label = GTK_LABEL(field->label);

        switch (owner) {
        case TTT_OWNER_NONE:
                gtk_label_set_markup(label, "");
                break;
        case TTT_OWNER_ONE:
                gtk_label_set_markup(label, TTT_PRE_MARKUP_ONE);
                break;
        case TTT_OWNER_TWO:
                gtk_label_set_markup(label, TTT_PRE_MARKUP_TWO);
                break;
        }
}

static void ttt_field_init(TttField *field)
{
        field->button = gtk_button_new_with_label("");
        field->label  = gtk_bin_get_child(GTK_BIN(field->button));
        field->view   = NULL;
        field->owner  = TTT_OWNER_NONE;

        /* Add style to button */
        GtkStyleContext *context = gtk_widget_get_style_context(field->button);
        gtk_style_context_add_class(context, TTT_CLASS_FIELD);
}

TttField *ttt_field_new(void)
{
        TttField *field;

        field = g_malloc(sizeof(TttField));

        ttt_field_init(field);

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
