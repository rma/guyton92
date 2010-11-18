/**
 * @mainpage The Guyton 1992 model of whole-body physiology.
 *
 * @section intro Introduction
 * This is a modular implementation of the Guyton 1992 model of whole-body
 * physiology, based on the original (monolithic) Fortran 77 code. The aim of
 * this project is to replace the renal block of the model with a significantly
 * more detailed model of renal function.
 *
 * @section struct Model Structure
 * The main simulation loop resides in \link guyton92.cpp\endlink. The rest of
 * the model is divided into \ref modules, \ref experiments, \ref instruments
 * and \ref filters.
 *
 * @subsection struct_m Modules
 * @copydetails modules
 *
 * @subsection struct_e Experiments
 * @copydetails experiments
 *
 * @subsection struct_i Instruments
 * @copydetails instruments
 *
 * @subsection struct_f Filters
 * @copydetails filters
 */

/**
 * \defgroup modules Modules
 * The Guyton 1992 model consists of many separate modules (blocks) that
 * capture the dynamics of different sub-systems. Each module is implemented
 * as a single function (each stored in a separate file) that contains a
 * faithful translation of the original Fortran 77 code for that block.
 */

/**
 * \defgroup experiments Experiments
 * The Guyton 1992 model allows for experiments that simulate phenomena such
 * as \ref exp_rapidreg() "rapid autoregulation" or \ref exp_transfuse()
 * "transfusion and blood loss". These experiments are similar to the
 * \ref modules, except that they are able to modify model parameters in
 * addition to modifying state variables.
 */

/**
 * \defgroup instruments Instruments
 * Instruments analyse the model state and produce output. Once an instrument
 * has been registered (by \link add_instrument() add_instrument\endlink), it
 * will receive notifications of the model state.
 *
 * By default, these notifications occur after each iteration of the main
 * simulation loop, but \ref filters can reduce the number of notifications.
 */

/**
 * \defgroup filters Filters
 * Filters determine which notifications of the model state reach the
 * registered \ref instruments. Once a filter has been registered
 * (by \link add_filter() add_filter\endlink), it can block any notification
 * of the model state from reaching the registered instruments.
 */
