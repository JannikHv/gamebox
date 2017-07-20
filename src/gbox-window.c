#include "gbox-window.h"
#include "gbox-preview.h"
#include "tictactoe/ttt-view.h"
#include "connect-four/cf-view.h"
#include "mill/mill-view.h"
#include "css-code.h"

#define GBOX_SIZE_WIN_X 670
#define GBOX_SIZE_WIN_Y 560

#define GBOX_SIZE_TTT_X 500
#define GBOX_SIZE_TTT_Y 500

#define GBOX_SIZE_CF_X 650
#define GBOX_SIZE_CF_Y 550

#define GBOX_SIZE_MILL_X 600
#define GBOX_SIZE_MILL_Y 600

#define GBOX_TITLE_PREV_TTT  "Tic Tac Toe"
#define GBOX_TITLE_PREV_CF   "Connect Four"
#define GBOX_TITLE_PREV_MILL "Mill"

#define GBOX_TITLE_GAME_TTT  "ttt"
#define GBOX_TITLE_GAME_CF   "cf"
#define GBOX_TITLE_GAME_MILL "mill"

#define GBOX_TITLE_MAIN "GameBox"

#define GBOX_CLASS_WINDOW "gbox-window"

struct _GBoxWindow {
        /* Container */
        GtkWidget *self;
        GtkWidget *hbar;
        GtkWidget *grid;
        GtkWidget *sidebar;
        GtkWidget *stack_main;
        GtkWidget *stack_prev;
        GtkWidget *stack_game;

        /* Header bar widgets */
        GtkWidget *btn_back;
        GtkWidget *btn_new;

        /* Previews */
        GBoxPreview *prev_ttt;
        GBoxPreview *prev_cf;
        GBoxPreview *prev_mill;

        /* Views (Games) */
        TttView  *view_ttt;
        CfView   *view_cf;
        MillView *view_mill;
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

static void start_game_button_clicked(GtkWidget *button,
                                      GBoxWindow *win)
{
        GtkWidget *visible_child;
        GtkWindow *window;
        GtkStack *stack_main, *stack_prev, *stack_game;
        const gchar *title_prev, *title_game;
        gboolean resizable;
        gint size_x, size_y;

        visible_child = win->stack_game;
        window        = GTK_WINDOW(win->self);
        stack_main    = GTK_STACK(win->stack_main);
        stack_prev    = GTK_STACK(win->stack_prev);
        stack_game    = GTK_STACK(win->stack_game);
        title_prev    = gtk_stack_get_visible_child_name(stack_prev);
        resizable     = TRUE;

        if (g_strcmp0(title_prev, GBOX_TITLE_PREV_TTT) == 0) {
                ttt_view_reset(win->view_ttt);
                title_game = GBOX_TITLE_GAME_TTT;
                size_x = GBOX_SIZE_TTT_X;
                size_y = GBOX_SIZE_TTT_Y;
        } else if (g_strcmp0(title_prev, GBOX_TITLE_PREV_CF) == 0) {
                cf_view_reset(win->view_cf);
                title_game = GBOX_TITLE_GAME_CF;
                size_x = GBOX_SIZE_CF_X;
                size_y = GBOX_SIZE_CF_Y;
        } else if (g_strcmp0(title_prev, GBOX_TITLE_PREV_MILL) == 0) {
                mill_view_reset(win->view_mill);
                title_game = GBOX_TITLE_GAME_MILL;
                resizable = FALSE;
                size_x = GBOX_SIZE_MILL_X;
                size_y = GBOX_SIZE_MILL_Y;
        }

        /* Hide preview stack in order to resize the window properly */
        gtk_widget_hide(win->stack_prev);

        /* Resize window properly */
        gtk_stack_set_visible_child_name(stack_game, title_game);
        gtk_stack_set_visible_child(stack_main, visible_child);
        gtk_window_set_default_size(window, size_x, size_y);
        gtk_window_resize(window, size_x, size_y);
        gdk_flush();
        gtk_window_set_resizable(window, resizable);

        /* Make header bar buttons sensitive */
        gtk_widget_set_sensitive(win->btn_back, TRUE);
        gtk_widget_set_sensitive(win->btn_new, TRUE);
}

static void new_game_button_clicked(GtkWidget *button,
                                    GBoxWindow *win)
{
        GtkStack *stack_game;
        const gchar *visible_name;

        stack_game   = GTK_STACK(win->stack_game);
        visible_name = gtk_stack_get_visible_child_name(stack_game);

        /* Reset game */
        if (g_strcmp0(visible_name, GBOX_TITLE_GAME_TTT) == 0)
                ttt_view_reset(win->view_ttt);
        else if (g_strcmp0(visible_name, GBOX_TITLE_GAME_CF) == 0)
                cf_view_reset(win->view_cf);
        else if (g_strcmp0(visible_name, GBOX_TITLE_GAME_MILL) == 0)
                mill_view_reset(win->view_mill);
}

static void back_button_clicked(GtkWidget *button,
                                GBoxWindow *win)
{
        GtkWindow *window;
        GtkHeaderBar *hbar;
        GtkWidget *visible_child;
        GtkStack *stack_main, *stack_game;
        const gchar *visible_name;
        gboolean resizable;

        window        = GTK_WINDOW(win->self);
        hbar          = GTK_HEADER_BAR(win->hbar);
        visible_child = win->stack_prev;
        stack_main    = GTK_STACK(win->stack_main);
        stack_game    = GTK_STACK(win->stack_game);
        visible_name  = gtk_stack_get_visible_child_name(stack_game);
        resizable     = gtk_window_get_resizable(window);

        /* Reset game */
        if (g_strcmp0(visible_name, GBOX_TITLE_GAME_TTT) == 0)
                ttt_view_reset(win->view_ttt);
        else if (g_strcmp0(visible_name, GBOX_TITLE_GAME_CF) == 0)
                cf_view_reset(win->view_cf);
        else if (g_strcmp0(visible_name, GBOX_TITLE_GAME_MILL) == 0)
                mill_view_reset(win->view_mill);

        ttt_view_reset(win->view_ttt);
        cf_view_reset(win->view_cf);
        mill_view_reset(win->view_mill);

        /* Show preview stack again */
        gtk_widget_show(win->stack_prev);

        if (!resizable)
                gtk_window_set_resizable(window, TRUE);

        gtk_window_set_default_size(window, GBOX_SIZE_WIN_X, GBOX_SIZE_WIN_Y);
        gtk_window_resize(window, GBOX_SIZE_WIN_X, GBOX_SIZE_WIN_Y);
        gtk_stack_set_visible_child(stack_main, win->grid);

        /* Change header bar title */
        gtk_header_bar_set_title(hbar, GBOX_TITLE_MAIN);

        /* Turn sensitivity of header bar buttons off */
        gtk_widget_set_sensitive(win->btn_back, FALSE);
        gtk_widget_set_sensitive(win->btn_new, FALSE);
}

static void gbox_window_add_interface(GBoxWindow *win)
{
        GtkStack *stack_prev, *stack_game;
        GtkWidget *button;
        gint i;

        stack_prev = GTK_STACK(win->stack_prev);
        stack_game = GTK_STACK(win->stack_game);

        const gchar *title_prev[3] = {
                GBOX_TITLE_PREV_TTT,
                GBOX_TITLE_PREV_CF,
                GBOX_TITLE_PREV_MILL,
        };

        const gchar *title_game[3] = {
                GBOX_TITLE_GAME_TTT,
                GBOX_TITLE_GAME_CF,
                GBOX_TITLE_GAME_MILL,
        };

        const gchar *image_name[3] = {
                "tictactoe-preview.png",
                "connect-four-preview.png",
                "mill-preview.png",
        };

        GtkWidget *view_prev[3] = {
                gbox_preview_get_viewport(win->prev_ttt),
                gbox_preview_get_viewport(win->prev_cf),
                gbox_preview_get_viewport(win->prev_mill),
        };

        GtkWidget *view_game[3] = {
                ttt_view_get_viewport(win->view_ttt),
                cf_view_get_viewport(win->view_cf),
                mill_view_get_viewport(win->view_mill),
        };

        GBoxPreview *preview[3] = {
                win->prev_ttt,
                win->prev_cf,
                win->prev_mill,
        };

        for (i = 0; i < 3; i++) {
                gtk_stack_add_titled(stack_prev, view_prev[i], title_prev[i], title_prev[i]);
                gtk_stack_add_titled(stack_game, view_game[i], title_game[i], title_game[i]);

                gbox_preview_set_title(preview[i], title_prev[i]);
                gbox_preview_set_image_name(preview[i], image_name[i]);

                button = gbox_preview_get_start_button(preview[i]);
                g_signal_connect(button, "clicked", G_CALLBACK(start_game_button_clicked), win);
        }

        /* Connect header bar to games */
        ttt_view_set_header_bar(win->view_ttt, win->hbar);
        cf_view_set_header_bar(win->view_cf, win->hbar);
        mill_view_set_header_bar(win->view_mill, win->hbar);
}

static void gbox_window_init(GBoxWindow *win)
{
        win->self       = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        win->hbar       = gtk_header_bar_new();
        win->grid       = gtk_grid_new();
        win->sidebar    = gtk_stack_sidebar_new();
        win->stack_main = gtk_stack_new();
        win->stack_prev = gtk_stack_new();
        win->stack_game = gtk_stack_new();

        win->btn_back = gtk_button_new();
        win->btn_new  = gtk_button_new_with_label("New Game");

        win->prev_ttt  = gbox_preview_new();
        win->prev_cf   = gbox_preview_new();
        win->prev_mill = gbox_preview_new();

        win->view_ttt  = ttt_view_new();
        win->view_cf   = cf_view_new();
        win->view_mill = mill_view_new();

        /* Window */
        gtk_style_context_add_class(gtk_widget_get_style_context(win->self), GBOX_CLASS_WINDOW);
        gtk_window_set_default_size(GTK_WINDOW(win->self), GBOX_SIZE_WIN_X, GBOX_SIZE_WIN_Y);
        gtk_window_set_titlebar(GTK_WINDOW(win->self), win->hbar);
        gtk_window_set_position(GTK_WINDOW(win->self), GTK_WIN_POS_CENTER);
        gbox_window_add_interface(win);
        gtk_container_add(GTK_CONTAINER(win->self), win->stack_main);
        g_signal_connect(win->self, "delete-event", gtk_main_quit, NULL);

        /* Header bar */
        gtk_header_bar_set_title(GTK_HEADER_BAR(win->hbar), GBOX_TITLE_MAIN);
        gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(win->hbar), TRUE);
        gtk_header_bar_pack_start(GTK_HEADER_BAR(win->hbar), win->btn_back);
        gtk_header_bar_pack_end(GTK_HEADER_BAR(win->hbar), win->btn_new);

        /* Grid */
        gtk_grid_attach(GTK_GRID(win->grid), win->sidebar,    0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(win->grid), win->stack_prev, 1, 0, 1, 1);

        /* Sidebar */
        gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(win->sidebar), GTK_STACK(win->stack_prev));
        gtk_widget_set_size_request(win->sidebar, 180, -1);
        gtk_widget_set_vexpand(win->sidebar, TRUE);

        /* Main stack */
        gtk_stack_add_titled(GTK_STACK(win->stack_main), win->grid, "Prev", "Prev");
        gtk_stack_add_titled(GTK_STACK(win->stack_main), win->stack_game, "Game", "Game");

        /* Preview stack */
        gtk_stack_set_transition_type(GTK_STACK(win->stack_prev), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
        gtk_stack_set_transition_duration(GTK_STACK(win->stack_prev), 500);
        gtk_widget_set_vexpand(win->stack_prev, TRUE);
        gtk_widget_set_hexpand(win->stack_prev, TRUE);

        /* Back button */
        g_signal_connect(win->btn_back, "clicked", G_CALLBACK(back_button_clicked), win);
        gtk_widget_set_sensitive(win->btn_back, FALSE);
        gtk_button_set_image(GTK_BUTTON(win->btn_back),
                             gtk_image_new_from_icon_name(
                             "go-previous-symbolic",
                             GTK_ICON_SIZE_BUTTON));

        /* New game button */
        g_signal_connect(win->btn_new, "clicked", G_CALLBACK(new_game_button_clicked), win);
        gtk_widget_set_sensitive(win->btn_new, FALSE);
        gtk_style_context_add_class(gtk_widget_get_style_context(win->btn_new), "suggested-action");

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
