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

      <a href="index.php">  
         <div class="imgButton divBase turnOffButton" style="background-image:url('back_button.png')">
         </div>
      </a>

      <a href="script.php">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('refresh_button.png')">
         </div>
      </a>

      <a href="volume.php?op=<?php echo("is" . $volume)?>">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_button.png')">
         </div>
      </a>

<?php 
      //<a href="script.php?op=$test">  
      //   <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('dilog_buttons/dk_rec.png')">
      //   </div>
      //</a>
      //<a href="script.php?op=play_script">  
      //   <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_play.png')">
      //   </div>
      //</a>

if ($scriptState == "1")
{
//   $test = $_GET['scrSel'];
   $test = "hello";
   echo <<<SCRIPTSTATEIDLE
      <form action="/script.php">
         <input type="text" id="opRecScript" name="opRecScript" 
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
         <input type="submit" value=""
            class="imgButton divBase row1TrippleSplit right myButton"
            style="background-image:url('dilog_buttons/dk_rec.png')">
         </input>
      </form>

      <form action="/script.php">
         <select id="opPlayScript" name="opPlayScript" 
            class="imgButton divBase row2TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
            <option value="bladerunner">bladerunner</option>
            <option value="redsquare">redsquare</option>
            <option value="pippi">pippi</option>
         </select>
         <input type="submit" value=""
            class="imgButton divBase row2TrippleSplit right myButton"
            style="background-image:url('dilog_buttons/dk_play.png')">
         </input>
      </form>

      <div class="imgButton divBase row3TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png'); opacity:0.5;">
      </div>
SCRIPTSTATEIDLE;
}
else if ($scriptState == "2")
{
  echo <<<SCRIPTSTATERECORDING
      <form action="/script.php">
         <input type="text" id="opRecScript" name="opRecScript" 
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
      </form>

      <form action="/script.php">
         <select id="opPlayScript" name="opPlayScript" 
            class="imgButton divBase row2TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
            <option value="bladerunner">bladerunner</option>
            <option value="redsquare">redsquare</option>
            <option value="pippi">pippi</option>
         </select>
      </form>
    
      <a href="script.php?op=stop_rec_script">  
         <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png')">
         </div>
      </a>

      <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_pause.png'); opacity:0.5;">
      </div>

      <div class="imgButton divBase row3TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png'); opacity:0.5;">
      </div>

SCRIPTSTATERECORDING;
}   
else if ($scriptState == "3")
{
  echo <<<SCRIPTSTATEPLAYING
      <form action="/script.php">
         <input type="text" id="opRecScript" name="opRecScript" 
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
      </form>

      <form action="/script.php">
         <select id="opPlayScript" name="opPlayScript" 
            class="imgButton divBase row2TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
            <option value="bladerunner">bladerunner</option>
            <option value="redsquare">redsquare</option>
            <option value="pippi">pippi</option>
         </select>
      </form>
    
      <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('dilog_buttons/dk_rec.png'); opacity:0.5;">
      </div>

      <a href="script.php?op=pause">  
         <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_pause.png')">
         </div>
      </a>

      <a href="script.php?op=stop_script">  
         <div class="imgButton divBase row3TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>
SCRIPTSTATEPLAYING;
}
else if ($scriptState == "4")
{
  echo <<<SCRIPTSTATEPAUSED
      <form action="/script.php">
         <input type="text" id="opRecScript" name="opRecScript" 
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
      </form>

      <form action="/script.php">
         <select id="opPlayScript" name="opPlayScript" 
            class="imgButton divBase row2TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
            <option value="bladerunner">bladerunner</option>
            <option value="redsquare">redsquare</option>
            <option value="pippi">pippi</option>
         </select>
      </form>
    
      <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('dilog_buttons/dk_rec.png'); opacity:0.5;">
      </div>

      <a href="script.php?op=play_script">  
         <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_play.png')">
         </div>
      </a>

      <a href="script.php?op=stop_script">  
         <div class="imgButton divBase row3TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>
SCRIPTSTATEPAUSED;
}
if (false)
{
  echo <<<NOSCRIPTSELECTED
      <div class="divBase row3TrippleSplit left" style="width: 58vw; height: 27vw; background-image:url('no_script_ticket.png'); background-size: 100% 100%; "> 
      </div>
NOSCRIPTSELECTED;
}
else if (false)
{
  echo <<<ENTERSCRIPTNAME
      <div class="divBase row3TrippleSplit left" style="width: 58vw; height: 27vw; background-image:url('enter_script_ticket.png'); background-size: 100% 100%; "> 
      </div>
ENTERSCRIPTNAME;
}
?>

   </body>
</html>


