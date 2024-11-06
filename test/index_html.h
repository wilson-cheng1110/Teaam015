#include <Arduino.h>

#ifndef INDEX_HTML
#define INDEX_HTML

static const char PROGMEM index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Access-Control-Allow-Headers" content="Origin, X-Requested-With, Content-Type, Accept" />
<meta http-equiv="Access-Control-Allow-Methods" content="GET,POST,PUT,DELETE,OPTIONS" />
<meta http-equiv="Access-Control-Allow-Origin" content="*" />
<html lang="zh-CN">
<html>

<head>
  <meta name="viewport"
    content="width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2.0, user-scalable=yes" />

  <title>somebot</title>

  <style>
    * {
      box-sizing: border-box;
    }

    .section {
      margin: 2px;
      padding: 10px;
    }

    .button {
      /* position: absolute; */
      /* right: 12%; */
      /* top: 5%; */
      background: #44444460;
      /* border:3px solid #25e72e; */
      border: none;
      border-radius: 8px;
      outline: none;
      color: #FFFFFF90;
      padding: 5px 8px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: medium;
    }

    .button:active {
      background: #FFFFFFe0;
      -webkit-transition-duration: 2;
      transition-duration: 2;
    }
  </style>


  <script language="javascript">

    window.requestAnimFrame =
      window.requestAnimationFrame ||
      window.webkitRequestAnimationFrame ||
      window.mozRequestAnimationFrame ||
      window.oRequestAnimationFrame ||
      window.msRequestAnimationFrame ||
      function (callback) {
        window.setTimeout(callback, 1000 / 60);
      };

    // ----------------------------------------------------- CheckBox -----------------------------------------------------
    var CheckBox = (function (container, parameters) {
      parameters = parameters || {};
      var title = (typeof parameters.title === "undefined" ? "0" : parameters.title),
        name = (typeof parameters.name === "undefined" ? "CheckBox" : parameters.name),
        width = (typeof parameters.width === "undefined" ? 0 : parameters.width),
        height = (typeof parameters.height === "undefined" ? 0 : parameters.height),
        marginToBorder = (typeof parameters.marginToBorder === "undefined" ? 3 : parameters.marginToBorder),
        isChecked = (typeof parameters.isChecked === "undefined" ? false : parameters.isChecked);

      // Create Canvas element and add it in the Container object
      var objContainer = document.getElementById(container);
      var canvas = document.createElement("canvas");
      var context = canvas.getContext("2d");
      canvas.id = title;
      if (width === 0) { width = objContainer.clientWidth; }
      if (height === 0) { height = objContainer.clientHeight; }
      console.log("checkbox: objContainer.clientWidth=%d, objContainer.clientHeight=%d, width=%d, height=%d",
        objContainer.clientWidth, objContainer.clientHeight, width, height);
      canvas.width = width;
      canvas.height = height;
      objContainer.appendChild(canvas);

      var circumference = 2 * Math.PI;
      var containerAbsOffsetX, containerAbsOffsetY;

      var sliderMarginToBorder = marginToBorder;
      var sliderWidth = width - sliderMarginToBorder * 2;
      var sliderHeight = height - sliderMarginToBorder * 2;
      var sliderCornerRadius = sliderHeight / 2;
      var radioRadius = sliderHeight * 7 / 20;
      var radioMarginToSlider = sliderWidth / 25; // radio shift 5% from slider
      var sliderMoveGap = sliderWidth - radioMarginToSlider * 2 - radioRadius * 2;

      var radioAnimationPlaying = false;
      var radioMovingStep = 4;
      var radioMovingDir = 1; // -1: right-->left; 1: left-->right
      var radioMovingCount = 0;
      var radioMovingCountMAX = sliderMoveGap / radioMovingStep;
      var radioMinX = sliderMarginToBorder + radioMarginToSlider + radioRadius;
      var radioMaxX = radioMinX + sliderMoveGap;
      // radioMinX = radioMaxX - radioMovingCountMAX * radioMovingStep;
      var radioStartX = radioMinX;
      var radioMovingX = radioMinX;
      var radioMovingY = height / 2;

      var isPressing = 0;
      var isMoving = 0;
      var touchedRadioX = 0;
      var touchedRadioY = 0;

      // Check if the device support the touch or not
      if ("ontouchstart" in document.documentElement) {
        canvas.addEventListener("touchstart", onTouchStart, false);
        canvas.addEventListener("touchmove", onTouchMove, false);
        canvas.addEventListener("touchend", onTouchEnd, false);
      }
      else {
        canvas.addEventListener("mousedown", onMouseDown, false);
        canvas.addEventListener("mousemove", onMouseMove, false);
        canvas.addEventListener("mouseup", onMouseUp, false);
      }

      initContainerOffsetToBody();
      // Draw the object
      drawSlider();
      // checkRadio(106, 23);
      drawRadio(radioMovingX, radioMovingY);
      // onRadioClick();

      /******************************************************
       * Private methods
       *****************************************************/
      function initContainerOffsetToBody() {
        var node = objContainer;
        var x = y = 0;
        do {
          console.log("checkbox[%s], nodeName=%s, offsetLeft=%d, offsetTop=%d", title, node.nodeName, node.offsetLeft, node.offsetTop);
          x += node.offsetLeft;
          y += node.offsetTop;
        } while (node = node.offsetParent);
        containerAbsOffsetX = x;
        containerAbsOffsetY = y;
        console.log("checkbox[%s], containerAbsOffsetX=%d, containerAbsOffsetY=%d", title, containerAbsOffsetX, containerAbsOffsetY);
      }

      function drawSlider() {
        context.beginPath();
        context.moveTo(sliderMarginToBorder + sliderCornerRadius, sliderMarginToBorder);
        context.lineTo(width - sliderMarginToBorder - sliderCornerRadius, sliderMarginToBorder);

        context.arcTo(width - sliderMarginToBorder, sliderMarginToBorder,
          width - sliderMarginToBorder, sliderMarginToBorder + sliderCornerRadius, sliderCornerRadius);
        context.arcTo(width - sliderMarginToBorder, height - sliderMarginToBorder,
          width - sliderMarginToBorder - sliderCornerRadius, height - sliderMarginToBorder, sliderCornerRadius);

        context.lineTo(sliderMarginToBorder + sliderCornerRadius, height - sliderMarginToBorder);

        context.arcTo(sliderMarginToBorder, height - sliderMarginToBorder,
          sliderMarginToBorder, sliderMarginToBorder + sliderCornerRadius, sliderCornerRadius);
        context.arcTo(sliderMarginToBorder, sliderMarginToBorder,
          sliderMarginToBorder + sliderCornerRadius, sliderMarginToBorder, sliderCornerRadius);

        context.closePath();
        if (true === isChecked) {
          context.fillStyle = "#00000060";
        }
        else {
          context.fillStyle = "#00000090";
        }
        context.fill();
      }

      function drawRadio(radioX, radioY) {
        context.beginPath();
        console.log("plane, radioX=%d, radioY=%d, radius=%d", radioX, radioY, radioRadius);
        context.arc(radioX, radioY, radioRadius, 0, circumference, false);
        context.fillStyle = "#FFFFFF90"; // "#666666FF"; // "#22222290";
        context.fill();
      }

      function radioMoving() {
        radioMovingCount++;
        if (radioMovingCount <= radioMovingCountMAX) {
          // Delete canvas
          context.clearRect(0, 0, canvas.width, canvas.height);

          drawSlider();

          radioMovingX = radioStartX + radioMovingCount * radioMovingStep * radioMovingDir;
          drawRadio(radioMovingX, radioMovingY);

          requestAnimationFrame(radioMoving);
        }
        else {
          radioAnimationPlaying = false;

        }
      }

      function playRadioAnimation() {
        var is = (touchedRadioX < (width / 2)) ? false : true;

        if (isChecked != is) {
          if (false == radioAnimationPlaying) {
            if ((isChecked == false) && (is == true)) {
              isChecked = !isChecked;
              onRadioClick();

              radioStartX = radioMinX;
              radioMovingDir = 1;
              radioMovingCount = 0;
              radioAnimationPlaying = true;
              requestAnimationFrame(radioMoving);
            }
            else {
              isChecked = !isChecked;
              onRadioClick();

              radioStartX = radioMaxX;
              radioMovingDir = -1;
              radioMovingCount = 0;
              radioAnimationPlaying = true;
              requestAnimationFrame(radioMoving);
            }
          }
        }
      }

      function getMousePos(event) {
        //  pageX = offsetX + containerAbsOffsetX; pageY = offsetY + containerAbsOffsetY; 
        if (0) {
          console.log("checkbox/mouse, pageX=%d, pageY=%d", event.pageX, event.pageY);
          console.log('checkbox/mouse, offsetX:' + event.offsetX + ',offsetY:' + event.offsetY);
          console.log('checkbox/mouse, layerX:' + event.layerX + ',layerY:' + event.layerY);
          console.log('checkbox/mouse, X:' + (event.clientX - canvas.getBoundingClientRect().left) + ',Y:' + (event.clientY - canvas.getBoundingClientRect().top));
          console.log("checkbox/mouse, canvas.offsetLeft=%d, canvas.offsetTop=%d", canvas.offsetLeft, canvas.offsetTop);
          console.log("checkbox/mouse, canvas.offsetParent.offsetLeft=%d, canvas.offsetParent.offsetTop=%d", canvas.offsetParent.offsetLeft, canvas.offsetParent.offsetTop);
        }

        touchedRadioX = event.offsetX ? event.offsetX : event.layerX;
        touchedRadioY = event.offsetY ? event.offsetY : event.layerY;
        console.log("checkbox/mouse, touchedRadioX=%d, movedY=%d", touchedRadioX, touchedRadioY);
      }


      function getTouchPos(event) {
        console.log("checkbox/touch, touches.length=%s, targetTouches.length=%s, event.changedTouches.length=%s",
          event.touches.length, event.targetTouches.length, event.changedTouches.length);

        if (event.targetTouches.length === 1 && event.targetTouches[0].target === canvas) {
          if (0) {
            console.log("checkbox/touch, screenX=%d, screenY=%d", event.targetTouches[0].screenX, event.targetTouches[0].screenY);
            console.log("checkbox/touch, pageX=%d, pageY=%d", event.targetTouches[0].pageX, event.targetTouches[0].pageY);
            console.log("checkbox/touch, clientX=%d, clientY=%d", event.targetTouches[0].clientX, event.targetTouches[0].clientY);
            console.log("checkbox/touch, canvas.offsetLeft=%d, canvas.offsetTop=%d", canvas.offsetLeft, canvas.offsetTop);
            console.log("checkbox/touch, canvas.offsetParent.offsetLeft=%d, canvas.offsetParent.offsetTop=%d", canvas.offsetParent.offsetLeft, canvas.offsetParent.offsetTop);
          }

          touchedRadioX = event.targetTouches[0].pageX;
          touchedRadioY = event.targetTouches[0].pageY;

          // Manage offset
          touchedRadioX = touchedRadioX - containerAbsOffsetX;
          touchedRadioY = touchedRadioY - containerAbsOffsetY;
          console.log("checkbox/touch, touchedRadioX=%d, touchedRadioY=%d", touchedRadioX, touchedRadioY);
        }
      }

      /**
       * @desc Events for manage touch
       */
      function onTouchStart(event) {
        isPressing = 1;

        getTouchPos(event);
      }

      function onTouchMove(event) {
        // Prevent the browser from doing its default thing (scroll, zoom)
        event.preventDefault();

        if (isPressing === 1 && event.targetTouches.length === 1 && event.targetTouches[0].target === canvas) {
          isMoving = 1;
        }
      }

      function onTouchEnd(event) {
        if (1 === isPressing) {
          isPressing = 0;
          isMoving = 0;

          playRadioAnimation();
        }
      }

      /**
       * @desc Events for manage mouse
       */
      function onMouseDown(event) {
        isPressing = 1;
      }

      function onMouseMove(event) {
        if (isPressing === 1) {
          isMoving = 1;
        }
      }

      function onMouseUp(event) {
        if (1 === isPressing) {
          isPressing = 0;
          isMoving = 0;

          getMousePos(event);

          playRadioAnimation();
        }
      }

      function onRadioClick() {
        var xmlhttp1 = new XMLHttpRequest();
        xmlhttp1.open("GET", "/action?" + name + "=" + isChecked, true);
        xmlhttp1.send();
      }

      /******************************************************
 * Public methods
 *****************************************************/
      this.setState = function (is) {
        if (isChecked != is) {
          if (false == radioAnimationPlaying) {
            if ((isChecked == false) && (is == true)) {
              isChecked = !isChecked;

              radioStartX = radioMinX;
              radioMovingDir = 1;
              radioMovingCount = 0;
              radioAnimationPlaying = true;
              requestAnimationFrame(radioMoving);
            }
            else {
              isChecked = !isChecked;

              radioStartX = radioMaxX;
              radioMovingDir = -1;
              radioMovingCount = 0;
              radioAnimationPlaying = true;
              requestAnimationFrame(radioMoving);
            }
          }
        }
      };

    });

  </script>

</head>


<body>

  <div style="position:relative;margin: 0px ;text-align:center;border:0px solid #500bf1;">
    <!-- <img src="" id="stream" crossOrigin="anonymous" src="https://img.alicdn.com/imgextra/i4/2206903807395/O1CN01Uy9yrR24UxiwdDlJW_!!2206903807395.jpg" style="max-width: 90% ; margin: 3px; padding: 3px;border:3px solid #F0A000;"> -->
    <div id="photoDiv"
      style="padding-left: 0px ;margin: 0px ; height: 250px; position: relative;overflow: hidden;border:0px solid #4d08ee;">
      <img id="photo" crossOrigin="anonymous" src="https://img.alicdn.com/imgextra/i4/2206903807395/O1CN01Uy9yrR24UxiwdDlJW_!!2206903807395.jpg" onload="loadImage(this)"
        alt="ESP32 Cam Streaming..." style="padding-left: 0px ;margin: 0px ;border:0px solid #06f84b;">
    </div>

    <div style="position:absolute;padding-left: 0px ;margin: 0px ;left:0px;top:0%;border:0px solid #f30a0a;">
      <canvas id="imageMask"></canvas>
    </div>
    <div style="position:absolute;display: inline-flex;right:0px;top:2%;border:0px solid #06f040;">
      <button id="btnRed" style="margin-right:10px;color: #FF0000;border:0px solid #0ff3f3;" class="button"
        onclick="buttonClick('red');">Red</button>
      <button id="btnGreen" style="margin-right:10px;color: #00FF00;border:0px solid #0ff3f3;" class="button"
        onclick="buttonClick('green');">Green</button>
      <button id="btnBlue" style="margin-right:10px;color: #0000FF;border:0px solid #0ff3f3;" class="button"
        onclick="buttonClick('blue');">Blue</button>
      <button id="btnRefresh" style="margin-right:10px;border:0px solid #0ff3f3;" class="button"
        onclick="buttonClick('RefreshStream');">Refresh</button>
      <button id="btnStart" style="margin-right:20px;border:0px solid #0ff3f3;" class="button"
        onclick="buttonClick('start');">Start</button>
      <div id="LabelLedSwitchShell" style="margin-right: 2px;border:0px solid #466868;">
        <label id="LabelLedSwitch" style="font-size:medium;color: #FFFFFF90;">LED</label>
      </div>
      <div id="LedSwitchDiv" style="margin-right: 5px;border:0px solid #0a0ef3;">
      </div>
    </div>

    <div style="position:absolute;padding-left: 0px ;margin: 0px ;left:0px;top:10%;border:0px solid #f30a0a;"
      class="section">
      <table>
        <tr>
          <td style="color: #FF0000">R min:&#160;&#160;&#160;<span id="RMINdemo" style="color: #FF0000"></span></td>
          <td><input type="range" id="rmin" min="0" max="255" value="0" class="slider"></td>
          <td style="color: #FF0000">R max:&#160;&#160;&#160;<span id="RMAXdemo" style="color: #FF0000"></span></td>
          <td><input type="range" id="rmax" min="0" max="255" value="255" class="slider"></td>
        </tr>
        <tr>
          <td style="color: #00FF00">G min:&#160;&#160;&#160;<span id="GMINdemo" style="color: #00FF00"></span></td>
          <td><input type="range" id="gmin" min="0" max="255" value="0" class="slider"></td>
          <td style="color: #00FF00">G max:&#160;&#160;&#160;<span id="GMAXdemo" style="color: #00FF00"></span></td>
          <td><input type="range" id="gmax" min="0" max="255" value="255" class="slider"></td>
        </tr>
        <tr>
          <td style="color: #0000FF">B min:&#160;&#160;&#160;<span id="BMINdemo" style="color: #0000FF"></span></td>
          <td><input type="range" id="bmin" min="0" max="255" value="0" class="slider"> </td>
          <td style="color: #0000FF">B max:&#160;&#160;&#160;<span id="BMAXdemo" style="color: #0000FF"></span></td>
          <td> <input type="range" id="bmax" min="0" max="255" value="255" class="slider"> </td>
        </tr>
      </table>
    </div>

    <div id="debug"
      style="position:absolute; top:50%; left:50%; transform:translate(-50%, -50%); font-size:medium;color: #FF0000;">
      Centered</div>
  </div>

  <script type="text/javascript">

    var imageMask = document.getElementById("imageMask");
    var imageMaskCxt = imageMask.getContext("2d");
    var imageMaskOffsetX = -1;
    var imageMaskOffsetY = -1;

    var objCfgColorWorkUid, objSortingWorkUid; // interval work
    var isARMSortingObjMode = false; // false for config(pick) object color; true for ARM sorting object;
    var objSortingStatus = 0; // 0 for unknown; 1 for idle(need detecting obj color then reply to ESP32); 2 for sorting is executing;     

    var objDetectInterval = 2000; // object detect interval, maybe fast for ARM picking object 
    var objDetectCount = 0;

    var isColorPicked = false;
    var imageMaskColorPickX = -1;
    var imageMaskColorPickY = -1;

    var imageView = document.getElementById("photo");

    function getImgTouchPos(event) {
      console.log("img/touch, touches.length=%s, targetTouches.length=%s, event.changedTouches.length=%s",
        event.touches.length, event.targetTouches.length, event.changedTouches.length);

      if (true) {
        var node = imageMask;
        let x = y = 0;
        do {
          console.log("imageMask, nodeName=%s, offsetLeft=%d, offsetTop=%d", node.nodeName, node.offsetLeft, node.offsetTop);
          x += node.offsetLeft;
          y += node.offsetTop;
        } while (node = node.offsetParent);
        imageMaskOffsetX = x;
        imageMaskOffsetY = y;
        console.log("imageMask, imageMaskOffsetX=%d, imageMaskOffsetY=%d", imageMaskOffsetX, imageMaskOffsetY);
      }

      if (event.targetTouches.length === 1 && event.targetTouches[0].target === imageMask) {
        imageMaskColorPickX = event.targetTouches[0].pageX;
        imageMaskColorPickY = event.targetTouches[0].pageY;

        imageMaskColorPickX = imageMaskColorPickX - imageMaskOffsetX;
        imageMaskColorPickY = imageMaskColorPickY - imageMaskOffsetY;
        isColorPicked = true;
        if (true == isARMSortingObjMode) {
          isColorPicked = false;
        }
        console.log("img/touch, imageMaskColorPickX=%d, imageMaskColorPickY=%d", imageMaskColorPickX, imageMaskColorPickY);
      }
    }

    /**
 * @desc Events for manage touch
 */
    function onImgTouchStart(event) {
      getImgTouchPos(event);
    }

    function onImgTouchMove(event) {
      // Prevent the browser from doing its default thing (scroll, zoom)
      // event.preventDefault();
    }

    function onImgTouchEnd(event) {
      console.log("onImgTouchEnd E");
    }

    function getImgMousePos(event) {
      console.log("img/mouse, pageX=%d, pageY=%d", event.pageX, event.pageY);
      console.log('img/mouse, offsetX:' + event.offsetX + ',offsetY:' + event.offsetY);
      console.log('img/mouse, layerX:' + event.layerX + ',layerY:' + event.layerY);

      imageMaskColorPickX = event.offsetX ? event.offsetX : event.layerX;
      imageMaskColorPickY = event.offsetY ? event.offsetY : event.layerY;
      isColorPicked = true;
      if (true == isARMSortingObjMode) {
        isColorPicked = false;
      }
      console.log("img/touch, imageMaskColorPickX=%d, imageMaskColorPickY=%d", imageMaskColorPickX, imageMaskColorPickY);
    }

    /**
     * @desc Events for manage mouse
     */
    function onImgMouseDown(event) {

    }

    function onImgMouseMove(event) {

    }

    function onImgMouseUp(event) {
      console.log("onImgMouseUp E");
      getImgMousePos(event);
    }

    // Check if the device support the touch or not
    if ("ontouchstart" in document.documentElement) {
      imageMask.addEventListener("touchstart", onImgTouchStart, false);
      imageMask.addEventListener("touchmove", onImgTouchMove, false);
      imageMask.addEventListener("touchend", onImgTouchEnd, false);
    }
    else {
      imageMask.addEventListener("mousedown", onImgMouseDown, false);
      imageMask.addEventListener("mousemove", onImgMouseMove, false);
      imageMask.addEventListener("mouseup", onImgMouseUp, false);
    }

    // (255, 217, 80, 40, 80, 40) 
    // (255, 218, 125, 85, 118, 78)
    // (51, 11, 128, 88, 234, 194)  
    var objDetectMode = 0; // 0 for red; 1 for green; 2 for blue;      

    // var redObjRmax = 255;
    // var redObjRmin = 217;
    // var redObjGmax = 125;
    // var redObjGmin = 85;
    // var redObjBmax = 118;
    // var redObjBmin = 78;
    // var redObjRGBSection = [255, 217, 125, 85, 118, 78];

    // var greenObjRmax = 0;
    // var greenObjRmin = 0;
    // var greenObjGmax = 0;
    // var greenObjGmin = 0;
    // var greenObjBmax = 0;
    // var greenObjBmin = 0;
    // var greenObjRGBSection = [255, 217, 125, 85, 118, 78];

    // var blueObjRmax = 0;
    // var blueObjRmin = 0;
    // var blueObjGmax = 0;
    // var blueObjGmin = 0;
    // var blueObjBmax = 0;
    // var blueObjBmin = 0;
    // var blueObjRGBSection = [255, 217, 125, 85, 118, 78];

    var objRGBSection = [
      [143, 103, 37, 13, 42, 10], // red obj
      [68, 41, 141, 85, 41, 3], // green obj
      [53, 25, 53, 29, 124, 103], // blue obj
    ];

    var RMAX = 255;
    var RMIN = 217;
    var GMAX = 125;
    var GMIN = 85;
    var BMAX = 118;
    var BMIN = 78;
    // console.log("objRGBSection=" + objRGBSection[objDetectMode][1]);

    var RMAXslider = document.getElementById("rmax");
    var RMAXoutput = document.getElementById("RMAXdemo");
    RMAXslider.value = objRGBSection[objDetectMode][0];
    RMAXoutput.innerHTML = objRGBSection[objDetectMode][0];
    RMAXslider.oninput = function () {
      RMAXoutput.innerHTML = this.value;
      objRGBSection[objDetectMode][0] = parseInt(RMAXoutput.innerHTML, 10);
      console.log("RMAX=" + objRGBSection[objDetectMode][0]);
    }
    console.log("RMAX=" + objRGBSection[objDetectMode][0]);

    var RMINslider = document.getElementById("rmin");
    var RMINoutput = document.getElementById("RMINdemo");
    RMINslider.value = objRGBSection[objDetectMode][1];
    RMINoutput.innerHTML = objRGBSection[objDetectMode][1];
    RMINslider.oninput = function () {
      RMINoutput.innerHTML = this.value;
      objRGBSection[objDetectMode][1] = parseInt(RMINoutput.innerHTML, 10);
      console.log("RMIN=" + objRGBSection[objDetectMode][1]);
    }
    console.log("RMIN=" + objRGBSection[objDetectMode][1]);

    var GMAXslider = document.getElementById("gmax");
    var GMAXoutput = document.getElementById("GMAXdemo");
    GMAXslider.value = objRGBSection[objDetectMode][2];
    GMAXoutput.innerHTML = objRGBSection[objDetectMode][2];
    GMAXslider.oninput = function () {
      GMAXoutput.innerHTML = this.value;
      objRGBSection[objDetectMode][2] = parseInt(GMAXoutput.innerHTML, 10);
    }
    console.log("GMAX=" + objRGBSection[objDetectMode][2]);

    var GMINslider = document.getElementById("gmin");
    var GMINoutput = document.getElementById("GMINdemo");
    GMINslider.value = objRGBSection[objDetectMode][3];
    GMINoutput.innerHTML = objRGBSection[objDetectMode][3];
    GMINslider.oninput = function () {
      GMINoutput.innerHTML = this.value;
      objRGBSection[objDetectMode][3] = parseInt(GMINoutput.innerHTML, 10);
    }
    console.log("GMIN=" + objRGBSection[objDetectMode][3]);

    var BMAXslider = document.getElementById("bmax");
    var BMAXoutput = document.getElementById("BMAXdemo");
    BMAXslider.value = objRGBSection[objDetectMode][4];
    BMAXoutput.innerHTML = objRGBSection[objDetectMode][4];
    BMAXslider.oninput = function () {
      BMAXoutput.innerHTML = this.value;
      objRGBSection[objDetectMode][4] = parseInt(BMAXoutput.innerHTML, 10);
    }
    console.log("BMAX=" + objRGBSection[objDetectMode][4]);

    var BMINslider = document.getElementById("bmin");
    var BMINoutput = document.getElementById("BMINdemo");
    BMINslider.value = objRGBSection[objDetectMode][5];
    BMINoutput.innerHTML = objRGBSection[objDetectMode][5];
    BMINslider.oninput = function () {
      BMINoutput.innerHTML = this.value;
      objRGBSection[objDetectMode][5] = parseInt(BMINoutput.innerHTML, 10);
    }
    console.log("BMIN=" + objRGBSection[objDetectMode][5]);

    var isOpencvReady = false;
    function onOpenCvReady() {
      document.getElementById("debug").innerText = "OpenCV is ready";
      console.log("OpenCV is ready!!!");
      isOpencvReady = true;
    }

    //window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
    window.onload = function loadStream() {
      console.log("loadStream E");
      var img = document.getElementById("photo");
      img.name = 1;
      img.src = window.location.href.slice(0, -1) + ":81/stream_one";
      console.log("loadStream X");

      getCarStatus();
    }

    // Function to get current readings on the webpage when it loads for the first time
    function getCarStatus() {
      var xmlhttp1 = new XMLHttpRequest();
      xmlhttp1.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var ledSw = 0, turnSw = 0, carSpeed = 0;
          var myObj = JSON.parse(this.responseText);
          console.log(myObj);
          ledSw = Number(myObj.mLedSwitch);
          turnSw = Number(myObj.mTurnSwitch);
          carSpeed = Number(myObj.mCarSpeed);
          console.log("ledSw=%d, turnSw=%d, carSpeed=%d", ledSw, turnSw, carSpeed);

          ledSwitch.setState((ledSw == 0) ? false : true);

        }
      };
      xmlhttp1.open("GET", "/status", true);
      xmlhttp1.send();
    }

    var availWidth = window.screen.availWidth;
    var availHeight = window.screen.availHeight;
    var imgViewHeight = availHeight * 1; //0.8; // 手机浏览器下最优预览高度
    // var imgViewWidth = imgViewHeight / 0.5625; // 16/9比例，不裁剪
    var imgViewWidth = document.body.scrollWidth;
    console.log("imgViewHeight=%d, availWidth=%d, imgViewWidth=%d", imgViewHeight, availWidth, imgViewWidth);

    var photoDiv = document.getElementById("photoDiv");
    photoDiv.style.height = imgViewHeight + 'px';
    photoDiv.style.width = imgViewWidth + 'px';

    var sliderWidth = imgViewWidth * 0.2;
    var sliderHeight = imgViewHeight * 0.18;
    var sliderMarginTop = 0;

    var checkBoxWidth = sliderWidth / 3;
    var checkBoxHeight = checkBoxWidth / 2;
    var checkBoxMarginToBorder = 2;
    // var checkBoxMarginTop = sliderHeight * 2 / 3 / 4 - checkBoxMarginToBorder - 1;
    var checkBoxMarginTop = sliderHeight * 2 / 3 / 2 - checkBoxHeight / 2;

    var btnRefresh = document.getElementById("btnRefresh");
    btnRefreshHeight = sliderHeight / 2;
    btnRefresh.style.height = btnRefreshHeight + 'px';

    var btnStart = document.getElementById("btnStart");
    btnResetHeight = sliderHeight / 2;
    btnStart.style.height = btnResetHeight + 'px';

    var labelLedSwitch = document.getElementById("LabelLedSwitch");
    var labelLedSwitchShell = document.getElementById("LabelLedSwitchShell");
    var labelLedSwitchLineHeight = sliderHeight / 2;
    labelLedSwitchShell.style.height = labelLedSwitchLineHeight + 'px';
    labelLedSwitch.style.lineHeight = labelLedSwitchLineHeight + 'px';
    labelLedSwitchShellMarginTop = 0;
    labelLedSwitchShell.style.marginTop = labelLedSwitchShellMarginTop + 'px';

    var ledSwitchWidth = checkBoxWidth;
    var ledSwitchHeight = sliderHeight / 2;
    var ledSwitchMarginTop = 0;

    var ledSwitchDiv = document.getElementById("LedSwitchDiv");
    ledSwitchDiv.style.marginTop = ledSwitchMarginTop + 'px';

    var ledSwitchParam = {
      "title": "12", "name": "LedSwitch", "width": ledSwitchWidth, "height": ledSwitchHeight,
      "marginToBorder": checkBoxMarginToBorder,
    };
    var ledSwitch = new CheckBox('LedSwitchDiv', ledSwitchParam);
    console.log("LedSwitchDiv ---------------");


    function buttonClick(name) {
      if ("RefreshStream" == name) {
        console.log("RefreshStream E");
        setTimeout(function () { location.reload(true); }, 500);
      } else if ("red" == name) {
        document.getElementById("debug").innerText = "red";
        objDetectMode = 0;
      } else if ("green" == name) {
        document.getElementById("debug").innerText = "green";
        objDetectMode = 1;
      } else if ("blue" == name) {
        document.getElementById("debug").innerText = "blue";
        objDetectMode = 2;
      } else if ("start" == name) {
        isARMSortingObjMode = !isARMSortingObjMode;
        document.getElementById("debug").innerText = (true == !isARMSortingObjMode) ? "Stoping" : "Starting";
        btnStart.innerText = (true == isARMSortingObjMode) ? "Stop" : "Start";

        if (true == isARMSortingObjMode) {
          clearInterval(objCfgColorWorkUid);

          objDetectInterval = 500;
          objDetectMode = 0;
          objDetectCount = 0;
          objSortingStatus = 0;
          objSortingWorkUid = setInterval(intervalFunc, objDetectInterval);
        } else {
          clearInterval(objSortingWorkUid);

          objDetectInterval = 1000;
          objDetectMode = 0;
          objDetectCount = 0;
          objCfgColorWorkUid = setInterval(intervalFunc, objDetectInterval);
        }

      } else {
        var xmlhttp1 = new XMLHttpRequest();
        xmlhttp1.open("GET", "/action?button=" + name, true);
        xmlhttp1.send();
        return;
      }

      RMAXslider.value = objRGBSection[objDetectMode][0];
      RMAXoutput.innerHTML = objRGBSection[objDetectMode][0];
      RMINslider.value = objRGBSection[objDetectMode][1];
      RMINoutput.innerHTML = objRGBSection[objDetectMode][1];
      GMAXslider.value = objRGBSection[objDetectMode][2];
      GMAXoutput.innerHTML = objRGBSection[objDetectMode][2];
      GMINslider.value = objRGBSection[objDetectMode][3];
      GMINoutput.innerHTML = objRGBSection[objDetectMode][3];
      BMAXslider.value = objRGBSection[objDetectMode][4];
      BMAXoutput.innerHTML = objRGBSection[objDetectMode][4];
      BMINslider.value = objRGBSection[objDetectMode][5];
      BMINoutput.innerHTML = objRGBSection[objDetectMode][5];
    }


    function clear_canvas(w, h) {
      // imageMask.style.width = imageView.style.width;
      // imageMask.style.height = imageView.style.height;
      // imageMask.style.left = imageView.style.left;
      // imageMask.style.position = 'relative';

      console.log("w=%d, h=%d, w=%d, h=%d", w, h, imageMask.width, imageMask.height);
      // imageMaskCxt.clearRect(0, 0, w, h);
      // imageMaskCxt.rect(0, 0, w, h);
      imageMaskCxt.clearRect(0, 0, imageMask.width, imageMask.height);
      imageMaskCxt.rect(0, 0, imageMask.width, imageMask.height);
      imageMaskCxt.fillStyle = "#FFFFFF00";
      imageMaskCxt.fill();
    }

    function MaxAreaArg(arr) {
      if (arr.length == 0) {
        return -1;
      }

      var max = arr[0];
      var maxIndex = 0;
      var dupIndexCount = 0; //duplicate max elements?

      if (arr[0] >= .90 * aarea) {
        max = 0;
      }

      for (var i = 1; i < arr.length; i++) {
        if (arr[i] > max && arr[i] < .99 * aarea) {
          maxIndex = i;
          max = arr[i];
          dupIndexCount = 0;
        }
        else if (arr[i] == max && arr[i] != 0) {
          dupIndexCount++;
        }
      }

      if (dupIndexCount == 0) {
        return maxIndex;
      } else {
        return -2;
      }
    }//end MaxAreaArg 

    async function objDetect(count, mode, Rmax, Rmin, Gmax, Gmin, Bmax, Bmin) {
      let src = cv.imread(imageView);
      arows = src.rows;
      acols = src.cols;
      aarea = arows * acols;
      adepth = src.depth();
      atype = src.type();
      achannels = src.channels();
      console.log("rows = " + arows);
      console.log("cols = " + acols);
      console.log("pic area = " + aarea);
      console.log("depth = " + adepth);
      console.log("type = " + atype);
      console.log("channels = " + achannels);
      document.getElementById("debug").innerText = acols + "x" + arows;

      clear_canvas(acols, arows);


      let M00Array = [0,];
      let mask = new cv.Mat();
      let mask1 = new cv.Mat();
      let contours = new cv.MatVector();
      let hierarchy = new cv.Mat();

      let cnt;
      let Moments;
      let M00;
      let M10;
      let x_cm = 0;
      let y_cm = 0;

      // let high = new cv.Mat(src.rows, src.cols, src.type(), [200, 100, 100, 255]);
      // let low = new cv.Mat(src.rows, src.cols, src.type(), [160, 60, 60, 0]);
      let high = new cv.Mat(src.rows, src.cols, src.type(), [objRGBSection[mode][0], objRGBSection[mode][2], objRGBSection[mode][4], 255]);
      let low = new cv.Mat(src.rows, src.cols, src.type(), [objRGBSection[mode][1], objRGBSection[mode][3], objRGBSection[mode][5], 0]);
      let contourMask = new cv.Mat(src.rows, src.cols, src.type(), [255, 255, 255, 0]);
      let boundingRectMask = new cv.Mat(src.rows, src.cols, src.type(), [255, 255, 255, 0]);


      console.log("imageMaskColorPickX=%d, imageMaskColorPickY=%d", imageMaskColorPickX, imageMaskColorPickY);
      // imageMaskColorPickY=274;
      // imageMaskColorPickX=405;
      if ((false == isARMSortingObjMode) && (true == isColorPicked) && (imageMaskColorPickX > 0) && (imageMaskColorPickY > 0)) {
        let row = imageMaskColorPickY.toFixed();
        let col = imageMaskColorPickX.toFixed();

        R = src.data[row * src.cols * src.channels() + col * src.channels()];
        G = src.data[row * src.cols * src.channels() + col * src.channels() + 1];
        B = src.data[row * src.cols * src.channels() + col * src.channels() + 2];
        A = src.data[row * src.cols * src.channels() + col * src.channels() + 3];
        console.log("RDATA = " + R);
        console.log("GDATA = " + G);
        console.log("BDATA = " + B);
        console.log("ADATA = " + A);
        RMAXslider.value = objRGBSection[objDetectMode][0] = R;
        RMAXoutput.innerHTML = objRGBSection[objDetectMode][0] = R;
        RMINslider.value = objRGBSection[objDetectMode][1] = R;
        RMINoutput.innerHTML = objRGBSection[objDetectMode][1] = R;

        GMAXslider.value = objRGBSection[objDetectMode][2] = G;
        GMAXoutput.innerHTML = objRGBSection[objDetectMode][2] = G;
        GMINslider.value = objRGBSection[objDetectMode][3] = G;
        GMINoutput.innerHTML = objRGBSection[objDetectMode][3] = G;

        BMAXslider.value = objRGBSection[objDetectMode][4] = B;
        BMAXoutput.innerHTML = objRGBSection[objDetectMode][4] = B;
        BMINslider.value = objRGBSection[objDetectMode][5] = B;
        BMINoutput.innerHTML = objRGBSection[objDetectMode][5] = B;

        let point4 = new cv.Point(imageMaskColorPickX, imageMaskColorPickY);
        cv.circle(boundingRectMask, point4, 5, [255, 255, 255, 255], 2, cv.LINE_AA, 0);

        // let point5 = new cv.Point(imageMaskColorPickX, imageMaskColorPickY-20);
        // cv.putText(boundingRectMask, "R=" + R + ";G=" + G + ";B=" + B, point5, cv.FONT_HERSHEY_PLAIN,1.0, [255, 255, 0, 255],1);
        isColorPicked = false;
      }

      cv.inRange(src, low, high, mask1);
      cv.threshold(mask1, mask, 150, 200, cv.THRESH_BINARY);

      let brushR = (0 == mode) ? 255 : 0;
      let brushG = (1 == mode) ? 255 : 0;
      let brushB = (2 == mode) ? 255 : 0;
      let brushScalar = new cv.Scalar(brushR, brushG, brushB, 255);
      try {
        cv.findContours(mask, contours, hierarchy, cv.RETR_CCOMP, cv.CHAIN_APPROX_SIMPLE);
        document.getElementById("debug").innerText = "SIZE = " + contours.size();
        console.log("CONTOUR_SIZE = " + contours.size());

        for (let i = 0; i < contours.size(); i++) {
          cv.drawContours(contourMask, contours, i, brushScalar, 2, cv.LINE_8, hierarchy, 100);
        }

        for (let k = 0; k < contours.size(); k++) {
          cnt = contours.get(k);
          Moments = cv.moments(cnt, false);
          M00Array[k] = Moments.m00;
        }

        let ArgMaxArea = MaxAreaArg(M00Array);
        console.log("ArgMaxArea = " + ArgMaxArea);

        if (ArgMaxArea >= 0) {
          cnt = contours.get(MaxAreaArg(M00Array));  //use the contour with biggest MOO
          Moments = cv.moments(cnt, false);
          M00 = Moments.m00;
          M10 = Moments.m10;
          M01 = Moments.m01;
          x_cm = M10 / M00;
          y_cm = M01 / M00;

          console.log("M00 = " + M00);
          console.log("XCM = " + Math.round(x_cm));
          console.log("YCM = " + Math.round(y_cm));

          //fetch(document.location.origin+'/?xcm='+Math.round(x_cm)+';stop');
          // fetch(document.location.origin+'/?cm='+Math.round(x_cm)+';'+Math.round(y_cm)+';stop');

          console.log("M00ARRAY = " + M00Array);

          //***************bounding rect***************************
          let rect = cv.boundingRect(cnt);
          let point1 = new cv.Point(rect.x, rect.y);
          let point2 = new cv.Point(rect.x + rect.width, rect.y + rect.height);

          cv.rectangle(boundingRectMask, point1, point2, brushScalar, 2, cv.LINE_AA, 0);
          //*************end bounding rect***************************


          //*************draw center point*********************
          let point3 = new cv.Point(x_cm, y_cm);
          cv.circle(boundingRectMask, point3, 1, brushScalar, 2, cv.LINE_AA, 0);
          //***********end draw center point*********************

        }//end if(ArgMaxArea >= 0)
        else {
          if (ArgMaxArea == -1) {
            console.log("ZERO ARRAY LENGTH");
          }
          else {              //ArgMaxArea=-2
            console.log("DUPLICATE MAX ARRAY-ELEMENT");
          }
        }
        cnt.delete();
      }//end try
      catch {
        console.log("ERROR TRACKER NO CONTOUR");
        // clear_canvas();
      }

      // cv.imshow('imageMask', mask);
      // cv.imshow('imageMask', src);
      // cv.imshow('imageMask', contourMask);
      cv.imshow('imageMask', boundingRectMask);

      src.delete();
      high.delete();
      low.delete();
      mask1.delete();
      mask.delete();
      contours.delete();
      hierarchy.delete();
      contourMask.delete();
      boundingRectMask.delete();

      return { c: count, m: mode, area: M00, x: x_cm, y: y_cm };
    }

    function getArmObjSortingStatus() {
      var xmlhttp1 = new XMLHttpRequest();
      xmlhttp1.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var myObj = JSON.parse(this.responseText);
          console.log(myObj);
          objSortingStatus = Number(myObj.mObjSortingStatus);
        }
      };
      xmlhttp1.open("GET", "/status", true);
      xmlhttp1.send();
    }

    var colorName = ["red", "green", "blue"];
    function intervalFunc() {
      if (true == isOpencvReady) {
        // (257, 217, 80, 40, 80, 40) 
        // (258, 218, 125, 85, 118, 78)
        // (51, 11, 128, 88, 234, 194)
        console.log("objDetectInterval=%d", objDetectInterval);

        if (false == isARMSortingObjMode) {
          objDetect(objDetectCount, objDetectMode, RMAX, RMIN, GMAX, GMIN, BMAX, BMIN);

        } else {
          // isARMSortingObjMode: false for config(pick) object color; true for ARM sorting object;
          // objSortingStatus: 0 for unknown; 1 for idle(need detecting obj color then reply to ESP32); 2 for sorting is executing;     

          if (0 == objSortingStatus) {
            console.log("ARM unknown!");
            getArmObjSortingStatus();
          } else if (1 == objSortingStatus) {
            console.log("ARM waiting obj color!");
            document.getElementById("debug").innerText = "Detecting";
            objDetect(objDetectCount, objDetectMode, RMAX, RMIN, GMAX, GMIN, BMAX, BMIN).then(function (res) {
              console.log("count=%d, mode=%d, area=%f, x=%d, y=%d", res.c, res.m, res.area, res.x, res.y);
              if ((res.x > 0) && (res.y > 0)) {
                // fetch(document.location.origin+'/?cm='+Math.round(x_cm)+';'+Math.round(y_cm)+';stop');
                var xmlhttp1 = new XMLHttpRequest();
                xmlhttp1.open("GET", "/action?color=" + colorName[res.m.toFixed()] + "&count=" + res.c
                  + "&area=" + res.area.toFixed() + "&objX=" + res.x.toFixed() + "&objY=" + res.y.toFixed(),
                  true);

                xmlhttp1.send();
              }
            });
            objDetectCount++;
            objDetectMode++;
            if (3 == objDetectMode) {
              objSortingStatus = 0;
            }
            objDetectMode = (3 == objDetectMode) ? 0 : objDetectMode;
          } else if (2 == objSortingStatus) {
            console.log("ARM sorting!");
            document.getElementById("debug").innerText = "Sorting";
            clear_canvas(0, 0);
            getArmObjSortingStatus();
          }
        }
      }
    }

    objCfgColorWorkUid = setInterval(intervalFunc, objDetectInterval);


    function AutoResizeImage(maxWidth, maxHeight, objImg) {
      var img = new Image();
      img.src = objImg.src;
      var hRatio;
      var wRatio;
      var Ratio = 1;
      var w = img.width;
      var h = img.height;
      wRatio = maxWidth / w;
      hRatio = maxHeight / h;
      if (maxWidth == 0 && maxHeight == 0) {
        Ratio = 1;
      } else if (maxWidth == 0) {
        if (hRatio < 1) Ratio = hRatio;
      } else if (maxHeight == 0) {
        if (wRatio < 1) Ratio = wRatio;
      } else if (wRatio < 1 || hRatio < 1) {
        Ratio = (wRatio <= hRatio ? wRatio : hRatio);
      }
      if (Ratio < 1) {
        w = w * Ratio;
        h = h * Ratio;
      }
      console.log("h=%d, w=%d", h, w);
      objImg.height = h;
      objImg.width = w;
    }

    function imgOnload(e) {
      var wid = e.parentNode.offsetWidth;//父容器限制高度
      var hei = e.parentNode.offsetHeight;//父容器限制宽度
      var realWidth = e.width; //储存图片实际宽度
      var realHeight = e.height; //储存图片实际高度
      console.log("wid=%d, hei=%d, realWidth=%d, realHeight=%d", wid, hei, realWidth, realHeight);
      //获取图片宽高比
      var realwh = e.width / e.height;
      if (realwh > wid / hei) {
        var w = ((hei / realHeight) * realWidth) + 'px';
        var h = hei + 'px';
        var l = '-' + ((hei / realHeight) * realWidth - wid) / 2 + 'px';
        e.style.width = w; //等比缩放宽度
        e.style.height = h; //跟div高度一致
        e.style.left = l; //设置图片相对自己位置偏移为img标签的宽度-高度的一半
        e.style.position = 'relative';
        console.log("width=%s, height=%s, left=%s", w, h, l);

        imageMask.style.width = w;
        imageMask.style.height = h;
        imageMask.style.left = l;
        imageMask.style.position = 'relative';
      } else {
        var w = wid + 'px';
        var h = ((wid / realWidth) * realHeight) + 'px';
        var t = '-' + ((wid / realWidth) * realHeight - hei) / 2 + 'px';
        e.style.width = w; //等比缩放宽度
        e.style.height = h; //跟div高度一致
        e.style.top = t; //设置图片相对自己位置偏移为img标签的宽度-高度的一半
        e.style.position = 'relative';
        console.log("width=%s, height=%s, left=%s", w, h, t);

        imageMask.style.width = w;
        imageMask.style.height = h;
        imageMask.style.top = t;
        imageMask.style.position = 'relative';
      }
      clear_canvas(0, 0);
      // console.log("imageMask.width=%d, imageMask.height=%d", imageMask.width, imageMask.height);
    }

    function loadImage(img) {
      console.log("loadImage E");
      var s = "";
      s += " 网页可见区域宽：" + document.body.clientWidth + "\n";
      s += " 网页可见区域高：" + document.body.clientHeight + "\n";
      s += " 网页可见区域宽：" + document.body.offsetWidth + " (包括边线和滚动条的宽)" + "\n";
      s += " 网页可见区域高：" + document.body.offsetHeight + " (包括边线的宽)" + "\n";
      s += " 网页正文全文宽：" + document.body.scrollWidth + "\n";
      s += " 网页正文全文高：" + document.body.scrollHeight + "\n";
      s += " 网页被卷去的高(ff)：" + document.body.scrollTop + "\n";
      s += " 网页被卷去的高(ie)：" + document.documentElement.scrollTop + "\n";
      s += " 网页被卷去的左：" + document.body.scrollLeft + "\n";
      s += " 网页正文部分上：" + window.screenTop + "\n";
      s += " 网页正文部分左：" + window.screenLeft + "\n";
      s += " 屏幕分辨率的高：" + window.screen.height + "\n";
      s += " 屏幕分辨率的宽：" + window.screen.width + "\n";
      s += " 屏幕可用工作区高度：" + window.screen.availHeight + "\n";
      s += " 屏幕可用工作区宽度：" + window.screen.availWidth + "\n";
      s += " 你的屏幕设置是 " + window.screen.colorDepth + " 位彩色" + "\n";
      s += " 你的屏幕设置 " + window.screen.deviceXDPI + " 像素/英寸" + "\n";
      console.log(s);

      // AutoResizeImage(window.screen.availWidth, 0, img);

      imgOnload(img);

      console.log("loadImage X");
    }

  </script>

  <script async src=" https://docs.opencv.org/master/opencv.js" onload="onOpenCvReady();"
    type="text/javascript"></script>
</body>

</html>
)rawliteral";

#endif
