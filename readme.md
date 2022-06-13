Create a build directory, it is listed in the .gitignore to prevent clutter.

Then, to compile, run the following commands:
```
cd build
cmake ../
make
```
```cmake ../``` is only neccesary the first time.

If desired, parameters can be changed in ```src/Parameters.h```

Results are stored in the Results directory. Running the code will rewrite these files.
