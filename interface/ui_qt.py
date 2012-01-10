from PySide import QtCore, QtGui

from simulation import SimulationThread
from generic_ui import GenericUI

from ui_qt_plots import Plots

class QtUI(GenericUI):
    def __init__(self, argv):
        GenericUI.__init__(self, argv)

        self.app = QtGui.QApplication(argv)

        self.plots = Plots()
        self.plots.add_plot("t", "pa")
        self.plots.add_plot("t", "qao")
        self.plots.add_plot("t", "vud")

        self.win = QtGui.QWidget()
        self.win.setGeometry(300, 300, 250, 150)
        self.win.setWindowTitle('Icon')

        self.btnP = QtGui.QPushButton('Start', self.win)
        self.set_button_image(self.btnP, "media-playback-start")
        self.btnP.clicked.connect(self.toggle_simulation)
        self.btnC = QtGui.QPushButton('Clear', self.win)
        self.set_button_image(self.btnC, "edit-clear")
        self.btnC.clicked.connect(self.clear_plots_ui)
        self.btnR = QtGui.QPushButton('Reset', self.win)
        self.set_button_image(self.btnR, "media-playback-stop")
        self.btnR.clicked.connect(self.reset_simulation)
        self.btnA = QtGui.QPushButton('Add Plot', self.win)
        self.set_button_image(self.btnA, "document-new")
        self.btnA.clicked.connect(self.add_plot_ui)
        self.btnL = QtGui.QPushButton('Load', self.win)
        self.set_button_image(self.btnL, "document-open")
        self.btnL.clicked.connect(self.load_sim_ui)
        self.btnS = QtGui.QPushButton('Save', self.win)
        self.set_button_image(self.btnS, "document-savee")
        self.btnL.clicked.connect(self.save_sim_ui)
        self.btnQ = QtGui.QPushButton('Quit', self.win)
        self.set_button_image(self.btnQ, "application-exit")
        self.btnQ.clicked.connect(self.quit)

        self.btnbox = QtGui.QHBoxLayout()
        align = QtCore.Qt.AlignHCenter
        self.btnbox.addWidget(self.btnP, stretch=0, alignment=align)
        self.btnbox.addWidget(self.btnC, stretch=0, alignment=align)
        self.btnbox.addWidget(self.btnR, stretch=0, alignment=align)
        self.btnbox.addWidget(self.btnA, stretch=0, alignment=align)
        self.btnbox.addWidget(self.btnL, stretch=0, alignment=align)
        self.btnbox.addWidget(self.btnS, stretch=0, alignment=align)
        self.btnbox.addWidget(self.btnQ, stretch=0, alignment=align)

        self.mainbox = QtGui.QVBoxLayout()
        align = QtCore.Qt.AlignTop
        self.mainbox.addWidget(self.plots, stretch=1, alignment=align)
        self.mainbox.addLayout(self.btnbox, stretch=0)

        self.win.setLayout(self.mainbox)
        self.win.show()

    def set_button_image(self, button, icon_name):
        if QtGui.QIcon.hasThemeIcon(icon_name):
            icon = QtGui.QIcon.fromTheme(icon_name)
            button.setIcon(icon)

    def start_ui(self, *args):
        try:
            self.app.exec_()
        except KeyboardInterrupt:
            self.quit()

    def quit_ui(self, *args):
        self.app.quit()

    def update_plots_ui(self, *args):
        pass

    def pause_ui(self, *args):
        self.btnP.setText("Resume")
        self.set_button_image(self.btnP, "media-playback-start")

    def resume_ui(self, *args):
        self.btnP.setText("Pause")
        self.set_button_image(self.btnP, "media-playback-pause")

    def reset_ui(self, *args):
        self.clear_plots_ui()
        self.btnP.setText("Start")
        self.set_button_image(self.btnP, "media-playback-start")

    def clear_plots_ui(self, *args):
        pass

    def prompt_for_text(self):
        pass

    def add_plot_ui(self, *args):
        pass

    def del_plot_ui(self, *args):
        pass

    def get_plotted_vars_ui(self, *args):
        pass

    def load_sim_ui(self, *args):
        self.btnP.setText("Start")
        self.set_button_image(self.btnP, "media-playback-start")
        pass

    def save_sim_ui(self, *args):
        pass
