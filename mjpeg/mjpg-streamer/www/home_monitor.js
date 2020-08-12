
var g_url;


function home_monitor_check()
{
	var url = window.location.href;

	var tmp_last_index = url.lastIndexOf(":");
	g_url = url.substr(0, tmp_last_index);
	//alert(g_url);
	
}



function jump_page(index)
{
	switch(index)
	{
		case 0:
		{
			window.location.href = g_url+"/home.html";
		}
		break;
	
		case 1:
		{
			window.location.href = g_url+"/led_control.html";
		}
		break;

		case 2:
		{
			window.location.href = g_url+"/send_message.html";
		}
		break;

		case 3:
		{
			window.location.href = g_url+"/home_humiture.html";
		}
		break;

		case 4:
		{
			window.location.href = g_url+":8000";
		}
		break;
		
		case 5:
		{
			window.location.href = g_url+"/home_mplayer.html";
		}
		break;
		
		case 0xFF:
		{
			window.location.href = g_url;
		}
		break;
		
		default:
		{
			;
		}
		break;
	}

}


function createXHR()//此JS函数即为创建XML对象，并兼容IE，firefox等浏览器
{
	//alert('in createXHR');
	var xmlhttp = null;
	
	if(window.XMLHttpRequest)
		xmlhttp = new XMLHttpRequest();
	else if(window.ActiveXObject)
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	else
		xmlhttp = new ActiveXObject('Msxml2.XMLHTTP');
			
	if(xmlhttp)
	{
		;//alert('create xmlhttp success!');
	}
	else
		alert('create xmlhttp error!');
	return xmlhttp;
}







function check_user(usrname, pwd, url_data, type)
{
	var xmlhttp_object = createXHR();
	var URL = g_url;
	if(0 == type)
		URL += "/cgi-bin/login_check.cgi?"+"usrname="+usrname+"&pwd="+pwd;
	else if(1 == type)
		URL += "/cgi-bin/login_check.cgi?"+url_data;
	//alert(URL);
	var xmlhttp_status;
	
	if(xmlhttp_object)
	{
		//alert('_1');
		xmlhttp_object.open("GET",URL,false);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();
		xmlhttp_status = xmlhttp_object.status;
		if(200 == xmlhttp_status)
		{
			//alert(xmlhttp_object.responseText);
			return xmlhttp_object.responseText;
		}
		xmlhttp_object = null;
	}
	else
	{
		alert('please check the Bowser!');
	}
}





