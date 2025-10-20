#!/bin/bash

# TODO delete
# APP_DEF=$(ls Kero*AppImage 2>/dev/null)
# echo $APP_DEF

# if [[ ${#APP_DEF} == 0 ]]
# then
#     echo "Not found file"
# else
#     echo "File OK"
# fi

# exit

# ----------------------------------------------------------------------
DEPLOY_DIR_PATH=$(pwd)
BUILD_DIR_PATH=$DEPLOY_DIR_PATH/build
RELEASE_DIR_PATH=$DEPLOY_DIR_PATH/release
BUILD_PROJECT_DIRS_PATH=$DEPLOY_DIR_PATH/../../..
VERSION_FILE=$DEPLOY_DIR_PATH/../../projectconf.h
QMAKE_PATH=~/Qt/5.15.2/gcc_64/bin/qmake
QML_PATH=$DEPLOY_DIR_PATH/../../qml
RESOURCES_DIR_PATH=$DEPLOY_DIR_PATH/../../resources

# Linuxdeployqt
TOOL_LINUXDEPLOYQT=linuxdeployqt-continuous-x86_64.AppImage
TOOL_LINUXDEPLOYQT_URL=https://github.com/probonopd/linuxdeployqt/releases/download/continuous/$TOOL_LINUXDEPLOYQT

# AppImage
APPIMG_DIR_NAME=appdir
APPIMG_DIR_PATH=$BUILD_DIR_PATH/$APPIMG_DIR_NAME
APPIMG_INSTALL_APP_DIR_PATH=$APPIMG_DIR_PATH/usr/bin

# DEB
DEB_DIR_NAME=deb
DEB_DIR_PATH=$BUILD_DIR_PATH/$DEB_DIR_NAME
DEB_INSTALL_APP_DIR_PATH=$DEB_DIR_PATH/opt/kego
TEMPLATE_DIR_PATH=$DEPLOY_DIR_PATH/template
# ----------------------------------------------------------------------

# Create release directory
if [[ ! -d $RELEASE_DIR_PATH ]]
then
    mkdir $RELEASE_DIR_PATH
fi

# Check args
if [[ $# -gt 0 ]]
then
    if [[ $(echo $1 | cut -c 1) == "/" ]]
    then
        BUILD_PROJECT_DIRS_PATH=$1
    else
        BUILD_PROJECT_DIRS_PATH=$DEPLOY_DIR_PATH/$1
    fi
fi
echo "Use path '$BUILD_PROJECT_DIRS_PATH' for BUILD_PROJECT_DIRS_PATH variable"

# Check version file
if [[ ! -f $VERSION_FILE ]]
then
    echo "Error: version file not found!"
    exit 1
fi

# Get version
VERSION=$(echo $(grep '^#define PROJECT_VERSION' $VERSION_FILE | awk -F'"' '{print $2}'))
echo "Deploy for version $VERSION"
VER_SIZE=${#VERSION}
if [[ $VER_SIZE -lt 3 ]]
then
    echo "Error: Version not found!"
    exit 1
fi

# Create build directories
if [[ ! -d $DEB_DIR_PATH ]]
then
    mkdir -p $DEB_DIR_PATH
fi
if [[ ! -d $APPIMG_DIR_PATH ]]
then
    mkdir -p $APPIMG_DIR_PATH
fi

# Download linuxdeployqt
# https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
cd $BUILD_DIR_PATH
if [[ ! -f $TOOL_LINUXDEPLOYQT ]]
then
    wget -c $TOOL_LINUXDEPLOYQT_URL
    if [[ ! -f $TOOL_LINUXDEPLOYQT ]]
    then
        echo "Error: linuxdeployqt-continuous-x86_64.AppImage not found!"
        exit 1
    fi
    chmod a+x linuxdeployqt-continuous-x86_64.AppImage 
fi

# Search applications in release directories and build deb packages
cd $BUILD_PROJECT_DIRS_PATH
find . -maxdepth 1 -type d -name 'build-kegotool*Release' -print0 | while read -d $'\0' FOLDER
do
    # Search application
    echo "----------------"
    cd $FOLDER
    APP=$(find . -maxdepth 1 -type f -perm /u=x | sed "s/.\///g")
    if [[ -z "$APP" ]]
    then
        echo "Application not found in folder $FOLDER"
        cd $BUILD_PROJECT_DIRS_PATH
        continue
    fi
    echo "Application $APP finded in $FOLDER"

    # Compose application directory name
    APP_DIR_NAME=$(echo "$APP" | awk '{print tolower($0)}')
    APP_DIR_PATH=$DEB_INSTALL_APP_DIR_PATH/$APP_DIR_NAME

    # Delete build data
    rm -r $APPIMG_DIR_PATH/*
    rm -r $DEB_DIR_PATH/*

    # AppImage creation --------------
    if [[ ! -d $APPIMG_DIR_PATH/usr/bin ]]
    then
        mkdir -p $APPIMG_DIR_PATH/usr/bin
    fi
    cp $APP $APPIMG_DIR_PATH/usr/bin/

    cd $RESOURCES_DIR_PATH/icons
    if [[ -f $APP.png && -f $APP.desktop ]]
    then
        cp $APP.png $APPIMG_DIR_PATH/
        cp $APP.desktop $APPIMG_DIR_PATH/
    else
        cp default.png $APPIMG_DIR_PATH/$APP.png
        cp default.desktop $APPIMG_DIR_PATH/$APP.desktop
        sed -i -e "s/default/$APP/g" $APPIMG_DIR_PATH/$APP.desktop
    fi

    cd $BUILD_DIR_PATH
    echo -e "\nCreating AppImage ..."

    # Run linuxdeployqt
    ./$TOOL_LINUXDEPLOYQT $APPIMG_DIR_PATH/usr/bin/$APP -qmake=$QMAKE_PATH -qmldir=$QML_PATH
    # Clear debug files
    find $APPIMG_DIR_PATH -name "*.debug" -delete -print
    # Create AppImage
    ./$TOOL_LINUXDEPLOYQT $APPIMG_DIR_PATH/usr/bin/$APP -qmake=$QMAKE_PATH -qmldir=$QML_PATH -appimage

    # APPIMG_PACKAGE_DEFAULT_NAME=$APP-x86_64.AppImage
    # if [[ ! -f $APPIMG_PACKAGE_DEFAULT_NAME ]]
    # then
    #     echo "Error! AppImage $APPIMG_PACKAGE_DEFAULT_NAME not created!"
    #     cd $BUILD_PROJECT_DIRS_PATH
    #     continue
    # fi

    APPIMG_PACKAGE_DEFAULT_NAME=$(ls $APP*AppImage 2>/dev/null)
    if [[ ${#APPIMG_PACKAGE_DEFAULT_NAME} == 0 ]]
    then
        echo "Error! AppImage not created!"
        cd $BUILD_PROJECT_DIRS_PATH
        continue
    fi

    APPIMG_PACKAGE_NAME=$APP_DIR_NAME\_$VERSION-amd64.AppImage
    mv $APPIMG_PACKAGE_DEFAULT_NAME $RELEASE_DIR_PATH/$APPIMG_PACKAGE_NAME
    echo "Creating AppImage $APPIMG_PACKAGE_NAME ... OK"

    # DEB creation --------------
    cp -r $TEMPLATE_DIR_PATH/DEBIAN $DEB_DIR_PATH

    if [[ ! -d $APP_DIR_PATH ]]
    then
        mkdir -p $APP_DIR_PATH
    fi
    cp -r $RESOURCES_DIR_PATH/icons $APP_DIR_PATH
    cp -r $APPIMG_DIR_PATH/usr/* $APP_DIR_PATH
    cp $TEMPLATE_DIR_PATH/KegoTool.sh $APP_DIR_PATH/$APP.sh
    sed -i -e "s/KegoTool/$APP/g" $APP_DIR_PATH/$APP.sh

    # Modification of DEBIAN files
    sed -i -e "s/KegoTool/$APP/g" $DEB_DIR_PATH/DEBIAN/control
    sed -i -e "s/^Version:.*/Version: $VERSION/g" $DEB_DIR_PATH/DEBIAN/control
    sed -i -e "s/KegoTool/$APP/g" $DEB_DIR_PATH/DEBIAN/prerm
    sed -i -e "s/KegoTool/$APP/g" $DEB_DIR_PATH/DEBIAN/postinst
    sed -i -e "s/kegotool/$APP_DIR_NAME/g" $DEB_DIR_PATH/DEBIAN/postinst

     # Create deb package
    DEB_PACKAGE_NAME=$APP_DIR_NAME\_$VERSION-amd64.deb
    echo -e "\nCreating deb package $DEB_PACKAGE_NAME ..."
    dpkg-deb --build ./$DEB_DIR_NAME
    if [[ ! -f $DEB_DIR_NAME.deb ]]
    then
        echo "Error: deb package $DEB_PACKAGE_NAME not created!"
        cd $BUILD_PROJECT_DIRS_PATH
        continue
    fi
    mv $DEB_DIR_NAME.deb $RELEASE_DIR_PATH/$DEB_PACKAGE_NAME
    echo "Creating deb package $DEB_PACKAGE_NAME ... OK"

    cd $BUILD_PROJECT_DIRS_PATH

    # Delete build data
    # rm -r $APPIMG_DIR_PATH/*
    # rm -r $DEB_DIR_PATH/*

    # Delete application build folder
    # rm -r $BUILD_DEB_DIR_PATH
    
done




