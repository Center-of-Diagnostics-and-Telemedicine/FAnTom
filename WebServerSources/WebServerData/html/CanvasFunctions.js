var ctrlPressed = false;
function ctrlUnPressed(event)
{
    if (event.shiftKey || (event.keyCode == 16) || event.which == 16)
    {
      ctrlPressed = false;
    }
};

window.addEventListener('keyup', ctrlUnPressed, true);
  function InitInteractiveImageDraw(canvas_id,img_type,slice_no_id)
  {

   var canvas_img = document.getElementById(canvas_id),
       context_img = canvas_img.getContext('2d'),
       MoveSliceCoords,
       StartPos,
       FinalPos,
       temp_img_id = 'imageTemp' + img_type,
       container = canvas_img.parentNode,
       canvas_temp = document.createElement('canvas');
   canvas_temp.id = temp_img_id;
   canvas_temp.width = canvas_img.width;
   canvas_temp.height = canvas_img.height;

   container.appendChild(canvas_temp);

    var canvas_lines_id = 'canvas_lines_' + img_type;
    var canvas_lines = document.createElement('canvas');
    canvas_lines.id = canvas_lines_id;
    canvas_lines.width = canvas_img.width;
    canvas_lines.height = canvas_img.height;
    container.appendChild(canvas_lines);

	  var context_temp = canvas_temp.getContext('2d'),
        img = InsertImage(img_type,slice_no_id);
    img.onload = function ()
    {
      context_img.drawImage(img,0,0);
    }


    var ShowOnHower =
    {
      mouseEntered:false,
      mouseOver:false,
      mouseenter: function(coords)
      {
        this.mouseEntered = true;
        this.mouseOver = true;
        ShowCoordsUniversal(img_type, coords);
      },
      mouseleave: function()
      {
        this.mouseEntered = false;
        this.mouseOver = false;
        document.getElementById('coordZ').innerHTML = '';
        document.getElementById('coordY').innerHTML = '';
        document.getElementById('coordX').innerHTML = '';
        document.getElementById('Value_HU').innerHTML = '';
      },
      mousemove: function(coords)
      {
            ShowCoordsUniversal(img_type, coords)
      },
    };

    var ScrollFunctions =
    {
      wheel: function(sign)
      {
            if (sign) document.getElementById(slice_no_id).value++;
            else document.getElementById(slice_no_id).value--;
            switch (img_type)
            {
              case 'axial':
                ChangeAxialComponents()
                break;
              case 'frontal':
                ChangeFrontalComponents()
                break;
              case 'sagittal':
                ChangeSagittalComponents()
                break;
              default:
            }
      },
    };
    var drawHandler =
    {
      isEmpty: true,
      isDrawing: false,
      modified: false,
      drawline: false,
      MoveSlice: false,
      // Clear all
      clear: function ()
      {
        var img = InsertImage(img_type,slice_no_id);
        img.onload = function ()
        {
          context_img.drawImage(img,0,0);
        }
        this.isEmpty = true;
        this.modified = true;
        ClearTableTemp();
      },
      mousedown: function (coords)
      {

        if (ctrlPressed)
        {
          context_temp.strokeStyle = '#00ff00';
          context_temp.lineWidth = 2;
          context_temp.beginPath();
          context_temp.moveTo(coords.x, coords.y);
          StartPos = coords;
          this.isDrawing = true;
        }
        else
        {
          MoveSliceCoords = coords;
          StartPos = coords;
          this.MoveSlice = true;

        }
      },
      mousemove: function (coords)
      {
        if (this.isDrawing&&ctrlPressed)
        {
          context_temp.clearRect(0, 0, canvas_temp.width, canvas_temp.height);
          context_temp.beginPath();
		      context_temp.moveTo(StartPos.x, StartPos.y);
		      context_temp.lineTo(coords.x, coords.y);
		      context_temp.stroke();
		      context_temp.closePath();
          FinalPos = coords;
          this.isEmpty = false;
          this.modified = true;
          this.drawline = true;
        }
        else if (this.MoveSlice)
        {
          var PreviousStep = MoveSliceCoords.y - StartPos.y;
          MoveSliceCoords = coords;
          var NewStep = MoveSliceCoords.y - StartPos.y;
          var diff = NewStep - PreviousStep;
          document.getElementById(slice_no_id).value -=(-diff);
          MoveSliceCoords = coords;
          switch (img_type)
          {
            case 'axial':
              ChangeAxialComponents()
              break;
            case 'frontal':
              ChangeFrontalComponents()
              break;
            case 'sagittal':
              ChangeSagittalComponents()
              break;
            default:
          }
        }
      },
      mouseup: function (coords)
      {
        if (this.isDrawing && this.drawline && ctrlPressed)
        {
          FinalPos = coords;
          this.mousemove(FinalPos);
          update_Image();
          AddRowTempTable(coords);
          this.isDrawing = false;
          this.drawline = false;
        }
        else if (this.MoveSlice)
        {
          this.MoveSlice = false;
        }

      },
      dblclick: function (coords)
      {
        this.clear();
      },
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
    function ClearTableTemp()
    {
      var table = document.getElementById('info_table_temp');
      var x = table.rows.length;
      for (var i = 1; i < x; ++i)
      {
        table.deleteRow(1);
      }
    }

    function AddRowTempTable(coords)
    {
      AddTempTableUniversal(img_type,StartPos,coords)
    }

    function update_Image()
    {
      context_img.drawImage(canvas_temp, 0, 0);
	    context_temp.clearRect(0, 0, canvas_temp.width, canvas_temp.height);
    }
    function drawMouse(event)
    {
      var rect = canvas_temp.getBoundingClientRect(),
          coords = { x: event.clientX  - rect.left,
        y: event.clientY  - rect.top};
//      var ctrl_key = (event.ctrlKey ? true : false)
      if (event.shiftKey) ctrlPressed = true;
      drawHandler[event.type](coords);
      event.preventDefault();
    }

    function ShowCoordinates(event)
    {
      var rect = canvas_temp.getBoundingClientRect(),
          coords = { x: event.clientX  - rect.left, y: event.clientY  - rect.top};
      ShowOnHower[event.type](coords);
      event.preventDefault();
    }
    function ScrollData(event)
    {
      var delta = event.deltaY || event.detail || event.wheelDelta,
          sign;
      if (delta > 0) sign = true;
      else sign = false;
      ScrollFunctions[event.type](sign);
      event.preventDefault();
    }
    canvas_temp.addEventListener('mousedown', drawMouse, false);
    canvas_temp.addEventListener('mousemove', drawMouse, false);
    canvas_temp.addEventListener('mousemove', ShowCoordinates, false);
    canvas_temp.addEventListener('mouseenter', ShowCoordinates, false);
    canvas_temp.addEventListener('mouseleave', ShowCoordinates, false);
    canvas_temp.addEventListener('wheel', ScrollData, false);
    canvas_temp.addEventListener('mouseup', drawMouse, false);
    canvas_temp.addEventListener('dblclick', drawMouse, false);
    function drawTouch(event)
    {
      var rect = canvas_temp.getBoundingClientRect();
      if (event.targetTouches && event.targetTouches[0])
      {
        var coords =
        {
          x: event.targetTouches[0].clientX - rect.left,
          y: event.targetTouches[0].clientY - rect.top
        };
        drawHandler[event.type](coords);
      }
      event.preventDefault();
    }
    canvas_temp.addEventListener('touchstart', drawTouch, false);
    canvas_temp.addEventListener('touchmove', drawTouch, false);
    canvas_temp.addEventListener('touchend', drawTouch, false);
  }



  function InsertImage(img_type,slice_no_id)
  {
    var img = new Image();
    img.src = FormImageSource_noImageId(img_type, slice_no_id);
    return img;
  }

  function InitCanvas()
  {
    document.getElementById('axialCanvas').width = document.getElementById('NFramesFrontal').value;
    document.getElementById('axialCanvas').height = document.getElementById('NFramesSagittal').value;
    document.getElementById('frontalCanvas').width = document.getElementById('NFramesSagittal').value;
    document.getElementById('frontalCanvas').height = document.getElementById('NFramesAxial').value;
    document.getElementById('sagittalCanvas').width = document.getElementById('NFramesFrontal').value;
    document.getElementById('sagittalCanvas').height = document.getElementById('NFramesAxial').value;
    InitAllImageDraw();
  }

  function InitAllImageDraw()
  {
    InitInteractiveImageDraw('axialCanvas','axial','axialRange');
    InitInteractiveImageDraw('frontalCanvas','frontal','frontalRange');
    InitInteractiveImageDraw('sagittalCanvas','sagittal','sagittalRange');
    DrawNavigationLines();
  }

  function DrawNavigationLines()
  {
    DrawLine('axial');
    DrawLine('frontal');
    DrawLine('sagittal');
  }

  function DrawLine(img_type)
  {
    var canvas_lines_id = 'canvas_lines_' + img_type;
    var lines_position =
    {
      horizontal: '',
      vertical: ''
    };
    switch (img_type) {
      case 'axial':
      lines_position.vertical = document.getElementById('frontalRange').value;
      lines_position.horizontal = document.getElementById('sagittalRange').value;
        break;
      case 'frontal':
      lines_position.vertical = parseInt(ReturnResponseFromCommand('?command=GetInterpolatedPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
      '&img_type=axial&slice_no='+ document.getElementById('axialRange').value));
      lines_position.horizontal = document.getElementById('sagittalRange').value;
        break;
      case 'sagittal':
      lines_position.vertical = parseInt(ReturnResponseFromCommand('?command=GetInterpolatedPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
      '&img_type=axial&slice_no='+ document.getElementById('axialRange').value));
      lines_position.horizontal = document.getElementById('frontalRange').value;
        break;
    }
    var canvas_lines = document.getElementById(canvas_lines_id);
    var context_temp = canvas_lines.getContext('2d');
    context_temp.strokeStyle = '#ffff1a';
    context_temp.clearRect(0, 0, canvas_lines.width, canvas_lines.height);
    context_temp.beginPath();
    context_temp.moveTo(0, lines_position.vertical);
    context_temp.lineTo(canvas_lines.width, lines_position.vertical);
    context_temp.moveTo(lines_position.horizontal, 0);
    context_temp.lineTo(lines_position.horizontal, canvas_lines.height);
    context_temp.stroke();
    context_temp.closePath();
  }

  function AddTempTableUniversal(img_type, StartPos, coords)
  {
    var table = document.getElementById('info_table_temp'),
        x = table.rows.length,
        row = table.insertRow(x),
        cell_X = row.insertCell(0),
        cell_Y = row.insertCell(1),
        cell_Z = row.insertCell(2),
        cell_S = row.insertCell(3),
        cell_T = row.insertCell(4),
//        text_cell = document.createElement('INPUT'),

        middle_coord_x = parseInt(coords.x - StartPos.x),
        middle_coord_y = parseInt(coords.y - StartPos.y),
        start_coord_x, start_coord_y, current_coord_x, current_coord_y,
        length_text;

//    text_cell.setAttribute('type', 'text');
//    text_cell.setAttribute('id', 'text_temp_row_' + x);
//    cell_T.appendChild(text_cell);
      var text_cell = document.createElement("INPUT");
      text_cell.setAttribute("list", "input_options_"+x);
      text_cell.setAttribute("id", "text_temp_table_"+x);
      cell_T.appendChild(text_cell);

      var datalist_var = document.createElement("DATALIST");
      datalist_var.setAttribute("id", "input_options_"+x);

      var s_var = document.createElement("OPTION");
      s_var.setAttribute("value", "с");
      datalist_var.appendChild(s_var);

      var c_var = document.createElement("OPTION");
      c_var.setAttribute("value", "п");
      datalist_var.appendChild(c_var);

      var m_var = document.createElement("OPTION");
      m_var.setAttribute("value", "м");
      datalist_var.appendChild(m_var);

      cell_T.appendChild(datalist_var);

    var coords_local = {
      z: '',
      x: '',
      y: '',
      hor_name: '',
      ver_name: '',
      z_interpolation: true,
      x_interpolation: true,
      y_interpolation: true,
    };
    switch (img_type)
    {
      case 'axial':
      coords_local.hor_name = 'sagittal';
      coords_local.ver_name = 'frontal';
      coords_local.z = document.getElementById('axialRange').value;
      coords_local.x = parseInt(StartPos.x + middle_coord_x/2);
      coords_local.y = parseInt(StartPos.y + middle_coord_y/2);
      coords_local.z_interpolation = false;
        break;
      case 'frontal':
      coords_local.hor_name = 'sagittal';
      coords_local.ver_name = 'axial';
      coords_local.z = parseInt(StartPos.y + middle_coord_y/2);
      coords_local.x = parseInt(StartPos.x + middle_coord_x/2);
      coords_local.y = document.getElementById('frontalRange').value;
      coords_local.y_interpolation = false;
        break;
      case 'sagittal':
      coords_local.hor_name = 'frontal';
      coords_local.ver_name = 'axial';
      coords_local.z = parseInt(StartPos.y + middle_coord_y/2);
      coords_local.x = document.getElementById('sagittalRange').value;
      coords_local.y = parseInt(StartPos.x + middle_coord_x/2);
      coords_local.x_interpolation = false;
        break;
    }
    start_coord_x = ReturnResponseFromCommand('?command=GetCoordinateInterpolated&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type='+coords_local.hor_name+'&coord='+  StartPos.x);

    start_coord_y = ReturnResponseFromCommand('?command=GetCoordinateInterpolated&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type='+coords_local.ver_name+'&coord='+  StartPos.y);

    current_coord_x = ReturnResponseFromCommand('?command=GetCoordinateInterpolated&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type='+coords_local.hor_name+'&coord='+  coords.x);


    current_coord_y = ReturnResponseFromCommand('?command=GetCoordinateInterpolated&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type='+coords_local.ver_name+'&coord='+  coords.y);


    length_text = Math.sqrt(Math.pow(current_coord_x - start_coord_x, 2)+Math.pow(current_coord_y - start_coord_y, 2));
    length_text = length_text.toFixed(1);

    cell_Z.innerHTML = parseInt(ReturnResponseFromCommand('?command=GetOriginalPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type=axial&coord='+  coords_local.z + '&coord_interpolation=' +coords_local.z_interpolation));

    cell_X.innerHTML = parseInt(ReturnResponseFromCommand('?command=GetOriginalPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type=sagittal&coord='+  coords_local.x + '&coord_interpolation=' +coords_local.x_interpolation));

    cell_Y.innerHTML = parseInt(ReturnResponseFromCommand('?command=GetOriginalPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type=frontal&coord='+  coords_local.y + '&coord_interpolation=' +coords_local.y_interpolation));

    cell_S.innerHTML = length_text;
  }

  function ShowCoordsUniversal(img_type, coords)
  {
    var coords_local = {
      z: '',
      x: '',
      y: '',
      z_interpolation: true,
      x_interpolation: true,
      y_interpolation: true,
      int_z: '',
      int_y: '',
      int_x: ''
    };
    var  hu_value;

    switch (img_type) {
      case 'axial':
        coords_local.z = document.getElementById('axialRange').value;
        coords_local.y = parseInt(coords.y);
        coords_local.x = parseInt(coords.x);
        coords_local.z_interpolation = false;
      break;
      case 'frontal':
      coords_local.z = parseInt(coords.y);
      coords_local.y = document.getElementById('frontalRange').value;
      coords_local.x = parseInt(coords.x);
      coords_local.y_interpolation = false;

      break;
      case 'sagittal':
      coords_local.z = parseInt(coords.y);
      coords_local.y = parseInt(coords.x);
      coords_local.x = document.getElementById('sagittalRange').value;
      coords_local.x_interpolation = false;
      break;
    }


    coords_local.int_z = parseInt(ReturnResponseFromCommand('?command=GetOriginalPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type=axial&coord='+  coords_local.z + '&coord_interpolation=' +coords_local.z_interpolation));

    coords_local.int_x = parseInt(ReturnResponseFromCommand('?command=GetOriginalPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type=sagittal&coord='+  coords_local.x + '&coord_interpolation=' +coords_local.x_interpolation));

    coords_local.int_y = parseInt(ReturnResponseFromCommand('?command=GetOriginalPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
    '&img_type=frontal&coord='+  coords_local.y + '&coord_interpolation=' +coords_local.y_interpolation));



    hu_value = parseInt(ReturnResponseFromCommand('?command=GetPointHU&accession_number='
     + document.getElementById('study_SOP').innerHTML +'&coords_z=' + coords_local.int_z + '&coords_y='
    + coords_local.int_y + '&coords_x=' + coords_local.int_x));
    document.getElementById('coordZ').innerHTML = coords_local.int_z;
    document.getElementById('coordY').innerHTML = coords_local.int_y;
    document.getElementById('coordX').innerHTML = coords_local.int_x;
    document.getElementById('Value_HU').innerHTML = hu_value;
  }

  function DrawCircle(current_clice, coords, pixel_length, img_type)
  {
    var coords_local = {
      main: '',
      horizontal: '',
      vertical: ''
    };
    var canvas_lines_id = 'canvas_lines_' + img_type;
    switch (img_type)
     {
      case 'axial':
        coords_local.main =  coords.z;
        coords_local.horizontal =   coords.x;
        coords_local.vertical =   coords.y;
        break;
      case 'frontal':
        coords_local.main =  coords.y;
        coords_local.horizontal =   coords.x;

        coords_local.vertical = parseInt(ReturnResponseFromCommand('?command=GetInterpolatedPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
        '&img_type=axial&slice_no='+ coords.z));

        break;
      case 'sagittal':
        coords_local.main =  coords.x;
        coords_local.horizontal =   coords.y;
        coords_local.vertical = parseInt(ReturnResponseFromCommand('?command=GetInterpolatedPixelCoordinate&accession_number=' + document.getElementById('study_SOP').innerHTML +
        '&img_type=axial&slice_no='+ coords.z));
        break;
    }
    var h = Math.abs(current_clice-coords_local.main);
    var new_radius = Math.sqrt(Math.pow(pixel_length,2) - Math.pow(2*h,2));
    var canvas_lines = document.getElementById(canvas_lines_id);
    var context_lines = canvas_lines.getContext('2d');
    context_lines.strokeStyle = '#00ff00';
    context_lines.beginPath();
    context_lines.arc(coords_local.horizontal,coords_local.vertical,new_radius,0,2*Math.PI);
    context_lines.stroke();
    context_lines.closePath();
  }

  function FormImageSource_noImageId(img_type, slice_no_id)
  {
    var aprox_value = document.getElementById('mipRange').value,
     mip_method_var = document.getElementById('MIPMethodValue').value,
     slice_no_var = document.getElementById(slice_no_id).value,
     black_var = document.getElementById('blackRange').value,
     accession_number = document.getElementById('study_SOP').innerHTML,
     white_var = document.getElementById('whiteRange').value,
     gamma_var = document.getElementById('gammaRange').value,
     host = 'http://'+window.location.hostname,
	img_format = 'png',

     new_image_source = host + ':5555/img_'+img_type+'.'+img_format +'?command=GetSlice&accession_number='+accession_number+'&img_type='+img_type+'&img_format='+img_format+'&slice_no='+
    slice_no_var+'&black_var='+black_var+'&white_var='+white_var+'&gamma_var='+gamma_var +'&aprox_value='+aprox_value+'&mip_method=' + mip_method_var;

    return new_image_source;
  }
