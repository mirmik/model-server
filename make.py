#!/usr/bin/env python3

import licant

licant.execute("morpheus.g.py")

licant.include("igris")
licant.include("nos")
licant.include("rabbit")

licant.cxx_static_and_shared("morpheus_libraries",
                             static_lib="morpheus.a",
                             shared_lib="morpheus.so",
                             mdepends=[
                                 "morpheus",
                                 "igris",
                                 "nos",
                                 "nos.input",
                                 "nos.inet",
                                 "rabbit",
                                 "rabbit.opengl",
                             ],
                             cxx_flags="-fPIC",
                             ld_flags="-fPIC"
                             )

licant.cxx_application("app",
                       sources=["morpheus/app/main.cpp"],
                       mdepends=[
                           "morpheus",
                           "igris",
                           "nos",
                           "nos.input",
                           "nos.inet",
                           "rabbit",
                           "rabbit.opengl",
                       ],
                       libs=["GLEW", "GL", "glfw", "GLU"],
                       include_paths=["."],
                       cxx_flags="-fPIC",
                       ld_flags="-fPIC"
                       )

licant.ex("morpheus_libraries")
licant.ex("app")
