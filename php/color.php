<?php
echo 'Current PHP version: ' . phpversion();

ini_set("display_errors",1);
error_reporting(E_ALL);

include './socket.php';

$RaspberryPiIP = "127.0.0.1"; // Change by your RaspberryPi/PC IP 

// Read port number from file: /home/pi/therminal/socket_config.txt
$myfile = fopen("/home/pi/fhx_manager/socket_config.txt", "r") or die("Unable to open file!");
$portString = fgets($myfile);
fclose($myfile);

$RaspberryPiPORT = (int)$portString; // Change by your RaspberryPi / PC Port Number ..
$connection = new Socket($RaspberryPiIP,$RaspberryPiPORT); // Create a new socket Connection object. 
$connection->init();

$connection->open_socket(); // Connect PHP to RaspberryPi or computer.

//////  Get Command from front end page

if (isset($_GET["op"]))
{
   $operation = $_GET["op"];

   $command = "LED" . $operation;
      
   echo 'Command: ' . $command;
      
   $connection->send_data($command); //Send command String

   $fhxManagerStatus = $connection->read_data();

   $connection->close_socket(); 
  
}
?>
<html>
   <head>
      <link rel="stylesheet" type="text/css" href="fhx_manager.css" >
   </head>
   <body>
   <?php 
      for ($y = 0; $y < 16; $y++)
         for ($x = 0; $x < 16; $x++)
         {
            $c = '(' .  $x * 16  . ',' . (255 - ($x * 16)) . ',' . $y * 16 . ')';
            echo '   <a href="?op=' . $c . '">';
            echo "\r\n";

            echo '      <div class="cpbBase" style="border:none; background-color:rgb' . $c . '; width:4vw; height:4vh; left:' . $x * 4 . 'vw; top:' . $y * 4 . 'vh;"></div>';
            echo "\r\n";
            echo '   </a>';
         }
   ?>
   </body>
</html>


