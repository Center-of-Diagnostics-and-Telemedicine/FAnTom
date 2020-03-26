
function PostSavingsToFile()
{
  var location = document.getElementById('doctor_id').innerHTML + "_" +  document.getElementById('study_SOP').innerHTML + '.txt';
  var table = document.getElementById('info_table_saved');
  var x = table.rows.length;
  if (x>1)
  {
    var data = '';
    data += 'Accession Number' +
    '\t' + table.rows[0].cells[0].innerHTML +
    '\t' + table.rows[0].cells[1].innerHTML +
    '\t' + table.rows[0].cells[2].innerHTML +
    '\t' + 'S' +
    '\t' + 'T';
    for (var i = 1; i < x; i++)
    {
      data += '\r\n';
      var row_text = document.getElementById('study_SOP').innerHTML +
      '\t' + table.rows[i].cells[0].innerHTML +
      '\t' + table.rows[i].cells[1].innerHTML +
      '\t' + table.rows[i].cells[2].innerHTML +
      '\t' + table.rows[i].cells[3].innerHTML +
      '\t' + table.rows[i].cells[4].innerHTML;
      data += row_text;
    }
    try
    {
      var request = new XMLHttpRequest();
      request.open("POST", location, false);

      request.setRequestHeader('Content-Type', 'text/plain')
      request.send(data);
    }
    catch(e)
    {
      alert(e);
    }
  }

}

function readTextFile(file)
{
    var rawFile = new XMLHttpRequest();
    rawFile.open("GET", file, false);
    rawFile.setRequestHeader('Content-Type', 'text/plain');
	  rawFile.send();
    var info = rawFile.responseText;
    return info;
}

function LocateFounding(founding_num)
{
  var table = document.getElementById('info_table_saved');
  var row = table.rows[founding_num];
  document.getElementById('sagittalRange').value = row.cells[0].innerHTML;
  document.getElementById('frontalRange').value = row.cells[1].innerHTML;
  document.getElementById('axialRange').value = row.cells[2].innerHTML;
  ChangeAllComponents();
}

function SaveFoundings()
{
  var table_to_write = document.getElementById('info_table_saved');
  var table_to_read = document.getElementById('info_table_temp');
  var x = table_to_read.rows.length;
  for (var i = 1; i < x; ++i)
  {
    var row = table_to_write.insertRow(i);
    var cell_X = row.insertCell(0);
    var cell_Y = row.insertCell(1);
    var cell_Z = row.insertCell(2);
    var cell_S = row.insertCell(3);
    var cell_T = row.insertCell(4);
    var cell_D = row.insertCell(5);
    cell_X.innerHTML = table_to_read.rows[i].cells[0].innerHTML;
    cell_Y.innerHTML = table_to_read.rows[i].cells[1].innerHTML;
    cell_Z.innerHTML = table_to_read.rows[i].cells[2].innerHTML;
    cell_S.innerHTML = table_to_read.rows[i].cells[3].innerHTML;
    cell_T.innerHTML = document.getElementById('text_temp_table_'+i).value;
  }
  for (var i = 1; i < table_to_write.rows.length; i++)
  {
    table_to_write.rows[i].removeAttribute('onclick');
    table_to_write.rows[i].deleteCell(5)
    var cell_D = table_to_write.rows[i].insertCell(5);
    var delete_cell = document.createElement('INPUT');
    delete_cell.setAttribute('type', 'button');
    delete_cell.setAttribute('id', 'delete_button_' + i);
    delete_cell.setAttribute('value', 'X');
    delete_cell.setAttribute('onclick', 'DeleteRowFromSavedTable('+i+')');
    cell_D.appendChild(delete_cell);
    table_to_write.rows[i].setAttribute('onclick', 'LocateFounding('+i+')');
  }
  for (var i = 1; i < x; ++i)
  {
    table_to_read.deleteRow(1);
  }
  DrawCirclesFromSavedTable();
}


function DeleteRowFromSavedTable(x)
{
  var table = document.getElementById('info_table_saved');
  var length = table.rows.length;
  if (x<=length)
  {
    table.rows[x].removeAttribute('onclick');
    table.deleteRow(x);
  }

  for (var i = 1; i < length-1; i++)
  {
    table.rows[i].removeAttribute('onclick');
    table.rows[i].deleteCell(5)
    var cell_D = table.rows[i].insertCell(5);
    var delete_cell = document.createElement('INPUT');
    delete_cell.setAttribute('type', 'button');
    delete_cell.setAttribute('id', 'delete_button_' + i);
    delete_cell.setAttribute('value', 'X');
    delete_cell.setAttribute('onclick', 'DeleteRowFromSavedTable('+i+')');
    cell_D.appendChild(delete_cell);
    table.rows[i].setAttribute('onclick', 'LocateFounding('+i+')');
  }
  ChangeAllComponents();
  DrawCirclesFromSavedTable();

}

function ReturnResponseFromCommand(commandPath)
{
  var location = commandPath;
  try
  {
    var request = new XMLHttpRequest();
    request.open('GET', location, false);
    request.setRequestHeader('Content-Type', 'text/plain');
	  request.send();
    var info = request.responseText;
    return info;
  }
  catch(e)
  {
    info = e;
  }
}

function SetRanges()
{
  document.getElementById('axialRange').max = document.getElementById('NFramesAxial_real').value;
  document.getElementById('frontalRange').max = document.getElementById('NFramesFrontal_real').value;
  document.getElementById('sagittalRange').max = document.getElementById('NFramesSagittal_real').value;
  document.getElementById('axialRange').min = 1;
  document.getElementById('frontalRange').min = 1;
  document.getElementById('sagittalRange').min = 1;
  document.getElementById('axialRange').value = Math.round(document.getElementById('NFramesAxial_real').value/2);
  document.getElementById('frontalRange').value = Math.round(document.getElementById('NFramesFrontal_real').value/2);
  document.getElementById('sagittalRange').value = Math.round(document.getElementById('NFramesSagittal_real').value/2);
}

function InitNumbers()
{
    var url_string = window.location.href;
    var url = new URL(url_string);
    var doc_id_var = url.searchParams.get("doctor_id");
    var acc_num = url.searchParams.get('accession_number');

    document.getElementById('study_SOP').innerHTML = acc_num;
    document.getElementById('doctor_id').innerHTML = doc_id_var;
    document.getElementById('NFramesAxial').value = ReturnResponseFromCommand('?command=GetNFramesInterpolated&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=axial');
    document.getElementById('NFramesFrontal').value = ReturnResponseFromCommand('?command=GetNFramesInterpolated&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=frontal');
    document.getElementById('NFramesSagittal').value = ReturnResponseFromCommand('?command=GetNFramesInterpolated&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=sagittal');
    document.getElementById('NFramesAxial_real').value = ReturnResponseFromCommand('?command=GetNFramesReal&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=axial');
    document.getElementById('NFramesFrontal_real').value = ReturnResponseFromCommand('?command=GetNFramesReal&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=frontal');
    document.getElementById('NFramesSagittal_real').value = ReturnResponseFromCommand('?command=GetNFramesReal&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=sagittal');
    document.getElementById('MIPMethodValue').value = document.getElementById('MIPpchoise2').value;
    SetRanges();

    var saved_text_file = doc_id_var + '_' + acc_num + '.txt';
    var saved_data = readTextFile(saved_text_file);
    var saved_data_array = saved_data.split(/\n/);
    saved_data_array.splice(0, 1);
    for (var i = 0; i < saved_data_array.length; i++)
    {
      if ( typeof saved_data_array[i][1] == 'undefined' || saved_data_array[i][0] == '' || saved_data_array[i].isEmpty)
      {
        saved_data_array.splice(i, 1);
        i--;
      }
    }
    for (var i = 1; i < saved_data_array.length+1; i++)
    {
      var line_array = saved_data_array[i-1].split(/\t/);
      if (line_array.length != 0)
      {
        line_array.splice(0, 1);
        var table_to_write = document.getElementById('info_table_saved');
        var row = table_to_write.insertRow(i);
        var cell_X = row.insertCell(0);
        var cell_Y = row.insertCell(1);
        var cell_Z = row.insertCell(2);
        var cell_S = row.insertCell(3);
        var cell_T = row.insertCell(4);
        var cell_D = row.insertCell(5);
        cell_X.innerHTML = line_array[0];
        cell_Y.innerHTML = line_array[1];
        cell_Z.innerHTML = line_array[2];
        cell_S.innerHTML = line_array[3];
        cell_T.innerHTML = line_array[4];
        var delete_cell = document.createElement('INPUT');
        delete_cell.setAttribute('type', 'button');
        delete_cell.setAttribute('id', 'delete_button_' + parseInt(i));
        delete_cell.setAttribute('value', 'X');
        delete_cell.setAttribute('onclick', 'DeleteRowFromSavedTable('+i+')');
        cell_D.appendChild(delete_cell);
        table_to_write.rows[i].setAttribute('onclick', 'LocateFounding('+i+')');
      }
  }

}
