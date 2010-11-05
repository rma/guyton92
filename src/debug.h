void print_model_state(const PARAMS &p, const VARS &v, char *prefix);
void set_debug_stream(FILE *stream);

/**
 * An instrument is a function that performs an analysis of the model state,
 * given a struct of model parameters and a struct of state variables. The
 * third argument (\c data) is a pointer to some arbitrary instrument-specific
 * data. For instruments that do not require this argument, set it to \c NULL.
 *
 * Note that the function \b must return \c true to remain registered for
 * future notifications; it will be removed when it returns \c false.
 */
typedef bool (*instrument)(const PARAMS &p, const VARS &v, void *data);

/**
 * A filter is a function that determines whether or not a notification of the
 * model state reaches the registered instruments, given a struct of model
 * parameters and a struct of state variables. The third argument (\c data) is
 * a pointer to some arbitrary filter-specific data. For filters that do not
 * require this argument, set it to \c NULL.
 *
 * The function \b must return \c true for the notification to reach the
 * registered instruments; if it returns \c false the notification will be
 * blocked.
 */
typedef bool (*filter)(const PARAMS &p, const VARS &v, void *data);

bool add_instrument(instrument instr, void *data);
bool add_filter(filter filter, void *data);
void notify_instruments(const PARAMS &p, const VARS &v);
