import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import threading

from client.rules import main as rules_main
from server.publisher import main as publisher_main

if __name__ == '__main__':
    threading.Thread(target=rules_main, daemon=True).start()
    publisher_main()
    