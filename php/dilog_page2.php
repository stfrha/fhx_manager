<?php
ini_set("display_errors",1);
error_reporting(E_ALL);
include './comms.php';
?>
<html>
   <head>
      <link rel="stylesheet" type="text/css" href="dilog.css" >
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
         <div class="imgButton divBase turnOffButton" style="background-image:url('main_button.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('dilog_buttons/dk_page1.png')">
         </div>
      </a>

      <a href="volume.php?op=<?php echo("is" . $volume)?>">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_button.png')">
         </div>
      </a>

      <a href="dilog_page2.php?op=dilogPower">  
         <div class="imgButton divBase buttonSize row1Page2 left" style="background-image:url('dilog_buttons/dk_power.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row1Page2 right" style="background-image:url('dilog_buttons/dk_mute.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row2Page2 left" style="background-image:url('dilog_buttons/dk_play.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row2Page2 middle" style="background-image:url('dilog_buttons/dk_stop.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row2Page2 right" style="background-image:url('dilog_buttons/dk_rec.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row3Page2 left" style="background-image:url('dilog_buttons/dk_prev.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row3Page2 middle" style="background-image:url('dilog_buttons/dk_pause.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row3Page2 right" style="background-image:url('dilog_buttons/dk_next.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row5 left" style="background-image:url('dilog_buttons/dk_menu.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase upDwnButtonSize row5 upDwnButtonPos" style="background-image:url('dilog_buttons/dk_up.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row5 right" style="background-image:url('dilog_buttons/dk_exit.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase tButtonSize leftButtonPos" style="background-image:url('dilog_buttons/dk_left.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase okButtonSize okButtonPos" style="background-image:url('dilog_buttons/dk_ok.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase tButtonSize rightButtonPos" style="background-image:url('dilog_buttons/dk_right.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row7 left" style="background-image:url('dilog_buttons/dk_play.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase upDwnButtonSize row7 upDwnButtonPos" style="background-image:url('dilog_buttons/dk_down.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase buttonSize row7 right" style="background-image:url('dilog_buttons/dk_rec.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase colorButtonSize row8 left" style="background-image:url('dilog_buttons/dk_red_slow.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase colorButtonSize row8 fourSplitCol2" style="background-image:url('dilog_buttons/dk_green_slow.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase colorButtonSize row8 fourSplitCol3" style="background-image:url('dilog_buttons/dk_yellow.png')">
         </div>
      </a>

      <a href="dilog_page2.php">  
         <div class="imgButton divBase colorButtonSize row8 right" style="background-image:url('dilog_buttons/dk_blue.png')">
         </div>
      </a>

   </body>
</html>


