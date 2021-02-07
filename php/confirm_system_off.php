<?php
ini_set("display_errors",1);
error_reporting(E_ALL);
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
      <a href="index.php?op=system_off">
         <div class="imgButton divBase row1NoSplit" style="background-image:url('turn_off_button.png')">
         </div>
      </a>

      <a href="index.php">
         <div class="imgButton divBase row2NoSplit" style="background-image:url('back_button.png')">
         </div>
      </a>
   </body>
</html>


