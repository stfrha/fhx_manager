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
$testYamahaComm = "TESTYAMAHACOMM--";
$systemOff = "SYSTEMOFF-------";
$volumeUp = "VOLUMEUP--------";
$volumeDown = "VOLUMEDOWN------";
$sourcePlaystation = "SOURCEPS--------";
$sourceChromeCast = "SOURCECC--------";
$sourceTelevision = "SOURCETV--------";
$sourceSpotify = "SOURCESPOTIFY---";
$sourcePhono = "SOURCEPHONO-----";
$sourceTuner = "SOURCETUNER-----";
$sourceAux = "SOURCEAUX-------";
$sourceUsb = "SOURCEUSB-------";
$statusRequest = "SREQ------------";
$data = "";

if (isset($_GET["op"]))
{
   $operation = $_GET["op"];

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
   else if ($operation =="test_yamaha")
   {
      $command = $testYamahaComm;
   }
   else if ($operation =="system_off")
   {
      $command = $systemOff;
   }
   else if ($operation =="vol_up")
   {
      $command = $volumeUp;
   }
   else if ($operation =="vol_down")
   {
      $command = $volumeDown;
   }
   else if ($operation =="sourcePs")
   {
      $command = $sourcePlaystation;
   }
   else if ($operation =="sourceCc")
   {
      $command = $sourceChromeCast;
   }
   else if ($operation =="sourceTv")
   {
      $command = $sourceTelevision;
   }
   else
   {
      $command = $statusRequest;
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
      <a href="?op=system_off">
         <div class="imgButton divBase turnOffButton" style="background-image:url('turn_off_button.png')">
         </div>
      </a>

      <a href="?op=vol_up">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('vol_up_button.png')">
         </div>
      </a>

      <a href="?op=vol_down">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_down_button.png')">
         </div>
      </a>

      <a href="?op=sourcePs">  
         <div class="imgButton divBase row1DualSplit left" style="background-image:url('ps_button.png')">
         </div>
      </a>

      <a href="?op=sourceCc">  
         <div class="imgButton divBase row1DualSplit right" style="background-image:url('cc_button.png')">
         </div>
      </a>

      <a href="?op=sourceTv">  
         <div class="imgButton divBase row2DualSplit left" style="background-image:url('tv_button.png')">
         </div>
      </a>

      <a href="?op=dummy">  
         <div class="imgButton divBase row2DualSplit right" style="background-image:url('other_button.png')">
         </div>
      </a>

      <a href="light.php">  
         <div class="imgButton divBase row3DualSplit left" style="background-image:url('light_button.png')">
         </div>
      </a>
   </body>
</html>


