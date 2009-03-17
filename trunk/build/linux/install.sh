#!/bin/bash

SAMPLE_DIR=/usr/share/elfviz

cp elfviz elf2txt gelfviz /usr/bin
cp libelfio* /usr/lib
mkdir -p $SAMPLE_DIR
cp sample/* $SAMPLE_DIR
