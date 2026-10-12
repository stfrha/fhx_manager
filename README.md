# fhx_manager
Raspberry Pi application with web-based interface for controlling the FHX home theater installation.

### This is the process for installing this app on a fresh Raspberry PI OS SD card:

### 1. Write a new SD card using Raspberry Pi Imager. User name (pi) and hostname (fhxmanager). Enabled ssh but not Rapsberi Pi Connect.
 
### 3. Window Power Shell: Change ip-address)

#### Run this to get the host name:

nmcli con show

#### Gives:

pi@fhxmanager:~ $ nmcli con show

NAME                              UUID                                  TYPE      DEVICE

netplan-wlan0-hoffman_in_the_air  ac737adb-700b-3554-b0c8-4e314de08f68  wifi      wlan0

lo                                a1cbfea5-7e25-4f9e-a086-fc58684346ce  loopback  lo

netplan-eth0                      75a1216a-9d1a-30cd-8aca-ace5526ec021  ethernet  --


So the name is "netplan-wlan0-hoffman_in_the_air"

Run this to change ip: 

sudo nmcli con mod "netplan-wlan0-hoffman_in_the_air" \\
     ipv4.method manual \\
     ipv4.addresses 192.168.0.202/24 \\
     ipv4.gateway 192.168.0.1 \\
     ipv4.dns "192.168.0.1"

sudo nmcli con up "netplan-wlan0-hoffman_in_the_air"

sudo reboot

#### Now the host name for the old pi must be removed from c:\\users\mail\.ssh\known_hosts, This is done in windows with:

ssh-keygen -R 192.168.0.202

Now pi can be accessed from windows using: ssh pi@192.168.0.202

### 3. Start vnc on pi:

sudo raspi-config nonint do_vnc 0

### 4. Start spi

sudo raspi-config nonint do_spi 0

sudo reboot

### 5. Instal web-server and php

sudo apt update

sudo apt install apache2 php libapache2-mod-php

### 6. Install build tools on pi

sudo apt install build-essential cmake git

Some extensions for code navigation and intellisence needs to be installed on the pi, it is done from VS Code.

### 7. Make sure git ssh-keys are working

### 8. Clon fhx-manager

mkdir -p ~/fhx_manager

cd fhx_manager

git clone git@github.com:stfrha/fhx_manager.git

### 9. Fetch  64 bit version of WiringPi (i.e. not my fork):

cd ~   [goto /home/pi/]

git clone https://github.com/WiringPi/WiringPi.git

cd WiringPi

./build debian

mv debian-template/wiringpi_*.deb .

sudo apt install ./wiringpi_*.deb

### 10. Get pugixml

git clone git@github.com:stfrha/pugixml.git

### 11. Instal some linux-libs:

sudo apt install lirc liblirc-dev

sudo apt install libcurl4-openssl-dev

### 12. Build

rm -rf build                        [Clean]

cmake -S . -B build                 [One time tool set-up]

cmake --build build -j4             [Build]

### 13. Create a directory for socket_config.txt: 

sudo mkdir -p /var/lib/fhx_manager

sudo chown pi:pi /var/lib/fhx_manager

### 14. Deploy the web pages

Copies the php/ folder to Apache's web root (/var/www/html). Run again after every change in php/:

cd ~/fhx_manager/fhx_manager

./deploy_php.sh

### 15. Copy movie scripts and videos

The movie scripts and videos are not in git. They must be placed next to the repo (not inside it):

/home/pi/fhx_manager/scripts          [theatre scripts, e.g. Tenet]

/home/pi/fhx_manager/video            [crowd.mp4, black_screen.mp4, comming_attractions.mp4, nu_blir_det_reklam.mp4]

/home/pi/fhx_manager/video/commercials   [commercial videos, picked at random]

Copy them from the backup.

### 16. Let the web server read the scripts directory

The web pages (e.g. feature_show.php) read /home/pi/fhx_manager/scripts, but Raspberry Pi OS makes /home/pi private (drwx------), so Apache (user www-data) gets "Permission denied". Allow others to pass through the home directory (they still can not list its contents):

chmod o+x /home/pi

Check with (the pi line should read drwx-----x):

namei -l /home/pi/fhx_manager/scripts

### 17. Force HDMI on and use a saved projector EDID

The Pi is connected to the BenQ projector through the Yamaha receiver. If the projector is off when the Pi boots, the Pi falls back to 1024x768, which the receiver does not pass through, so videos are not shown. Therefore HDMI is forced on (D = always enable the output).

A forced output never reads the projector's EDID, and without EDID the Pi treats the screen as a DVI monitor without audio, so there is no HDMI sound. Therefore the projector's EDID is saved in the repo (edid/benq_pj.bin) and given to the kernel. It must also be in the initramfs, since the vc4 graphics driver is loaded from there. With the EDID the projector's preferred 1920x1080 is used (the receiver passes it through); 1280x720 in video= is only used if the EDID can not be loaded.

sudo mkdir -p /lib/firmware/edid

sudo cp ~/fhx_manager/fhx_manager/edid/benq_pj.bin /lib/firmware/edid/

sudo cp ~/fhx_manager/fhx_manager/edid/initramfs_hook_edid /etc/initramfs-tools/hooks/edid

sudo update-initramfs -u

lsinitramfs /boot/firmware/initramfs8 | grep edid      [should list lib/firmware/edid/benq_pj.bin]

Then add both settings to cmdline.txt. cmdline.txt must stay a single line:

sudo cp /boot/firmware/cmdline.txt /boot/firmware/cmdline.txt.bak

sudo sed -i '1 s/$/ video=HDMI-A-1:1280x720@60D drm.edid_firmware=HDMI-A-1:edid\/benq_pj.bin/' /boot/firmware/cmdline.txt

cat /boot/firmware/cmdline.txt      [should end with video=HDMI-A-1:1280x720@60D drm.edid_firmware=HDMI-A-1:edid/benq_pj.bin]

sudo reboot

Check after reboot (should be 256, and HDMI should be listed under Sinks):

wc -c < /sys/class/drm/card0-HDMI-A-1/edid

wpctl status

To read the EDID again (e.g. new projector), with the projector on and connected without the forced settings:

sudo i2cdump -y 2 0x50 b            [HDMI DDC bus on Raspberry Pi 3, EDID is at address 0x50]

To test the resolution without rebooting (lasts until next reboot):

wlr-randr --output HDMI-A-1 --custom-mode 1280x720@60

### 18. Send sound to HDMI at full volume

Videos are played with VLC, which uses the default audio output. Raspberry Pi OS defaults to the headphone jack at 40%. Make HDMI the default and set it to 100%, the volume is then controlled on the Yamaha receiver. Both settings are remembered after reboot.

wpctl status                        [find the id of "Built-in Audio Digital Stereo (HDMI)" under Sinks]

wpctl set-default <id>

wpctl set-volume @DEFAULT_AUDIO_SINK@ 1.0

wpctl get-volume @DEFAULT_AUDIO_SINK@   [should show Volume: 1.00]

### 19. Enable serial port for DALI

DALI is controlled through the Pi's serial port (/dev/serial0, 19200 baud). The BenQ projector uses a USB serial adapter (/dev/ttyUSB0) and needs nothing here. Enable the serial hardware and disable the serial login console:

sudo raspi-config nonint do_serial_hw 0

sudo raspi-config nonint do_serial_cons 1

sudo reboot

Test with:

./build/dali_debug

### 20. Black desktop

When the Raspberry Pi source is shown on the receiver, only black should be visible between and around the videos. Set a black background without desktop icons, and let the panel auto-hide (it slides in when the mouse is moved to the top edge, e.g. over VNC):

mkdir -p ~/.config/pcmanfm/default ~/.config/wf-panel-pi

printf '[*]\nwallpaper_mode=color\ndesktop_bg=#000000000000\ndesktop_shadow=#000000000000\nshow_documents=0\nshow_trash=0\nshow_mounts=0\n' > ~/.config/pcmanfm/default/desktop-items-0.conf

printf '[panel]\nautohide=true\nautohide_duration=500\n' > ~/.config/wf-panel-pi/wf-panel-pi.ini

pkill -x pcmanfm; pkill -x wf-panel-pi      [both restart automatically with the new settings]

### 21. Create autostart of fhx_manager (has to be done after desktop-boot since videos are shown from the desktop)

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


#### To stop the app, the start-up script need to be stopped at the same time:

pkill -f start_fhx.sh; pkill fhx_manager


#### Restart with:

setsid ~/fhx_manager/fhx_manager/start_fhx.sh &


#### Check log:

tail -f ~/fhx_manager/fhx_manager/fhx.log



