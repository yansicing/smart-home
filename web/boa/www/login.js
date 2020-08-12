
function check_user(usrname, pwd, url_data, type)
{
	var xmlhttp_object = createXHR();
	var URL;
	var other_data = SUNHOME_LOGIN_CMD + ":" + usrname + "&" + pwd;
	if(0 == type){
		URL = "/cgi-bin/deal_cmd.cgi?" + other_data;
	}
	else if(1 == type)
		URL = "/cgi-bin/deal_cmd.cgi?"+url_data;
	
	var xmlhttp_status;
	
	if(xmlhttp_object)
	{
		xmlhttp_object.open("GET",URL,false);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();
		xmlhttp_status = xmlhttp_object.status;
		if(200 == xmlhttp_status)
		{
		//	alert(xmlhttp_object.responseText);
			return xmlhttp_object.responseText;
		}
		xmlhttp_object = null;
	}
	else
	{
		alert('please check the Bowser!');
	}
}

function login()
{
	all_check();
	var usrname = document.getElementById('UserName').value;
	var pwd	= document.getElementById('Password').value;
	if(usrname=='')
	{
		alert("no username!please input username.");
		return;
	}
	else
	{
		if(pwd=='')
		{
			alert("no password!please input password.");
			return;
		}
		else
		{
			var data = check_user(usrname, pwd, 0, 0);
			var ret = data.split("&");
			//alert(ret);
			if("0" == ret[1])
			{
				alert("Congratulations to you, Welcome to SunPlusApp Smart Home System!");
				//var url = "/home.html?"+"usrname="+usrname+"&pwd="+pwd;
				//window.location.href = url;
				jump_page(0);
			}
			else
			{	
				document.getElementById('UserName').value='';
				document.getElementById('Password').value='';
				alert("username or password error!");
			}
			
		}
	}
}


function exit()
{
	window.close();
}
