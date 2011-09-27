#!/usr/bin/python

import model_api
import threads

api = model_api.load_api()

final_vars = model_api.run_experiment(api)
print "T:", final_vars.t, "\tPA:", final_vars.pa
final_vars = model_api.run_simulation(api)
print "T:", final_vars.t, "\tPA:", final_vars.pa
