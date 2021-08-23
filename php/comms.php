<?php
ini_set("display_errors",1);
error_reporting(E_ALL);

include './socket.php';
include './commands.php';

$RaspberryPiIP = "127.0.0.1"; // Change by your RaspberryPi/PC IP 

// Read port number from file: /home/pi/therminal/socket_config.txt
$myfile = fopen("/home/pi/fhx_manager/socket_config.txt", "r") or die("Unable to open file!");
$portString = fgets($myfile);
fclose($myfile);

$RaspberryPiPORT = (int)$portString; // Change by your RaspberryPi / PC Port Number ..
$connection = new Socket($RaspberryPiIP,$RaspberryPiPORT); // Create a new socket Connection object. 
$connection->init();

$connection->open_socket(); // Connect PHP to RaspberryPi or computer.

$foundOp = false;

if (isset($_GET["op"]))
{
   $operation = $_GET["op"];

   // echo("Searching op...");
   echo("Operation is:" . $operation);

   foreach ($cmdArray as &$value) 
   {
      // echo("Testing op: " . $value->op);

      if ($value->op == $operation)
      {
         echo("Found op, num of args: " . $value->numOfArgs);
         if ($value->numOfArgs > 0)
         {
            echo("First arg");
            if (isset($_GET["arg"]))
            {
               $value->argument = $_GET["arg"];
            }
            else
            {
               // Found cmd that requires argument but could not parse argument
               // $foundOp is false, and command will end up as status request 
               // below.
               break;
            }

         }

         if ($value->numOfArgs > 1)
         {
            echo("Second arg");
            if (isset($_GET["arg2"]))
            {
               $value->argument = $value->argument . ';' . $_GET["arg2"];
            }
            else
            {
               // Found cmd that requires argument but could not parse argument
               // $foundOp is false, and command will end up as status request 
               // below.
               break;
            }

         }

         if ($value->numOfArgs > 2)
         {
            echo("Third arg");
            if (isset($_GET["arg3"]))
            {
               $value->argument = $value->argument . ';' . $_GET["arg3"];
            }
            else
            {
               // Found cmd that requires argument but could not parse argument
               // $foundOp is false, and command will end up as status request 
               // below.
               break;
            }

         }
         
         $command = $value->getCommand();
         
         $foundOp = true;
         
         break;
      }
   }
    
}

if (!$foundOp)
{
   // Get status request command as default
   $command = $cmdArray[0]->getCommand();
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
$theatreState = $statusArray[8];
$theatreFeature = $statusArray[9];
$startHour = $statusArray[10];
$startMinute = $statusArray[11];
$featureDuration = $statusArray[12];
$endCredits = $statusArray[13];

$connection->close_socket(); 

?>
