#!/usr/bin/env python3

import licant

licant.execute("morpheus.g.py")

licant.include("igris")
licant.include("nos")
licant.include("rabbit")
licant.include("ralgo")

modules = ["morpheus",
           "igris",
           "nos",
           "nos.input",
           "nos.inet",
           "rabbit",
           "rabbit.opengl",
           "ralgo"]

licant.cxx_static_and_shared("morpheus_libraries",
                             static_lib="morpheus.a",
                             shared_lib="morpheus.so",
                             mdepends=modules,
                             cxx_flags="-fPIC",
                             ld_flags="-fPIC",
                             cxxstd="c++20",
                             )

licant.cxx_application("app",
                       sources=["morpheus/app/main.cpp"],
                       mdepends=modules,
                       libs=["GLEW", "GL", "glfw", "GLU"],
                       include_paths=["."],
                       cxx_flags="-fPIC",
                       ld_flags="-fPIC",
                       cxxstd="c++20",
                       )


licant.cxx_application("cube",
                       sources=["morpheus/app/cube.cpp"],
                       mdepends=modules,
                       libs=["GLEW", "GL", "glfw", "GLU"],
                       include_paths=["."],
                       cxx_flags="-fPIC",
                       ld_flags="-fPIC",
                       cxxstd="c++20",
                       )


licant.ex("morpheus_libraries")
licant.ex("app")
licant.ex("cube")
