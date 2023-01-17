#!/usr/bin/env/python3

import licant

licant.module("morpheus",
    sources = ["morpheus/ModelServer.cpp"],
    includes = ["."],
)