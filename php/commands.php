<?php
class CommandStruct
{
   public $op;
   public $numOfArgs;
   public $messageId;
   public $argument;   
   

   public function __construct(int $id, $op, int $numOfArgs, $argument)
   {
      $this->op = $op;
      $this->id = $id;
      $this->numOfArgs = $numOfArgs;
      $this->argument = $argument;
   }

   function getCommand()
   {
      $len = 4 + strlen($this->argument);
      $cmdStr = pack("CCna*", 0x5a, $len, $this->id, $this->argument);
      return $cmdStr;
   }
}

$cmdArray = array(
   new CommandStruct(0x00, "statusReq",      0, ""),
   new CommandStruct(0x01, "lightAllOn",     0, ""),
   new CommandStruct(0x02, "lightAllOff",    0, ""),
   new CommandStruct(0x03, "lightPreMovie",  0, ""),
   new CommandStruct(0x04, "lightMovie",     0, ""),
   new CommandStruct(0x05, "lightPause",     0, ""),
   new CommandStruct(0x06, "lightKidsMovie", 0, ""),
   new CommandStruct(0x07, "lightEndCred",   0, ""),
   new CommandStruct(0x08, "ledStrip",       1, ""),
   new CommandStruct(0x10, "system_off",     0, ""),
   new CommandStruct(0x11, "volUp",          0, ""),
   new CommandStruct(0x12, "volDown",        0, ""),
   new CommandStruct(0x13, "volSet",         1, ""),
   new CommandStruct(0x14, "sourcePs",       0, ""),
   new CommandStruct(0x15, "sourceCc",       0, ""),
   new CommandStruct(0x16, "sourceTv",       0, ""),
   new CommandStruct(0x17, "sourceSpotify",  0, ""),
   new CommandStruct(0x18, "sourceVinyl",    0, ""),
   new CommandStruct(0x19, "sourceTuner",    0, ""),
   new CommandStruct(0x1a, "sourceRpi",      0, ""),
   new CommandStruct(0x1b, "sourceAux",      0, ""),
   new CommandStruct(0x1c, "sourceBr",       0, ""),
   new CommandStruct(0x20, "dilogPower",     0, ""),
   new CommandStruct(0x21, "dilogMute",      0, ""),
   new CommandStruct(0x22, "dilogList",      0, ""),
   new CommandStruct(0x23, "dilogTime",      0, ""),
   new CommandStruct(0x24, "dilog1",         0, ""),
   new CommandStruct(0x25, "dilog2",         0, ""),
   new CommandStruct(0x26, "dilog3",         0, ""),
   new CommandStruct(0x27, "dilog4",         0, ""),
   new CommandStruct(0x28, "dilog5",         0, ""),
   new CommandStruct(0x29, "dilog6",         0, ""),
   new CommandStruct(0x2a, "dilog7",         0, ""),
   new CommandStruct(0x2b, "dilog8",         0, ""),
   new CommandStruct(0x2c, "dilog9",         0, ""),
   new CommandStruct(0x2d, "dilog0",         0, ""),
   new CommandStruct(0x2e, "dilogInfo",      0, ""),
   new CommandStruct(0x2f, "dilogMenu",      0, ""),
   new CommandStruct(0x30, "dilogEpg",       0, ""),
   new CommandStruct(0x31, "dilogUp",        0, ""),
   new CommandStruct(0x32, "dilogExit",      0, ""),
   new CommandStruct(0x33, "dilogLeft",      0, ""),
   new CommandStruct(0x34, "dilogOk",        0, ""),
   new CommandStruct(0x35, "dilogRight",     0, ""),
   new CommandStruct(0x36, "dilogSubtitle",  0, ""),
   new CommandStruct(0x37, "dilogDown",      0, ""),
   new CommandStruct(0x38, "dilogText",      0, ""),
   new CommandStruct(0x39, "dilogStop",      0, ""),
   new CommandStruct(0x3a, "dilogPlay",      0, ""),
   new CommandStruct(0x3b, "dilogPause",     0, ""),
   new CommandStruct(0x3c, "dilogRecord",    0, ""),
   new CommandStruct(0x3d, "dilogRed",       0, ""),
   new CommandStruct(0x3e, "dilogGreen",     0, ""),
   new CommandStruct(0x3d, "dilogYellow",    0, ""),
   new CommandStruct(0x40, "dilogBlue",      0, ""),
   new CommandStruct(0x41, "dilogNext",      0, ""),
   new CommandStruct(0x42, "dilogPrevious",  0, ""),
   new CommandStruct(0x50, "bdPower",        0, ""),
   new CommandStruct(0x51, "bdSubtitle",     0, ""),
   new CommandStruct(0x52, "bdEject",        0, ""),
   new CommandStruct(0x53, "bdRewind",       0, ""),
   new CommandStruct(0x54, "bdPlay",         0, ""),
   new CommandStruct(0x55, "bdFf",           0, ""),
   new CommandStruct(0x56, "bdPrevious",     0, ""),
   new CommandStruct(0x57, "bdPause",        0, ""),
   new CommandStruct(0x58, "bdStop",         0, ""),
   new CommandStruct(0x59, "bdNext",         0, ""),
   new CommandStruct(0x5a, "bdTopMenu",      0, ""),
   new CommandStruct(0x5b, "bdUp",           0, ""),
   new CommandStruct(0x5c, "bdBack",         0, ""),
   new CommandStruct(0x5d, "bdLeft",         0, ""),
   new CommandStruct(0x5e, "bdOk",           0, ""),
   new CommandStruct(0x5f, "bdRight",        0, ""),
   new CommandStruct(0x60, "bdPopupMenu",    0, ""),
   new CommandStruct(0x70, "bdDown",         0, ""),
   new CommandStruct(0x71, "bdOptions",      0, ""),
   new CommandStruct(0x72, "bdRed",          0, ""),
   new CommandStruct(0x73, "bdGreen",        0, ""),
   new CommandStruct(0x74, "bdYellow",       0, ""),
   new CommandStruct(0x75, "bdBlue",         0, ""),
   new CommandStruct(0x76, "bdPowerOnLan",   0, ""),
   new CommandStruct(0x80, "playScript",     1, ""),
   new CommandStruct(0x81, "pauseScript",    0, ""),
   new CommandStruct(0x82, "stopPlayScript", 0, ""),
   new CommandStruct(0x83, "recScript",      3, ""),
   new CommandStruct(0x84, "stopRecScript",  0, ""),   
   new CommandStruct(0x85, "continueScript", 0, ""),   
   new CommandStruct(0x86, "startTheatreProgram", 3, ""),
   new CommandStruct(0x87, "abortTheatreProgram", 0, ""),
   new CommandStruct(0x88, "pauseTheatreProgram", 0, ""),
   new CommandStruct(0x89, "continueTheatreProgram", 0, "")   
);
   
// print_r($array);


// $cmdStr = $array[2]->getCommand();

// $unpacked = unpack("Cmagic/Clen/nid/a*argument", $cmdStr);

// print_r($unpacked);
?>

