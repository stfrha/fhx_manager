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
$connection = new Socket($RaspberryPiIP,$RaspberryPiPORT); // Create a new socet Connection object. 
$connection->init();

$connection->open_socket(); // Connect PHP to RaspberryPi or computer.

//////  Get Command from front end page

$allOn = "ALON------------";
$allOff = "ALOF------------";
$preMovie = "PRMV------------";
$movie = "MOVI------------";
$kidsMovie = "KDMO------------";
$pause = "PAUS------------";
$endCredits = "ENCR------------";
$statusRequest = "SREQ------------";
$data = "";

if (isset($_GET["op"]))
{
   $operation = $_GET["op"];
   
   if ($operation[0] == '(')
   {
      $command = "LCC" . $operation;
         
      echo 'Command: ' . $command;
   }
   else
   {
      if ($operation =="all_on")
      {
         $command = $allOn;  
      }
      else if ($operation =="all_off")
      {
         $command = $allOff;
      }
      else if ($operation =="pre_movie")
      {
         $command = $preMovie;
      }
      else if ($operation =="movie")
      {
         $command = $movie;
      }
      else if ($operation =="kids_movie")
      {
         $command = $kidsMovie;
      }
      else if ($operation =="pause")
      {
         $command = $pause;
      }
      else if ($operation =="end_cred")
      {
         $command = $endCredits;
      }
      else
      {
         $command = $statusRequest;
      }
   }
}
else
{
   // If no command is specified we query and display status
   $command = $statusRequest;
}
	
echo 'Command: ' . $command;
   
$connection->send_data($command); //Send command String

$fhxManagerStatus = $connection->read_data();

// Latest status is on the form:
// {lights on},{state} where lights on: 1=on, 2=off, and status: 1=all on, 2=all off, 3=pre movie, 4=movie, 5=pause, 6=end credits
// Example "1,4"

echo 'status: ' . $fhxManagerStatus;

$statusArray = explode(";", $fhxManagerStatus);
$lightsOn = $statusArray[0];
$state  = $statusArray[1];

$connection->close_socket(); 

?>
<html>
   <head>
      <link rel="stylesheet" type="text/css" href="fhx_manager.css" >
   </head>
   <style>
       { margin: 0; padding: 0; }

       html { 
           background: url('curtains.jpg') no-repeat center center fixed; 
           -webkit-background-size: cover;
           -moz-background-size: cover;
           -o-background-size: cover;
           background-size: cover;
       }
   </style>
   <body>

<!--   
      <a href="?op=all_on">  
         <div class="divBase row1DualSplit left <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "1") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
            <img src="turn_off_button.png" />
         </div>
      </a>
-->
      <a href="index.php">  
         <div class="imgButton divBase turnOffButton" style="background-image:url('back_button.png')">
         </div>
      </a>

      <a href="light.php?op=dummy">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('vol_up_button.png')">
         </div>
      </a>

      <a href="light.php?op=dummy">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_down_button.png')">
         </div>
      </a>

      <a href="light.php?op=all_on">  
         <div class="imgButton divBase row1TrippleSplit left" style="background-image:url('light_all_on_button.png')">
         </div>
      </a>

      <a href="light.php?op=all_off">  
         <div class="imgButton divBase row1TrippleSplit middle" style="background-image:url('light_all_off_button.png')">
         </div>
      </a>

      <a href="light.php?op=kids_movie">  
         <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('light_kids_movie_button.png')">
         </div>
      </a>

      <a href="light.php?op=pre_movie">  
         <div class="imgButton divBase row2TrippleSplit left" style="background-image:url('light_before_button.png')">
         </div>
      </a>

      <a href="light.php?op=movie">  
         <div class="imgButton divBase row2TrippleSplit middle" style="background-image:url('light_movie_button.png')">
         </div>
      </a>

      <a href="light.php?op=end_cred">  
         <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('light_end_button.png')">
         </div>
      </a>

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
   
      for ($x = 0; $x < 13; $x++)
         for ($y = 0; $y < 9; $y++)
         {
            // Build color string of type "(129,054,255)"
            // I.e. pad so all decimal numbers are three characters
            $c = '(' .  str_pad($reds[$y][$x], 3, '0', STR_PAD_LEFT) . ',' . str_pad($greens[$y][$x], 3, '0', STR_PAD_LEFT) . ',' . str_pad($blues[$y][$x] , 3, '0', STR_PAD_LEFT) . ')';
            echo '   <a href="?op=' . $c . '">';
            echo "\r\n";

            echo '      <div class="cpbBase" style="border:none; background-color:rgb' . $c . '; width:7vw; height:3vh; left:' . (0.64 + $x * 7.64) . 'vw; top:' . (($y * 3.64) + 58)  . 'vh;"></div>';
            echo "\r\n";
            echo '   </a>';
         }
   ?>

<!--
      <a href="?op=all_off">  
         <div class="divBase row1DualSplit right <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "2") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
           <div class="centered">
             <h2>Main Off</h2>
           </div>
         </div>
      </a>

      <a href="?op=pre_movie">  
         <div class="divBase row2DualSplit left <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "3") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
           <div class="centered">
             <h2>Before Movie</h2>
           </div>
         </div>
      </a>

      <a href="?op=kids_movie">  
         <div class="divBase row2DualSplit right <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "7") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
           <div class="centered">
             <h2>Kids Movie</h2>
           </div>
         </div>
      </a>

      <a href="?op=movie">  
         <div class="divBase row3DualSplit left <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "4") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
           <div class="centered">
             <h2>Movie</h2>
           </div>
         </div>
      </a>

      <a href="?op=pause">  
         <div class="divBase row3DualSplit right <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "5") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
           <div class="centered">
             <h2>Pause</h2>
           </div>
         </div>
      </a>

      <a href="?op=end_cred">  
         <div class="divBase row4DualSplit left <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "6") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
           <div class="centered">
             <h2>End Credits</h2>
           </div>
         </div>
      </a>

      <a href="index.php">  
         <div class="divBase row41DualSplit rightLeft enabled notSelected">
           <div class="centered">
             <h2>Refresh</h2>
           </div>
         </div>
      </a>

      <a href="color.php">  
         <div class="divBase row41DualSplit right enabled notSelected">
           <div class="centered">
             <h2>Colors</h2>
           </div>
         </div>
      </a>
-->      
      
      
   </body>
</html>


