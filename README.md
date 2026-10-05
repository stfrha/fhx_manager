# fhx_manager
Raspberry Pi application with web-based interface for controlling the FHX home theater installation.

This is the process for installing this app on a fresh Raspberry PI OS SD card:

1. Write a new SD card using Raspberry Pi Imager. User name (pi) and hostname (fhxmanager). Enabled ssh but not Rapsberi Pi Connect.
2. Window Power Shell: Change ip-address)
Run this to get the host name:
nmcli con show

Gives:
pi@fhxmanager:~ $ nmcli con show
NAME                              UUID                                  TYPE      DEVICE
netplan-wlan0-hoffman_in_the_air  ac737adb-700b-3554-b0c8-4e314de08f68  wifi      wlan0
lo                                a1cbfea5-7e25-4f9e-a086-fc58684346ce  loopback  lo
netplan-eth0                      75a1216a-9d1a-30cd-8aca-ace5526ec021  ethernet  --

So the name is "netplan-wlan0-hoffman_in_the_air"

Run this to change ip: 
sudo nmcli con mod "netplan-wlan0-hoffman_in_the_air" \
     ipv4.method manual \
     ipv4.addresses 192.168.0.202/24 \
     ipv4.gateway 192.168.0.1 \
     ipv4.dns "192.168.0.1"
sudo nmcli con up "netplan-wlan0-hoffman_in_the_air"

sudo reboot

Now the host name for the old pi must be removed from c:\\users\mail\.ssh\known_hosts, This is done in windows with:
ssh-keygen -R 192.168.0.202

Now pi can be accessed from windows using: ssh pi@192.168.0.202

3. Start vnc on pi:
sudo raspi-config nonint do_vnc 0

4. Start spi
sudo raspi-config nonint do_spi 0
sudo reboot

5. Instal web-server and php
sudo apt update
sudo apt install apache2 php libapache2-mod-php

6. Install build tools on pi
sudo apt install build-essential cmake git

Some extensions for code navigation and intellisence needs to be installed on the pi, it is done from VS Code.

7. Make sure git ssh-keys are working

8. Clon fhx-manager
mkdir -p ~/fhx_manager
cd fhx_manager
git clone git@github.com:stfrha/fhx_manager.git

9. Fetch  64 bit version of WiringPi (i.e. not my fork):
cd ~   [goto /home/pi/]
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build debian
mv debian-template/wiringpi_*.deb .
sudo apt install ./wiringpi_*.deb

10. Get pugixml
git clone git@github.com:stfrha/pugixml.git

11. Instal some linux-libs:
sudo apt install lirc liblirc-dev
sudo apt install libcurl4-openssl-dev

12. Build
rm -rf build                        [Clean]
cmake -S . -B build                 [One time tool set-up]
cmake --build build -j4             [Build]

13. Create a directory for socket_config.txt: 
sudo mkdir -p /var/lib/fhx_manager
sudo chown pi:pi /var/lib/fhx_manager

14. Create autostart of fhx_manager (has to be done after desktop-boot since videos are shown from the desktop)
sudo raspi-config
Go to System Options → Boot / Auto Login → Desktop Autologin.
mkdir -p ~/.config/autostart
nano ~/.config/autostart/fhx_manager.desktop

Add this to the file:
[Desktop Entry]
Type=Application
Name=FHX Manager
Path=/home/pi/fhx_manager/fhx_manager/build
Exec=/home/pi/fhx_manager/fhx_manager/start_fhx.sh
Terminal=false

Create a new start-up script: 
nano /home/fredrik/fhx_manager/start_fhx.sh

Add this to the file:
#!/bin/bash
cd /home/pi/fhx_manager/fhx_manager/build
sleep 5   # let the desktop and network settle
while true; do
    ./fhx_manager >> /home/pi/fhx_manager/fhx_manager/fhx.log 2>&1
    echo "$(date) fhx_manager exited ($?), restarting" >> /home/pi/fhx_manager/fhx_manager/fhx.log
    sleep 3
done

chmod +x /home/fredrik/fhx_manager/start_fhx.sh
sudo rebooot

To stop the app, the start-up script need to be stopped at the same time:
pkill -f start_fhx.sh; pkill fhx_manager

Restart with:
setsid ~/fhx_manager/fhx_manager/start_fhx.sh &

Check log:
tail -f ~/fhx_manager/fhx_manager/fhx.log



