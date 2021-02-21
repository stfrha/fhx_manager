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

// VOLUMEVAL(-45.5)  - Volume, could also be VOLUMEVAL(2)----
$volumeValue = "VOLUMEVAL-------";

$sourcePlaystation = "SOURCEPS--------";
$sourceChromeCast = "SOURCECC--------";
$sourceTelevision = "SOURCETV--------";
$sourceSpotify = "SOURCESPOTIFY---";
$sourceVinyl = "SOURCEVINYL-----";
$sourceTuner = "SOURCETUNER-----";
$sourceRpi = "SOURCERPI-------";
$sourceAux = "SOURCEAUX-------";
$sourceUsb = "SOURCEUSB-------";
$sourceBr = "SOURCEBR--------";
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
      if (substr($operation, 0, 3) == "vol")
      {
         $command = "VOLUMEVAL(" .  substr($operation, 3) . ")------";
         $command = substr($command, 0, 16);
      }
      else if ($operation =="all_on")
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
      else if ($operation =="sourceSpotify")
      {
         $command = $sourceSpotify;
      }
      else if ($operation =="sourceVinyl")
      {
         $command = $sourceVinyl;
      }
      else if ($operation =="sourceTuner")
      {
         $command = $sourceTuner;
      }
      else if ($operation =="sourceRpi")
      {
         $command = $sourceRpi;
      }
      else if ($operation =="sourceAux")
      {
         $command = $sourceAux;
      }
      else if ($operation =="sourceBr")
      {
         $command = $sourceBr;
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
$yamahaPower = $statusArray[2];
$benqPower = $statusArray[3];
$volume = $statusArray[4];
$source = $statusArray[5];

$connection->close_socket(); 

?>
