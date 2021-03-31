<?php
ini_set("display_errors",1);
error_reporting(E_ALL);

include './commands.php';

$operation = "lightAllOn";
$arg = "test-arg";

// Find op in all commands:

$foundOp = false;
foreach ($cmdArray as &$value) 
{
   if ($value->op == $operation)
   {
      if ($value->hasArgs)
      {
         $value->argument = $arg;
      }
      
      $command = $value->getCommand();
      
      $foundOp = true;
      
      break;
   }
}

if (!$foundOp)
{
   $command = $statusRequest;
}

print_r($command); 

$unpacked = unpack("Cmagic/Clen/nid/a*argument", $command);

print_r($unpacked);

?>
