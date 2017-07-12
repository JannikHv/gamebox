#include "gbox-window.h"
#include "css-code.h"
#include "tictactoe/ttt-view.h"
#include "connect-four/cf-view.h"
#include "mill/mill-view.h"

#include <gtk/gtk.h>

#define SIZE_GBOX_X 525
#define SIZE_GBOX_Y 500

#define SIZE_TTT_X 500
#define SIZE_TTT_Y 500

#define SIZE_CF_X 650
#define SIZE_CF_Y 550

#define SIZE_MILL_X 600
#define SIZE_MILL_Y 600

#define LABEL_TTT  "Tic Tac Toe"
#define LABEL_CF   "Connect Four"
#define LABEL_MILL "Mill"
#define LABEL_MAIN "Main"

#define MARKUP_TTT  "<span size='25000' color='#FFFFFF'>Tic Tac Toe</span>"
#define MARKUP_CF   "<span size='25000' color='#FFFFFF'>Connect Four</span>"
#define MARKUP_MILL "<span size='25000' color='#FFFFFF'>Mill</span>"

#define GBOX_ICON_NAME "applications-games"

#define GBOX_TITLE_MAIN "GameBox"

#define GBOX_CLASS_ACTION "gbox-action"
#define GBOX_CLASS_BUTTON "gbox-button"
#define GBOX_CLASS_GRID   "gbox-grid"
#define GBOX_CLASS_WIN    "gbox-window"

struct _GBoxWindow {
        /* Containers */
        GtkWidget *self;
        GtkWidget *hbar;
        GtkWidget *grid;
        GtkWidget *stack;

        /* Header bar widgets */
        GtkWidget *btn_new;
        GtkWidget *btn_back;

        /* Views */
        TttView   *view_ttt;
        CfView    *view_cf;
        MillView  *view_mill;

        /* Buttons */
        GtkWidget *btn_ttt;
        GtkWidget *btn_cf;
        GtkWidget *btn_mill;
};

static void load_style_context(void)
{
        GtkCssProvider *css_prov;

        css_prov = gtk_css_provider_new();

        gtk_css_provider_load_from_data(css_prov, CSS_DATA, -1, NULL);

        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                                  GTK_STYLE_PROVIDER(css_prov),
                                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void back_button_clicked(GtkWidget *button,
                                GBoxWindow *win)
{
        GtkStack *stack;
        GtkWindow *window;
        GtkHeaderBar *hbar;

        stack  = GTK_STACK(win->stack);
        window = GTK_WINDOW(win->self);
        hbar   = GTK_HEADER_BAR(win->hbar);

        /* Reset game */
        ttt_view_set_active(win->view_ttt, TRUE);
        cf_view_set_active(win->view_cf, TRUE);
        mill_view_set_active(win->view_mill, TRUE);

        gtk_stack_set_visible_child_name(stack, LABEL_MAIN);
        gtk_window_set_resizable(window, TRUE);
        gtk_window_resize(window, SIZE_GBOX_X, SIZE_GBOX_Y);
        gtk_header_bar_set_title(hbar, GBOX_TITLE_MAIN);
}

static void new_game_button_clicked(GtkWidget *button,
                                    GBoxWindow *win)
{
        GtkStack *stack;
        const gchar *visible_name;

        stack        = GTK_STACK(win->stack);
        visible_name = gtk_stack_get_visible_child_name(stack);

        if (g_strcmp0(visible_name, LABEL_TTT) == 0)
                ttt_view_set_active(win->view_ttt, TRUE);
        else if (g_strcmp0(visible_name, LABEL_CF) == 0)
                cf_view_set_active(win->view_cf, TRUE);
        else if (g_strcmp0(visible_name, LABEL_MILL) == 0)
                mill_view_set_active(win->view_mill, TRUE);
}

static void view_button_clicked(GtkWidget *button,
                                GBoxWindow *win)
{
        GtkStack *stack;
        GtkWindow *window;
        const gchar *label;

        stack  = GTK_STACK(win->stack);
        window = GTK_WINDOW(win->self);
        label  = gtk_button_get_label(GTK_BUTTON(button));

        if (g_strcmp0(label, LABEL_TTT) == 0) {
                ttt_view_set_active(win->view_ttt, TRUE);
                gtk_window_resize(window, SIZE_TTT_X, SIZE_TTT_Y);
        } else if (g_strcmp0(label, LABEL_CF) == 0) {
                cf_view_set_active(win->view_cf, TRUE);
                gtk_window_resize(window, SIZE_CF_X, SIZE_CF_Y);
        } else if (g_strcmp0(label, LABEL_MILL) == 0) {
                mill_view_set_active(win->view_mill, TRUE);
                gtk_window_unmaximize(window);
                gtk_window_resize(window, SIZE_MILL_X, SIZE_MILL_Y);

                /* In case the window was maximized, force resize to finish */
                gdk_flush();
                gtk_window_set_resizable(window, FALSE);
        }

        gtk_stack_set_visible_child_name(stack, label);
}

static void gbox_window_add_interface(GBoxWindow *win)
{
        GtkStyleContext *context;
        GtkStack *stack;
        GtkGrid *grid;
        gint i;

        stack = GTK_STACK(win->stack);
        grid  = GTK_GRID(win->grid);

        GtkWidget *button[3] = {win->btn_ttt, win->btn_cf, win->btn_mill};
        gchar *label[3]      = {LABEL_TTT, LABEL_CF, LABEL_MILL};
        gchar *markup[3]     = {MARKUP_TTT, MARKUP_CF, MARKUP_MILL};

        GtkWidget *child[3] = {
                gtk_bin_get_child(GTK_BIN(win->btn_ttt)),
                gtk_bin_get_child(GTK_BIN(win->btn_cf)),
                gtk_bin_get_child(GTK_BIN(win->btn_mill)),
        };

        GtkWidget *view[3] = {
                ttt_view_get_viewport(win->view_ttt),
                cf_view_get_viewport(win->view_cf),
                mill_view_get_viewport(win->view_mill),
        };

        gtk_stack_add_titled(stack, win->grid, LABEL_MAIN, LABEL_MAIN);

        for (i = 0; i < 3; i++) {
                gtk_stack_add_titled(stack, view[i], label[i], label[i]);
                gtk_label_set_markup(GTK_LABEL(child[i]), markup[i]);
                gtk_grid_attach(grid, button[i], 0, i, 1, 1);
                g_signal_connect(button[i], "clicked", G_CALLBACK(view_button_clicked), win);
                gtk_container_set_border_width(GTK_CONTAINER(button[i]), 7);

                /* Add style to button */
                context = gtk_widget_get_style_context(button[i]);
                gtk_style_context_add_class(context, GBOX_CLASS_BUTTON);
        }

        /* Connect header bar to views */
        ttt_view_set_header_bar(win->view_ttt, win->hbar);
        cf_view_set_header_bar(win->view_cf, win->hbar);
        mill_view_set_header_bar(win->view_mill, win->hbar);
}

static void gbox_window_init(GBoxWindow *win)
{
        win->self  = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        win->hbar  = gtk_header_bar_new();
        win->grid  = gtk_grid_new();
        win->stack = gtk_stack_new();

        win->btn_new  = gtk_button_new_with_label("New Game");
        win->btn_back = gtk_button_new();

        win->view_ttt  = ttt_view_new();
        win->view_cf   = cf_view_new();
        win->view_mill = mill_view_new();

        win->btn_ttt  = gtk_button_new_with_label(LABEL_TTT);
        win->btn_cf   = gtk_button_new_with_label(LABEL_CF);
        win->btn_mill = gtk_button_new_with_label(LABEL_MILL);

        /* Window */
        gtk_window_set_default_size(GTK_WINDOW(win->self), SIZE_GBOX_X, SIZE_GBOX_Y);
        gtk_window_set_position(GTK_WINDOW(win->self), GTK_WIN_POS_CENTER);
        gtk_window_set_titlebar(GTK_WINDOW(win->self), win->hbar);
        gtk_window_set_default_icon_name(GBOX_ICON_NAME);
        gbox_window_add_interface(win);
        gtk_container_add(GTK_CONTAINER(win->self), win->stack);
        g_signal_connect(win->self, "delete-event", gtk_main_quit, NULL);
        gtk_style_context_add_class(gtk_widget_get_style_context(win->self), GBOX_CLASS_WIN);

        /* Header bar */
        gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(win->hbar), TRUE);
        gtk_header_bar_set_title(GTK_HEADER_BAR(win->hbar), GBOX_TITLE_MAIN);
        gtk_header_bar_pack_start(GTK_HEADER_BAR(win->hbar), win->btn_back);
        gtk_header_bar_pack_end(GTK_HEADER_BAR(win->hbar), win->btn_new);

        /* Grid */
        gtk_grid_set_column_homogeneous(GTK_GRID(win->grid), TRUE);
        gtk_grid_set_row_homogeneous(GTK_GRID(win->grid), TRUE);
        gtk_container_set_border_width(GTK_CONTAINER(win->grid), 25);
        gtk_style_context_add_class(gtk_widget_get_style_context(win->grid), GBOX_CLASS_GRID);

        /* Stack */
        gtk_stack_set_transition_type(GTK_STACK(win->stack),
                                      GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);

        /* New Game button */
        g_signal_connect(win->btn_new, "clicked", G_CALLBACK(new_game_button_clicked), win);
        gtk_style_context_add_class(gtk_widget_get_style_context(win->btn_new), GBOX_CLASS_ACTION);

        /* Back button */
        g_signal_connect(win->btn_back, "clicked", G_CALLBACK(back_button_clicked), win);
        gtk_button_set_image(GTK_BUTTON(win->btn_back),
                             gtk_image_new_from_icon_name(
                             "go-previous-symbolic",
                             GTK_ICON_SIZE_BUTTON));

        load_style_context();
}

GtkWidget *gbox_window_new(void)
{
        GBoxWindow *win;

        win = g_malloc(sizeof(GBoxWindow));

        gbox_window_init(win);

        return win->self;
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
