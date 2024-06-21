#!/bin/bash

# Compilation du projet
make
if [ $? -eq 0 ]; then
    # lauch the app
    ./opengl_app
else
    echo "Echec de la compilation"
fi