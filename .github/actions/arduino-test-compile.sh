#!/bin/bash

# arduino-test-compile.sh
# Bash script to do a test-compile of one or more Arduino programs in a repository each with different compile parameters.
#
# Copyright (C) 2020-2022  Armin Joachimsmeyer
# https://github.com/ArminJo/Github-Actions
# License: MIT
#

# Input parameter, which is normally not used for Githup actions
CLI_VERSION="$1"
SKETCH_NAMES="$2"
SKETCH_NAMES_FIND_START="$3"
ARDUINO_BOARD_FQBN="$4"
ARDUINO_PLATFORM="$5"
PLATFORM_DEFAULT_URL="$6"
PLATFORM_URL="$7"
REQUIRED_LIBRARIES="$8"
SKETCHES_EXCLUDE="$9"
EXAMPLES_EXCLUDE="${10}"
BUILD_PROPERTIES="${11}"
EXAMPLES_BUILD_PROPERTIES="${12}"
EXTRA_ARDUINO_CLI_ARGS="${13}"
EXTRA_ARDUINO_LIB_INSTALL_ARGS="${14}"
SET_BUILD_PATH="${15}"
DEBUG_COMPILE="${16}"
DEBUG_INSTALL="${17}"

readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly BLUE='\033[0;34m'

#
# Get env parameter from action run with higher priority, which enables the script to run directly in a step
#
if [[ -n $ENV_CLI_VERSION ]]; then CLI_VERSION=$ENV_CLI_VERSION; fi
if [[ -n $ENV_SKETCH_NAMES ]]; then SKETCH_NAMES=$ENV_SKETCH_NAMES; fi
if [[ -n $ENV_SKETCH_NAMES_FIND_START ]]; then SKETCH_NAMES_FIND_START=$ENV_SKETCH_NAMES_FIND_START; fi
if [[ -n $ENV_ARDUINO_BOARD_FQBN ]]; then ARDUINO_BOARD_FQBN=$ENV_ARDUINO_BOARD_FQBN; fi
if [[ -n $ENV_ARDUINO_PLATFORM ]]; then ARDUINO_PLATFORM=$ENV_ARDUINO_PLATFORM; fi
if [[ -n $ENV_PLATFORM_DEFAULT_URL ]]; then PLATFORM_DEFAULT_URL=$ENV_PLATFORM_DEFAULT_URL; fi
if [[ -n $ENV_PLATFORM_URL ]]; then PLATFORM_URL=$ENV_PLATFORM_URL; fi
if [[ -n $ENV_REQUIRED_LIBRARIES ]]; then REQUIRED_LIBRARIES=$ENV_REQUIRED_LIBRARIES; fi
if [[ -n $ENV_SKETCHES_EXCLUDE ]]; then SKETCHES_EXCLUDE=$ENV_SKETCHES_EXCLUDE; fi
if [[ -n $ENV_EXAMPLES_EXCLUDE ]]; then EXAMPLES_EXCLUDE=$ENV_EXAMPLES_EXCLUDE; fi #deprecated
if [[ -n $ENV_BUILD_PROPERTIES ]]; then BUILD_PROPERTIES=$ENV_BUILD_PROPERTIES; fi
if [[ -n $ENV_EXAMPLES_BUILD_PROPERTIES ]]; then EXAMPLES_BUILD_PROPERTIES=$ENV_EXAMPLES_BUILD_PROPERTIES; fi #deprecated
if [[ -n $ENV_EXTRA_ARDUINO_CLI_ARGS ]]; then EXTRA_ARDUINO_CLI_ARGS=$ENV_EXTRA_ARDUINO_CLI_ARGS; fi
if [[ -n $ENV_EXTRA_ARDUINO_LIB_INSTALL_ARGS ]]; then EXTRA_ARDUINO_LIB_INSTALL_ARGS=$ENV_EXTRA_ARDUINO_LIB_INSTALL_ARGS; fi
if [[ -n $ENV_SET_BUILD_PATH ]]; then SET_BUILD_PATH=$ENV_SET_BUILD_PATH; fi

if [[ -n $ENV_DEBUG_COMPILE ]]; then DEBUG_COMPILE=$ENV_DEBUG_COMPILE; fi
if [[ -n $ENV_DEBUG_INSTALL ]]; then DEBUG_INSTALL=$ENV_DEBUG_INSTALL; fi

#
# Handle deprecated names
#
if [[ -z $SKETCHES_EXCLUDE && -n $EXAMPLES_EXCLUDE ]]; then
  echo "Please change parameter name from \"examples-exclude\" to \"sketches-exclude\""
  SKETCHES_EXCLUDE=${EXAMPLES_EXCLUDE}
fi
if [[ -z $BUILD_PROPERTIES && -n $EXAMPLES_BUILD_PROPERTIES ]]; then
  echo "Please change parameter name from \"examples-build-properties\" to \"build-properties\""
  BUILD_PROPERTIES=${EXAMPLES_BUILD_PROPERTIES}
fi

#
# Enforce defaults. Required at least for script version. !!! MUST be equal the defaults in action.yml !!!
#
echo -e "\r\n${YELLOW}Set defaults"
if [[ -z $ARDUINO_BOARD_FQBN ]]; then
  echo "Set ARDUINO_BOARD_FQBN to default value: \"arduino:avr:uno\""
  ARDUINO_BOARD_FQBN='arduino:avr:uno'
fi
if [[ -z $PLATFORM_URL && -n $PLATFORM_DEFAULT_URL ]]; then
  echo -e "Set PLATFORM_URL to default value: \"${PLATFORM_DEFAULT_URL}\""
  PLATFORM_URL=$PLATFORM_DEFAULT_URL
fi
if [[ -z $CLI_VERSION ]]; then
  echo "Set CLI_VERSION to default value: \"latest\""
  CLI_VERSION='latest'
fi
if [[ -z $SKETCH_NAMES ]]; then
  echo -e "Set SKETCH_NAMES to default value: \"*.ino\""
  SKETCH_NAMES='*.ino'
fi
if [[ -z $SKETCH_NAMES_FIND_START ]]; then
  echo -e "Set SKETCH_NAMES_FIND_START to default value: \".\" (root of repository)"
  SKETCH_NAMES_FIND_START='.'
fi
if [[ -z $SET_BUILD_PATH ]]; then
  echo -e "Set SET_BUILD_PATH to default value: \"false\""
  SET_BUILD_PATH='false'
fi

#
# Echo input parameter
#
echo -e "\r\n${YELLOW}Echo input parameter"
echo CLI_VERSION=$CLI_VERSION
echo SKETCH_NAMES=$SKETCH_NAMES
echo SKETCH_NAMES_FIND_START=$SKETCH_NAMES_FIND_START
echo ARDUINO_BOARD_FQBN=$ARDUINO_BOARD_FQBN
echo ARDUINO_PLATFORM=$ARDUINO_PLATFORM
echo PLATFORM_DEFAULT_URL=$PLATFORM_DEFAULT_URL
echo PLATFORM_URL=$PLATFORM_URL
echo REQUIRED_LIBRARIES=$REQUIRED_LIBRARIES
echo SKETCHES_EXCLUDE=$SKETCHES_EXCLUDE
echo BUILD_PROPERTIES=$BUILD_PROPERTIES
echo EXTRA_ARDUINO_CLI_ARGS=$EXTRA_ARDUINO_CLI_ARGS
echo EXTRA_ARDUINO_LIB_INSTALL_ARGS=$EXTRA_ARDUINO_LIB_INSTALL_ARGS
echo SET_BUILD_PATH=$SET_BUILD_PATH

echo DEBUG_COMPILE=$DEBUG_COMPILE
echo DEBUG_INSTALL=$DEBUG_INSTALL

VERBOSE_PARAMETER=
if [[ $DEBUG_INSTALL == true ]]; then
  VERBOSE_PARAMETER=--verbose
  echo
  echo HOME=$HOME                         # /home/runner
  echo PWD=$PWD                           # ***
  echo GITHUB_WORKSPACE=$GITHUB_WORKSPACE # /home/runner/work/<repoName>/<repoName>
#set
#ls -lR $GITHUB_WORKSPACE
fi

# Show calling parameters
declare -p BASH_ARGV

#
# Download and install arduino IDE, if not already cached
#
echo -n -e "\r\n${YELLOW}arduino-cli "
if [[ -f $HOME/arduino_ide/arduino-cli ]]; then
  echo -e "cached: ${GREEN}\xe2\x9c\x93" # never seen :-(
else
  echo -n "downloading: "
  wget --quiet https://downloads.arduino.cc/arduino-cli/arduino-cli_${CLI_VERSION}_Linux_64bit.tar.gz
  if [[ $? -ne 0 ]]; then
    echo -e "${RED}\xe2\x9c\x96"
    echo "::error:: Unable to download arduino-cli_${CLI_VERSION}_Linux_64bit.tar.gz"
    exit 3
  else
    echo -e "${GREEN}\xe2\x9c\x93"
  fi
  echo -n "Upacking arduino-cli to ${HOME}/arduino_ide:  "
  if [[ ! -d $HOME/arduino_ide/ ]]; then
    mkdir $HOME/arduino_ide
  fi
  tar xf arduino-cli_${CLI_VERSION}_Linux_64bit.tar.gz -C $HOME/arduino_ide/
  if [[ $? -ne 0 ]]; then
    echo -e "${RED}\xe2\x9c\x96"
  else
    echo -e "${GREEN}\xe2\x9c\x93"
  fi
#  ls -l $HOME/arduino_ide/* # LICENSE.txt + arduino-cli
#  ls -l $HOME # only arduino_ide
fi

# add the arduino CLI to our PATH
export PATH="$HOME/arduino_ide:$PATH"

#print version
arduino-cli version

#
# Add *Custom* directories to Arduino library directory
#
# if ls $GITHUB_WORKSPACE/*Custom* >/dev/null 2>&1; then
#   echo -e "\r\n${YELLOW}Add *Custom* as Arduino library"
#   mkdir --parents $HOME/Arduino/libraries
#   rm --force --recursive $GITHUB_WORKSPACE/*Custom*/.git # do not want to move the whole .git directory
#   # mv to avoid the library examples to be test compiled
#   echo mv --no-clobber $VERBOSE_PARAMETER $GITHUB_WORKSPACE/\*Custom\* $HOME/Arduino/libraries/
#   mv --no-clobber $VERBOSE_PARAMETER $GITHUB_WORKSPACE/*Custom* $HOME/Arduino/libraries/
# fi

echo "Download Depends Arduino library"
mkdir --parents $HOME/Arduino/libraries

if [[ -f $GITHUB_WORKSPACE/library.properties ]]; then
  OLD_IFS="$IFS"
  IFS=$'\n'
  for line in $(cat $GITHUB_WORKSPACE/library.properties); do
    result=$(echo $line | grep "depends=")
    if [[ "$result" != "" ]]; then
      depends_str=${line##*depends=}
      IFS=","
      for lib in ${depends_str[@]}; do
        echo "download $lib"
        arduino-cli lib install $lib
      done
    fi
  done
  IFS="$OLD_IFS"
fi

echo "Check Arduino library"
repo_name="${GITHUB_WORKSPACE##*/}"
if [[ -d $HOME/Arduino/libraries/$repo_name ]]; then
  rm -r $HOME/Arduino/libraries/$repo_name
  echo "Same Arduino library: ${repo_name}, replace this repository"
fi

cp -r $GITHUB_WORKSPACE $HOME/Arduino/libraries/
ls $HOME/Arduino/libraries/$repo_name

# Link this repository as Arduino library
#
# Check if repo is an Arduino library
# if [[ -f $GITHUB_WORKSPACE/library.properties ]]; then
#   echo -e "\r\n${YELLOW}Link this repository as Arduino library"
#   mkdir --parents $HOME/Arduino/libraries
#   repo_name="${GITHUB_WORKSPACE##*/}"
#   if [[ -d $HOME/Arduino/libraries/$repo_name ]]; then
#     rm -r $HOME/Arduino/libraries/$repo_name
#     echo -e "\r\nSame Arduino library: ${repo_name}, replace this repository"
#   fi
#   ln --symbolic $GITHUB_WORKSPACE $HOME/Arduino/libraries/.
#   if [[ $DEBUG_INSTALL == true ]]; then
#     echo ln --symbolic $GITHUB_WORKSPACE $HOME/Arduino/libraries/.
#     rm --force --recursive $HOME/Arduino/libraries/*/.git # do not want to list the whole .git directory
#     echo ls -l --dereference --recursive --all $HOME/Arduino/libraries/
#     ls -l --dereference --recursive --all $HOME/Arduino/libraries/
#   fi
# fi

#
# Update index and install the required board platform
#
echo -e "\r\n${YELLOW}Update index and install the required board platform"
if [[ -z $ARDUINO_PLATFORM ]]; then
  # ARDUINO_PLATFORM is empty -> derive platform from the 2 first elements of the arduino-board-fqbn
  remainder=${ARDUINO_BOARD_FQBN#*:}
  provider=${ARDUINO_BOARD_FQBN%%:*}
  PLATFORM=${provider}:${remainder%%:*}
else
  # Remove @latest from specified platform
  PLATFORM=${ARDUINO_PLATFORM%@latest}
fi
echo PLATFORM=${PLATFORM} # e.g. digistump:avr
if [[ ${PLATFORM} != *arduino* && -z $PLATFORM_URL ]]; then
  # check if the requested platform is manually installed
  if [[ ! -d $HOME/.arduino15/packages/${provider} ]]; then
    echo -e "::error::Non Arduino platform $PLATFORM requested, but \"platform-url\" parameter is missing."
    exit 1
  fi
fi

if [[ -n $PLATFORM_URL ]]; then
  PLATFORM_URL=${PLATFORM_URL// /,} # replace space by comma to enable multiple urls which are space separated
  PLATFORM_URL_COMMAND="--additional-urls"
fi

PLATFORM=${PLATFORM//,/ } # replace all comma by space to enable multiple platforms which are comma separated
declare -a PLATFORM_ARRAY=($PLATFORM)
if [[ $DEBUG_INSTALL == true ]]; then
  declare -p PLATFORM_ARRAY # print properties of PLATFORM_ARRAY
fi
for single_platform in "${PLATFORM_ARRAY[@]}"; do # Loop over all platforms specified
  if [[ $DEBUG_INSTALL == true ]]; then
    echo -e "arduino-cli core update-index $PLATFORM_URL_COMMAND $PLATFORM_URL --verbose"
    arduino-cli core update-index $PLATFORM_URL_COMMAND $PLATFORM_URL --verbose # must specify --additional-urls here
    if [[ $? -ne 0 ]]; then
      echo "::error::Updating index of $PLATFORM_URL failed"
      exit 1
    fi
    echo -e "arduino-cli core install $single_platform $PLATFORM_URL_COMMAND $PLATFORM_URL -v"
    arduino-cli core install $single_platform $PLATFORM_URL_COMMAND $PLATFORM_URL --verbose
  else
    echo -e "arduino-cli core update-index $PLATFORM_URL_COMMAND $PLATFORM_URL > /dev/null"
    arduino-cli core update-index $PLATFORM_URL_COMMAND $PLATFORM_URL >/dev/null # must specify --additional-urls here
    if [[ $? -ne 0 ]]; then
      echo "::error::Updating index of $PLATFORM_URL failed"
      exit 1
    fi
    echo -e "arduino-cli core install $single_platform $PLATFORM_URL_COMMAND $PLATFORM_URL > /dev/null"
    arduino-cli core install $single_platform $PLATFORM_URL_COMMAND $PLATFORM_URL >/dev/null
  fi
  if [[ $? -ne 0 ]]; then
    echo "::error::Install core for $single_platform failed"
    exit 1
  fi
done

#
# Special esp8266 and esp32 platform handling
#
echo -e "\r\n${YELLOW}Special esp8266 and esp32 platform handling"
if [[ ${PLATFORM} == esp8266:esp8266 && ! -f /usr/bin/python3 ]]; then
  # python3 is a link in the esp8266 package: /github/home/.arduino15/packages/esp8266/tools/python3/3.7.2-post1/python3 -> /usr/bin/python3
  echo -e "\r\n${YELLOW}install python3 for ESP8266"
  apt-get install -qq python3 >/dev/null
fi

if [[ $PLATFORM == esp32:esp32 ]]; then
  if [[ ! -f /usr/bin/pip && ! -f /usr/bin/python ]]; then
    echo -e "\r\n${YELLOW}install python and pip for ESP32"
    # Here we would get the warning: The directory '/github/home/.cache/pip/http' or its parent directory is not owned by the current user and the cache has been disabled.
    #                                Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.
    apt-get install -qq python-pip >/dev/null 2>&1 # this installs also python
  fi
  pip install pyserial
fi

#
# List installed boards with their FQBN
#
echo -e "\r\n${YELLOW}List installed boards with their FQBN"
if [[ $DEBUG_INSTALL == true ]]; then
  echo arduino-cli board listall --verbose
  arduino-cli board listall --verbose
else
  echo arduino-cli board listall
  arduino-cli board listall
fi

#
# Install required libraries
#
echo -e "\n${YELLOW}Install required libraries"
if [[ -z $REQUIRED_LIBRARIES ]]; then
  echo No additional libraries to install
else
  echo Install libraries $REQUIRED_LIBRARIES
  BACKUP_IFS="$IFS"
  # Split comma separated library list
  IFS=$','
  declare -a REQUIRED_LIBRARIES_ARRAY=($REQUIRED_LIBRARIES)
  IFS="$BACKUP_IFS"
  if [[ $DEBUG_INSTALL == true ]]; then
    arduino-cli lib install "${REQUIRED_LIBRARIES_ARRAY[@]}" $EXTRA_ARDUINO_LIB_INSTALL_ARGS
  else
    arduino-cli lib install "${REQUIRED_LIBRARIES_ARRAY[@]}" $EXTRA_ARDUINO_LIB_INSTALL_ARGS >/dev/null 2>&1
  fi
  if [[ $? -ne 0 ]]; then
    echo "::error::Installation of $REQUIRED_LIBRARIES failed"
    exit 1
  fi
fi

# Save generated files to the build subfolder of the sketch
export ARDUINO_SKETCH_ALWAYS_EXPORT_BINARIES=true

#
# Get the build property map
#
echo -e "\n${YELLOW}Compiling sketches / examples for board $ARDUINO_BOARD_FQBN\n"

# If matrix.build-properties are specified, create an associative shell array
# Input example: { "WhistleSwitch": "-DINFO -DFREQUENCY_RANGE_LOW", "SimpleFrequencyDetector": "-DINFO" }
# Converted to: [All]="-DDEBUG -DINFO" [WhistleSwitch]="-DDEBUG -DINFO"
if [[ -n $BUILD_PROPERTIES && $BUILD_PROPERTIES != "null" ]]; then # contains "null", if passed as environment variable
  echo BUILD_PROPERTIES=$BUILD_PROPERTIES
  BUILD_PROPERTIES=${BUILD_PROPERTIES#\{} # remove the "{". The "}" is required as end token
  # (\w*): * first token before the colon and spaces, ([^,}]*) token after colon and spaces up to "," or "}", [,}] trailing characters
  if [[ $DEBUG_COMPILE == true ]]; then
    echo BUILD_PROPERTIES is converted to:$(echo $BUILD_PROPERTIES | sed -E 's/"(\w*)": *([^,}]*)[,}]/\[\1\]=\2/g')
  fi
  declare -A PROP_MAP="( $(echo $BUILD_PROPERTIES | sed -E 's/"(\w*)": *([^,}]*)[,}]/\[\1\]=\2/g') )"
  declare -p PROP_MAP # print properties of PROP_MAP
else
  declare -A PROP_MAP=([dummy]=dummy) # declare an accociative array
fi

#
# Finally, we compile all examples
#
# Split comma separated sketch name list
BACKUP_IFS="$IFS"
IFS=$','
SKETCH_NAMES=${SKETCH_NAMES// /}              # replace all spaces
GLOBIGNORE=*:?:[                              # Disable filename expansion (globbing) of *.ino to abc.ino if abc.ino is a file in the directory
declare -a SKETCH_NAMES_ARRAY=($SKETCH_NAMES) # declare an indexed array
GLOBIGNORE=                                   # Enable it for cp command below
if [[ $DEBUG_COMPILE == true ]]; then
  declare -p SKETCH_NAMES_ARRAY # print properties of SKETCH_NAMES_ARRAY
fi
IFS="$BACKUP_IFS"
COMPILED_SKETCHES=
for sketch_name in "${SKETCH_NAMES_ARRAY[@]}"; do # Loop over all sketch names
  if [[ $SET_BUILD_PATH == true ]]; then
    # must use $GITHUB_WORKSPACE/$SKETCH_NAMES_FIND_START, since arduino-cli does not support relative path for --build-path
    declare -a SKETCHES=($(find ${GITHUB_WORKSPACE}/${SKETCH_NAMES_FIND_START} -type f -name "$sketch_name")) # only search for files
  else
    declare -a SKETCHES=($(find ${SKETCH_NAMES_FIND_START} -type f -name "$sketch_name")) # only search for files
  fi
  if [[ $DEBUG_COMPILE == true ]]; then
    declare -p SKETCHES
  fi

  # Check if find command found a file
  if [[ -z ${SKETCHES[0]} ]]; then
    GLOBIGNORE=*:?:[ # Disable filename expansion (globbing) of *.ino to abc.ino if abc.ino is a file in the directory
    echo -e "::error::\nNo files found to compile with sketch_name=${sketch_name}."
    echo "sketch-names=${SKETCH_NAMES} and sketch-names-find-start=${SKETCH_NAMES_FIND_START}"
    GLOBIGNORE=
    # No files found -> list start directory and execute find command to see what we did
    echo -e "find command is: find ${GITHUB_WORKSPACE}/${SKETCH_NAMES_FIND_START} -type f -name \"$sketch_name\""
    echo "\"sketch-names-find-start\" directory content listing with: ls -l ${GITHUB_WORKSPACE}/${SKETCH_NAMES_FIND_START}"
    ls -l ${GITHUB_WORKSPACE}/${SKETCH_NAMES_FIND_START}
    echo
  fi

  for sketch in "${SKETCHES[@]}"; do                       # Loop over all sketch files
    SKETCH_PATH=$(dirname $sketch)                         # complete path to sketch
    SKETCH_DIR=${SKETCH_PATH##*/}                          # directory of sketch, must match sketch basename
    SKETCH_FILENAME=$(basename $sketch)                    # complete name of sketch
    SKETCH_EXTENSION=${SKETCH_FILENAME##*.}                # extension of sketch
    SKETCH_BASENAME=${SKETCH_FILENAME%%.$SKETCH_EXTENSION} # name without extension / basename of sketch, must match directory name
    if [[ $DEBUG_COMPILE == true ]]; then
      echo -n "Process $sketch with filename $SKETCH_FILENAME and extension $SKETCH_EXTENSION"
    fi
    echo -e "\n"
    if [[ $SKETCHES_EXCLUDE == *"$SKETCH_BASENAME"* ]]; then
      echo -e "Skipping $SKETCH_PATH \xe2\x9e\x9e" # Right arrow
    else
      # If sketch name does not end with .ino, rename it locally
      if [[ $SKETCH_EXTENSION != ino ]]; then
        echo "Rename ${SKETCH_PATH}/${SKETCH_FILENAME} to ${SKETCH_PATH}/${SKETCH_BASENAME}.ino"
        mv ${SKETCH_PATH}/${SKETCH_FILENAME} ${SKETCH_PATH}/${SKETCH_BASENAME}.ino
      fi
      # If directory name does not match sketch name, create an appropriate directory, copy the files recursively and compile
      if [[ $SKETCH_DIR != $SKETCH_BASENAME ]]; then
        mkdir $HOME/$SKETCH_BASENAME
        echo "Creating directory $HOME/$SKETCH_BASENAME and copy ${SKETCH_PATH}/* to it"
        cp --recursive ${SKETCH_PATH}/* $HOME/$SKETCH_BASENAME
        SKETCH_PATH=$HOME/$SKETCH_BASENAME
      fi
      if [[ $SET_BUILD_PATH == true ]]; then
        BUILD_PATH_PARAMETER="--build-path $SKETCH_PATH/build/"
      fi
      # Check if there is an entry in the associative array and create compile parameter to put in compiler.*.extra_flags
      # This flags are also defined in platform.txt as empty, to be overwritten by a platform.local.txt definition.
      # But I never saw a distribution using this fature, so we can go on here :-)
      echo -n "Compiling $SKETCH_BASENAME "
      if [[ -n ${PROP_MAP[$SKETCH_BASENAME]} ]]; then
        GCC_EXTRA_FLAGS=${PROP_MAP[$SKETCH_BASENAME]}
        echo -n "with $GCC_EXTRA_FLAGS "
      elif [[ -n ${PROP_MAP[All]} ]]; then
        GCC_EXTRA_FLAGS=${PROP_MAP[All]}
        echo -n "with $GCC_EXTRA_FLAGS "
      else
        GCC_EXTRA_FLAGS=
      fi
      if [[ -z $GCC_EXTRA_FLAGS ]]; then
        build_stdout=$(arduino-cli compile -e --verbose --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH 2>&1)
        # build_stdout=$(arduino-cli compile -e --verbose --warnings all --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH 2>&1)
        # build_stdout=$(arduino-cli compile -e --log-level error --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH 2>&1)
      else
        build_stdout=$(arduino-cli compile -e --verbose --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags="${GCC_EXTRA_FLAGS}" --build-property compiler.c.extra_flags="${GCC_EXTRA_FLAGS}" --build-property compiler.S.extra_flags="${GCC_EXTRA_FLAGS}" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH 2>&1)
        # build_stdout=$(arduino-cli compile -e --verbose --warnings all --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags="${GCC_EXTRA_FLAGS}" --build-property compiler.c.extra_flags="${GCC_EXTRA_FLAGS}" --build-property compiler.S.extra_flags="${GCC_EXTRA_FLAGS}" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH 2>&1)
        # build_stdout=$(arduino-cli compile -e --log-level error --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags="${GCC_EXTRA_FLAGS}" --build-property compiler.c.extra_flags="${GCC_EXTRA_FLAGS}" --build-property compiler.S.extra_flags="${GCC_EXTRA_FLAGS}" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH 2>&1)
      fi
      if [[ $? -ne 0 ]]; then
        echo -e ""${RED}"\xe2\x9c\x96" # If ok output a green checkmark else a red X and the command output.
        if [[ -z $GCC_EXTRA_FLAGS ]]; then
          echo "arduino-cli compile -e --verbose --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --verbose --warnings all --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --log-level error --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
        else
          echo "arduino-cli compile -e --verbose --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.c.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.S.extra_flags=\"${GCC_EXTRA_FLAGS}\" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --verbose --warnings all --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.c.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.S.extra_flags=\"${GCC_EXTRA_FLAGS}\" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --log-level error --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.c.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.S.extra_flags=\"${GCC_EXTRA_FLAGS}\" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
        fi
        echo "::error::Compile of $SKETCH_BASENAME ${GCC_EXTRA_FLAGS} failed"
        echo -e "$build_stdout \n"
        # repeat the info after hundred lines of output :-)
        echo "Compile of $SKETCH_BASENAME ${GCC_EXTRA_FLAGS} failed"
        exit_code=1
      else
        echo -e "${GREEN}\xe2\x9c\x93"
        if [[ -z $GCC_EXTRA_FLAGS ]]; then
          echo "arduino-cli compile -e --verbose --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --verbose --warnings all --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --log-level error --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
        else
          echo "arduino-cli compile -e --verbose --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.c.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.S.extra_flags=\"${GCC_EXTRA_FLAGS}\" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --verbose --warnings all --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.c.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.S.extra_flags=\"${GCC_EXTRA_FLAGS}\" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
          # echo "arduino-cli compile -e --log-level error --fqbn ${ARDUINO_BOARD_FQBN%|*} $BUILD_PATH_PARAMETER --build-property compiler.cpp.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.c.extra_flags=\"${GCC_EXTRA_FLAGS}\" --build-property compiler.S.extra_flags=\"${GCC_EXTRA_FLAGS}\" $EXTRA_ARDUINO_CLI_ARGS $SKETCH_PATH"
        fi
        if [[ $DEBUG_COMPILE == true || $SET_BUILD_PATH == true ]]; then
          echo "Debug mode enabled => compile output will be printed also for successful compilation and sketch directory is listed after compilation"
          echo -e "$build_stdout \n"
          echo -e "\nls -l --recursive $SKETCH_PATH/build/"
          ls -l --recursive $SKETCH_PATH/build/
          echo -e "\r\n"
        fi
      fi
      COMPILED_SKETCHES="$COMPILED_SKETCHES $SKETCH_NAME"
    fi
  done
done
if [ -z "$COMPILED_SKETCHES" ]; then
  echo "::error::Did not find any sketches to compile, probably misconfigured or used checkout twice without \"path:\" parameter?"
  exit_code=2
fi

echo "compiled all example, export bin file..."
pwd
find . -name "*.bin"

exit $exit_code
