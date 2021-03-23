<?php

echo "Current PHP version: " . phpversion();

?>
<?php

/* ini_set("display_errors",1);
error_reporting(E_ALL);

// Define all commands, messages and op-strings
class CommandStruct
{
   public $op;
   public int $messageId;
   public $argument;   
   

   public function __construct(int $id,  $op, $argument)
   {
      $this->op = $op;
      $this->id = $id;
      $this->argument = $argument;
   }

   function getCommand()
   {
      $len = 3 + strlen($this->argument);
      $cmdStr = pack("CCna", 0x5a, $len, $this->id, $this->argument);
      return $cmdStr;
   }
}

$array = array(
   new CommandStruct(0x21, "dilog_rec", ""),
   new CommandStruct(0x22, "dilog_play", ""),
   new CommandStruct(0x23, "dilog_pause", ""),
   new CommandStruct(0x24, "dilog_rew", ""),
   new CommandStruct(0x25, "dilog_ff", ""),
   );
   
print_r($array);

 */?>

