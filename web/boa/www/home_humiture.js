var g_home_humiture_timer;
var g_temp_max;
var g_temp_min;
var g_hum_max;
var g_hum_min;

var g_humiture_first_flag = false;
var g_temp = -200;
var g_hum = -200;




function display_data(temp, humi)
{
//	var tmp_index1 = data.indexOf('T', 0);
//	var tmp_index2 = data.indexOf('H', 0);

//	var cur_temp = parseFloat(data.substr(tmp_index1+1, tmp_index2-tmp_index1-1));
//	var cur_hum = parseFloat(data.substring(tmp_index2+1));

	var cur_temp = parseFloat(temp);
	var cur_hum = parseFloat(humi);

	document.getElementById("temp_data").innerHTML = cur_temp+".C";
	document.getElementById("hum_data").innerHTML = cur_hum+"%";
	
	g_temp = cur_temp;
	g_hum = cur_hum;

	if((cur_temp <= g_temp_max) && (cur_temp >= g_temp_min))
	{
		document.getElementById("img_temp").src="./img/temp.png";
	}
	else
	{
		document.getElementById("img_temp").src="./img/temp_warn.png";
	}
	
	if((cur_hum <= g_hum_max) && (cur_hum >= g_hum_min))
	{
		document.getElementById("img_hum").src="./img/hum.png";
	}
	else
	{
		document.getElementById("img_hum").src="./img/hum_warn.png";
	}
}



function get_current_humiture()
{
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/deal_cmd.cgi?";
	URL += SUNHOME_TEMP_HUMI_GET_CMD + ":NULL";
	
	var xmlhttp_status;
	
	if(xmlhttp_object)
	{
		xmlhttp_object.onreadystatechange=function()
		{
			if (xmlhttp_object.readyState == 4)
			{
				//alert(xmlhttp_object.status);
				if (xmlhttp_object.status == 200)
				{	
					//alert(xmlhttp_object.responseText);
					var returnValue = xmlhttp_object.responseText.split("&");
					
					display_data(returnValue[1], returnValue[2]);
				}
			}
		}
		xmlhttp_object.open("GET",URL,true);
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");//disable cache in IE
		xmlhttp_object.send();
	}
	else
	{
		alert('please check the Bowser!');
	}
}


function display_humiture_setting(data)
{
	//alert(data);
	
	var temp_max,temp_min,hum_max, hum_min;
	var index1 = 0;
	var index2 = 0;
	index1 = data.indexOf('T', 0);
	index2 = data.indexOf('t', 0);
	temp_max = data.substr(index1+1,index2-index1-1);
	g_temp_max = parseFloat(temp_max);
	//alert(g_temp_max);
	document.getElementById("temp_max").value=temp_max;
	index1 = data.indexOf('t', 0);
	index2 = data.indexOf('H', 0);
	temp_min = data.substr(index1+1,index2-index1-1);
	g_temp_min = parseFloat(temp_min);
	//alert(g_temp_min);
	document.getElementById("temp_min").value=temp_min;
	index1 = data.indexOf('H', 0);
	index2 = data.indexOf('h', 0);
	hum_max = data.substr(index1+1,index2-index1-1);
	g_hum_max = parseFloat(hum_max);
	//alert(g_hum_max);
	document.getElementById("hum_max").value=hum_max;
	index1 = data.indexOf('h', 0);
	hum_min = data.substring(index1+1);
	g_hum_min = parseFloat(hum_min);
	//alert(g_hum_min);
	document.getElementById("hum_min").value=hum_min;

	if(data.indexOf('O', 0)==0 && data.indexOf('K', 0)==1)
	{
		alert("Setting Successfully!");
	}

}


function Home_Humiture_Onload()
{
	all_check();
	get_current_humiture();
	
	g_home_humiture_timer=setInterval("get_current_humiture()", 1200);
}


function submit_humiture()
{
	var temp_max = document.getElementById("temp_max").value;
	var temp_min = document.getElementById("temp_min").value;
	var hum_max = document.getElementById("hum_max").value;
	var hum_min = document.getElementById("hum_min").value;
	var send_data = "T"+temp_max+"t"+temp_min+"H"+hum_max+"h"+hum_min;
	display_humiture_setting(send_data);
}
