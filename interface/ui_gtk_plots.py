import gobject
import gtk

from ui_gtk_plot import Plot

class Plots(gtk.Notebook):
    def __init__(self, *args, **kwargs):
        gtk.Notebook.__init__(self, *args, **kwargs)
        self.plot_list = []

    def add_plot(self, x_name, y_name, activate=False):
        plot = Plot(x_name, y_name)
        plot_widget = plot.get_canvas()
        plot_title = y_name.upper()

        self.plot_list.append(plot)

        page_label = self.make_tab_widget(plot_title, plot_widget)
        self.append_page(plot.get_canvas(), tab_label=page_label)
        #self.set_tab_label_text(plot.get_canvas(), y_name.upper())
        # NOTE: pages that are added after the parent window is shown
        #       (ie, window.show_all()) will not be visible until the
        #       the following call to self.show_all().
        self.show_all()
        if activate:
            self.set_current_page(-1)

    def plot_count(self):
        return len(self.plot_list)

    def get_plot(self, ix):
        return self.plot_list[ix]

    def has_plot_of(self, plot_var):
        for plot in self.plot_list:
            if plot.get_plot_var() == plot_var:
                return True
        return False

    def plotted_variables(self):
        return [plot.get_plot_var() for plot in self.plot_list]

    def update_plots(self, simThread, acquireLock=True):
        for plot in self.plot_list:
            plot.update_plot(simThread, acquireLock)

    def clear_plots(self, acquireLock=True):
        for plot in self.plot_list:
            plot.clear_plot(acquireLock)

    def remove_current_plot(self):
        current_plot_ix = self.get_current_page()
        if current_plot_ix >= 0:
            self.remove_page(current_plot_ix)

    def on_close_tab_clicked(self, sender, widget):
        #get the page number of the tab we wanted to close
        pagenum = self.page_num(widget)
        #and close it
        self.remove_page(pagenum)

    def make_tab_widget(self, title, page_contents):
        #hbox will be used to store a label and button, as notebook tab title
        hbox = gtk.HBox(False, 0)
        label = gtk.Label(title)
        hbox.pack_start(label)

        #get a stock close button image
        close_image = gtk.image_new_from_stock(gtk.STOCK_CLOSE,
                                               gtk.ICON_SIZE_MENU)
        image_w, image_h = gtk.icon_size_lookup(gtk.ICON_SIZE_MENU)

        #make the close button
        btn = gtk.Button()
        btn.set_relief(gtk.RELIEF_NONE)
        btn.set_focus_on_click(False)
        btn.add(close_image)
        hbox.pack_start(btn, False, False)

        #this reduces the size of the button
        style = gtk.RcStyle()
        style.xthickness = 0
        style.ythickness = 0
        btn.modify_style(style)

        #connect the close button
        btn.connect('clicked', self.on_close_tab_clicked, page_contents)

        hbox.show_all()
        return hbox
