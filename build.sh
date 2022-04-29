#!/bin/bash
set -x

cc -ggdb -pthread -o server *.c
