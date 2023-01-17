#!/usr/bin/env python3

import licant

licant.execute("morpheus.g.py")

licant.cxx_static_and_shared("morpheus",
    static_lib = "morpheus.a",
    shared_lib = "morpheus.so",
    mdepends = ["morpheus"],
)

licant.ex("morpheus")