There is an executable in the build directory. To run it, simply use ```./main``` in the build directory

To recompile, run the following commands:
```
cd build
cmake ../
make
```
```cmake ../``` is only neccesary the first time.

If desired, parameters can be changed in ```src/Parameters.h```

Results are stored in the Results directory. Running the code will rewrite these files.
