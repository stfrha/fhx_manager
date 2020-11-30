<?php
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

   $command = "LCC" . $operation;
      
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
   <h1>Click on a color for led stripe</h1>
   <?php 
   
      $reds = array (
         array( 51,  51,  51,  25,   0,   0,   0,   0,   0,  25,  51,  51,   0),
         array(102, 102, 102,  51,   0,   0,   0,   0,   0,  51, 102, 102,  32),
         array(153, 153, 153,  76,   0,   0,   0,   0,   0,  76, 153, 153,  64),
         array(204, 204, 204, 102,   0,   0,   0,   0,   0, 102, 204, 204,  96),
         array(255, 255, 255, 128,   0,   0,   0,   0,   0, 128, 255, 255, 128),
         array(255, 255, 255, 153,  51,  51,  51,  51,  51, 153, 255, 255, 160),
         array(255, 255, 255, 178, 102, 102, 102, 102, 102, 178, 255, 255, 192),
         array(255, 255, 255, 204, 153, 153, 153, 153, 153, 204, 255, 255, 224),
         array(255, 255, 255, 229, 204, 204, 204, 204, 204, 229, 255, 255, 255)
      );

      $greens = array (
         array(  0,  25,  51,  51,  51,  51,  51,  25,   0,   0,   0,   0,   0),
         array(  0,  51, 102, 102, 102, 102, 102,  51,   0,   0,   0,   0,  32),
         array(  0,  76, 153, 153, 153, 153, 153,  76,   0,   0,   0,   0,  64),
         array(  0, 102, 204, 204, 204, 204, 204, 102,   0,   0,   0,   0,  96),
         array(  0, 128, 255, 255, 255, 255, 255, 128,   0,   0,   0,   0, 128),
         array( 51, 153, 255, 255, 255, 255, 255, 153,  51,  51,  51,  51, 160),
         array(102, 178, 255, 255, 255, 255, 255, 178, 102, 102, 102, 102, 192),
         array(153, 204, 255, 255, 255, 255, 255, 204, 153, 153, 153, 153, 224),
         array(204, 229, 255, 255, 255, 255, 255, 229, 204, 204, 204, 204, 255)
      );

      $blues = array (
         array(  0,   0,   0,   0,  0,  25,  51,  51,  51,  51,  51,  25,   0),
         array(  0,   0,   0,   0,  0,  51, 102, 102, 102, 102, 102,  51,  32),
         array(  0,   0,   0,   0,  0,  76, 153, 153, 153, 153, 153,  76,  64),
         array(  0,   0,   0,   0,  0, 102, 204, 204, 204, 204, 204, 102,  96),
         array(  0,   0,   0,   0,  0, 128, 255, 255, 255, 255, 255, 128, 128),
         array( 51,  51,  51,  51, 51, 153, 255, 255, 255, 255, 255, 153, 160),
         array(102, 102, 102, 102,102, 178, 255, 255, 255, 255, 255, 178, 192),
         array(153, 153, 153, 153,153, 204, 255, 255, 255, 255, 255, 204, 224),
         array(204, 204, 204, 204,204, 229, 255, 255, 255, 255, 255, 229, 255)
      );
   
      for ($y = 0; $y < 13; $y++)
         for ($x = 0; $x < 9; $x++)
         {
            // Build color string of type "(129,054,255)"
            // I.e. pad so all decimal numbers are three characters
            $c = '(' .  str_pad($reds[$x][$y], 3, '0', STR_PAD_LEFT) . ',' . str_pad($greens[$x][$y], 3, '0', STR_PAD_LEFT) . ',' . str_pad($blues[$x][$y] , 3, '0', STR_PAD_LEFT) . ')';
            echo '   <a href="?op=' . $c . '">';
            echo "\r\n";

            echo '      <div class="cpbBase" style="border:none; background-color:rgb' . $c . '; width:8vw; height:4vh; left:' . $x * 8 . 'vw; top:' . (($y * 4) + 10)  . 'vh;"></div>';
            echo "\r\n";
            echo '   </a>';
         }
   ?>
      <a href="index.php">  
         <div class="divBase row41DualSplit rightLeft enabled notSelected">
           <div class="centered">
             <h2>Main</h2>
           </div>
         </div>
      </a>
   
   </body>
</html>


