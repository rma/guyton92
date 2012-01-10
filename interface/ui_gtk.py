import gobject
import gtk

from simulation import SimulationThread
from generic_ui import GenericUI

from ui_gtk_plots import Plots

class GtkUI(GenericUI):
    def __init__(self, argv):
        GenericUI.__init__(self, argv)

        self.plots = Plots()
        self.plots.add_plot("t", "pa")
        self.plots.add_plot("t", "qao")
        self.plots.add_plot("t", "vud")

        self.win = gtk.Window()
        self.win.connect("destroy", self.quit)
        self.win.set_default_size(400,300)
        self.win.set_title("Guyton model")

        self.gtk_settings = gtk.settings_get_default()
        self.gtk_settings.set_long_property("gtk-button-images", 1, "rob")

        self.btnP = gtk.Button(label="Start")
        self.set_button_image(self.btnP, gtk.STOCK_MEDIA_PLAY)
        self.btnP.connect("clicked", self.toggle_simulation)
        self.btnC = gtk.Button(label="Clear")
        self.set_button_image(self.btnC, gtk.STOCK_CLEAR)
        self.btnC.connect("clicked", self.clear_plots_ui)
        self.btnR = gtk.Button(label="Reset")
        self.set_button_image(self.btnR, gtk.STOCK_MEDIA_STOP)
        self.btnR.connect("clicked", self.reset_simulation)
        self.btnL = gtk.Button(label="Load")
        self.set_button_image(self.btnL, gtk.STOCK_OPEN)
        self.btnL.connect("clicked", self.load_sim_ui)
        self.btnS = gtk.Button(label="Save")
        self.set_button_image(self.btnS, gtk.STOCK_SAVE)
        self.btnS.connect("clicked", self.save_sim_ui)
        self.btnQ = gtk.Button(label="Quit")
        self.set_button_image(self.btnQ, gtk.STOCK_QUIT)
        self.btnQ.connect("clicked", self.quit)
        self.btnA = gtk.Button(label="Add Plot")
        self.set_button_image(self.btnA, gtk.STOCK_NEW)
        self.btnA.connect("clicked", self.add_plot_ui)
        self.btnD = gtk.Button(label="Remove Plot")
        self.set_button_image(self.btnD, gtk.STOCK_CLOSE)
        self.btnD.connect("clicked", self.del_plot_ui)

        self.btnbox = gtk.HBox()
        self.btnbox.pack_start(self.btnP, expand=True, fill=False)
        self.btnbox.pack_start(self.btnC, expand=True, fill=False)
        self.btnbox.pack_start(self.btnR, expand=True, fill=False)
        self.btnbox.pack_start(self.btnA, expand=True, fill=False)
        self.btnbox.pack_start(self.btnL, expand=True, fill=False)
        self.btnbox.pack_start(self.btnS, expand=True, fill=False)
        self.btnbox.pack_start(self.btnQ, expand=True, fill=False)

        self.mainbox = gtk.VBox()
        self.mainbox.pack_start(self.plots, expand=True, fill=True)
        self.mainbox.pack_start(self.btnbox, expand=False, fill=False)
        self.win.add(self.mainbox)

        self.win.show_all()

    def set_button_image(self, button, stock_image):
        img = gtk.Image()
        img.set_from_stock(stock_image, gtk.ICON_SIZE_BUTTON)
        button.set_image(img)

    def start_ui(self, *args):
        gobject.threads_init()
        gtk.gdk.threads_init()

        try:
            gtk.gdk.threads_enter()
            gtk.main()
            gtk.gdk.threads_leave()
        except KeyboardInterrupt:
            self.quit()

    def quit_ui(self, *args):
        gtk.main_quit()

    def update_plots_ui(self, *args):
        self.plots.update_plots(self.sim())

    def pause_ui(self, *args):
        self.btnP.set_label("Resume")
        self.set_button_image(self.btnP, gtk.STOCK_MEDIA_PLAY)

    def resume_ui(self, *args):
        self.btnP.set_label("Pause")
        self.set_button_image(self.btnP, gtk.STOCK_MEDIA_PAUSE)

    def reset_ui(self, *args):
        self.clear_plots_ui()
        self.btnP.set_label("Start")
        self.set_button_image(self.btnP, gtk.STOCK_MEDIA_PLAY)

    def clear_plots_ui(self, *args):
        self.plots.clear_plots(acquireLock=False)

    def prompt_for_text(self):
        dialog = gtk.MessageDialog(
            parent=self.win,
            type=gtk.MESSAGE_QUESTION,
            flags=gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
            buttons=gtk.BUTTONS_OK_CANCEL)
        dialog.set_markup('Add a new plot')
        entry = gtk.Entry()
        entry.connect("activate",
                      lambda entry, dialog, resp: dialog.response(resp),
                      dialog, gtk.RESPONSE_OK)
        hbox = gtk.HBox()
        hbox.pack_start(gtk.Label("Variable name:"), False, 5, 5)
        hbox.pack_end(entry)
        dialog.format_secondary_markup(
            "This variable will be plotted as a <i>time series</i>.")
        dialog.vbox.pack_end(hbox, True, True, 0)
        dialog.show_all()
        response = dialog.run()
        if response == gtk.RESPONSE_OK:
            text = entry.get_text()
            dialog.destroy()
            return text
        dialog.destroy()
        return None

    def add_plot_ui(self, *args):
        var_name = self.prompt_for_text()
        if var_name is None:
            return
        var_name = var_name.lower()
        for (vname, _) in self.sim().var_names():
            if vname.lower() == var_name:
                self.plots.add_plot("t", vname, activate=True)
                return
        dialog = gtk.MessageDialog(
            parent=self.win,
            type=gtk.MESSAGE_ERROR,
            flags=gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
            buttons=gtk.BUTTONS_OK)
        dialog.set_markup('Invalid variable name "%s"' % (var_name,))
        dialog.show_all()
        dialog.run()
        dialog.destroy()

    def del_plot_ui(self, *args):
        self.pause_simulation()
        self.plots.remove_current_plot()

    def get_plotted_vars_ui(self, *args):
        return self.plots.plotted_variables()

    def load_sim_ui(self, *args):
        dialog = gtk.FileChooserDialog(
            parent=self.win,
            title="Load simulation",
            action=gtk.FILE_CHOOSER_ACTION_OPEN,
            buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                     gtk.STOCK_OPEN, gtk.RESPONSE_OK))
        dialog.set_default_response(gtk.RESPONSE_OK)
        dialog.show_all()
        response = dialog.run()
        if response == gtk.RESPONSE_OK:
            self.load_simulation( dialog.get_filename() )
        self.plots.clear_plots(acquireLock=False)
        plot_vars = self.sim().experiment_outputs()
        for var_name in plot_vars:
            if not self.plots.has_plot_of(var_name):
                self.plots.add_plot("t", var_name)
        self.btnP.set_label("Start")
        self.set_button_image(self.btnP, gtk.STOCK_MEDIA_PLAY)
        dialog.destroy()

    def save_sim_ui(self, *args):
        dialog = gtk.FileChooserDialog(
            parent=self.win,
            title="Save simulation",
            action=gtk.FILE_CHOOSER_ACTION_SAVE,
            buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                     gtk.STOCK_SAVE, gtk.RESPONSE_OK))
        dialog.set_default_response(gtk.RESPONSE_OK)
        dialog.set_do_overwrite_confirmation(True)
        dialog.show_all()
        response = dialog.run()
        if response == gtk.RESPONSE_OK:
            save_file = dialog.get_filename()
            self.save_simulation( dialog.get_filename() )
        dialog.destroy()
