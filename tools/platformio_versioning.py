import os
import inspect
dir_path = os.path.dirname(os.path.realpath(inspect.getfile(lambda: None)))
auto_buildnumber_path = os.path.join(dir_path, "auto_buildnumber") # https://github.com/JackGruber/auto_buildnumber

print("")
try:
    import sys
    sys.path.insert(0, auto_buildnumber_path)
    import versioning
    print("auto versioning")
    versioning.UpdateVersionFile("include/version.h", "DEFINEHEADER", False, "include/version_build.h")
except:
    print("No auto versioning")
print("")