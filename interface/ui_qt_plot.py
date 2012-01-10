from PySide import QtCore, QtGui

from matplotlib.figure import Figure
from matplotlib.backends.backend_qt4agg \
    import FigureCanvasQTAgg as FigureCanvas

class Plot(Figure):
    def __init__(self, x_name, y_name, *args, **kwargs):
        Figure.__init__(self, *args, **kwargs)
        self.plot_axes = self.add_subplot(111, xlabel=x_name,
                                          ylabel=y_name.upper())
        self.plot_line = None
        self.canvas = FigureCanvas(self)
        self.plot_xname = x_name
        self.plot_yname = y_name
        self.plot_xdata = []
        self.plot_ydata = []
        self.plot_line, = self.plot_axes.plot(self.plot_xdata, self.plot_ydata)
        self.plot_max_points = 200

    def get_plot_line(self):
        return self.plot_line

    def get_canvas(self):
        return self.canvas

    def get_plot_var(self):
        return self.plot_yname

    def get_max_points(self):
        return self.plot_max_points

    def set_max_points(self, max_points):
        self.plot_max_points = max_points

    def __update_canvas(self):
        self.plot_axes.relim()
        self.plot_axes.autoscale_view(tight=False)
        if self.canvas is not None:
            self.canvas.draw()
        return False

    def update_plot(self, simThread):
        self.plot_xdata.append(simThread.var(self.plot_xname))
        self.plot_ydata.append(simThread.var(self.plot_yname))

        if len(self.plot_xdata) > self.plot_max_points:
            offset = len(self.plot_xdata) - self.plot_max_points
            self.plot_xdata = self.plot_xdata[offset:]
        if len(self.plot_ydata) > self.plot_max_points:
            offset = len(self.plot_ydata) - self.plot_max_points
            self.plot_ydata = self.plot_ydata[offset:]

        self.redraw_plot()

    def clear_plot(self):
        self.plot_xdata = []
        self.plot_ydata = []
        self.redraw_plot()

    def redraw_plot(self):
        self.plot_line.set_xdata(self.plot_xdata)
        self.plot_line.set_ydata(self.plot_ydata)
        self.__update_canvas()
