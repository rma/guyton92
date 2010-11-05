#include <cfloat>

#include "params.h"
#include "vars.h"
#include "filter_times.h"

/**
 * This filter restricts the notifications to occur only six times during the
 * simulation: at a reference time t, then at t plus 5 minutes, 1 hour, 1 day,
 * 2 weeks and 3 weeks.
 *
 * @param[in] p The struct of model parameters.
 * @param[in] v The struct of state variables.
 * @param[in] data An optional pointer to a boolean, indicating whether the
 *            first notification should also be permitted.
 *
 * @return \c true if the notification should be passed to the registered
 *         instruments, or \c false if it should not.
 */
bool filter_times(const PARAMS &p, const VARS &v, void *data) {
  /* The times at which to print a summary of the model state.
     NOTE: The fifth value was originally 40140 (just under 21 days). */
  static double time[] = {10070, 10075, 10130, 11510, 30230, 40310, DBL_MAX};
  /* The index of the next time at which to print the model state. */
  static int index = 0;
  /* A flag that indicates if this is the first call to the filter. */
  static bool first_call = true;

  if (first_call) {
    first_call = false;
    /* If data points to a true value, permit the first notification. */
    if (data && *(bool *) data) {
      return true;
    }
  }

  /* Check whether a model state notification should be made. */
  if (v.t >= time[index]) {
    index++;
    return true;
  }
  return false;
}
