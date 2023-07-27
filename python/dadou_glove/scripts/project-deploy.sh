#!/bin/bash

#improve this shit ?
if [[ "$1" != "gl" ]] && [[ "$1" != "gr" ]] && [[ ! "$PROJECT_NAME" ]]; then
  if [[ "$HOSTNAME" != "gloveLeft" ]]; then
    export PROJECT_NAME="gloveLeft"
  elif [[ "$HOSTNAME" != "gloveRight" ]]; then
    export PROJECT_NAME="gloveRight"
  else
    printf "${RED}wrong glove param $1 $HOSTNAME${RED}\n"
    exit 0
  fi
fi

if [ "$1" == "gl" ]; then
  export PROJECT_NAME="gloveLeft"
  export USER_HOST="$1"
  export ROOT_HOST="$1r"
elif [ "$1" == "gr" ]; then
  export PROJECT_NAME="gloveRight"
  export USER_HOST="$1"
  export ROOT_HOST="$1r"
fi

#printf "${RED}deploy $PROJECT_NAME ${RED}\n"
#exit 0

####################### PROJECT PARAMS #############################

export PROJECT_PATH=~/Nextcloud/Didier/python/dadou_glove/


export RPI_HOST_NAME="$PROJECT_NAME.local"

export INSTALL_LIB="yes"
#export SET_USB_AUDIO="yes"
export ACTIVATE_I2C="yes"
export SET_BASHRC="yes"
export SET_VIMRC="yes"
export INSTALL_SERVICE="yes"
#export INSTALL_AUTOSTART="yes"
export SERVICE_NAME="$PROJECT_NAME"
export LOG_FILE="disk.log"

export PROJECT_SYSTEM_LIB=
export PROJECT_PYTHON_LIB="adafruit-blinka-displayio adafruit-circuitpython-bitmap_font adafruit-circuitpython-display-text adafruit-circuitpython-matrixkeypad Adafruit-SSD1306 adafruit-circuitpython-displayio-ssd1306 adafruit-circuitpython-rgb-display adafruit-circuitpython-ssd1322 fonts-dejavu python3-pil"

####################################################################

export RPI_HOME=/home/didier
export LOCAL_HOME=~

export UTILS_PROJECT=$LOCAL_HOME/Nextcloud/Didier/python/dadou_utils
export UTILS_SCRIPTS=$UTILS_PROJECT/scripts/deploy

declare -A PROJECT_DEPENDENCIES
PROJECT_DEPENDENCIES[0]=$LOCAL_HOME/Nextcloud/Didier/python/dadou_robot/dadourobot
PROJECT_DEPENDENCIES[1]=$UTILS_PROJECT
export PROJECT_DEPENDENCIES
#printf "$PROJECT_DEPENDENCIES[@]"

if [[ "$1" == "read_param" ]]; then
  printf "${CYAN}Only read param${CYAN}\n"
  if [ -d "$RPI_SCRIPTS" ]; then
    printf "$RPI_SCRIPTS/params.sh\n"
  fi
  if [ -d "$UTILS_SCRIPTS" ]; then
    printf "$UTILS_SCRIPTS/params.sh\n"
  fi
else
  source $UTILS_SCRIPTS/deploy-utils.sh "$2"
fi
