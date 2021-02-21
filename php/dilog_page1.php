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

      <a href="dilog_page2.php">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('dilog_buttons/dk_page2.png')">
         </div>
      </a>

      <a href="volume.php?op=<?php echo("is" . $volume)?>">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_button.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row1Page1 left" style="background-image:url('dilog_buttons/dk_1.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row1Page1 middle" style="background-image:url('dilog_buttons/dk_2.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row1Page1 right" style="background-image:url('dilog_buttons/dk_3.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row2Page1 left" style="background-image:url('dilog_buttons/dk_4.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row2Page1 middle" style="background-image:url('dilog_buttons/dk_5.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row2Page1 right" style="background-image:url('dilog_buttons/dk_6.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row3Page1 left" style="background-image:url('dilog_buttons/dk_7.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row3Page1 middle" style="background-image:url('dilog_buttons/dk_8.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row3Page1 right" style="background-image:url('dilog_buttons/dk_9.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row4Page1 left" style="background-image:url('dilog_buttons/dk_epg.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row4Page1 middle" style="background-image:url('dilog_buttons/dk_0.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row4Page1 right" style="background-image:url('dilog_buttons/dk_info.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row5 left" style="background-image:url('dilog_buttons/dk_menu.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase upDwnButtonSize row5 upDwnButtonPos" style="background-image:url('dilog_buttons/dk_up.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row5 right" style="background-image:url('dilog_buttons/dk_exit.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase tButtonSize leftButtonPos" style="background-image:url('dilog_buttons/dk_left.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase okButtonSize okButtonPos" style="background-image:url('dilog_buttons/dk_ok.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase tButtonSize rightButtonPos" style="background-image:url('dilog_buttons/dk_right.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row7 left" style="background-image:url('dilog_buttons/dk_play.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase upDwnButtonSize row7 upDwnButtonPos" style="background-image:url('dilog_buttons/dk_down.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase buttonSize row7 right" style="background-image:url('dilog_buttons/dk_rec.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase colorButtonSize row8 left" style="background-image:url('dilog_buttons/dk_red.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase colorButtonSize row8 fourSplitCol2" style="background-image:url('dilog_buttons/dk_green.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase colorButtonSize row8 fourSplitCol3" style="background-image:url('dilog_buttons/dk_yellow.png')">
         </div>
      </a>

      <a href="dilog_page1.php">  
         <div class="imgButton divBase colorButtonSize row8 right" style="background-image:url('dilog_buttons/dk_blue.png')">
         </div>
      </a>

   </body>
</html>


