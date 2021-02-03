<?php
ini_set("display_errors",1);
error_reporting(E_ALL);

if (isset($_GET["op"]))
{
   $volume = $_GET["op"];
}
else
{
   // Default value is non is provided
   $volume = "-80.0";
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
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 25px;
  background: #d3d3d3;
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
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}
</style>
   <link rel="stylesheet" type="text/css" href="fhx_manager.css" >
</head>
<body>

<div class="slidecontainer">
  <input type="range" min="-160" max="33" value="<?php echo ($volume * 2) ?>" class="slider" id="myRange">
  <p style = "font-family: Tahoma, sans-serif; font-size: 6vw; font-style: bold; color: white; text-align: center;">Volume:</p>
  <p style = "font-family: Tahoma, sans-serif; font-size: 6vw; font-style: bold; color: white; text-align: center;"><span id="demo"></span></p>
</div>

<a href="#" id='DisplaySliderResult' onClick='changeLink()'>
   <div class="imgButton divBase row1NoSplit" style="background-image:url('set_volume_button.png')">
   </div>
</a>

<a href="index.php">
   <div class="imgButton divBase row2NoSplit" style="background-image:url('back_button.png')">
   </div>
</a>

<script>
var slider = document.getElementById("myRange");
var output = document.getElementById("demo");
output.innerHTML = slider.value / 2.0;

slider.oninput = function() {
  output.innerHTML = this.value / 2.0;
}

function changeLink() {
   var slider = document.getElementById("myRange");

   window.location.href = "index.php?op=vol" + (slider.value / 2.0) .toString();
}

</script>

</body>
</html>