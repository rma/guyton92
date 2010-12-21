/** A parameter change is identified by the parameter name and new value. */
struct PARAM_CHANGE {
  const char *name; /** The name of the parameter to change. */
  double value; /** The new value for the parameter. */
};

/** A vector of parameter changes. */
typedef std::vector<PARAM_CHANGE> vec_changes;

/** A set of parameter changes is scheduled to occur at a specific time. */
struct PARAM_CHANGES {
  double at_time; /** The scheduled time in the simulation (mins). */
  vec_changes changes; /** The vecotr of parameter changes to apply. */
};

/** A queue of scheduled parameter changes. */
typedef std::queue<PARAM_CHANGES> param_changes;

class Experiment {
private:
  param_changes changes;
  PARAMS &params;
  std::string *err;
  std::vector<double> times;
  std::vector<std::string> outputs;
public:
  Experiment(PARAMS &p, std::istream &input);
  ~Experiment();
  void update(double time);
  double stop_at();
  bool failed();
  const std::string* errmsg();
  const double* output_times();
  const std::vector<std::string>& output_vars() const;
};
