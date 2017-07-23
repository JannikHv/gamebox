#include "chks-check.h"
#include "chks-field.h"

#include <gtk/gtk.h>

ChksTurn chks_check_get_turn(ChksField *field[8][8],
                             ChksField *start,
                             ChksField *dest)
{
        gint start_x, start_y, dest_x, dest_y, diff_x, diff_y, mid_x, mid_y;
        ChksOwner owner, enemy, owner_mid, owner_dest;
        gboolean is_queen;

        start_x    = chks_field_get_col(start);
        start_y    = chks_field_get_row(start);
        dest_x     = chks_field_get_col(dest);
        dest_y     = chks_field_get_row(dest);
        diff_x     = dest_x - start_x;
        diff_y     = dest_y - start_y;
        owner      = chks_field_get_owner(start);
        enemy      = (owner == CHKS_OWNER_ONE) ? CHKS_OWNER_TWO : CHKS_OWNER_ONE;
        owner_dest = chks_field_get_owner(dest);
        is_queen   = chks_field_get_is_queen(start);

        /* Check destination field */
        if (owner_dest != CHKS_OWNER_NONE)
                return CHKS_TURN_NONE;

        /* Check for one regular step */
        if ((diff_x * diff_x == 1)) {
                if (is_queen && (diff_y * diff_y == 1))
                        return CHKS_TURN_MOVE;

                if (owner == CHKS_OWNER_ONE && diff_y == -1)
                        return CHKS_TURN_MOVE;

                if (owner == CHKS_OWNER_TWO && diff_y == 1)
                        return CHKS_TURN_MOVE;
        }

        /* Check for two steps */
        if ((diff_x * diff_x == 4)) {
                mid_x     = (dest_x + start_x) / 2;
                mid_y     = (dest_y + start_y) / 2;
                owner_mid = chks_field_get_owner(field[mid_x][mid_y]);

                if (owner_mid != enemy)
                        return CHKS_TURN_NONE;

                if (is_queen && (diff_y * diff_y) == 4)
                        return CHKS_TURN_TAKE;

                if (owner == CHKS_OWNER_ONE && diff_y == -2)
                        return CHKS_TURN_TAKE;

                if (owner == CHKS_OWNER_TWO && diff_y == 2)
                        return CHKS_TURN_TAKE;
        }

        return CHKS_TURN_NONE;
}

gboolean chks_check_get_can_jump_again(ChksField *field[8][8],
                                       ChksField *piece)
{
        gint start_x, start_y, dest_x, dest_y, i, k;
        ChksTurn turn;

        start_x   = chks_field_get_col(piece);
        start_y   = chks_field_get_row(piece);

        /* Check top/bottom left/right pieces */
        for (i = -2; i <= 2; i += 4) {
                for (k = -2; k <= 2; k += 4) {
                        dest_x = start_x + i;
                        dest_y = start_y + k;

                        /* Check if destination field is too far away */
                        if (dest_x < 0 || dest_x > 7 ||
                            dest_y < 0 || dest_y > 7)
                                continue;

                        /* Check if it's possible to jump there */
                        turn = chks_check_get_turn(field, piece, field[dest_x][dest_y]);

                        if (turn == CHKS_TURN_TAKE)
                                return TRUE;
                }
        }

        return FALSE;
}

gboolean chks_check_get_becomes_queen(ChksField *piece)
{
        ChksOwner owner;
        gint row;

        owner = chks_field_get_owner(piece);
        row   = chks_field_get_row(piece);

        if ((owner == CHKS_OWNER_ONE && row == 0) ||
            (owner == CHKS_OWNER_TWO && row == 7))
                return TRUE;
        else
                return FALSE;
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
