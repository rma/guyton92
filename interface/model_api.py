import os

from model_defn import *

def load_library(libname = "libg92.so"):
    try:
        path = os.path.abspath(__file__)
        dir_path = os.path.dirname(path)
    except NameError:
        dir_path = os.path.abspath(os.getcwd())
    lib_path = os.path.join(dir_path, "..", "build", libname)
    return CDLL(lib_path)

def struct_fields(struct):
    return [fname for (fname, ftype) in struct.contents._fields_]

class ModelAPI:
    def new_params(self):
        return self.lib.new_params()

    def new_vars(self):
        return self.lib.new_vars()

    def del_params(self, p):
        return self.lib.del_params(p)

    def del_vars(self, v):
        return self.lib.del_vars(v)

    def step(self, p, v, e):
        return self.lib.guyton92_step(p, v, e)

    def exp_file_stream(self, filename):
        return self.lib.exp_file_stream(filename)

    def exp_new(self, p, istream):
        return self.lib.exp_new(p, istream)

    def exp_delete(self, exp):
        return self.lib.exp_delete(exp)

    def exp_failed(self, exp):
        return self.lib.exp_failed(exp)

    def exp_errmsg(self, exp):
        return self.lib.exp_errmsg(exp)

    def exp_stop_at(self, exp):
        return self.lib.exp_stop_at(exp)

    def exp_output_count(self, exp):
        return self.lib.exp_output_count(exp)

    def exp_output_var(self, exp, ix):
        return self.lib.exp_output_var(exp, ix)

    def exp_output_times(self, exp):
        return self.lib.exp_output_times(exp)

    def exp_sentinel_time(self, exp):
        return self.lib.exp_sentinel_time(exp)

    def __init__(self, lib):
        self.lib = lib
        PPARAMS = POINTER(MPARAMS)
        PVARS = POINTER(MVARS)
        # new_params()
        lib.new_params.restype = PPARAMS
        # new_vars()
        lib.new_vars.restype = PVARS
        # del_params()
        lib.del_params.argtypes = [PPARAMS]
        lib.del_params.restype = None
        # del_vars()
        lib.del_vars.argtypes = [PVARS]
        lib.del_vars.restype = None
        # step()
        lib.guyton92_step.argtypes = [PPARAMS, PVARS, c_void_p]
        lib.guyton92_step.restype = None
        # exp_file_stream()
        lib.exp_file_stream.argtypes = [c_char_p]
        lib.exp_file_stream.restype = c_void_p
        # exp_new()
        lib.exp_new.argtypes = [PPARAMS, c_void_p]
        lib.exp_new.restype = c_void_p
        # exp_delete()
        lib.exp_delete.argtypes = [c_void_p]
        lib.exp_delete.restype = None
        # exp_failed()
        lib.exp_failed.argtypes = [c_void_p]
        lib.exp_failed.restype = c_bool
        # exp_errmsg()
        lib.exp_errmsg.argtypes = [c_void_p]
        lib.exp_errmsg.restype = c_char_p
        # exp_stop_at()
        lib.exp_stop_at.argtypes = [c_void_p]
        lib.exp_stop_at.restype = c_double
        # exp_output_count()
        lib.exp_output_count.argtypes = [c_void_p]
        lib.exp_output_count.restype = c_int
        # exp_output_var()
        lib.exp_output_var.argtypes = [c_void_p, c_uint]
        lib.exp_output_var.restype = c_char_p
        # exp_output_times()
        lib.exp_output_times.argtypes = [c_void_p]
        lib.exp_output_times.restype = POINTER(c_double)
        # exp_sentinel_time()
        lib.exp_sentinel_time.argtypes = [c_void_p]
        lib.exp_sentinel_time.restype = c_double

def load_api(libname = "libg92.so"):
    return ModelAPI(load_library(libname))

def run_simulation(api, t_end=21590):
    p = api.new_params()
    cp = p.contents
    cp.newkidney = 0
    v = api.new_vars()
    cv = v.contents

    #print "0", "-->", t_end

    while (cv.t < t_end):
        #prev_t = cv.t
        api.step(p, v, None)
        #print prev_t, "-->", cv.t

    return cv

class ModelExperiment:
    def __init__(self, api, params, expfile):
        self.api = api
        self.params = params
        istream = api.exp_file_stream(expfile)
        self.exp = api.exp_new(params, istream)
        self._as_parameter_ = self.exp

    def delete(self):
        return self.api.exp_delete(self.exp)

    def failed(self):
        return self.api.exp_failed(self.exp)

    def errmsg(self):
        return self.api.exp_errmsg(self.exp)

    def stop_at(self):
        return self.api.exp_stop_at(self.exp)

    def outputs(self):
        count = self.api.exp_output_count(self.exp)
        output_vars = []
        for i in range(count):
            output_vars.append(self.api.exp_output_var(self.exp, i))
        return output_vars

    def output_times(self):
        times = self.api.exp_output_times(self.exp)
        sentinel = self.api.exp_sentinel_time(self.exp)
        i = 0
        while times[i] != sentinel:
            i += 1
        return times[0:i]

    def step(self, v):
        self.api.step(self.params, v, self.exp)

def run_experiment(api, exp_file="../exps/hypertension.exp"):
    p = api.new_params()
    cp = p.contents
    cp.newkidney = 0
    exp = ModelExperiment(api, p, exp_file)
    #print exp.outputs()
    #print exp.output_times()
    t_end = exp.stop_at()
    v = api.new_vars()
    cv = v.contents

    while (cv.t < t_end):
        #prev_t = cv.t
        exp.step(v)
        #print prev_t, "-->", cv.t

    return cv
