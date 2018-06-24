#!/bin/awk -f
function process(l) {
  l = gensub(/\]/, "", "g", gensub(/\[/, "", "g", l));
  split(l, w, " ", seps);
  cts = w[1];
  if (!f && cts == lts) {
    f = 1;
    return;
  }
  if (f) {
    lts = cts;
    print l >> "dmesg_out";
  }
}
BEGIN {
  while (1) {
    getline lts < "dmesg_ts";
    close("dmesg_ts");
    f = length(lts) == 0 ? 1 : 0;

    while (("dmesg" | getline l) > 0) {
      process(l);                      
    }            
    close("dmesg");
                   
    print lts > "dmesg_ts";
    close("dmesg_ts");     
                      
    system("usleep 500000");
  }                         
}

