
function Send_Message_Onload()
{
	all_check();
}


function check_tel_number(phone)
{   
	var flag = false;   
	var reg0 =  /^(([0\+]\d{2,3}-)?(0\d{2,3})-)?(\d{7,8})(-(\d{3,}))?$/;   //判断 固话   
	var reg1 =/^((\(\d{2,3}\))|(\d{3}\-))?(13|15|18)\d{9}$/;               //判断 手机   
	if (reg0.test(phone)) flag=true;    
	if (reg1.test(phone)) flag=true;    
	return flag;
}

function len(s) 
{ 
	var l = 0; 
	var a = s.split(""); 
	for (var i=0;i<a.length;i++)
		{ 
			if (a[i].charCodeAt(0)<299) 
			{ 
				l++; 
			} 
			else 
			{ 
				l+=2; 
			} 
	} 
	return l; 
} 

function set_owner_tel(flag)
{
	var tmp_tel_number = document.getElementById('tel_number_text').value;
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/deal_cmd.cgi";
	
	var set_tel_number = "";
	if(flag == 1){
		set_tel_number = SUNHOME_MPLAYER_PAUSE_CMD + '&' + "NULL";
	}else if(flag == 2){
		set_tel_number = SUNHOME_MPLAYER_START_CMD + '&' + "NULL";
	}
	
	
/*	if('' == tmp_tel_number)
	{
		alert('no telephone number! please input');
		return;
	}
	if(!check_tel_number(tmp_tel_number))
	{
		alert('invalid telephone number! please check and input again');
		document.getElementById('tel_number_text').value = '';
		return;
	}*/
	
	if(xmlhttp_object)
	{
		xmlhttp_object.open("POST",URL,false);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.setRequestHeader("contentType","text/html;charset=GB2312")//
		xmlhttp_object.send(set_tel_number);
		
	/*	if(4 == xmlhttp_object.readyState)
		{
			if(200 == xmlhttp_object.status)
			{
				if(xmlhttp_object.responseText=="error")
				{
					alert('set error! please check the connection and SIM card.');
				}
				else if(xmlhttp_object.responseText=="ok")
				{
					alert('set successfully!');
				}	
				else
					alert('please check the network connection!');
			}
		}*/
	}
}

  
function send_message()
{
	var tmp_tel_number = document.getElementById('tel_number_text').value;
	var tmp_send_text = document.getElementById('message_text_area').value;
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/deal_cmd.cgi";
	var xmlhttp_status;
	var cmd1 = "";
	
	if('' == tmp_tel_number)
	{
		alert('no telephone number! please input');
		return;
	}
	if(!check_tel_number(tmp_tel_number))
	{
		alert('invalid telephone number! please check and input again');
		document.getElementById('tel_number_text').value = '';
		return;
	}
	if('' == tmp_send_text)
	{
		alert('no messages! please input');
		return;
	}
	else
	{
		//alert(len(tmp_send_text));
		if(len(tmp_send_text) > 70)
		{
			alert('messages is too long! please input less than 70 characters');
			return;
		}
	}
	//alert('6');
	
	document.getElementById('send_bt').disabled = true;
	document.getElementById('gprs_status').innerHTML = 'sending...';
	cmd1 += SUNHOME_GPRS_SEND_MSG_CMD + ":";
	cmd1 += 86;
	cmd1 += tmp_tel_number;
	cmd1 += '&';
	cmd1 += tmp_send_text;

	//alert(cmd1);
	
	
	if(xmlhttp_object)
	{
		//cmd1=encodeURI(cmd1);
		//cmd1=encodeURI(cmd1);
		xmlhttp_object.open("POST",URL,true);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");

		xmlhttp_object.onreadystatechange = function()	//创建回调函数
		{
			if(4 == xmlhttp_object.readyState)
			{	
				//alert(xmlhttp_object.status);
				if(200 == xmlhttp_object.status)
				{
				//	alert(xmlhttp_object.responseText);
				//	var data = xmlhttp_object.responseText.split("&");
					document.getElementById('send_bt').disabled = false;
					document.getElementById('gprs_status').innerHTML = 'successful!!';
					alert("send successful!!");
				}
			}
		}

		xmlhttp_object.send(cmd1);
	}
	else
	{
		alert('please check the Bowser!');
	}
	
}
