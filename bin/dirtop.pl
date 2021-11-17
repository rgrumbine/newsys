#!/usr/bin/perl
#Robert Grumbine

while ( $line = <STDIN> ) {
  chop ($line);
  @full = split(/\s+/,$line);
  @path = split(/\//,$full[$#full]) ;
  $n    = 1;
  #print $full[1], " ", $#full,"\n";

  $name=$path[0];
  for ($i = 1; $i < $#path ; $i++) {
    $name = $name . "/" . $path[$i];
  }
  #print $line," ",$n," ",$name,"\n";

  $counts{$name} += $n;

}

while ( ($key, $value) = each(%counts) ) {
  printf "%8d of %s\n",$value, $key;
}
