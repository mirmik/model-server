#!/usr/bin/env/python3

import licant

licant.module("morpheus",
    sources = ["morpheus/*.cpp"],
    include_paths = ["."],
)