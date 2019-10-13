
route -n
MYEDEVICE=$(ls /sys/class/net/ | xargs | tr -s ' ' '\n' | grep "^e")
sudo ifmetric $MYEDEVICE 800
route -n
