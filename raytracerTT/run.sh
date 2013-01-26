#!/bin/bash

g++ Render.cpp -o Render
./Render testreflect.nff test2.ppm
eog test2.ppm
