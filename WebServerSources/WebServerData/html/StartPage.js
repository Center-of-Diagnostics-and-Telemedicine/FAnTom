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



function GetAccessionNamesFromString(acc_names_response)
{
    var acc_names_array;

}

function GoToDICOMPage()
{
  var acc_num_var = document.getElementById("accession_number_list").value;
  if (acc_num_var != "")
  {
    var jspage = "/DICOM_Viewer.html?accession_number="+acc_num_var;
    location.assign(jspage);
  }
  else
  {
    document.getElementById("accession_number_list").focus();
    alert('Введено пустое поле. Введите идентификатор серии');
  }
}

//function CreateLoginPage(doc_id_var)
function CreateLoginPage()
{
  var acc_num_p_var = document.getElementById('acc_num_p');
  acc_num_p.innerHTML = '  Введите идентификатор исследования (Accession Number)  ';
  var acc_num_input = document.createElement('INPUT');
  acc_num_input.setAttribute('list', 'input_options');
  acc_num_input.setAttribute('id', 'accession_number_list');
  acc_num_p.appendChild(acc_num_input);

  var acc_names_var = document.createElement("DATALIST");
  acc_names_var.setAttribute("id", "input_options");

   var acc_names_response = ReturnResponseFromCommand('?command=GetAccessionNames');
   var arrayOfAccNames = acc_names_response.split(' ');
   for (var i = 0; i < arrayOfAccNames.length; i++)
  {
    var option_var = document.createElement("OPTION");
    option_var.setAttribute("value", arrayOfAccNames[i]);
   acc_names_var.appendChild(option_var);
   }
  acc_num_p.appendChild(acc_names_var);

  var acc_num_button = document.createElement('INPUT');
  acc_num_button.setAttribute('type', 'button');
  acc_num_button.setAttribute('value', 'Загрузить исследование');
  acc_num_button.setAttribute('onclick', 'GoToDICOMPage()');
  acc_num_p.appendChild(acc_num_button);
  Init();
  document.getElementById("doc_id_span_info").innerHTML = 'Добро пожаловать!';

}

function Init()
{
  var accession_input = document.getElementById("accession_number_list");
  accession_input.addEventListener("keyup", function (e)
   {
      if (e.keyCode == 13)
      {  //checks whether the pressed key is "Enter"
        GoToDICOMPage();
      };
   }, true);
}
