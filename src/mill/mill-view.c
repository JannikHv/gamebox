#include "mill-view.h"
#include "mill-field.h"
#include "mill-check.h"

#include <gtk/gtk.h>

#define MILL_TITLE_PLACE_ONE "Place: Player Green"
#define MILL_TITLE_PLACE_TWO "Place: Player Red"

#define MILL_TITLE_MOVE_ONE  "Move: Player Green"
#define MILL_TITLE_MOVE_TWO  "Move: Player Red"

#define MILL_TITLE_TAKE_ONE  "Take: Player Green"
#define MILL_TITLE_TAKE_TWO  "Take: Player Red"

#define MILL_TITLE_WON_ONE   "Player Green has won!"
#define MILL_TITLE_WON_TWO   "Player Red has won!"

#define MILL_CLASS_GRID "mill-grid"

typedef enum {
        MILL_STATUS_PLACE,
        MILL_STATUS_MOVE,
        MILL_STATUS_TAKE,
        MILL_STATUS_DONE,
} MillStatus;

struct _MillView {
        /* Container */
        GtkWidget *grid;

        /* Connection to header bar */
        GtkWidget *hbar;

        /* Fields */
        MillField *field[7][7];
        MillField *last;

        /* States */
        MillStatus status;
        gint round;
        gint mill_cnt;
        gint pieces_one;
        gint pieces_two;
};

static void mill_field_enter(GtkWidget *button,
                             MillField *piece)
{
        MillView *view;
        MillStatus status;
        MillOwner owner, owner_now;
        gboolean in_range, can_jump;
        gint pieces_left;

        view        = mill_field_get_view(piece);
        status      = view->status;
        owner       = mill_field_get_owner(piece);
        owner_now   = (view->round % 2 == 0) ? MILL_OWNER_ONE : MILL_OWNER_TWO;
        pieces_left = (view->round % 2 == 0) ? view->pieces_one : view->pieces_two;
        can_jump    = (pieces_left > 3) ? FALSE : TRUE;

        if (owner  != MILL_OWNER_NONE  ||
            status == MILL_STATUS_DONE ||
            status == MILL_STATUS_TAKE)
                return;

        if (status == MILL_STATUS_MOVE) {
                if (!view->last) return;

                in_range = mill_check_get_field_in_range(view->field, view->last, piece);

                if (!in_range && !can_jump) return;
        }

        mill_field_set_preowner(piece, owner_now);
}

static void mill_field_leave(GtkWidget *button,
                             MillField *piece)
{
        MillView *view;
        MillStatus status;
        MillOwner owner;

        view   = mill_field_get_view(piece);
        status = view->status;
        owner  = mill_field_get_owner(piece);

        if (owner  != MILL_OWNER_NONE  ||
            status == MILL_STATUS_DONE ||
            status == MILL_STATUS_TAKE)
                return;

        mill_field_set_preowner(piece, MILL_OWNER_NONE);
}

static void mill_view_set_status(MillView *view,
                                 MillStatus status)
{
        view->status = status;

        GtkHeaderBar *hbar;
        const gchar *title;
        gint round;

        hbar  = GTK_HEADER_BAR(view->hbar);
        round = view->round;

        switch (status) {
        case MILL_STATUS_PLACE:
                title = (round % 2 == 0) ? MILL_TITLE_PLACE_ONE : MILL_TITLE_PLACE_TWO;
                break;
        case MILL_STATUS_MOVE:
                title = (round % 2 == 0) ? MILL_TITLE_MOVE_ONE : MILL_TITLE_MOVE_TWO;
                break;
        case MILL_STATUS_TAKE:
                title = (round % 2 == 0) ? MILL_TITLE_TAKE_ONE : MILL_TITLE_TAKE_TWO;
                break;
        case MILL_STATUS_DONE:
                title = (round % 2 == 0) ? MILL_TITLE_WON_ONE : MILL_TITLE_WON_TWO;
                break;
        }

        gtk_header_bar_set_title(hbar, title);
}

static void mill_field_take(MillView *view,
                            MillField *field[7][7],
                            MillField *piece)
{
        gint round,free_fields, in_mill, pieces_left;
        MillOwner owner, enemy;

        round       = view->round;
        owner       = mill_field_get_owner(piece);
        enemy       = (round % 2 == 0) ? MILL_OWNER_TWO : MILL_OWNER_ONE;
        free_fields = mill_check_get_free_fields(field, enemy);
        in_mill     = mill_check_get_field_in_mill(field, piece);

        /* Check if the piece belongs to the enemy */
        if (owner != enemy) return;

        /* Check if fields in in mill */
        if (in_mill > 0 && free_fields > 0) return;

        mill_field_set_owner(piece, MILL_OWNER_NONE);

        pieces_left = (round % 2 == 0) ? --view->pieces_two : --view->pieces_one;

        /* Check for win */
        if (pieces_left < 3) {
                mill_view_set_status(view, MILL_STATUS_DONE);
                return;
        }

        /* Check if two mills have been closed */
        if (--view->mill_cnt > 0) return;

        view->round++;

        if (round > 17)
                mill_view_set_status(view, MILL_STATUS_MOVE);
        else
                mill_view_set_status(view, MILL_STATUS_PLACE);

}

static void mill_field_move(MillView *view,
                            MillField *field[7][7],
                            MillField *piece)
{
        gboolean in_range;
        gint round, pieces_left;
        MillOwner owner, owner_now;

        round       = view->round;
        pieces_left = (round % 2 == 0) ? view->pieces_one : view->pieces_two;
        owner       = mill_field_get_owner(piece);
        owner_now   = (round % 2 == 0) ? MILL_OWNER_ONE : MILL_OWNER_TWO;

        /* Check if a field got selected */
        if (!view->last) {
                if (owner_now == owner)
                        view->last = piece;

                return;
        }

        /* Check if field should change */
        if (owner_now == owner) {
                view->last = piece;
                return;
        }

        if (owner != MILL_OWNER_NONE) return;

        /* Check if destination is out of range */
        in_range = mill_check_get_field_in_range(view->field, view->last, piece);
        if (!in_range && pieces_left > 3) return;

        /* Move field */
        mill_field_set_owner(view->last, MILL_OWNER_NONE);
        mill_field_set_owner(piece, owner_now);
        view->last = NULL;

        /* Check mill count */
        view->mill_cnt = mill_check_get_field_in_mill(view->field, piece);

        if (view->mill_cnt > 0) {
                mill_view_set_status(view, MILL_STATUS_TAKE);
                return;
        }

        view->round++;
        mill_view_set_status(view, MILL_STATUS_MOVE);
}

static void mill_field_place(MillView *view,
                             MillField *field[7][7],
                             MillField *piece)
{
        gint round;
        MillOwner owner, owner_now;

        round     = view->round;
        owner     = mill_field_get_owner(piece);
        owner_now = (round % 2 == 0) ? MILL_OWNER_ONE : MILL_OWNER_TWO;

        /* Check if field is assigned */
        if (owner != MILL_OWNER_NONE) return;

        /* Assign owner to field */
        mill_field_set_owner(piece, owner_now);

        /* Check mill count */
        view->mill_cnt = mill_check_get_field_in_mill(view->field, piece);

        if (view->mill_cnt > 0) {
                mill_view_set_status(view, MILL_STATUS_TAKE);
                return;
        }

        view->round++;

        /* Check if all pieces were placed */
        if (round == 17)
                mill_view_set_status(view, MILL_STATUS_MOVE);
        else
                mill_view_set_status(view, MILL_STATUS_PLACE);
}

static void mill_field_clicked(GtkWidget *button,
                               MillField *piece)
{
        MillView *view;
        MillStatus status;

        view   = mill_field_get_view(piece);
        status = view->status;

        switch (status) {
        case MILL_STATUS_PLACE:
                mill_field_place(view, view->field, piece);
                break;
        case MILL_STATUS_MOVE:
                mill_field_move(view, view->field, piece);
                break;
        case MILL_STATUS_TAKE:
                mill_field_take(view, view->field, piece);
                break;
        case MILL_STATUS_DONE:
                return;
        }
}

/**
 * Accessors
 */
void mill_view_set_active(MillView *view,
                          gboolean active)
{
        if (!active) return;

        MillOwner owner;
        gint i, k;

        for (i = 0; i < 7; i++) {
                for (k = 0; k < 7; k++) {
                        owner = mill_field_get_owner(view->field[i][k]);

                        if (owner == MILL_OWNER_BLOCK) continue;

                        mill_field_set_owner(view->field[i][k], MILL_OWNER_NONE);
                }
        }

        view->round      = 0;
        view->pieces_one = 9;
        view->pieces_two = 9;

        mill_view_set_status(view, MILL_STATUS_PLACE);
}

GtkWidget *mill_view_get_viewport(MillView *view)
{
        return view->grid;
}

void mill_view_set_header_bar(MillView *view,
                              GtkWidget *hbar)
{
        view->hbar = hbar;
}

static void mill_view_add_fields(MillView *view,
                                 MillField *field[7][7])
{
        gint i, k;
        GtkGrid *grid;
        GtkWidget *button;
        MillOwner owner;

        grid = GTK_GRID(view->grid);

        for (i = 0; i < 7; i++) {
                for (k = 0; k < 7; k++) {
                        field[i][k] = mill_field_new_at(i, k);
                        owner = mill_field_get_owner(field[i][k]);
                        if (owner == MILL_OWNER_BLOCK) continue;
                        mill_field_set_view(field[i][k], view);
                        button = mill_field_get_button(field[i][k]);
                        gtk_grid_attach(grid, button, i, k, 1, 1);
                        g_signal_connect(button, "clicked",
                                         G_CALLBACK(mill_field_clicked),
                                         field[i][k]);
                        g_signal_connect(button, "enter",
                                         G_CALLBACK(mill_field_enter),
                                         field[i][k]);
                        g_signal_connect(button, "leave",
                                         G_CALLBACK(mill_field_leave),
                                         field[i][k]);
                }
        }
}

static void mill_view_init(MillView *view)
{
        view->grid       = gtk_grid_new();
        view->hbar       = NULL;
        view->last       = NULL;
        view->status     = MILL_STATUS_PLACE;
        view->round      = 0;
        view->mill_cnt   = 0;
        view->pieces_one = 9;
        view->pieces_two = 9;

        /* Grid */
        GtkStyleContext *context = gtk_widget_get_style_context(view->grid);
        gtk_style_context_add_class(context, MILL_CLASS_GRID);
        gtk_grid_set_column_homogeneous(GTK_GRID(view->grid), TRUE);
        gtk_grid_set_row_homogeneous(GTK_GRID(view->grid), TRUE);
        mill_view_add_fields(view, view->field);
}

MillView *mill_view_new(void)
{
        MillView *view;

        view = g_malloc(sizeof(MillView));

        mill_view_init(view);

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
