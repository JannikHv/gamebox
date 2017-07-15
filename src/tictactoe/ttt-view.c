#include "ttt-view.h"
#include "ttt-field.h"
#include "ttt-check.h"

#include <gtk/gtk.h>

#define TTT_TITLE_TURN_ONE "Turn: Player X"
#define TTT_TITLE_TURN_TWO "Turn: Player O"

#define TTT_TITLE_WON_ONE "Player X has won!"
#define TTT_TITLE_WON_TWO "Player O has won!"

#define TTT_TITLE_DRAW "Draw!"

struct _TttView {
        /* Container */
        GtkWidget *grid;

        /* Connection to header bar */
        GtkWidget *hbar;

        /* Fields */
        TttField *field[3][3];

        /* States */
        gboolean running;
        gint round;
};

static void ttt_field_enter(GtkWidget *button,
                            TttField *field)
{
        TttView *view;
        TttOwner owner, owner_now;

        view      = ttt_field_get_view(field);
        owner     = ttt_field_get_owner(field);
        owner_now = (view->round % 2 == 0) ? TTT_OWNER_ONE : TTT_OWNER_TWO;

        if (!view->running) return;

        if (owner != TTT_OWNER_NONE) return;

        ttt_field_set_preowner(field, owner_now);
}

static void ttt_field_leave(GtkWidget *button,
                            TttField *field)
{
        TttView *view;
        TttOwner owner;

        view  = ttt_field_get_view(field);
        owner = ttt_field_get_owner(field);

        if (!view->running) return;

        if (owner != TTT_OWNER_NONE) return;

        ttt_field_set_preowner(field, TTT_OWNER_NONE);
}

static void ttt_field_clicked(GtkWidget *button,
                              TttField *field)
{
        TttView *view;
        GtkHeaderBar *hbar;
        TttOwner owner, owner_now;

        view      = ttt_field_get_view(field);
        hbar      = GTK_HEADER_BAR(view->hbar);
        owner     = ttt_field_get_owner(field);
        owner_now = (view->round % 2 == 0) ? TTT_OWNER_ONE : TTT_OWNER_TWO;

        /* Check if game is running */
        if (!view->running) return;

        /* Check if field is assigned already */
        if (owner != TTT_OWNER_NONE) return;

        /* Assign owner to field */
        ttt_field_set_owner(field, owner_now);

        /* Check for win */
        if (ttt_check_get_won(view->field)) {
                if (view->round % 2 == 0)
                        gtk_header_bar_set_title(hbar, TTT_TITLE_WON_ONE);
                else
                        gtk_header_bar_set_title(hbar, TTT_TITLE_WON_TWO);

                view->running = FALSE;
                return;
        }

        /* Check for draw */
        if (view->round == 8) {
                gtk_header_bar_set_title(hbar, TTT_TITLE_DRAW);
                return;
        }

        /* End turn */
        if (view->round % 2 == 0)
                gtk_header_bar_set_title(hbar, TTT_TITLE_TURN_TWO);
        else
                gtk_header_bar_set_title(hbar, TTT_TITLE_TURN_ONE);

        view->round++;
}

/**
 * Accessors
 */
void ttt_view_reset(TttView *view)
{
        GtkHeaderBar *hbar;
        gint i, k;

        view->running = TRUE;
        hbar = GTK_HEADER_BAR(view->hbar);

        for (i = 0; i < 3; i++) {
                for (k = 0; k < 3; k++)
                        ttt_field_set_owner(view->field[i][k], TTT_OWNER_NONE);
        }

        gtk_header_bar_set_title(hbar, TTT_TITLE_TURN_ONE);
        view->round = 0;
}

GtkWidget *ttt_view_get_viewport(TttView *view)
{
        return view->grid;
}

void ttt_view_set_header_bar(TttView *view,
                             GtkWidget *hbar)
{
        view->hbar = hbar;
}

static void ttt_view_add_fields(TttView *view,
                                TttField *field[3][3])
{
        gint i, k;
        GtkGrid *grid;
        GtkWidget *button;

        grid = GTK_GRID(view->grid);

        for (i = 0; i < 3; i++) {
                for (k = 0; k < 3; k++) {
                        field[i][k] = ttt_field_new();
                        button = ttt_field_get_button(field[i][k]);
                        ttt_field_set_view(field[i][k], view);
                        g_signal_connect(button, "clicked", G_CALLBACK(ttt_field_clicked), field[i][k]);
                        g_signal_connect(button, "enter",   G_CALLBACK(ttt_field_enter),   field[i][k]);
                        g_signal_connect(button, "leave",   G_CALLBACK(ttt_field_leave),   field[i][k]);
                        gtk_grid_attach(grid, button, i, k, 1, 1);
                }
        }
}

static void ttt_view_init(TttView *view)
{
        view->grid    = gtk_grid_new();
        view->hbar    = NULL;
        view->running = TRUE;
        view->round   = 0;

        /* Grid */
        gtk_grid_set_column_homogeneous(GTK_GRID(view->grid), TRUE);
        gtk_grid_set_row_homogeneous(GTK_GRID(view->grid), TRUE);
        ttt_view_add_fields(view, view->field);
}

TttView *ttt_view_new(void)
{
        TttView *view;

        view = g_malloc(sizeof(TttView));

        ttt_view_init(view);

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
