#!/usr/bin/python3
import sys

st = sys.argv[1]
c = st.split(" ")

suma = int(c[0])+int(c[2])+int(c[-1])
print(suma)