# ver.h generator for neonOS

import os

comid = os.popen("git rev-parse HEAD").read()

verfile = open("src/include/ver.h", "r")

content = verfile.read()
verfile.close()

verfile = open("src/include/ver.h", "w")

template = """// 1 2 3 4 5
#ifndef VER_H
#define VER_H

#define GVER 1        // Global Version
#define LVER 2        // Local Version
#define PATCH 3       // Patch Number
#define BUILD 4       // Build
#define COMMIT_ID "5" // Commit ID
#define BRANCH_NAME "master" // branch name

#endif // !VER_H
// This file automatically generated for neonOS"""

vermk_template = """GVER=1
LVER=2
PATCH=3
BUILD=4

all: ;
	echo Dummy all target;

.PHONY: all"""

print("Generating ver.h...")

vals = content.split(' ')
print(vals[1])
print(vals[2])
print(vals[3])
print(vals[4])
print(vals[5])

# vals[4] = str(int(vals[4]) + 1)
vals[5] = comid

template = template.replace("1", vals[1])
template = template.replace("2", vals[2])
template = template.replace("3", vals[3])
template = template.replace("4", vals[4])
template = template.replace("5", vals[5].replace('\n', '')[0:7])

print(template)
print("Writing ver.h...")

verfile.write(template)
verfile.close()

print("Creating ver.mk")
vermkfile = open("src/ver.mk", "w")
vermk_template = vermk_template.replace("1", vals[1])
vermk_template = vermk_template.replace("2", vals[2])
vermk_template = vermk_template.replace("3", vals[3])
vermk_template = vermk_template.replace("4", vals[4])

vermkfile.write(vermk_template)
vermkfile.close()

exit(0)
