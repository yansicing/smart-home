
function Send_Message_Onload()
{
	all_check();
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
	
	
	if(xmlhttp_object)
	{
		xmlhttp_object.open("POST",URL,false);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.setRequestHeader("contentType","text/html;charset=GB2312")//
		xmlhttp_object.send(set_tel_number);
		
	}
}

  
