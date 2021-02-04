# Python in 3ds Max: Imports

## Importing modules
Imports in Python through 3ds Max are kind of weird.

### Example
Assuming our package has a hierarchy like the below:
```
root/
    main.py
    __init__.py
    functions/
        some_file.py
```

`some_file.py` has some functions in it that we want to use.
```Python
def testFunction:
    print("It worked!")
```

Then, in `main.py`, you can simply do a relative import based on the root directory.

```Python
# Standard imports
import sys
sys.path.append(os.path.dirname(os.path.realpath(__file__))) # Add 'this' directory to path

# Package imports
from functions import helpers

helpers.testFunction()
```
