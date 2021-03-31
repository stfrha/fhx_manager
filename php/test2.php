<?php
ini_set("display_errors",1);
error_reporting(E_ALL);
include './comms.php';

// Now find all files in script directory
$directory = '/home/pi/fhx_manager/scripts';
$scannedDirectory = array_diff(scandir($directory), array('..', '.'));


echo($scriptState . "\n");
echo($scriptName . "\n");
echo($theatreState . "\n");
echo($theatreFeature . "\n");

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

      <a href="index.php">  
         <div class="imgButton divBase turnOffButton" style="background-image:url('back_button.png')">
         </div>
      </a>

      <a href="feature_show.php">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('refresh_button.png')">
         </div>
      </a>

      <a href="volume.php">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_button.png')">
         </div>
      </a>
      
      <div class="imgButton divBase left" style="width: 92vw; height: 43vw; top: 26vh; background-image:url('now_showing_billboard.png');" >
      </div>
      
      <div class="divBase" style="top: 32vh; left: 15vw;" >
         <p class="ticketFeature" style="color: black;">$theatreFeature</p>
      </div>

      <a href="feature_show.php?op=pauseTheatreProgram">  
         <div class="imgButton divBase row5DualSplit left" style="background-image:url('dilog_buttons/dk_pause.png');">
         </div>
      </a>

      <a href="feature_show.php?op=abortTheatreProgram">  
         <div class="imgButton divBase row5DualSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>

      <a href="script.php">  
         <div class="imgButton divBase row6DualSplit right" style="background-image:url('script_button.png')">
         </div>
      </a>

   </body>
</html>


