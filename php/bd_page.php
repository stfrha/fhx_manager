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

      <a href="volume.php?op=<?php echo("is" . $volume)?>">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_button.png')">
         </div>
      </a>

<!--
      <a href="bd_page.php?op=bdPower">  
         <div class="imgButton divBase buttonSize row1Page2 left" style="background-image:url('dilog_buttons/dk_power.png')">
         </div>
      </a>
-->
      <a href="bd_page.php?op=bdSubtitle">  
         <div class="imgButton divBase buttonSize row1Page2 middle" style="background-image:url('dilog_buttons/dk_subtutle.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdEject">  
         <div class="imgButton divBase buttonSize row1Page2 right" style="background-image:url('dilog_buttons/bd_eject.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdRewind">  
         <div class="imgButton divBase buttonSize row2Page2 left" style="background-image:url('dilog_buttons/bd_rewind.png')">
         </div>
      </a>

      <a href="bd_page.php?op=dbPlay">  
         <div class="imgButton divBase buttonSize row2Page2 middle" style="background-image:url('dilog_buttons/dk_play.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdFf">  
         <div class="imgButton divBase buttonSize row2Page2 right" style="background-image:url('dilog_buttons/bd_ff.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdPrevious">  
         <div class="imgButton divBase colorButtonSize row3Page2 left" style="background-image:url('dilog_buttons/dk_prev.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdPause">  
         <div class="imgButton divBase colorButtonSize row3Page2 fourSplitCol2" style="background-image:url('dilog_buttons/dk_pause.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdStop">  
         <div class="imgButton divBase colorButtonSize row3Page2 fourSplitCol3" style="background-image:url('dilog_buttons/dk_stop.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdNext">  
         <div class="imgButton divBase colorButtonSize row3Page2 right" style="background-image:url('dilog_buttons/dk_next.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdTopMenu">  
         <div class="imgButton divBase buttonSize row5 left" style="background-image:url('dilog_buttons/bd_top_menu.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdUp">  
         <div class="imgButton divBase upDwnButtonSize row5 upDwnButtonPos" style="background-image:url('dilog_buttons/bd_up.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdBack">  
         <div class="imgButton divBase buttonSize row5 right" style="background-image:url('dilog_buttons/bd_back.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdLeft">  
         <div class="imgButton divBase tButtonSize leftButtonPos" style="background-image:url('dilog_buttons/bd_left.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdOk">  
         <div class="imgButton divBase okButtonSize okButtonPos" style="background-image:url('dilog_buttons/dk_ok.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdRight">  
         <div class="imgButton divBase tButtonSize rightButtonPos" style="background-image:url('dilog_buttons/bd_right.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdPopupMenu">  
         <div class="imgButton divBase buttonSize row7 left" style="background-image:url('dilog_buttons/bd_popup_menu.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdDown">  
         <div class="imgButton divBase upDwnButtonSize row7 upDwnButtonPos" style="background-image:url('dilog_buttons/bd_down.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdOptions">  
         <div class="imgButton divBase buttonSize row7 right" style="background-image:url('dilog_buttons/bd_options.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdRed">  
         <div class="imgButton divBase colorButtonSize row8 left" style="background-image:url('dilog_buttons/dk_red.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdGreen">  
         <div class="imgButton divBase colorButtonSize row8 fourSplitCol2" style="background-image:url('dilog_buttons/dk_green.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdYellow">  
         <div class="imgButton divBase colorButtonSize row8 fourSplitCol3" style="background-image:url('dilog_buttons/dk_yellow.png')">
         </div>
      </a>

      <a href="bd_page.php?op=bdBlue">  
         <div class="imgButton divBase colorButtonSize row8 right" style="background-image:url('dilog_buttons/dk_blue.png')">
         </div>
      </a>

   </body>
</html>


