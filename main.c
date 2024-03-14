#include "gtkgl.h"
int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new("nook.inc", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_thread_join(thr);
    g_object_unref(app);
    return status;
}