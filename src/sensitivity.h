/* Define a type that points to a module function. */
typedef void (*modulefn)(const PARAMS &p, VARS &v);

extern std::map<std::string,modulefn> modules;

void init_modules();

bool isnan(double x);

void analyse(modulefn f, char* control_var, double min_val, double inc_val,
             double max_val, char** outputs, int output_count, string outfile);

void gnuplot(char* control_var, int output_count, string scriptfile,
             string datafile, string epsfile);

int main(int argc, char *argv[]);
