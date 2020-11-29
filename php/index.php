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

$allOn = "ALON";
$allOff = "ALOF";
$preMovie = "PRMV";
$movie = "MOVI";
$kidsMovie = "KDMO";
$pause = "PAUS";
$endCredits = "ENCR";
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
   else
   {
      $command = "SREQ";
   }
}
else
{
   // If no command is specified we query and display status
   $command = "SREQ";
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
   
      <a href="?op=all_on">  
         <div class="divBase row1DualSplit left <?php if ($lightsOn == "1") echo("enabled"); else echo("disabled");?> <?php if (($state == "1") && ($lightsOn == "1")) echo("selected"); else echo("notSelected"); ?>">
           <div class="centered">
             <h2>Main On</h2>
           </div>
         </div>
      </a>

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
         <div class="divBase row4DualSplit right enabled notSelected">
           <div class="centered">
             <h2>Refresh</h2>
           </div>
         </div>
      </a>
   </body>
</html>


