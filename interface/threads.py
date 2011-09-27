import threading

class StoppableThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.stop_event = threading.Event()

    def stop(self):
        """Signal to this thread that it must terminate."""
        if self.isAlive() == True:
            # set event to signal thread to terminate
            self.stop_event.set()
            # block calling thread until thread really has terminated
            self.join()

    def run(self):
        """Start an infinite loop of calls to self.run_body()."""
        while self.stop_event.is_set() == False:
            run_body()

    def run_body(self):
        """Override this method in your subclass."""
        pass
