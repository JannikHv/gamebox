#include "cf-field.h"
#include "cf-view.h"

#include <gtk/gtk.h>

#define CF_CLASS_FIELD "cf-field"

#define CF_CLASS_ONE "cf-player1"
#define CF_CLASS_TWO "cf-player2"

struct _CfField {
        /* Widget */
        GtkWidget *button;

        /* Connection to view */
        CfView *view;

        /* States */
        CfOwner owner;
        gint col;
        gint row;
};

/**
 * Accessors
 */
GtkWidget *cf_field_get_button(CfField *field)
{
        return field->button;
}

CfView *cf_field_get_view(CfField *field)
{
        return field->view;
}

void cf_field_set_view(CfField *field,
                       CfView *view)
{
        field->view = view;
}

CfOwner cf_field_get_owner(CfField *field)
{
        return field->owner;
}

void cf_field_set_owner(CfField *field,
                        CfOwner owner)
{
        field->owner = owner;

        GtkStyleContext *context;

        context = gtk_widget_get_style_context(field->button);

        switch (owner) {
        case CF_OWNER_NONE:
                gtk_style_context_remove_class(context, CF_CLASS_ONE);
                gtk_style_context_remove_class(context, CF_CLASS_TWO);
                break;
        case CF_OWNER_ONE:
                gtk_style_context_add_class(context, CF_CLASS_ONE);
                break;
        case CF_OWNER_TWO:
                gtk_style_context_add_class(context, CF_CLASS_TWO);
                break;
        }
}


void cf_field_set_preowner(CfField *field,
                           CfOwner owner)
{
        if (field->owner != CF_OWNER_NONE) return;

        GtkStyleContext *context;

        context = gtk_widget_get_style_context(field->button);

        switch (owner) {
        case CF_OWNER_NONE:
                gtk_style_context_remove_class(context, CF_CLASS_ONE);
                gtk_style_context_remove_class(context, CF_CLASS_TWO);
                break;
        case CF_OWNER_ONE:
                gtk_style_context_remove_class(context, CF_CLASS_TWO);
                gtk_style_context_add_class(context, CF_CLASS_ONE);
                break;
        case CF_OWNER_TWO:
                gtk_style_context_remove_class(context, CF_CLASS_ONE);
                gtk_style_context_add_class(context, CF_CLASS_TWO);
                break;
        }
}

gint cf_field_get_col(CfField *field)
{
        return field->col;
}

gint cf_field_get_row(CfField *field)
{
        return field->row;
}

static void cf_field_init(CfField *field)
{
        field->button = gtk_button_new_with_label("");
        field->view   = NULL;
        field->owner  = CF_OWNER_NONE;

        /* Button */
        gtk_container_set_border_width(GTK_CONTAINER(field->button), 10);

        /* Add style to button */
        GtkStyleContext *context = gtk_widget_get_style_context(field->button);
        gtk_style_context_add_class(context, CF_CLASS_FIELD);
}

CfField *cf_field_new_at(gint col,
                         gint row)
{
        CfField *field;

        field = g_malloc(sizeof(CfField));

        field->col = col;
        field->row = row;
 
        cf_field_init(field);

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
