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
      else if ($operation =="dilogPower") $command = "DH660POWER------";
      else if ($operation =="dilogMute") $command = "DH660MUTE-------";
      else if ($operation =="dilogPower") $command = "DH660LIST-------";
      else if ($operation =="dilogPower") $command = "DH660TIME-------";
      else if ($operation =="dilog1") $command = "DH660K1---------";
      else if ($operation =="dilog2") $command = "DH660K2---------";
      else if ($operation =="dilog3") $command = "DH660K3---------";
      else if ($operation =="dilog4") $command = "DH660K4---------";
      else if ($operation =="dilog5") $command = "DH660K5---------";
      else if ($operation =="dilog6") $command = "DH660K6---------";
      else if ($operation =="dilog7") $command = "DH660K7---------";
      else if ($operation =="dilog8") $command = "DH660K8---------";
      else if ($operation =="dilog9") $command = "DH660K9---------";
      else if ($operation =="dilog0") $command = "DH660K0---------";
      // else if ($operation =="dilogPower") $command = "DH660CHANUP-----";
      // else if ($operation =="dilogPower") $command = "DH660CHANDWN----";
      else if ($operation =="dilogInfo") $command = "DH660INFO-------";
      // else if ($operation =="dilogPower") $command = "DH660DIRECTORY--";
      else if ($operation =="dilogMenu") $command = "DH660MENU-------";
      else if ($operation =="dilogEpg") $command = "DH660EPG--------";
      else if ($operation =="dilogUp") $command = "DH660UP---------";
      else if ($operation =="dilogExit") $command = "DH660EXIT-------";
      else if ($operation =="dilogLeft") $command = "DH660LEFT-------";
      else if ($operation =="dilogOk") $command = "DH660SELECT-----";
      else if ($operation =="dilogRight") $command = "DH660RIGHT------";
      else if ($operation =="dilogSubtitle") $command = "DH660SUBTITLE---";
      else if ($operation =="dilogDown") $command = "DH660DOWN-------";
      else if ($operation =="dilogText") $command = "DH660TEXT-------";
      else if ($operation =="dilogStop") $command = "DH660STOP-------";
      else if ($operation =="dilogPlay") $command = "DH660PLAY-------";
      else if ($operation =="dilogPause") $command = "DH660PAUSE------";
      else if ($operation =="dilogRecord") $command = "DH660RECORD-----";
      else if ($operation =="dilogRed") $command = "DH660RED--------";
      else if ($operation =="dilogGreen") $command = "DH660GREEN------";
      else if ($operation =="dilogYellow") $command = "DH660YELLOW-----";
      else if ($operation =="dilogBlue") $command = "DH660BLUE-------";
      else if ($operation =="dilogNext") $command = "DH660NEXT-------";
      else if ($operation =="dilogPrevious") $command = "DH660PREVIOUS---";
      //else if ($operation =="dilogPower") $command = "DH660AUDIO------";
      //else if ($operation =="dilogPower") $command = "DH660ZOOM-------";

      else if ($operation =="bdPower")    $command = "IRCCPOWER-------";
      else if ($operation =="bdSubtitle") $command = "IRCCSUBTITLE----";
      else if ($operation =="bdEject")    $command = "IRCCEJECT-------";
      else if ($operation =="bdRewind")   $command = "IRCCREWIND------";
      else if ($operation =="dbPlay")     $command = "IRCCPLAY--------";
      else if ($operation =="bdFf")       $command = "IRCCFORWARD-----";
      else if ($operation =="bdPrevious") $command = "IRCCPREV--------";
      else if ($operation =="bdPause")    $command = "IRCCPAUSE-------";
      else if ($operation =="bdStop")     $command = "IRCCSTOP--------";
      else if ($operation =="bdNext")     $command = "IRCCNEXT--------";
      else if ($operation =="bdTopMenu")  $command = "IRCCTOPMENU-----";
      else if ($operation =="bdUp")       $command = "IRCCUP----------";
      else if ($operation =="bdBack")     $command = "IRCCRETURN------";
      else if ($operation =="bdLeft")     $command = "IRCCLEFT--------";
      else if ($operation =="bdOk")       $command = "IRCCCONFIRM-----";
      else if ($operation =="bdRight")    $command = "IRCCRIGHT-------";
      else if ($operation =="bdPopupMenu")$command = "IRCCPOPUPMENU---";
      else if ($operation =="bdDown")     $command = "IRCCDOWN--------";
      else if ($operation =="bdOptions")  $command = "IRCCOPTIONS-----";
      else if ($operation =="bdRed")      $command = "IRCCRED---------";
      else if ($operation =="bdGreen")    $command = "IRCCGREEN-------";
      else if ($operation =="bdYellow")   $command = "IRCCYELLOW------";
      else if ($operation =="bdBlue")     $command = "IRCCBLUE--------";
      
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
	
$connection->send_data($command); //Send command String

$fhxManagerStatus = $connection->read_data();

$statusArray = explode(";", $fhxManagerStatus);
$lightsOn = $statusArray[0];
$state  = $statusArray[1];
$yamahaPower = $statusArray[2];
$benqPower = $statusArray[3];
$volume = $statusArray[4];
$source = $statusArray[5];
$scriptState = $statusArray[6];
$scriptName = $statusArray[7];

$connection->close_socket(); 

?>
