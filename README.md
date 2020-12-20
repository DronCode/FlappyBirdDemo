Flappy Bird Game
=================

Description
-----------

`It's just a demo game.`

CI status
---------

[![Build status](https://ci.appveyor.com/api/projects/status/roum4p326855okuy/branch/main?svg=true)](https://ci.appveyor.com/project/DronCode/FlappyBirdDemo/branch/main)

Build instructions (PC, x86)
-----------------------

```
git clone https://github.com/DronCode/FlappyBirdDemo
cd FlappyBirdDemo
```

After that we must apply the patch for `oxygine-framework` (adds support python 3, skip if you have python 2):
```
cd libs/oxygine-framework/
git apply ../../patches/oxygine_python3_support.patch
cd ../..
```

And install python dependencies via PIP:
```
pip install pillow
```

after that we can generate assets cache
```
cd flappy_bird 
./gen-atlasses.bat
```

So, now we can build our project:
```
cd project/proj.cmake
mkdir build
cd build
cmake -A Win32 -G "Visual Studio 16 2019" ..
cmake --build . --config Release
cp -R ../../../data/* Release/
```

Why only x86
-------------

I don't have enough time to fix all issues in build scripts in oxygine but if will have enough time I will fix that.