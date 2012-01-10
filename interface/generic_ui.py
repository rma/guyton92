from model_api import load_api
from simulation import SimulationThread

class GenericUI():
    def __init__(self, argv):
        self.api = load_api()
        self.simThread = SimulationThread(self.api, self.update_plots_ui,
                                          delay=0.05)

    def api(self):
        return self.api

    def sim(self):
        return self.simThread

    def quit(self, *args):
        self.simThread.stop()
        self.quit_ui(*args)

    def pause_simulation(self, *args):
        if self.simThread.is_alive():
            self.simThread.stop()
            self.pause_ui(*args)

    def toggle_simulation(self, *args):
        if self.simThread.is_alive():
            self.simThread.stop()
            self.pause_ui(*args)
        else:
            self.simThread.start()
            self.resume_ui(*args)

    def reset_simulation(self, *args):
        if self.simThread.is_alive():
            self.simThread.stop()
        self.simThread.reset()
        self.reset_ui(*args)

    def load_simulation(self, filename):
        self.simThread.load_experiment(filename)
        return self.simThread.experiment_outputs()

    def save_simulation(self, filename):
        output_vars = self.get_plotted_vars_ui()
        self.simThread.save_to_file(filename, output_vars)

    def start_ui(self, *args):
        raise NotImplementedError

    def quit_ui(self, *args):
        raise NotImplementedError

    def update_plots_ui(self, *args):
        raise NotImplementedError

    def pause_ui(self, *args):
        raise NotImplementedError

    def resume_ui(self, *args):
        raise NotImplementedError

    def reset_ui(self, *args):
        raise NotImplementedError

    def clear_plots_ui(self, *args):
        raise NotImplementedError

    def add_plot_ui(self, *args):
        raise NotImplementedError

    def del_plot_ui(self, *args):
        raise NotImplementedError

    def get_plotted_vars_ui(self):
        raise NotImplementedError
