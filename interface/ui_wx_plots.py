import wx

from ui_wx_plot import Plot

class Plots(wx.Notebook):
    def __init__(self, parent, *args, **kwargs):
        wx.Notebook.__init__(self, parent, style=wx.NB_TOP,*args, **kwargs)
        self.plot_list = []
        self.parent = parent

    def add_plot(self, x_name, y_name, activate=False):
        plot_panel = wx.ScrolledWindow(self, -1)
        plot_panel.SetScrollbars(1, 1, 1, 1)
        plot_sizer = wx.BoxSizer(wx.VERTICAL)
        plot = Plot(x_name, y_name, plot_panel)
        plot_widget = plot.get_canvas()
        self.plot_list.append(plot)
        plot_title = y_name.upper()
        plot_sizer.Add(plot_widget, 1, wx.EXPAND)
        plot_panel.SetSizer(plot_sizer)

        self.AddPage(plot_panel, plot_title, activate)
        if activate:
            page_index = self.indexOf(plot_panel)
            if page_index is not None:
                self.SetSelection(page_index)

    def indexOf(self, widget):
        for i in range(self.GetPageCount()):
            if self.GetPage(i) == widget:
                return i
        return None

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

    def update_plots(self, simThread):
        for plot in self.plot_list:
            plot.update_plot(simThread)

    def clear_plots(self):
        for plot in self.plot_list:
            plot.clear_plot()

    def remove_current_plot(self):
        current_plot_ix = self.GetSelection()
        if current_plot_ix >= 0:
            self.DeletePage(current_plot_ix)
