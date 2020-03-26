   function UpdateImage()
      {
        // save canvas image as data url (png format by default)
        var dataURL = canvas.toDataURL();
        document.getElementById('edit1').value = dataURL;
        //document.getElementById('p1').innerHTML = dataURL;
      }

      function escapeXml(str)
      {
        return str.replace(/[<>&]/g, function (c)
          {
            switch (c)
            {
              case '<': return '&lt;';
              case '>': return '&gt;';
              case '&': return '&amp;';
            }
          });
      }

      function cutText(str, count)
      {
        if (str.length <= count)
          return str;
        return str.substr(0, count) + '...';
      }

      function PostImage(infoId, sendRaw)
      {
        var location = 'SaveMask()';
        if (sendRaw)
        {
          location += '.raw';
        }
        else
        {
          location += '.png';
        }
        var data = canvas.toDataURL();
        try
        {
          var request = new XMLHttpRequest();
          request.open("POST", location, false);

          if (sendRaw)
          {
            request.setRequestHeader('Content-Type', 'text/plain')
//	    request.file = data;
           request.send(data);
          }
          else
          {
            var b = atob(data.substr(22));
            var bytes = new Uint8Array(b.length);
            for (var i = 0; i < b.length; ++i)
            {
              bytes[i] = b.charCodeAt(i);
            }
            request.setRequestHeader('Content-Type', 'image/png')
  //	    request.file = new Blob([bytes], {type: 'image/png'});
            request.send(new Blob([bytes], {type: 'image/png'}));
          }
          var info = 'Data posted to "' + location + '".'
          info += ' State=' + request.readyState;
          if (request.readyState == 4)
          {
            info += ' Status=' + request.status + ' "' + request.statusText + '"';
            info += ' Data="' + escapeXml(cutText(request.responseText, 200)) + '"';
          }
          document.getElementById(infoId).innerHTML = info;
        }
        catch(e)
        {
          //alert(e);
          document.getElementById(infoId).innerHTML = e;
        }
      }

      function SubmitImage(infoId)
      {
        var data = canvas.toDataURL();
        document.getElementById('ImageData').value = data;
        document.getElementById('ImagePost').submit()
        document.getElementById(infoId).innerHTML = "Submitted";
      }

      function GetTest(infoId)
      {
        var location = document.location.href;
        var request = new XMLHttpRequest();
	request.Body = http.MaxBytesReader(w, r.Body, max)
        request.open("GET", location, false)
        try
        {
          request.send(null);
          var info = 'Data received from "' + location + '".'
          info += ' State=' + request.readyState;
          if (request.readyState == 4)
          {
            info += ' Status=' + request.status + ' "' + request.statusText + '"';
            info += ' Data="' + escapeXml(cutText(request.responseText, 200)) + '"';
          }
          document.getElementById(infoId).innerHTML = info;
        }
        catch(e)
        {
          //alert(e);
          document.getElementById(infoId).innerHTML = e;
        }
      }