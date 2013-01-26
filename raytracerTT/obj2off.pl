#! /usr/bin/perl

$eol = "\n";	# or "\r\n" for Windows or "\r" for Mac

if(-t STDIN && ! -f $ARGV[0]) {
  print STDERR "Usage:  $0   file.obj  > file.off
Converts a Wavefront .wf 3-D model into geomview/OOGL \"OFF\" format.
Discards materials, normals, texture coordinates.
";
  exit(1);
}


while(<>) {
  @F = split(' ');
  if($F[0] eq "v" && @F>=4) {
    push(@v, join(" ", @F[1..3]));
  } elsif($F[0] eq "f") {
    local(@tf, $_);
    shift(@F);
    foreach $_ (@F) {
	s'/.*'';
	$_ -= 1;
	push(@tf, $_);
    }
    push(@f, join(" ", (0+@tf), @tf));
  }
}

if(@f && @v) {
  print "OFF\n";
  printf "%d %d 0\n\n", (0+@v), (0+@f);
  while(@v) {
    print shift(@v), "\n";
  }
  print "\n";
  while(@f) {
    print shift(@f), "\n";
  }
} else {
  print STDERR "$ARGV doesn't look like a .obj file?\n";
}
