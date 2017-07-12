#include "cf-check.h"
#include "cf-field.h"

#include <gtk/gtk.h>

#define CF_MARKUP_WON "<span size='30000' color='#000000'>✓</span>"

static void cf_check_set_mark_won(CfField *field[4])
{
        gint i;
        GtkWidget *widget, *label;

        for (i = 0; i < 4; i++) {
                widget = cf_field_get_button(field[i]);
                label = gtk_bin_get_child(GTK_BIN(widget));
                gtk_label_set_markup(GTK_LABEL(label), CF_MARKUP_WON);
        }
}

static gboolean cf_check_get_match(CfField *group[4])
{
        gint i;
        CfOwner owner[4];

        /* Collect owners */
        for (i = 0; i < 4; i++)
                owner[i] = cf_field_get_owner(group[i]);

        /* Check for empty field */
        for (i = 0; i < 4; i++)
                if (owner[i] == CF_OWNER_NONE)
                        return FALSE;

        if (owner[0] == owner[1] &&
            owner[1] == owner[2] &&
            owner[2] == owner[3]) {
                cf_check_set_mark_won(group);
                return TRUE;
        } else {
                return FALSE;
        }
}

gboolean cf_check_get_won(CfField *field[7][6])
{
        gint i, k, m;
        CfField *group[4];

        static gint col[24][4] = {
                {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3},
                {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3},

                {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
                {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1},
                {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2},
                {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3},

                {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3},
                {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}
        };

        static gint row[24][4] = {
                {0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2},
                {3, 3, 3, 3}, {4, 4, 4, 4}, {5, 5, 5, 5},

                {0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4, 5},
                {0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4, 5},
                {0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4, 5},
                {0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4, 5},

                {0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4 ,5},
                {5, 4, 3, 2}, {4, 3, 2, 1}, {3, 2, 1, 0}
        };

        for (i = 0; i < 4; i++) {
                for (k = 0; k < 24; k++) {
                        for (m = 0; m < 4; m++)
                                group[m] = field[col[k][m]+i][row[k][m]];

                        if (cf_check_get_match(group))
                                return TRUE;
                }
        }

        return FALSE;
}

CfField *cf_check_get_lowest_field(CfField *field[7][6],
                                   CfField *start)
{
        gint col, row;
        CfOwner owner;

        col = cf_field_get_col(start);

        for (row = 5; row >= 0; row--) {
                owner = cf_field_get_owner(field[col][row]);

                if (owner == CF_OWNER_NONE)
                        return field[col][row];
        }

        return NULL;
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
