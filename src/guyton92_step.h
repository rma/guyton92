extern "C" void guyton92_step(PARAMS &p, VARS &v, Experiment *e);
extern "C" PARAMS * new_params();
extern "C" VARS * new_vars();
extern "C" void del_params(PARAMS *p);
extern "C" void del_vars(VARS *v);
extern "C" istream * exp_file_stream(const char *filename);
extern "C" Experiment * exp_new(PARAMS &p, istream &input);
extern "C" void exp_delete(Experiment *e);
extern "C" bool exp_failed(Experiment *e);
extern "C" const char * exp_errmsg(Experiment *e);
extern "C" double exp_stop_at(Experiment *e);
extern "C" int exp_output_count(Experiment *e);
extern "C" const char * exp_output_var(Experiment *e, unsigned int i);
extern "C" const double * exp_output_times(Experiment *e);
extern "C" double exp_sentinel_time();
