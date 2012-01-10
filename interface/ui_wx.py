import wx

from simulation import SimulationThread
from generic_ui import GenericUI

from ui_wx_plots import Plots

class WxUI(GenericUI):
    def __init__(self, argv):
        GenericUI.__init__(self, argv)

        self.sim().delay = 0.15

        self.app = wx.App(False)
        self.win = wx.Frame(None, wx.ID_ANY, "Guyton Model")

        self.plots = Plots(parent=self.win)
        self.plots.add_plot("t", "pa")
        self.plots.add_plot("t", "qao")
        self.plots.add_plot("t", "vud")

        self.btnbox = wx.BoxSizer(wx.HORIZONTAL)
        self.mainbox = wx.BoxSizer(wx.VERTICAL)

        self.btnP = wx.Button(self.win, label="Start")
        self.win.Bind(wx.EVT_BUTTON, self.toggle_simulation, self.btnP)
        self.btnbox.Add(self.btnP, 0)
        self.btnbox.AddStretchSpacer()
        self.btnC = wx.Button(self.win, label="Clear")
        self.win.Bind(wx.EVT_BUTTON, self.clear_plots_ui, self.btnC)
        self.btnbox.Add(self.btnC, 0)
        self.btnbox.AddStretchSpacer()
        self.btnR = wx.Button(self.win, label="Reset")
        self.win.Bind(wx.EVT_BUTTON, self.reset_simulation, self.btnR)
        self.btnbox.Add(self.btnR, 0)
        self.btnbox.AddStretchSpacer()
        self.btnA = wx.Button(self.win, label="Add Plot")
        self.win.Bind(wx.EVT_BUTTON, self.add_plot_ui, self.btnA)
        self.btnbox.Add(self.btnA, 0)
        self.btnbox.AddStretchSpacer()
        self.btnD = wx.Button(self.win, label="Remove Plot")
        self.win.Bind(wx.EVT_BUTTON, self.del_plot_ui, self.btnD)
        self.btnbox.Add(self.btnD, 0)
        self.btnbox.AddStretchSpacer()
        self.btnL = wx.Button(self.win, label="Load")
        self.win.Bind(wx.EVT_BUTTON, self.load_sim_ui, self.btnL)
        self.btnbox.Add(self.btnL, 0)
        self.btnbox.AddStretchSpacer()
        self.btnS = wx.Button(self.win, label="Save")
        self.win.Bind(wx.EVT_BUTTON, self.save_sim_ui, self.btnS)
        self.btnbox.Add(self.btnS, 0)
        self.btnbox.AddStretchSpacer()
        self.btnQ = wx.Button(self.win, label="Quit")
        self.win.Bind(wx.EVT_BUTTON, self.quit, self.btnQ)
        self.btnbox.Add(self.btnQ, 0)
        self.btnbox.AddStretchSpacer()

        self.mainbox.Add(self.plots, 1, wx.EXPAND | wx.ALIGN_TOP)
        self.mainbox.Add(self.btnbox, 0, wx.EXPAND | wx.ALIGN_BOTTOM)

        self.mainbox.Fit(self.win)
        self.win.SetSizer(self.mainbox)
        self.win.SetAutoLayout(1)
        self.win.Layout()
        self.win.Show(True)

    def start_ui(self, *args):
        try:
            self.app.MainLoop()
        except KeyboardInterrupt:
            self.quit()

    def quit_ui(self, *args):
        self.win.Close(True)

    def update_plots_ui(self, *args):
        self.plots.update_plots(self.sim())

    def pause_ui(self, *args):
        self.btnP.SetLabel("Resume")

    def resume_ui(self, *args):
        self.btnP.SetLabel("Pause")

    def reset_ui(self, *args):
        self.clear_plots_ui()
        self.btnP.SetLabel("Start")

    def clear_plots_ui(self, *args):
        self.plots.clear_plots()

    def add_plot_ui(self, *args):
        pass

    def del_plot_ui(self, *args):
        self.pause_simulation()
        self.plots.remove_current_plot()

    def get_plotted_vars_ui(self, *args):
        return self.plots.plotted_variables()

    def load_sim_ui(self, *args):
        pass

    def save_sim_ui(self, *args):
        pass
