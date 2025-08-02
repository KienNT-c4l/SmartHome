import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import threading

from client.rules import main as rules_main

if __name__ == '__main__':
    rules_main()