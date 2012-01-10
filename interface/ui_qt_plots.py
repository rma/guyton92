from PySide import QtCore, QtGui

from ui_qt_plot import Plot

class Plots(QtGui.QTabWidget):
    def __init__(self):
        QtGui.QTabWidget.__init__(self)
        self.plot_list = []

    def add_plot(self, x_name, y_name, activate=False):
        plot = Plot(x_name, y_name)
        plot_widget = plot.get_canvas()
        plot_title = y_name.upper()
        self.plot_list.append(plot)

        # self.addTab(plot_widget, unicode(plot_title, 'utf-8'))

        self.show()
        if activate:
            self.setCurrentIndex(self.indexOf(plot_widget))

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
        current_plot_ix = self.currentIndex()()
        if current_plot_ix >= 0:
            self.removeTab(current_plot_ix)
