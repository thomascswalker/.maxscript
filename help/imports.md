# Python in 3ds Max: Imports

## Importing modules
Imports in Python through 3ds Max are kind of weird. The easiest way to maintain imports is to utilize the `__init__.py` file to initialize adding the root package directory to the system path list.

### Example
Assuming our package has a hierarchy like the below:
```
root/
    main.py
    __init__.py
    functions/
        some_file.py
```

In the root init file, we can add:

```Python
# __init__
import sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
```

Then, in `main.py`, you can simply do a relative import based on the root directory.

```Python
# main
from functions import some_file
```
