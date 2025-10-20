from pathlib import Path
import re

# -----------------------------------------------------

licPath = "license"
srcPath = ["../src", "../"]
eraseOnly = False

# -----------------------------------------------------

def checkDir(path: str, lic: str):
    print(f"checkDir: {path}")

    p = Path(path)
    for file in p.rglob("*"):

        if file.is_symlink():
            continue

        if file.suffix in {".c", ".cpp", ".hpp", ".hxx", ".h", ".qml"}:

            with open(f"{file}", "r") as srcFile:
                srcData = srcFile.read()
                reg = '^(\n)*/\*(.|\n)*?\*/(\n)*'
                match = re.search(reg, srcData)
                if match:
                    srcData = srcData.replace(match.group(), '')
            
            with open(f"{file}", "w") as editFile:
                if eraseOnly == True:
                    print(f"Removing an existing license in {file}")
                    editFile.write(srcData)
                else:
                    print(f"Writing license data to {file}")
                    editFile.write(lic)
                    editFile.write(srcData)

# -----------------------------------------------------

def main():
    print("Checking source files ...")

    with open(f"{licPath}", "r") as licFile:
        lic = licFile.read()

    for dir in srcPath:
        checkDir(dir, lic)

    print("Finished")

# -----------------------------------------------------


main()









