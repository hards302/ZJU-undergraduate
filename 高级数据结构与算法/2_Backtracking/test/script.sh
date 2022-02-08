#!/bin/bash
diff <(./safefruit.exe < $1) <(./safefruit_shh.exe < $1)
# diff <(./safefruit.exe < $1) <(./safefruit_hjc.exe < $1)

