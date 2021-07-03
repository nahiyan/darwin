import os
import sys
if len(sys.argv) == 2 and (sys.argv[1] == "control-panel" or sys.argv[1] == "cp"):
    os.system("cd modules/control-panel; yarn app:start")
else:
    print("Unknown target.")
