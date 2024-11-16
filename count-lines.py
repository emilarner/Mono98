import os

lines = 0

def recurse(dir: str):
    global lines

    for file in os.listdir(dir):
        if file in [".", ".."]:
            continue

        if not os.path.isfile(f"{dir}/{file}"):
            recurse(f"{dir}/{file}")
        else:
            if file.split(".")[-1] not in ["ld", "c", "s", "h"]:
                continue

            with open(f"{dir}/{file}", "rb") as fp:
                print(file)
                lines += len(fp.readlines())

recurse(".")
print(lines)

