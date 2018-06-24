// true if output should use canvas size (see csize)
var target_scaled = true;

// commands array
var commands = new Array()

// stroke settings
var strokeWidth = 10;
var strokeColor = "black";

// target ratio
var ratio = 0.5;
// target scale
var sbscale = target_scaled ? 1.0 : 1.0/ratio;
// target size of canvas
var tsize = { w:1920, h:1080 };
// canvas size
var csize = { w:tsize.w*ratio, h:tsize.h*ratio };
// original rect size
var orsize = { w:ratio*360, h:ratio*768 };
// rect size
var rsize = { w:csize.w * (orsize.w / tsize.w), h:csize.h * (orsize.h / tsize.h) };

var wh = { w:30, h:60 };
var pos = { x:160, y: 270-wh.h };
//var hpad = ratio*40;

// left curve
var lpt1 = { x:pos.x, y:pos.y };
var lcp1 = { x:pos.x-wh.w*0.75, y:pos.y+wh.h };
var lcp2 = { x:pos.x-wh.w*0.75, y:pos.y };
var lpt2 = { x:pos.x, y:pos.y+wh.h };
var lpt3 = { x:pos.x, y:pos.y+wh.h*2.0 };
var lcp3 = { x:pos.x+wh.w*0.75, y:pos.y };
var lcp4 = { x:pos.x+wh.w*0.75, y:pos.y+wh.h };
var lcp5 = { x:pos.x-wh.w*0.75, y:pos.y+wh.h*2.0 };
var lcp6 = { x:pos.x+wh.w*0.75, y:pos.y+wh.h*2.0 };
var lpt4 = vbezier(0.5, lpt2, lcp1, lcp2, lpt1);

// right curve (update values so they mirror the values from the left side)
var rpt1 = { x:csize.w-lpt1.x, y:lpt1.y };
var rcp1 = { x:rpt1.x+wh.w*0.75, y:lpt2.y };
var rcp2 = { x:rpt1.x+wh.w*0.75, y:lpt2.y-wh.h };
var rpt2 = { x:rpt1.x, y:lpt2.y };
var rcp3 = { x:rpt1.x-wh.w*0.75, y:rcp2.y };
var rcp4 = { x:rpt2.x-wh.w*0.75, y:rcp1.y };
var rpt3 = { x:rpt1.x, y:lpt3.y };
var rcp5 = { x:rcp4.x, y:rpt3.y };
var rcp6 = { x:rcp1.x, y:rpt3.y };
var rpt4 = vbezier(0.5, rpt2, rcp1, rcp2, rpt1);

// split curves
var ptsleft = beziersplit(0.5, lpt2, lcp1, lcp2, lpt1);
var ptsright = beziersplit(0.5, rpt2, rcp1, rcp2, rpt1);

var lastpos = pos;

var si = 0;
var i = si;
var t = 0;
var vel = 0.05;

function lerp(a, b, t)
{
  var s = 1 - t;
  var ret = { x:a.x*s + b.x*t, y:a.y*s + b.y*t };
  return ret;
}

function vbezier(t, p0, p1, p2, p3)
{
  var cX = 3 * (p1.x - p0.x),
      bX = 3 * (p2.x - p1.x) - cX,
      aX = p3.x - p0.x - cX - bX;

  var cY = 3 * (p1.y - p0.y),
      bY = 3 * (p2.y - p1.y) - cY,
      aY = p3.y - p0.y - cY - bY;

  var x = (aX * Math.pow(t, 3)) + (bX * Math.pow(t, 2)) + (cX * t) + p0.x;
  var y = (aY * Math.pow(t, 3)) + (bY * Math.pow(t, 2)) + (cY * t) + p0.y;

  var ret = { x: x, y: y };

  return ret;
}

function dotext(str,pt)
{
  text(str, pt.x, pt.y);
}

function dobezier(t, p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, trace)
{
  var plot = vbezier(t, { x:p0x, y:p0y }, { x:p1x, y:p1y }, { x:p2x, y:p2y }, { x:p3x, y:p3y });
  lastpos = { x: plot.x-rsize.w*0.5, y: plot.y-rsize.h*0.5 };

  if (trace)
  {
    stroke(255, 102, 0);
    for (var j=0; j<1; j+=0.05)
    {
      plot = vbezier(j,{ x:p0x, y:p0y }, { x:p1x, y:p1y }, { x:p2x, y:p2y }, { x:p3x, y:p3y });
      point(plot.x, plot.y); 
    }

    stroke(115, 102, 0);
    line(p0x, p0y, p1x, p1y);
    line(p3x, p3y, p2x, p2y);

    if (commands.length < seq.length) {
      logbezier(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y);
    }
  }
  else
  {
    stroke(0, 0, 0);
    rect(lastpos.x, lastpos.y, rsize.w, rsize.h);
  }
}

function dotranslate(t, p0x, p0y, p1x, p1y, trace)
{
  var x = p0x + (t * (p1x - p0x));
  var y = p0y + (t * (p1y - p0y));
  lastpos = { x: x-rsize.w*0.5, y: y-rsize.h*0.5 };

  if (trace)
  {
    stroke(255, 102, 0);
    line(p0x, p0y, p1x, p1y);

    if (commands.length < seq.length) {
      logtranslate(p0x, p0y, p1x, p1y);
    }
  }
  else
  {
    stroke(0, 0, 0);
    rect(lastpos.x, lastpos.y, rsize.w, rsize.h);
  }
}

function donoop(t)
{
  rect(lastpos.x, lastpos.y, rsize.w, rsize.h);
}

function tagSvgPathTranslate(strokeColor, strokeOpacity, strokeWidth, p0s, p1s)
{
  // return "<path stroke='" + strokeColor + "' stroke-opacity='" + strokeOpacity + "' stroke-width='" + strokeWidth + "' marker-end='url(#Triangle)' fill='none' d=\"M" + p0s + " L" + p1s + " Z\" />";
  return "<path stroke='" + strokeColor + "' stroke-opacity='" + strokeOpacity + "' stroke-width='" + strokeWidth + "' marker-end='url(#Triangle)' fill='none' d=\"M" + p0s + " L" + p1s + " \" />";
}

function tagSvgPathBezier(strokeColor, strokeOpacity, strokeWidth, p0s, p1s, p2s, p3s)
{
  // return "<path stroke='" + strokeColor + "' stroke-opacity='" + strokeOpacity + "' stroke-width='" + strokeWidth + "' marker-end='url(#Triangle)' fill='none' d=\"M" + p0s + " C" + p1s + " " + p2s + " " + p3s + " Z\" />";
  return "<path stroke='" + strokeColor + "' stroke-opacity='" + strokeOpacity + "' stroke-width='" + strokeWidth + "' marker-end='url(#Triangle)' fill='none' d=\"M" + p0s + " C" + p1s + " " + p2s + " " + p3s + " \" />";
}

function logbezier(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y)
{
  var strokeOpacity = 0.05 + commands.length / seq.length;

  var p0s = String(p0x*sbscale) + "," + String(p0y*sbscale);
  var p1s = String(p1x*sbscale) + "," + String(p1y*sbscale);
  var p2s = String(p2x*sbscale) + "," + String(p2y*sbscale);
  var p3s = String(p3x*sbscale) + "," + String(p3y*sbscale);

  commands.push(tagSvgPathBezier(strokeColor, strokeOpacity, strokeWidth, p0s, p1s, p2s, p3s));
}

function logtranslate(p0x, p0y, p1x, p1y)
{
  var strokeOpacity = 0.05 + commands.length / seq.length;

  var p0s = String(p0x*sbscale) + "," + String(p0y*sbscale);
  var p1s = String(p1x*sbscale) + "," + String(p1y*sbscale);

  commands.push(tagSvgPathTranslate(strokeColor, strokeOpacity, strokeWidth, p0s, p1s));
}

function beziersplit(t,p0,p1,p2,p3)
{
  var p4 = lerp(p0, p1, t);
  var p5 = lerp(p1, p2, t);
  var p6 = lerp(p2, p3, t);
  var p7 = lerp(p4, p5, t);
  var p8 = lerp(p5, p6, t);
  var p9 = lerp(p7, p8, t);

  var firsthalf = [p0, p4, p7, p9];
  var secondhalf =  [p9, p8, p6, p3];

  return [firsthalf, secondhalf];
}

var seq = [
  // top-left top curve
  function(t,trace) {
    dobezier(t,
      ptsleft[1][0].x, ptsleft[1][0].y,
      ptsleft[1][1].x, ptsleft[1][1].y,
      ptsleft[1][2].x, ptsleft[1][2].y,
      ptsleft[1][3].x, ptsleft[1][3].y,
      trace
    );
  },
  // top-right curve
  function(t,trace) {
    dobezier(t,
      lpt1.x, lpt1.y,
      lcp3.x, lcp3.y,
      lcp4.x, lcp4.y,
      lpt2.x, lpt2.y,
      trace
    );
  },
  // bottom-left
  function(t,trace) {
    dobezier(t,
      lpt2.x, lpt2.y,
      lcp1.x, lcp1.y,
      lcp5.x, lcp5.y,
      lpt3.x, lpt3.y,
      trace
    );
  },
  // bottom-right
  function(t,trace) {
    dobezier(t,
      lpt3.x, lpt3.y,
      lcp6.x, lcp6.y,
      lcp4.x, lcp4.y,
      lpt2.x, lpt2.y,
      trace
    );
  },
  // top-left bottom curve
  function(t,trace) {
    dobezier(t,
      ptsleft[0][0].x, ptsleft[0][0].y,
      ptsleft[0][1].x, ptsleft[0][1].y,
      ptsleft[0][2].x, ptsleft[0][2].y,
      ptsleft[0][3].x, ptsleft[0][3].y,
      trace
    );
  },
  // move-right
  function(t,trace) {
    dotranslate(t,
      ptsleft[0][3].x, ptsleft[0][3].y,
      ptsright[1][0].x, ptsright[1][0].y,
      trace
    );
  },
  // right top-right top curve
  function(t,trace) {
    dobezier(t,
      ptsright[1][0].x, ptsright[1][0].y,
      ptsright[1][1].x, ptsright[1][1].y,
      ptsright[1][2].x, ptsright[1][2].y,
      ptsright[1][3].x, ptsright[1][3].y,
      trace
    );
  },
  // right top-left curve
  function(t,trace) {
    dobezier(t,
      rpt1.x, rpt1.y,
      rcp3.x, rcp3.y,
      rcp4.x, rcp4.y,
      rpt2.x, rpt2.y,
      trace
    );
  },
  // right bottom-right curve
  function(t,trace) {
    dobezier(t,
      rpt2.x, rpt2.y,
      rcp1.x, rcp1.y,
      rcp6.x, rcp6.y,
      rpt3.x, rpt3.y,
      trace
    );
  },
  // right bottom-left curve
  function(t,trace) {
    dobezier(t,
      rpt3.x, rpt3.y,
      rcp5.x, rcp5.y,
      rcp4.x, rcp4.y,
      ptsright[0][0].x, ptsright[0][0].y,
      trace
    );
  },
  // right top-right bottom curve
  function(t,trace) {
    dobezier(t,
      ptsright[0][0].x, ptsright[0][0].y,
      ptsright[0][1].x, ptsright[0][1].y,
      ptsright[0][2].x, ptsright[0][2].y,
      ptsright[0][3].x, ptsright[0][3].y,
      trace
    );
  },
  // move-left
  function(t,trace) {
    dotranslate(t,
      ptsright[0][3].x, ptsright[0][3].y,
      ptsleft[1][0].x, ptsleft[1][0].y,
      trace
    );
  },
];

void setup()
{
  size(csize.w, csize.h);
  strokeWeight(4);
  frameRate(25);

  // write full svg to commands array
  for (var k=0; k < seq.length; k++)
  {
    seq[k](t,true);
  }

  // write svg to console
  var svghdr = "<svg xmlns='http://www.w3.org/2000/svg' width='" + tsize.w * (target_scaled?ratio:1.0) +"' height='" + tsize.h * (target_scaled?ratio:1.0) + "'>";
  svghdr += "<marker id='Trianglea' orient='auto' markerWidth='2' markerHeight='4' refX='0.1' refY='1'>";
  svghdr += "  <path d='M0,0 V2 L1,1 Z' fill='red' />";
  svghdr += "</marker>";
  svghdr += "<marker id='Triangle' orient='auto' markerWidth='2' markerHeight='4' refX='0.5' refY='1.0'>";
  svghdr += "  <path d='M0,0 V1 L0.5,0.5 Z' fill='orange'/>";
  svghdr += "</marker>";

  console.log(svghdr + commands.join("") + "</svg>");

  loop();
}

void draw()
{
  background(195);
  noFill();

  // draw full trace
  for (var k=0; k < seq.length; k++)
  {
    seq[k](t,true);
  }

  // draw pt trace
  dotext("lpt1", lpt1);
  dotext("lpt2", lpt2);
  dotext("lpt3", lpt3);
  dotext("lcp1", lcp1);
  dotext("lcp2", lcp2);
  dotext("lcp3", lcp3);
  dotext("lcp4", lcp4);
  dotext("lcp5", lcp5);
  dotext("lcp6", lcp6);

  dotext("rpt1", rpt1);
  dotext("rcp1", rcp1);
  dotext("rcp2", rcp2);
  dotext("rpt2", rpt2);
  dotext("rcp3", rcp3);
  dotext("rcp4", rcp4);
  dotext("rpt3", rpt3);
  dotext("rcp5", rcp5);
  dotext("rcp6", rcp6);

  dotext("lpt4", lpt4);
  dotext("rpt4", rpt4);

  // draw phase
  seq[i](t);

  if (t < 1) {
    t += vel;
  }
  else {
    i = i < seq.length-1 ? i+1 : si;
    t = vel;
  }
}
