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

      <a href="feature_show.php">  
         <div class="imgButton divBase turnOffButton" style="background-image:url('back_button.png')">
         </div>
      </a>

      <a href="script.php">  
         <div class="imgButton divBase volumeUpButton" style="background-image:url('refresh_button.png')">
         </div>
      </a>

      <a href="volume.php">  
         <div class="imgButton divBase volumeDownButton" style="background-image:url('vol_button.png')">
         </div>
      </a>

<?php 
 
if ($scriptState == "1")
{
   echo <<<SCRIPTSTATEIDLE1
      <form action="/script.php">
         <input type="submit" name="op" value="recScript"
            class="imgButton divBase row3TrippleSplit right myButton"
            style="background-image:url('dilog_buttons/dk_rec.png'); text-indent: -9999px;">
         </input>
         <input type="text" id="arg2" name="arg2" value="$featureDuration"
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
         <input type="text" id="arg3" name="arg3" value="$endCredits"
            class="myInput imgButton divBase row2TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
         <input type="text" id="arg" name="arg" value="$scriptName"
            class="myInput imgButton divBase row3TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
      </form>

      <form action="/script.php">
         <input type="submit" name="op" value="playScript"
            class="imgButton divBase row4TrippleSplit right myButton"
            style="background-image:url('dilog_buttons/dk_play.png'); text-indent: -9999px;">
         </input>
         <select id="arg" name="arg" 
            class="imgButton divBase row4TrippleSplit left selectBig"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
SCRIPTSTATEIDLE1;
   foreach ($scannedDirectory as $value)
   {
      echo('            <option ');
      
      if ($value == $scriptName)
      {
         echo('selected="selected" ');
      }

      echo('value="' . $value . '">' . $value . '</option>\n');
   }
   echo <<<SCRIPTSTATEIDLE2
         </select>
      </form>

      <div class="divBase row1TrippleSplit right" style="top: 21vh;">
         <p class="durationLabel" style="color: #ffffff;">Feature duration</p>
      </div>

      <div class="divBase row1TrippleSplit right" style="top: 24vh;">
         <p class="durExLabel" style="color: #ffffff;">1.52.45</p>
      </div>

      <div class="divBase row2TrippleSplit right" style="top: 36vh;">
         <p class="durationLabel" style="color: #ffffff;">End credits</p>
      </div>

      <div class="divBase row2TrippleSplit right" style="top: 39vh;">
         <p class="durExLabel" style="color: #ffffff;">1.48.37</p>
      </div>

SCRIPTSTATEIDLE2;
}
else if ($scriptState == "2")
{
  echo <<<SCRIPTSTATERECORDING1
      <form action="/script.php">
         <input type="text" id="arg2" name="arg2" value="$featureDuration"
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
         <input type="text" id="arg3" name="arg3" value="$endCredits"
            class="myInput imgButton divBase row2TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d"">
         </input>
         <input type="text" id="arg" name="arg" value="$scriptName"
            class="myInput imgButton divBase row3TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
         </input>
      </form>

      <form action="/script.php">
         <select id="arg" name="arg" 
            class="imgButton divBase row4TrippleSplit left selectBig"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
SCRIPTSTATERECORDING1;
   foreach ($scannedDirectory as $value)
   {
      echo('            <option ');
      
      if ($value == $scriptName)
      {
         echo('selected="selected" ');
      }

      echo('value="' . $value . '">' . $value . '</option>\n');
   }
   echo <<<SCRIPTSTATERECORDING2
         </select>
      </form>

      <a href="script.php?op=stopRecScript">  
         <div class="imgButton divBase row3TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png')">
         </div>
      </a>

      <div class="imgButton divBase row4TrippleSplit right" style="background-image:url('dilog_buttons/dk_play.png'); opacity:0.5;">
      </div>

      <div class="divBase row1TrippleSplit right" style="top: 21vh;">
         <p class="durationLabel" style="color: #ffffff;">Feature duration</p>
      </div>

      <div class="divBase row1TrippleSplit right" style="top: 24vh;">
         <p class="durExLabel" style="color: #ffffff;">1.52.45</p>
      </div>

      <div class="divBase row2TrippleSplit right" style="top: 36vh;">
         <p class="durationLabel" style="color: #ffffff;">End credits</p>
      </div>

      <div class="divBase row2TrippleSplit right" style="top: 39vh;">
         <p class="durExLabel" style="color: #ffffff;">1.48.37</p>
      </div>


SCRIPTSTATERECORDING2;
}   
else if ($scriptState == "3")
{
  echo <<<SCRIPTSTATEPLAYING1
      <form action="/script.php">
         <input type="text" id="arg" name="arg"
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
         </input>
      </form>

      <form action="/script.php">
         <select id="arg" name="arg" value="$scriptName"
            class="imgButton divBase row2TrippleSplit left selectBig"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
SCRIPTSTATEPLAYING1;
   foreach ($scannedDirectory as $value)
   {
      echo('            <option ');
      
      if ($value == $scriptName)
      {
         echo('selected="selected" ');
      }

      echo('value="' . $value . '">' . $value . '</option>\n');
   }
   echo <<<SCRIPTSTATEPLAYING2
         </select>
      </form>

      <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('dilog_buttons/dk_rec.png'); opacity:0.5;">
      </div>

      <a href="script.php?op=stopPlayScript">  
         <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>
SCRIPTSTATEPLAYING2;
}
else if ($scriptState == "4")
{
  echo <<<SCRIPTSTATEPAUSED1
      <form action="/script.php">
         <input type="text" id="arg" name="arg"
            class="myInput imgButton divBase row1TrippleSplit left"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
         </input>
      </form>

      <form action="/script.php">
         <select id="arg" name="arg" value="$scriptName"
            class="imgButton divBase row2TrippleSplit left selectBig"
            style="width: 58vw; font-size: 8vw; color:#4d4d4d">
SCRIPTSTATEPAUSED1;
   foreach ($scannedDirectory as $value)
   {
      echo('            <option ');
      
      if ($value == $scriptName)
      {
         echo('selected="selected" ');
      }

      echo('value="' . $value . '">' . $value . '</option>\n');
   }
   echo <<<SCRIPTSTATEPAUSED2
         </select>
      </form>
      <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('dilog_buttons/dk_rec.png'); opacity:0.5;">
      </div>

      <a href="script.php?op=continueScript">  
         <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_play.png')">
         </div>
      </a>

      <a href="script.php?op=stopPlayScript">  
         <div class="imgButton divBase row3TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>
SCRIPTSTATEPAUSED2;
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


