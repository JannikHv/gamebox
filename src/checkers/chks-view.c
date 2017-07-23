#include "chks-view.h"
#include "chks-field.h"
#include "chks-check.h"

#include <gtk/gtk.h>

#define CHKS_TITLE_MOVE_ONE "Move: Player White"
#define CHKS_TITLE_MOVE_TWO "Move: Player Black"
#define CHKS_TITLE_TAKE_ONE "Take: Player White"
#define CHKS_TITLE_TAKE_TWO "Take: Player Black"
#define CHKS_TITLE_WON_ONE  "Player White has won!"
#define CHKS_TITLE_WON_TWO  "Player Black has won!"

#define CHKS_CLASS_GRID "chks-grid"

typedef enum {
        CHKS_STATUS_NONE,
        CHKS_STATUS_MOVE,
        CHKS_STATUS_JUMP_AGAIN,
} ChksStatus;

struct _ChksView {
        /* Container */
        GtkWidget *grid;

        /* Connection to header bar */
        GtkWidget *hbar;

        /* Fields */
        ChksField *field[8][8];
        ChksField *last;

        /* States */
        ChksStatus status;
        gint round;
        gint pieces_one;
        gint pieces_two;
};

static void chks_field_leave(GtkWidget *button,
                             ChksField *piece)
{
        ChksOwner owner;

        owner = chks_field_get_owner(piece);

        if (owner != CHKS_OWNER_NONE) return;

        chks_field_set_preowner(piece, CHKS_OWNER_NONE);
}

static void chks_field_enter(GtkWidget *button,
                             ChksField *piece)
{
        ChksView *view;
        ChksOwner owner, owner_now;
        ChksTurn turn;

        view      = chks_field_get_view(piece);
        owner     = chks_field_get_owner(piece);
        owner_now = (view->round % 2 == 0) ? CHKS_OWNER_ONE : CHKS_OWNER_TWO;

        /* Check if game is running */
        if (view->status == CHKS_STATUS_NONE) return;

        /* Check owner of destination field */
        if (owner != CHKS_OWNER_NONE) return;

        /* Check if field is selected */
        if (!view->last) return;

        turn = chks_check_get_turn(view->field, view->last, piece);

        if (turn == CHKS_TURN_NONE) return;

        chks_field_set_preowner(piece, owner_now);
}

static void chks_view_set_status(ChksView *view,
                                 ChksStatus status)
{
        view->status = status;

        GtkHeaderBar *hbar;
        const gchar *title;

        hbar = GTK_HEADER_BAR(view->hbar);

        switch (status) {
        case CHKS_STATUS_NONE:
                title = (view->round % 2 == 0) ? CHKS_TITLE_WON_ONE : CHKS_TITLE_WON_TWO;
                break;
        case CHKS_STATUS_MOVE:
                title = (view->round % 2 == 0) ? CHKS_TITLE_MOVE_ONE : CHKS_TITLE_MOVE_TWO;
                break;
        case CHKS_STATUS_JUMP_AGAIN:
                title = (view->round % 2 == 0) ? CHKS_TITLE_TAKE_ONE : CHKS_TITLE_TAKE_TWO;
                break;
        }

        gtk_header_bar_set_title(hbar, title);
}

static void chks_field_jump(ChksView *view,
                            ChksField *field[8][8],
                            ChksField *piece)
{
        ChksOwner owner_now, owner_dest;
        gint start_x, start_y, dest_x, dest_y, mid_x, mid_y;

        owner_now  = (view->round % 2 == 0) ? CHKS_OWNER_ONE : CHKS_OWNER_TWO;
        owner_dest = chks_field_get_owner(piece);
        start_x    = chks_field_get_col(view->last);
        start_y    = chks_field_get_row(view->last);
        dest_x     = chks_field_get_col(piece);
        dest_y     = chks_field_get_row(piece);
        mid_x      = (start_x + dest_x) / 2;
        mid_y      = (start_y + dest_y) / 2;

        /* Check destination field */
        if (owner_dest != CHKS_OWNER_NONE) return;

        /* Check what turn is intended */
        if (chks_check_get_turn(field, view->last, piece) != CHKS_TURN_TAKE)
                return;

        /* Take away middle piece */
        chks_field_set_owner(field[mid_x][mid_y], CHKS_OWNER_NONE);

        /* Move pieces */
        chks_field_set_owner(view->last, CHKS_OWNER_NONE);
        chks_field_set_owner(piece, owner_now);
        chks_field_set_is_queen(piece, chks_field_get_is_queen(view->last));
        chks_field_set_is_queen(view->last, FALSE);

        /* Check if piece is now queen */
        if (chks_check_get_becomes_queen(piece))
                chks_field_set_is_queen(piece, TRUE);

        /* Substract a piece */
        if (owner_now == CHKS_OWNER_ONE)
                view->pieces_two--;
        else
                view->pieces_one--;

        /* Check if you have to jump again */
        if (chks_check_get_can_jump_again(field, piece)) {
                view->last = piece;
                chks_view_set_status(view, CHKS_STATUS_JUMP_AGAIN);
                return;
        }

        /* Check if someone has won */
        if (view->pieces_one == 0 ||
            view->pieces_two == 0) {
                chks_view_set_status(view, CHKS_STATUS_NONE);
                return;
        }

        view->last = NULL;
        view->round++;

        chks_view_set_status(view, CHKS_STATUS_MOVE);
}

static void chks_field_move(ChksView *view,
                            ChksField *field[8][8],
                            ChksField *piece)
{
        ChksOwner owner, owner_now;
        ChksTurn turn;

        owner     = chks_field_get_owner(piece);
        owner_now = (view->round % 2 == 0) ? CHKS_OWNER_ONE : CHKS_OWNER_TWO;

        if (owner == owner_now) {
                view->last = piece;
                return;
        }

        if (!view->last) return;

        /* Check what turn is intended */
        turn = chks_check_get_turn(field, view->last, piece);

        switch (turn) {
        case CHKS_TURN_NONE:
                return;
        case CHKS_TURN_MOVE:
                break;
        case CHKS_TURN_TAKE:
                chks_field_jump(view, field, piece);
                return;
        }

        /* Move field */
        chks_field_set_owner(view->last, CHKS_OWNER_NONE);
        chks_field_set_owner(piece, owner_now);
        chks_field_set_is_queen(piece, chks_field_get_is_queen(view->last));
        chks_field_set_is_queen(view->last, FALSE);
        view->last = NULL;

        /* Check if piece is now queen */
        if (chks_check_get_becomes_queen(piece))
                chks_field_set_is_queen(piece, TRUE);

        view->round++;

        chks_view_set_status(view, CHKS_STATUS_MOVE);
}

static void chks_field_clicked(GtkWidget *button,
                               ChksField *piece)
{
        ChksView *view;

        view = chks_field_get_view(piece);

        switch (view->status) {
        case CHKS_STATUS_NONE:
                return;
        case CHKS_STATUS_MOVE:
                chks_field_move(view, view->field, piece);
                break;
        case CHKS_STATUS_JUMP_AGAIN:
                chks_field_jump(view, view->field, piece);
                break;
        }
}

static void chks_view_add_fields(ChksView *view,
                                 ChksField *field[8][8])
{
        gint i, k;
        GtkGrid *grid;
        GtkWidget *button;

        grid = GTK_GRID(view->grid);

        for (i = 0; i < 8; i++) {
                for (k = 0; k < 8; k++) {
                        field[i][k] = chks_field_new_at(i, k);
                        button = chks_field_get_button(field[i][k]);
                        chks_field_set_view(field[i][k], view);
                        gtk_grid_attach(grid, button, i, k, 1, 1);
                        g_signal_connect(button, "clicked",
                                         G_CALLBACK(chks_field_clicked),
                                         field[i][k]);
                        g_signal_connect(button, "enter",
                                         G_CALLBACK(chks_field_enter),
                                         field[i][k]);
                        g_signal_connect(button, "leave",
                                         G_CALLBACK(chks_field_leave),
                                         field[i][k]);
                }
        }
}

static void chks_view_init(ChksView *view)
{
        view->grid       = gtk_grid_new();
        view->hbar       = NULL;
        view->last       = NULL;
        view->status     = CHKS_STATUS_MOVE;
        view->round      = 0;
        view->pieces_one = 12;
        view->pieces_two = 12;

        /* Grid */
        GtkStyleContext *context = gtk_widget_get_style_context(view->grid);
        gtk_style_context_add_class(context, CHKS_CLASS_GRID);
        gtk_grid_set_column_homogeneous(GTK_GRID(view->grid), TRUE);
        gtk_grid_set_row_homogeneous(GTK_GRID(view->grid), TRUE);
        chks_view_add_fields(view, view->field);
}

/**
 * Accessors
 */
void chks_view_reset(ChksView *view)
{
        gint i, k;
        ChksOwner owner_init;

        for (i = 0; i < 8; i++) {
                for (k = 0; k < 8; k++) {
                        owner_init = chks_field_get_initial_owner(view->field[i][k]);
                        chks_field_set_owner(view->field[i][k], CHKS_OWNER_NONE);
                        chks_field_set_owner(view->field[i][k], owner_init);
                        chks_field_set_is_queen(view->field[i][k], FALSE);
                }
        }

        view->last       = NULL;
        view->round      = 0;
        view->pieces_one = 12;
        view->pieces_two = 12;

        chks_view_set_status(view, CHKS_STATUS_MOVE);
}

GtkWidget *chks_view_get_viewport(ChksView *view)
{
        return view->grid;
}

void chks_view_set_header_bar(ChksView *view,
                              GtkWidget *hbar)
{
        view->hbar = hbar;
}

ChksView *chks_view_new(void)
{
        ChksView *view;

        view = g_malloc(sizeof(ChksView));

        chks_view_init(view);

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
