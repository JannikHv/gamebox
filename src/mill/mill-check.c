#include "mill-check.h"
#include "mill-field.h"

#include <gtk/gtk.h>

static gboolean mill_check_get_is_mill(MillField *group[3])
{
        gint i;
        MillOwner owner[3];

        for (i=0; i<3; i++) {
                owner[i] = mill_field_get_owner(group[i]);

                if (owner[i] == MILL_OWNER_NONE)
                        return FALSE;
        }

        if (owner[0] == owner[1] &&
            owner[1] == owner[2])
                return TRUE;

        return FALSE;
}

gint mill_check_get_free_fields(MillField *field[7][7],
                                MillOwner  owner)
{
        gint i, k, cnt;
        gint free = 0;
        MillOwner tmp;

        for (i=0; i<7; i++) {
                for (k=0; k<7; k++) {
                        tmp = mill_field_get_owner(field[i][k]);
                        cnt = mill_check_get_field_in_mill(field, field[i][k]);

                        /* If the field belongs to the enemy and is not in a mill */
                        if (cnt == 0 && tmp == owner)
                                free++;
                }
        }

        return free;
}

gboolean mill_check_get_field_in_range(MillField *field[7][7],
                                       MillField *piece,
                                       MillField *dest)
{
        gint start_x, start_y;
        gint dest_x, dest_y;
        gint diff_x, diff_y;
        gint tmp_x, tmp_y;
        gint diff;
        gboolean vertical;
        MillOwner owner;

        start_x = mill_field_get_col(piece);
        start_y = mill_field_get_row(piece);

        dest_x = mill_field_get_col(dest);
        dest_y = mill_field_get_row(dest);

        diff_x = dest_x - start_x;
        diff_y = dest_y - start_y;

        diff = diff_x + diff_y;
        diff = (diff < 0) ? diff * -1 : diff;

        /**
         * If same field is selected
         * or trying to move diagonally
         */
        if ((diff_x == 0 && diff_y == 0) ||
            (diff_x != 0 && diff_y != 0))
                return FALSE;

        /* One step always works */
        if (diff == 1)
                return TRUE;

        /* Check if the range is too big */
        if (diff > 3)
                return FALSE;

        /* Checking fields in between start and destination */
        vertical = (diff_y == 0) ? FALSE : TRUE;

        if (vertical) {
                tmp_y = (dest_y > start_y) ? dest_y : start_y;
                tmp_x = dest_x;
                while (--diff > 0) {
                        owner = mill_field_get_owner(field[tmp_x][tmp_y-diff]);

                        if ((tmp_x == 3 && tmp_y-diff == 3) ||
                             owner != MILL_OWNER_BLOCK)
                                return FALSE;

                        diff--;
                }
        } else {
                tmp_x = (dest_x > start_x) ? dest_x : start_x;
                tmp_y = dest_y;
                while (--diff > 0) {
                        owner = mill_field_get_owner(field[tmp_x-diff][tmp_y]);

                        if ((tmp_x-diff == 3 && tmp_y == 3) ||
                             owner != MILL_OWNER_BLOCK)
                                return FALSE;

                        diff--;
                }
        }

        return TRUE;
}

gint mill_check_get_field_in_mill(MillField *field[7][7],
                                  MillField *piece)
{
        MillField *group[3];
        gint mill_cnt = 0;
        gint i, k;

        static gint mill_x[16][3] = {
                {0, 3, 6}, {1, 3, 5}, {2, 3, 4},
                {0, 1, 2}, {4, 5, 6},
                {2, 3, 4}, {1, 3, 5}, {0, 3, 6},

                {0, 0, 0}, {1, 1, 1}, {2, 2, 2},
                {3, 3, 3}, {3, 3, 3},
                {4, 4, 4}, {5, 5, 5}, {6, 6, 6},
        };

        static gint mill_y[16][3] = {
                {0, 0, 0}, {1, 1, 1}, {2, 2, 2},
                {3, 3, 3}, {3, 3, 3},
                {4, 4, 4}, {5, 5, 5}, {6, 6, 6},

                {0, 3, 6}, {1, 3 ,5}, {2, 3, 4},
                {0, 1, 2}, {4, 5, 6},
                {2, 3, 4}, {1, 3, 5}, {0, 3, 6},
        };

        for (i=0; i<16; i++) {
                for (k=0; k<3; k++)
                        group[k] = field[mill_x[i][k]][mill_y[i][k]];

                if (group[0] != piece &&
                    group[1] != piece &&
                    group[2] != piece)
                        continue;

                if (mill_check_get_is_mill(group))
                        mill_cnt++;
        }

        return mill_cnt;
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
