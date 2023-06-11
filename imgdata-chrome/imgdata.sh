#!/bin/bash

# extract pixel data from an image using chrome headless
# use:
#   $0 <URI> | xmllint --html --xpath 'string(//body)' - | perl -ne 'foreach(split(/\s/)){print(hex($_)?"*":" ")}print"\n"'

FPATH=$1

[ "x" = "x$FPATH" ] && exit 1

CHROME=$(mdfind "kMDItemCFBundleIdentifier == 'com.google.Chrome'")

[ "x" = "x$CHROME" ] && exit 2

CHROME="$CHROME/Contents/MacOS/Google Chrome"

[ ! -x "$CHROME" ] && exit 3

DIR=$(dirname $(readlink -f "${BASH_SOURCE[0]}"))
IMG=$(echo $FPATH | awk -v DIR=$DIR '{ if ($1 ~ /^.+:/) { print $0; } else { print "file://"DIR"/"$1; } }')

if [ "x$(echo $IMG | grep -q -E '^file\:' && echo 1)" = "x1" ]; then
  IMG="data:$(file --mime-type $FPATH | awk '{ print $2 }');base64,$(cat $FPATH | base64)"
fi

IFS='' read -r -d '' HTML <<EOF
<html>
<body>
<script>
try {
  var t = "";
  var img = new Image();
  img.onload = function() {
    function h(n) { return n.toString(16).padStart(2, '0'); }
    var canvas = document.createElement('canvas');
    canvas.width = this.width;
    canvas.height = this.height;
    canvas.getContext('2d').drawImage(this, 0, 0);
    var d = canvas.getContext('2d').getImageData(0, 0, this.width, this.height).data;
    for (var i=0; i<d.length/4; i++) {
      t += (i&&i%this.width==0?"":" ")+"0x"+h(d[i*4+0])+h(d[i*4+1])+h(d[i*4+2])+h(d[i*4+3])+((i+1)%this.width==0?"\n":"");
    }
    document.open();
    document.write(t);
    document.close();
  };
  img.crossOrigin = "anonymous";
  img.src = window.location.hash.substr(1) || "$IMG";
} catch(e) {
  document.body.innerHTML = e;
}
</script>
</body>
</html>
EOF

URI="data:text/html;charset=utf-8;base64,$(echo -n $HTML | base64)"

"$CHROME" --args \
  --headless \
  --default-background-color=00000000 \
  --disable-web-security \
  --run-all-compositor-stages-before-draw \
  --allow-file-access-from-files \
  --enable-local-file-accesses \
  --virtual-time-budget=25000 \
  --dump-dom \
  "$URI" \
  2>/dev/null

