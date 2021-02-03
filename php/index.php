<?php
ini_set("display_errors",1);
error_reporting(E_ALL);
include './comms.php';
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
      <a href="confirm_system_off.php">
         <div class="imgButton divBase turnOffButton" style="background-image:url('turn_off_button.png')">
         </div>
      </a>

      <a href="index.php">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('refresh_button.png')">
         </div>
      </a>

      <a href="volume.php?op=<?php echo($volume)?>">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourcePs">  
         <div class="imgButton divBase row1DualSplit left" style="background-image:url('ps_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceCc">  
         <div class="imgButton divBase row1DualSplit right" style="background-image:url('cc_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceTv">  
         <div class="imgButton divBase row2DualSplit left" style="background-image:url('tv_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceBr">  
         <div class="imgButton divBase row2DualSplit right" style="background-image:url('blu_ray_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceRpi">  
         <div class="imgButton divBase row3DualSplit left" style="background-image:url('rpi_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceSpotify">  
         <div class="imgButton divBase row4DualSplit left" style="background-image:url('spotify_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceVinyl">  
         <div class="imgButton divBase row4DualSplit right" style="background-image:url('vinyl_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceAux">  
         <div class="imgButton divBase row5DualSplit left" style="background-image:url('aux_button.png')">
         </div>
      </a>

      <a href="confirm_system_on.php?op=sourceTuner">  
         <div class="imgButton divBase row5DualSplit right" style="background-image:url('tuner_button.png')">
         </div>
      </a>

      <a href="light.php">  
         <div class="imgButton divBase row6DualSplit left" style="background-image:url('light_button.png')">
         </div>
      </a>
   </body>
</html>


