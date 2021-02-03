<?php
ini_set("display_errors",1);
error_reporting(E_ALL);

if (isset($_GET["op"]))
{
   $operation = $_GET["op"];

   if ($operation =="sourcePs")
   {
      $image = "ps_button.png";  
   }
   else if ($operation =="sourceCc")
   {
      $image = "cc_button.png";  
   }
   else if ($operation =="sourceTv")
   {
      $image = "tv_button.png";  
   }
   else if ($operation =="sourceBr")
   {
      $image = "blu_ray_button.png";  
   }
   else if ($operation =="sourceRpi")
   {
      $image = "rpi_button.png";  
   }
   else if ($operation =="sourceSpotify")
   {
      $image = "spotify_button.png";  
   }
   else if ($operation =="sourceVinyl")
   {
      $image = "vinyl_button.png";  
   }
   else if ($operation =="sourceAux")
   {
      $image = "aux_button.png";  
   }
   else if ($operation =="sourceTuner")
   {
      $image = "tuner_button.png";  
   }
   else
   {
      $image = "back_button.png";  
   }
}
else
{
   // If no command is specified we query and display status
   $command = $statusRequest;
}

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
      <div class="imgButton divBase curtainsOverlay" style="background-image:url('open_curtains.png')">
      </div>

      <a href="index.php?op=<?php echo($operation); ?>">
         <div class="imgButton divBase row1NoSplit" style="background-image:url('<?php echo($image); ?>')">
         </div>
      </a>

      <a href="index.php">
         <div class="imgButton divBase row2NoSplit" style="background-image:url('back_button.png')">
         </div>
      </a>
   </body>
</html>


