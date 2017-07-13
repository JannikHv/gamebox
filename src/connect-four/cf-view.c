#include "cf-view.h"
#include "cf-field.h"
#include "cf-check.h"

#include <gtk/gtk.h>

#define CF_TITLE_TURN_ONE "Turn: Player Yellow"
#define CF_TITLE_TURN_TWO "Turn: Player Red"
#define CF_TITLE_WON_ONE "Player Yellow has won!"
#define CF_TITLE_WON_TWO "Player Red has won!"
#define CF_TITLE_DRAW "Draw!"

#define CF_CLASS_GRID "cf-grid"

struct _CfView {
        /* Container */
        GtkWidget *grid;

        /* Connection to header bar */
        GtkWidget *hbar;

        /* Fields */
        CfField *field[7][6];

        /* States */
        gboolean running;
        gint round;
};

static void cf_field_leave(GtkWidget *button,
                           CfField *field)
{
        CfView *view;

        view = cf_field_get_view(field);

        if (!view->running) return;

        cf_field_set_preowner(field, CF_OWNER_NONE);
}

static void cf_field_enter(GtkWidget *button,
                           CfField *field)
{
        CfView *view;
        CfOwner owner;

        view  = cf_field_get_view(field);
        owner = (view->round % 2 == 0) ? CF_OWNER_ONE : CF_OWNER_TWO;

        if (!view->running) return;

        cf_field_set_preowner(field, owner);
}

static void cf_field_clicked(GtkWidget *button,
                             CfField *field)
{
        CfView *view;
        CfField *lowest;
        GtkHeaderBar *hbar;
        CfOwner owner, enemy, owner_now;

        view      = cf_field_get_view(field);
        lowest    = cf_check_get_lowest_field(view->field, field);
        hbar      = GTK_HEADER_BAR(view->hbar);
        owner     = cf_field_get_owner(field);
        enemy     = (view->round % 2 == 0) ? CF_OWNER_TWO : CF_OWNER_ONE;
        owner_now = (view->round % 2 == 0) ? CF_OWNER_ONE : CF_OWNER_TWO;

        /* Check if game is running */
        if (!view->running) return;

        /* Check if field is assigned */
        if (owner != CF_OWNER_NONE) return;

        /* Check if column has no free fields */
        if (!lowest) return;

        /* Assign owner to player */
        cf_field_set_owner(lowest, owner_now);

        /* Swap preowner from initial field */
        cf_field_set_preowner(field, enemy);

        /* Check if someone has won */
        if (cf_check_get_won(view->field)) {
                if (view->round % 2 == 0)
                        gtk_header_bar_set_title(hbar, CF_TITLE_WON_ONE);
                else
                        gtk_header_bar_set_title(hbar, CF_TITLE_WON_TWO);

                cf_field_set_preowner(field, CF_OWNER_NONE);

                view->running = FALSE;
                return;
        }

        /* Check for a draw */
        if (view->round == 41) {
                gtk_header_bar_set_title(hbar, CF_TITLE_DRAW);
                view->running = FALSE;
                return;
        }

        /* Switch owner */
        if (view->round % 2 == 0)
                gtk_header_bar_set_title(hbar, CF_TITLE_TURN_TWO);
        else
                gtk_header_bar_set_title(hbar, CF_TITLE_TURN_ONE);

        view->round++;
}

/**
 * Accessors
 */
void cf_view_set_active(CfView *view,
                        gboolean active)
{
        view->running = active;

        if (!active) return;

        GtkHeaderBar *hbar;
        GtkWidget *widget, *label;
        gint i, k;

        hbar = GTK_HEADER_BAR(view->hbar);

        for (i = 0; i < 7; i++) {
                for (k = 0; k < 6; k++) {
                        widget = cf_field_get_button(view->field[i][k]);
                        label  = gtk_bin_get_child(GTK_BIN(widget));
                        cf_field_set_owner(view->field[i][k], CF_OWNER_NONE);
                        gtk_label_set_markup(GTK_LABEL(label), "");
                }
        }

        gtk_header_bar_set_title(hbar, CF_TITLE_TURN_ONE);
        view->round = 0;
}

GtkWidget *cf_view_get_viewport(CfView *view)
{
        return view->grid;
}

void cf_view_set_header_bar(CfView *view,
                            GtkWidget *hbar)
{
        view->hbar = hbar;
}

static void cf_view_add_fields(CfView *view,
                               CfField *field[7][6])
{
        gint i, k;
        GtkGrid *grid;
        GtkWidget *button;

        grid = GTK_GRID(view->grid);

        for (i = 0; i < 7; i++) {
                for (k = 0; k < 6; k++) {
                        field[i][k] = cf_field_new_at(i, k);
                        button = cf_field_get_button(field[i][k]);
                        cf_field_set_view(field[i][k], view);
                        g_signal_connect(button, "clicked", G_CALLBACK(cf_field_clicked), field[i][k]);
                        g_signal_connect(button, "enter",   G_CALLBACK(cf_field_enter),   field[i][k]);
                        g_signal_connect(button, "leave",   G_CALLBACK(cf_field_leave),   field[i][k]);
                        gtk_grid_attach(grid, button, i, k, 1, 1);
                }
        }
}

static void cf_view_init(CfView *view)
{
        view->grid    = gtk_grid_new();
        view->hbar    = NULL;
        view->running = FALSE;
        view->round   = 0;

        /* Grid */
        GtkStyleContext *context = gtk_widget_get_style_context(view->grid);
        gtk_style_context_add_class(context, CF_CLASS_GRID);
        gtk_grid_set_column_homogeneous(GTK_GRID(view->grid), TRUE);
        gtk_grid_set_row_homogeneous(GTK_GRID(view->grid), TRUE);
        cf_view_add_fields(view, view->field);
}

CfView *cf_view_new(void)
{
        CfView *view;

        view = g_malloc(sizeof(CfView));

        cf_view_init(view);

        return view;
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
