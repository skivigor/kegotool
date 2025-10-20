from pathlib import Path
import shutil
import os
import re
import subprocess
import sys
import datetime


DEPLOY_PATH = Path.cwd().as_posix()
BUILD_PATH = DEPLOY_PATH + "/build"
RELEASE_DIR_PATH = DEPLOY_PATH + "/release"
# APP_PATH = BUILD_PATH + "/app"
INSTALLER_PATH = BUILD_PATH + "/installer"
TEMPLATE_PATH = DEPLOY_PATH + "/template"

BUILD_PROJECT_DIRS_PATH = DEPLOY_PATH + "/../../.."
VERSION_FILE= DEPLOY_PATH + "/../../projectconf.h"
QML_PATH = DEPLOY_PATH + "/../../qml"
RESOURCES_DIR_PATH = DEPLOY_PATH + "/../../resources"

PACKAGE = "com.kego."
VERSION = "0"

def modifyFile(path: str, pattern: str, content: str, group: int):
    data = ""
    with open(f"{path}", "r") as f:
        data = f.read()
        # print(f"DATA: {data}")
        match = re.search(pattern, data)
        # print(f"MATCH: {match.group()}")
        if match:
            data = data.replace(match.group(group), content)

    # print(f"MODIFIED: {data}")
    
    with open(f"{path}", "w") as f:
        f.write(data)
    

# -----------------------------------------------------
def deploy(path: str):
    print("\n-----------------------------")
    print(f"Deploy for {path}")

    # Search application
    os.chdir(BUILD_PROJECT_DIRS_PATH + "/" + path)
    apps = [f for f in os.listdir() if re.match('.*\\.exe', f)]
    print(f"Finded application: {apps}")
    if len(apps) == 0:
        print(f"Application not found in folder {path}")
        return
    if len(apps) > 1:
        print(f"Too much applications in folder {path}. Skip!!!")
        return
    fullAppName = apps[0]
    appName = fullAppName.split('.')[0]
    appNameLower = appName.lower()

    # Copy template to target directory
    packageName = PACKAGE + appNameLower
    packagePath = INSTALLER_PATH + "/packages/" + packageName
    shutil.copytree(src = TEMPLATE_PATH + "/config", dst = INSTALLER_PATH + "/config")
    shutil.copytree(src = TEMPLATE_PATH + "/meta", dst = packagePath + "/meta")

    # Create app dir (data) and copy application
    dataPath = packagePath + "/data"
    # Path(dataPath).mkdir(parents = True, exist_ok = True)
    shutil.copytree(src = TEMPLATE_PATH + "/lib", dst = dataPath)
    shutil.copy(fullAppName, dataPath)
    shutil.copy(RESOURCES_DIR_PATH + "/icons/" + appName + ".ico", dataPath)
    
    # shutil.copy(TEMPLATE_PATH + "/lib", dataPath)
    os.chdir(dataPath)

    # TODO delete
    # return

    # Run windeployqt
    print("Run windeployqt ... wait!")
    proc = subprocess.run(["windeployqt.exe", "--qmldir", QML_PATH, fullAppName], capture_output = True, text = True)
    if proc.returncode != 0:
        print("The command failed with return code", proc.returncode)
        print(f"Error windeployqt: {proc.stderr}")
        sys.exit(1)

    os.chdir(INSTALLER_PATH)

    # Modification config.xml
    configFile = INSTALLER_PATH + "/config/config.xml"
    modifyFile(configFile, "KegoTool", appName, 0)
    modifyFile(configFile, "<Version>[0-9.]{3,}", "<Version>" + VERSION, 0)

    # Modification installscript.qs
    installScriptFile = packagePath + "/meta/installscript.qs"
    modifyFile(installScriptFile, "KegoTool", appName, 0)

    # Modification package.xml
    packageFile = packagePath + "/meta/package.xml"
    dt = datetime.datetime.now().strftime("%Y-%m-%d")
    modifyFile(packageFile, "KegoTool", appName, 0)
    modifyFile(packageFile, "<Version>[0-9.]{3,}", "<Version>" + VERSION, 0)
    modifyFile(packageFile, "<ReleaseDate>[0-9-]{10}", "<ReleaseDate>" + dt, 0)

    # Create installer
    instName = appNameLower + "_setup_" + VERSION + "-amd64.exe"
    print("Run binarycreator ... wait!")
    proc = subprocess.run(["binarycreator.exe", "-c", "config/config.xml", "-p", "packages", instName], capture_output = True, text = True)

    if proc.returncode != 0:
        print("The command failed with return code", proc.returncode)
        print(f"Error binarycreator: {proc.stderr}")
        sys.exit(1)

    if os.path.exists(RELEASE_DIR_PATH + "/" + instName):
        os.remove(RELEASE_DIR_PATH + "/" + instName)
    shutil.move(instName, RELEASE_DIR_PATH)
    print(f"Installer created: {instName}")

    # Create portable package
    print("Create portable package ... wait!")
    os.chdir(RELEASE_DIR_PATH)
    zipPath = packagePath + "/zip"
    zipDir = appNameLower + "_" + VERSION
    zipName = zipDir + "-amd64"
    if os.path.exists(RELEASE_DIR_PATH + "/" + zipName):
        os.remove(RELEASE_DIR_PATH + "/" + zipName)
        
    shutil.move(dataPath, zipPath + "/" + zipDir)
    shutil.make_archive(zipName, 'zip', zipPath)
    shutil.move(zipPath + "/" + zipDir, dataPath)
    print(f"Portable package created {zipName}")

# -----------------------------------------------------

def main():
    print("Project deployment ...")

    # Check version file
    varFile = Path(VERSION_FILE)
    if varFile.is_file() == False:
        print(f"Error: version file {VERSION_FILE} not found!")
        sys.exit(1)

    # Get version
    global VERSION
    with open(f"{varFile}", "r") as f:
        srcData = f.read()
        reg = '#define PROJECT_VERSION.*"([a-zA-Z0-9.]{3,})'
        match = re.search(reg, srcData)
        if match:
            VERSION = match.group(1)
    if len(VERSION) < 3:
        print("Error: Version not found!")
        sys.exit(1)

    print(f"Version: {VERSION}")

    # Create release directory
    Path(RELEASE_DIR_PATH).mkdir(parents = True, exist_ok = True)

    # Search applications in projects build dirs
    os.chdir(BUILD_PROJECT_DIRS_PATH)
    dirs = [d for d in os.listdir() if re.match('^build-kego.*Release', d)]
    print(f"Finded build dirs: {dirs}")

    # Deploy
    for d in dirs:
        shutil.rmtree(BUILD_PATH, ignore_errors = True)
        deploy(d)

    print("Deploy finished!")

# -----------------------------------------------------


main()


