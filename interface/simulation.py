from time import sleep

from threads import StoppableThread
from model_api import ModelExperiment
from model_defn import MPARAMS, MVARS

class SimulationThread(StoppableThread):
    def __init__(self, api, callback, mpars=None, mvars=None, delay=None):
        StoppableThread.__init__(self)
        self.api = api
        self.callback = callback
        self.experiment = None

        if delay is None:
            self.delay = 0.25
        else:
            self.delay = delay

        self.reset()

    def param_names(self):
        return self.api.param_names()

    def var_names(self):
        return self.api.var_names()

    def reset(self, mpars=None, mvars=None):
        if mpars is None:
            self.mpars = self.api.new_params()
        else:
            self.mpars = mpars

        if mvars is None:
            self.mvars = self.api.new_vars()
        else:
            self.mvars = mvars

        self.cpars = self.mpars.contents
        self.cvars = self.mvars.contents

        self.cpars.newkidney = 0

    def load_experiment(self, filename):
        if self.is_alive():
            self.stop()
            btnP.set_label("Resume")

        self.delay = 0.5
        self.reset()

        self.experiment = ModelExperiment(self.api, self.mpars, filename)

    def experiment_outputs(self):
        if self.experiment is not None:
            return self.experiment.outputs()
        else:
            return []

    def var(self, name):
        return getattr(self.cvars, name)

    def par(self, name):
        return getattr(self.cpars, name)

    def get_pars(self):
        return self.mpars

    def get_vars(self):
        return self.mvars

    def get_delay(self):
        return self.delay

    def run_body(self):
        self.api.step(self.mpars, self.mvars, self.experiment)
        self.callback(self)
        if self.delay is not None:
            sleep(self.delay)

    def save_to_file(self, save_file, output_vars, save_vars=False):
        with open(save_file, 'w') as f:
            if len(output_vars) > 1:
                outputs = "o= " + " ".join(output_vars)
                f.write("%s\n\n" % (outputs,))

            for (param_name, _) in MPARAMS._fields_:
                # write the parameter name and value
                param_value = self.par(param_name)
                f.write("%s %s\n" % (param_name, param_value))

            if save_vars:
                for (var_name, _) in MVARS._fields_:
                    # write the variable name and value
                    var_value = self.var(var_name)
                    f.write("%s %s\n" % (var_name, var_value))
