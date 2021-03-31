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

<?php 
if ($theatreState == "1")
{
   if ($scriptState == "1")
   {
      echo <<<SCRIPTSTATEIDLE1
         <form action="/feature_show.php">
            <input type="submit" name="op" value="startTheatreProgram"
               class="imgButton divBase row3TrippleSplit right myButton"
               style="background-image:url('dilog_buttons/dk_ok.png'); text-indent: -9999px;">
            </input>
            <select id="arg" name="arg" 
               class="imgButton divBase row2TrippleSplit left selectBig"
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
            <select id="arg2" name="arg2" 
               class="imgButton divBase row3TrippleSplit left selectSmall"
               style="font-size: 8vw; color:#4d4d4d">
               <option value="00">00</option>
               <option value="01">01</option>
               <option value="02">02</option>
               <option value="03">03</option>
               <option value="04">04</option>
               <option value="05">05</option>
               <option value="06">06</option>
               <option value="07">07</option>
               <option value="08">08</option>
               <option value="09">09</option>
               <option value="10">10</option>
               <option value="11">11</option>
               <option value="12">12</option>
               <option value="13">13</option>
               <option value="14">14</option>
               <option value="15">15</option>
               <option value="16">16</option>
               <option value="17">17</option>
               <option value="18">18</option>
               <option value="19">19</option>
               <option value="20">20</option>
               <option value="21">21</option>
               <option value="22">22</option>
               <option value="23">23</option>
            </select>
            <select id="arg3" name="arg3" 
               class="imgButton divBase row3TrippleSplit middle selectSmall"
               style="font-size: 8vw; color:#4d4d4d">
               <option value="00">00</option>
               <option value="05">05</option>
               <option value="10">10</option>
               <option value="15">15</option>
               <option value="20">20</option>
               <option value="25">25</option>
               <option value="30">30</option>
               <option value="35">35</option>
               <option value="40">40</option>
               <option value="45">45</option>
               <option value="50">50</option>
               <option value="55">55</option>
            </select>
         </form>
SCRIPTSTATEIDLE2;
   }
   else if ($scriptState == "2")
   {
     echo <<<SCRIPTSTATERECORDING1
         <form action="/feature_show.php">
            <input type="text" id="arg" name="arg" value="$scriptName"
               class="myInput imgButton divBase row1TrippleSplit left"
               style="width: 58vw; font-size: 8vw; color:#4d4d4d">
            </input>
         </form>

         <form action="/feature_show.php">
            <select id="arg" name="arg" 
               class="imgButton divBase row2TrippleSplit left selectBig"
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

         <a href="feature_show.php?op=stopRecScript">  
            <div class="imgButton divBase row1TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png')">
            </div>
         </a>

         <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_play.png'); opacity:0.5;">
         </div>


SCRIPTSTATERECORDING2;
   }   
   else if ($scriptState == "3")
   {
     echo <<<SCRIPTSTATEPLAYING1
         <form action="/feature_show.php">
            <input type="text" id="arg" name="arg"
               class="myInput imgButton divBase row1TrippleSplit left"
               style="width: 58vw; font-size: 8vw; color:#4d4d4d">
            </input>
         </form>

         <form action="/feature_show.php">
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

         <a href="feature_show.php?op=stopPlayScript">  
            <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
            </div>
         </a>
SCRIPTSTATEPLAYING2;
   }
   else if ($scriptState == "4")
   {
     echo <<<SCRIPTSTATEPAUSED1
         <form action="/feature_show.php">
            <input type="text" id="arg" name="arg"
               class="myInput imgButton divBase row1TrippleSplit left"
               style="width: 58vw; font-size: 8vw; color:#4d4d4d">
            </input>
         </form>

         <form action="/feature_show.php">
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

         <a href="feature_show.php?op=continueScript">  
            <div class="imgButton divBase row2TrippleSplit right" style="background-image:url('dilog_buttons/dk_play.png')">
            </div>
         </a>

         <a href="feature_show.php?op=stopPlayScript">  
            <div class="imgButton divBase row3TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
            </div>
         </a>
SCRIPTSTATEPAUSED2;
   }
}
else
{
   if (($theatreState == "2") || ($theatreState == "3"))
   {
      echo <<<THEATREARMED

      
      <div class="imgButton divBase left" style="width: 92vw; height: 30vh; top: 26vh; background-image:url('armed_theatre_ticket.png');" >
      </div>
      
      <div class="divBase" style="top: 33vh; left: 16vw;" >
         <p class="ticketFeature" style="color: #552200;">$theatreFeature</p>
      </div>

      <div class="divBase" style="top: 38vh; left: 16vw;" >
         <p class="ticketStartsAt" style="color: #552200;">Starts at: $startHour:$startMinute</p>
      </div>

      <a href="feature_show.php?op=pauseTheatreProgram">  
         <div class="imgButton divBase row5DualSplit left" style="background-image:url('dilog_buttons/dk_pause.png');">
         </div>
      </a>

      <a href="feature_show.php?op=abortTheatreProgram">  
         <div class="imgButton divBase row5DualSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>
     
THEATREARMED;
   }

   // Now show theatre mode page
   else if ($theatreState == "4")
   {
      echo <<<THEATFEATUREE

      <div class="imgButton divBase left" style="width: 92vw; height: 43vw; top: 26vh; background-image:url('now_showing_billboard.png');" >
      </div>
      
      <div class="divBase" style="top: 32vh; left: 15vw;" >
         <p class="ticketFeature" style="color: black;">$theatreFeature</p>
      </div>

      <a href="feature_show.php?op=pauseTheatreProgram">  
         <div class="imgButton divBase row5TrippleSplit left" style="background-image:url('dilog_buttons/dk_pause.png');">
         </div>
      </a>

      <a href="feature_show.php?op=abortTheatreProgram">  
         <div class="imgButton divBase row5TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>
     
THEATFEATUREE;
   }
   else if ($theatreState == "5")
   {
      echo <<<PAUSEFEATURE

      <div class="imgButton divBase left" style="width: 92vw; height: 43vw; top: 26vh; background-image:url('now_showing_billboard.png');" >
      </div>
      
      <div class="divBase" style="top: 32vh; left: 15vw;" >
         <p class="ticketFeature" style="color: black;">$theatreFeature</p>
      </div>

      <a href="feature_show.php?op=continueTheatreProgram">  
         <div class="imgButton divBase row5TrippleSplit left" style="background-image:url('dilog_buttons/dk_play.png');">
         </div>
      </a>

      <a href="feature_show.php?op=abortTheatreProgram">  
         <div class="imgButton divBase row5TrippleSplit right" style="background-image:url('dilog_buttons/dk_stop.png');">
         </div>
      </a>
     
PAUSEFEATURE;
   }
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
      <a href="script.php">  
         <div class="imgButton divBase row6DualSplit right" style="background-image:url('script_button.png')">
         </div>
      </a>

   </body>
</html>


