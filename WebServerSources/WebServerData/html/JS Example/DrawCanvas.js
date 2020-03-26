     
var canvas = document.getElementById('canvas1');
      var context = canvas.getContext('2d');

      // draw cloud
//      context.beginPath();
 //     context.moveTo(170, 80);
//      context.bezierCurveTo(130, 100, 130, 150, 230, 150);
 //     context.bezierCurveTo(250, 180, 320, 180, 340, 150);
//      context.bezierCurveTo(420, 150, 420, 120, 390, 100);
 //     context.bezierCurveTo(430, 40, 370, 30, 340, 50);
//      context.bezierCurveTo(320, 5, 250, 20, 250, 50);
//      context.bezierCurveTo(200, 5, 150, 20, 170, 80);
 //     context.closePath();
//      context.lineWidth = 5;
//      context.fillStyle = '#8ED6FF';
//      context.fill();
 //     context.strokeStyle = '#0000ff';
//      context.stroke();

      var drawHandler =
      {
        isEmpty: true,
        isDrawing: false,
        modified: false,
        // Clear all
        clear: function ()
        {
          var context = canvas.getContext('2d');
          var width = canvas.width;
          var height = canvas.height;
          context.clearRect(0, 0, width, height);
          this.isEmpty = true;
          this.modified = true;
        },
        // Mouse
        mousedown: function (coords)
        {
          var context = canvas.getContext('2d');
          context.strokeStyle = '#ff0000';
          context.beginPath();
          context.moveTo(coords.x, coords.y);
          this.isDrawing = true;
        },
        mousemove: function (coords)
        {
          if (this.isDrawing)
          {
            var context = canvas.getContext('2d');
            context.lineTo(coords.x, coords.y);
            context.lineWidth = 1;
            context.stroke();
            this.isEmpty = false;
            this.modified = true;
          }
        },
        mouseup: function (coords)
        {
          if (this.isDrawing)
          {
            this.mousemove(coords);
            this.isDrawing = false;
          }
        },
        dblclick: function (coords)
        {
          this.clear();
        },
        // Touch screen support
        touchstart: function (coords)
        {
          this.mousedown(coords);
        },
        touchmove: function (coords)
        {
          this.mousemove(coords);
        },
        touchend: function (coords)
        {
          this.mouseup(coords);
        }
      };

      // Mouse
      function drawMouse(event)
      {
        var coords = { x: event.pageX - canvas.offsetLeft,
          y: event.pageY - canvas.offsetTop};
        drawHandler[event.type](coords);
        event.preventDefault();
      }
      canvas.addEventListener('mousedown', drawMouse, false);
      canvas.addEventListener('mousemove', drawMouse, false);
      canvas.addEventListener('mouseup', drawMouse, false);
      canvas.addEventListener('dblclick', drawMouse, false);
      // Touch screen support
      function drawTouch(event)
      {
        if (event.targetTouches && event.targetTouches[0])
        {
          var coords =
          {
            x: event.targetTouches[0].pageX - canvas.offsetLeft,
            y: event.targetTouches[0].pageY - canvas.offsetTop
          };
          drawHandler[event.type](coords);
        }
        event.preventDefault();
      }
      canvas.addEventListener('touchstart', drawTouch, false);
      canvas.addEventListener('touchmove', drawTouch, false);
      canvas.addEventListener('touchend', drawTouch, false);

   
