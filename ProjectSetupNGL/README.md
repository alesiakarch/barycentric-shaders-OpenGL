# BlankNGL

![alt tag](http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/Demos/BlankNGL.png)

This is an empty boilerplate framework for NGL projects, it creates an empty window and draws nothing.

To build you need NGL installed as per the instructions [here](https://github.com/NCCA/NGL)

Windows

```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=~/NGL ..
cmake --build .

```

Mac Linux


```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=~/NGL ..
make 
```

// AK adding my project notes

18/12/2024 added my own mesh in, has to be a triangulated .obj
 maybe I should write a loader for the user to specify which mesh they want?  
 improve the relative path so u can load shaders and meshes from terminal in build directory