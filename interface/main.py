#!/usr/bin/python

import argparse
import platform
import sys

def default_platform_ui(argv):
    this_system = platform.system()
    err_msg = ("ERROR: unknown system %s, "
               "cannot choose an appropriate interface")
    log_msg = "Detected system:\t%s\nStarting interface:\t%s"

    if this_system == 'Linux':
        print (log_msg % (this_system, 'PyGTK'))
        from ui_gtk import GtkUI
        return GtkUI(argv)
    elif this_system == 'Darwin':
        print (log_msg % (this_system, 'NOT IMPLEMENTED'))
        raise NotImplementedError
    elif this_system == 'Windows':
        print (log_msg % (this_system, 'NOT IMPLEMENTED'))
        raise NotImplementedError
    elif this_system[:6] == 'CYGWIN':
        print (log_msg % (this_system, 'NOT IMPLEMENTED'))
        raise NotImplementedError
    else:
        print (err_msg % (this_system,))
        return None


def main(argv=None):
    if argv is None:
        argv = sys.argv

    parser = argparse.ArgumentParser(
        description='Analyse simulations of the Guyton model.')
    parser.add_argument("-i", "--interface", choices=['gtk', 'qt', 'wx'],
                        help="Select which interface to use.")

    args = parser.parse_args(argv[1:])

    if args.interface is None:
        ui = default_platform_ui(argv)
    elif args.interface == "gtk":
        from ui_gtk import GtkUI
        ui = GtkUI(argv)
    elif args.interface == "qt":
        from ui_qt import QtUI
        ui = QtUI(argv)
    elif args.interface == "wx":
        from ui_wx import WxUI
        ui = WxUI(argv)
    else:
        raise NotImplementedError

    ui.start_ui()
    return 0

if __name__ == "__main__":
    sys.exit(main())
