import sys
import os

if len(sys.argv) < 2:
    print "No files specified."
    sys.exit(0)

counter = 1

for key in range(len(sys.argv)):
    if key == 0:
        continue

    path = sys.argv[key]

    if not os.path.exists(path):
        print "NOT FOUND: " + path
        continue

    dirname = os.path.dirname(path)
    filename = os.path.basename(path)
    nameparts = filename.split(".", 1)

    while True:
        new_filename = str(counter)
        if len(nameparts) == 2:
            new_filename += "." + nameparts[1]

        new_path = os.path.join(dirname, new_filename)

        if os.path.exists(new_path):
            counter += 1
            continue
        else:
            break

    try:
        os.rename(path, new_path)
        status = "RENAMED: "
    except:
        status = "RENAME FAILED: "

    print status + path + " => " + new_path

    counter += 1


