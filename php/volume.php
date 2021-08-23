<?php
ini_set("display_errors",1);
error_reporting(E_ALL);

include './comms.php';

// For safety
if (abs(floatval($volume)) < 0.01)
{
   $volume = "-40.0";
}

$newM5Vol = floatval($volume) - 5.0;
if ($newM5Vol < "-80.0")
{
   $newM5Vol = "-80.0";
}

$newM1Vol = floatval($volume) - 1.0;
if ($newM1Vol < "-80.0")
{
   $newM1Vol = "-80.0";
}

$newP5Vol = floatval($volume) + 5.0;
if ($newP5Vol > "16.5")
{
   $newP5Vol = "16.5";
}

$newP1Vol = floatval($volume) + 1.0;
if ($newP1Vol > "16.5")
{
   $newP1Vol = "16.5";
}

?>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
{ margin: 0; padding: 0; }

html { 
  background: url('curtains.jpg') no-repeat center center fixed; 
  -webkit-background-size: cover;
  -moz-background-size: cover;
  -o-background-size: cover;
  background-size: cover;
}

.slidecontainer {
  width: 80%;
  position: fixed;
  left: 50%;
  transform: translate(-50%);
  
}

.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 1vh;
  background: #505050;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  background: url('pointer.png');
  background-size: 100% 100%;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: url('pointer.png');
  background-size: 100% 100%;
  cursor: pointer;
}
</style>
   <link rel="stylesheet" type="text/css" href="fhx_manager.css" >
</head>
<body>

   <a href="index.php">
      <div class="imgButton divBase turnOffButton" style="background-image:url('back_button.png')">
      </div>
   </a>

   <div class="slidecontainer" style="top: 28vh;">
     <input type="range" min="-160" max="33" value="<?php echo ($volume * 2) ?>" class="slider" id="myRange">
     <div  style="margin: auto; overflow: hidden; width: 80%;">
        <p style = "float: left; font-family: Tahoma, sans-serif; font-size: 3vh; font-style: bold; color: #d0d0d0; ">Volume:</p>
        <p style = "float: right; font-family: Tahoma, sans-serif; font-size: 3vh; font-style: bold; color: #d0d0d0;"><span id="demo"></span></p>
     </div>
   </div>

   <a href="#" id='DisplaySliderResult' onClick='changeLink()'>
      <div class="imgButton divBase row1Vol" style="background-image:url('set_volume_button.png')">
      </div>
   </a>

   <a  href="volume.php?op=volSet&arg=<?php echo($newM5Vol); ?>">
      <div class="imgButton divBase row2Vol left" style="background-image:url('minus5bd_button.png')">
      </div>
   </a>

   <a  href="volume.php?op=volSet&arg=<?php echo($newP5Vol); ?>">
      <div class="imgButton divBase row2Vol right" style="background-image:url('plus5db_button.png')">
      </div>
   </a>

   <a  href="volume.php?op=volSet&arg=<?php echo($newM1Vol); ?>">
      <div class="imgButton divBase row3Vol left" style="background-image:url('minus1bd_button.png')">
      </div>
   </a>

   <a  href="volume.php?op=volSet&arg=<?php echo($newP1Vol); ?>">
      <div class="imgButton divBase row3Vol right" style="background-image:url('plus1db_button.png')">
      </div>
   </a>

   <div class="imgButton divBase" style="height: 8vh; width: 28vw; top: 67vh; left: 36vw; background-image:url('volume_symbol.png')">
   </div>


<script>
var slider = document.getElementById("myRange");
var output = document.getElementById("demo");
output.innerHTML = slider.value / 2.0;

slider.oninput = function() {
  output.innerHTML = this.value / 2.0;
}

function changeLink() {
   var slider = document.getElementById("myRange");
   window.location.href = "volume.php?op=volSet&arg=" + (slider.value / 2.0) .toString();
}

</script>

</body>
</html>