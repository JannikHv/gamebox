#include "ttt-check.h"
#include "ttt-field.h"

#include <gtk/gtk.h>

static gboolean ttt_check_get_match(TttOwner owner[3])
{
        gint i;

        /* Look for non-assigned field */
        for (i = 0; i < 3; i++)
                if (owner[i] == TTT_OWNER_NONE)
                        return FALSE;

        return (owner[0] == owner[1]) && (owner[1] == owner[2]);
}

gboolean ttt_check_get_won(TttField *field[3][3])
{
        gint i, k;
        TttOwner group[3];

        for (i = 0; i < 3; i++) {
                for (k = 0; k < 3; k++)
                        group[k] = ttt_field_get_owner(field[i][k]);

                if (ttt_check_get_match(group))
                        return TRUE;

                for (k = 0; k < 3; k++)
                        group[k] = ttt_field_get_owner(field[k][i]);

                if (ttt_check_get_match(group))
                        return TRUE;
        }

        group[1] = ttt_field_get_owner(field[1][1]);

        group[0] = ttt_field_get_owner(field[0][0]);
        group[2] = ttt_field_get_owner(field[2][2]);

        if (ttt_check_get_match(group))
                return TRUE;

        group[0] = ttt_field_get_owner(field[0][2]);
        group[2] = ttt_field_get_owner(field[2][0]);

        if (ttt_check_get_match(group))
                return TRUE;

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
