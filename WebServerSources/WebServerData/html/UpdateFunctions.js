function throttle(func, ms)
{

  var isThrottled = false,
    savedArgs,
    savedThis;

  function wrapper() {

    if (isThrottled) { // (2)
      savedArgs = arguments;
      savedThis = this;
      return;
    }

    func.apply(this, arguments); // (1)

    isThrottled = true;

    setTimeout(function() {
      isThrottled = false; // (3)
      if (savedArgs) {
        wrapper.apply(savedThis, savedArgs);
        savedArgs = savedThis = null;
      }
    }, ms);
  }

  return wrapper;
}

var ThrottleUpdateCanvases =
{
  ax_up: '',
  fr_up: '',
  sag_up: '',

  InitThrottle: function()
  {
    var axial_update = function()
    {
      UpdateCanvas_noImage('axialCanvas','axial', 'axialRange');
    };
    var frontal_update = function()
    {
      UpdateCanvas_noImage('frontalCanvas','frontal', 'frontalRange');
    };
    var sagittal_update = function()
    {
      UpdateCanvas_noImage('sagittalCanvas','sagittal', 'sagittalRange');
    };
      var ms = 85;
      this.ax_up = throttle(axial_update, ms);
      this.fr_up = throttle(frontal_update, ms);
      this.sag_up = throttle(sagittal_update, ms);
  }
};


var ColorRanges = {
  black: document.getElementById('blackRange'),
  white: document.getElementById('whiteRange'),
  gamma: document.getElementById('gammaRange')
};

function ChangeAllComponents()
{
  document.getElementById('CurrentSliceaxial').innerHTML = document.getElementById('axialRange').value;
  ThrottleUpdateCanvases.ax_up();
  document.getElementById('CurrentSlicefrontal').innerHTML = document.getElementById('frontalRange').value;
  ThrottleUpdateCanvases.fr_up();
  document.getElementById('CurrentSlicesagittal').innerHTML = document.getElementById('sagittalRange').value;
  ThrottleUpdateCanvases.sag_up();
  DrawNavigationLines();
  DrawCirclesFromSavedTable();
}

function ChangeAxialComponents()
{
  document.getElementById('CurrentSliceaxial').innerHTML = document.getElementById('axialRange').value;
  ThrottleUpdateCanvases.ax_up();
  DrawNavigationLines();
  DrawCirclesFromSavedTable();
}

function ChangeFrontalComponents()
{
  document.getElementById('CurrentSlicefrontal').innerHTML = document.getElementById('frontalRange').value;
  ThrottleUpdateCanvases.fr_up();
  DrawNavigationLines();
  DrawCirclesFromSavedTable();
}

function ChangeSagittalComponents()
{
  document.getElementById('CurrentSlicesagittal').innerHTML = document.getElementById('sagittalRange').value;
  ThrottleUpdateCanvases.sag_up();
  DrawNavigationLines();
  DrawCirclesFromSavedTable();
}

function UpdateCanvasBW()
{
  if ((ColorRanges.black.value < ColorRanges.white.value))
  {
    UpdateAllCanvas();
  }
}

function UpdateCanvasGamma()
{
  document.getElementById('gammaValue_out').innerHTML = document.getElementById('gammaRange').value;
  UpdateAllCanvas();
}
function UpdateCanvasMIP()
{
  document.getElementById('mipRange_out').innerHTML = document.getElementById('mipRange').value;
  UpdateAllCanvas();
}

function UpdateCanvas_noImage(canvas_id,img_type,slice_no_id)
{
  var canvas = document.getElementById(canvas_id),
      context = canvas.getContext('2d'),
      img = InsertImage(img_type,slice_no_id);
  img.onload = function ()
  {
     context.drawImage(img,0,0);
  }
}

function UpdateAllCanvas()
{
  ThrottleUpdateCanvases.ax_up();
  ThrottleUpdateCanvases.fr_up();
  ThrottleUpdateCanvases.sag_up();
}

function GoToStartPage()
{
    var new_location = '/login_page.html';
  location.assign(new_location);
}

function CloseTomogramm()
{
  var acc_num_var = document.getElementById('study_SOP').innerHTML;
  ReturnResponseFromCommand('!CloseTomogram('+acc_num_var+')');
}

function SetMIPMethod()
{
  if (document.getElementById('MIPpchoise1').checked)
  {
    document.getElementById('MIPMethodValue').value = document.getElementById('MIPpchoise1').value;
  }
  if (document.getElementById('MIPpchoise2').checked)
  {
    document.getElementById('MIPMethodValue').value = document.getElementById('MIPpchoise2').value;
  }
  UpdateAllCanvas();
}

function SetBWRanges(black_value, white_value)
{
    ColorRanges.black.value = black_value;
    ColorRanges.white.value = white_value;
    UpdateAllCanvas();
}
