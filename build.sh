#!/bin/bash
set -x

cc -Wall -ggdb -pthread -o server *.c
