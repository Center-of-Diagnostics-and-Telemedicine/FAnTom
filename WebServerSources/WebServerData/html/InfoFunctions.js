function InitInfo_start()
{
  document.getElementById('InfoScreenDimensionAxial').innerHTML = document.getElementById('TomogramDimensionAxial').value;
  document.getElementById('InfoScreenDimensionFrontal').innerHTML = document.getElementById('TomogramDimensionFrontal').value;
  document.getElementById('InfoScreenDimensionSagittal').innerHTML = document.getElementById('TomogramDimensionSagittal').value;
  document.getElementById('CurrentSliceaxial').innerHTML = document.getElementById('axialRange').value;
  document.getElementById('CurrentSlicefrontal').innerHTML = document.getElementById('frontalRange').value;
  document.getElementById('CurrentSlicesagittal').innerHTML = document.getElementById('sagittalRange').value;
  document.getElementById('InfoGV_max').innerHTML = document.getElementById('gammaRange').max;
  document.getElementById('InfoGV_min').innerHTML = document.getElementById('gammaRange').min;
  document.getElementById('InfoMIP_max').innerHTML = document.getElementById('mipRange').max;
  document.getElementById('InfoMIP_min').innerHTML = document.getElementById('mipRange').min;
  document.getElementById('mipRange_out').innerHTML = document.getElementById('mipRange').value;
  document.getElementById('gammaValue_out').innerHTML = document.getElementById('gammaRange').value;
}
