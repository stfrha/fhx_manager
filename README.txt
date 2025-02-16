OHH NOOO: The following was for therminal. But, it will probably be needed for FHX Manager also.


Need the following repos to build:

git@github.com:stfrha/WiringPi.git
  - Run ./build to build
git@github.com:stfrha/pugixml.git
  
Install apache2 webserver and php


sudo apt-get install apache2 -y
sudo apt-get install php libapache2-mod-php -y

Run the following command lines:
sudo cp -a /home/pi/git/therminal/php/. /var/www/html/
sudo rm /var/www/html/index.html

