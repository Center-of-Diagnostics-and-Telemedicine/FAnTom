function LocateFounding(founding_num)
{
  var table = document.getElementById('info_table_saved');
  var row = table.rows[founding_num];
  document.getElementById('sagittalRange').value = row.cells[0].innerHTML;
  document.getElementById('frontalRange').value = row.cells[1].innerHTML;
  document.getElementById('axialRange').value = row.cells[2].innerHTML;
  ChangeAllComponents();
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
  document.getElementById('axialRange').max = document.getElementById('TomogramDimensionAxial').value;
  document.getElementById('frontalRange').max = document.getElementById('TomogramDimensionFrontal').value;
  document.getElementById('sagittalRange').max = document.getElementById('TomogramDimensionSagittal').value;
  document.getElementById('axialRange').min = 1;
  document.getElementById('frontalRange').min = 1;
  document.getElementById('sagittalRange').min = 1;
  document.getElementById('axialRange').value = Math.round(document.getElementById('TomogramDimensionAxial').value/2);
  document.getElementById('frontalRange').value = Math.round(document.getElementById('TomogramDimensionFrontal').value/2);
  document.getElementById('sagittalRange').value = Math.round(document.getElementById('TomogramDimensionSagittal').value/2);
}

function InitNumbers()
{
    var url_string = window.location.href;
    var url = new URL(url_string);
    var acc_num = url.searchParams.get('accession_number');

    document.getElementById('study_SOP').innerHTML = acc_num;
    document.getElementById('ScreenDimensionAxial').value = ReturnResponseFromCommand('?command=GetScreenDimension&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=axial');
    document.getElementById('ScreenDimensionFrontal').value = ReturnResponseFromCommand('?command=GetScreenDimension&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=frontal');
    document.getElementById('ScreenDimensionSagittal').value = ReturnResponseFromCommand('?command=GetScreenDimension&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=sagittal');
    document.getElementById('TomogramDimensionAxial').value = ReturnResponseFromCommand('?command=GetTomogramDimension&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=axial');
    document.getElementById('TomogramDimensionFrontal').value = ReturnResponseFromCommand('?command=GetTomogramDimension&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=frontal');
    document.getElementById('TomogramDimensionSagittal').value = ReturnResponseFromCommand('?command=GetTomogramDimension&accession_number=' + document.getElementById('study_SOP').innerHTML  + '&img_type=sagittal');
    document.getElementById('MIPMethodValue').value = document.getElementById('MIPpchoise2').value;
    SetRanges();


}
