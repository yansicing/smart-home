var g_led_flag = 0x00;
var g_allled_flag = 0x00;



function display_change(tmp)
{
	if(tmp & 0x02)
		document.getElementById('led1').src = './img/led_on.png';
	else
		document.getElementById('led1').src = './img/led_off.png';

	if(tmp & 0x04)
		document.getElementById('led2').src = './img/led_on.png';
	else
		document.getElementById('led2').src = './img/led_off.png';

	if(tmp & 0x08)
		document.getElementById('led3').src = './img/led_on.png';
	else
		document.getElementById('led3').src = './img/led_off.png';

	if(0x0f == tmp)
	{
		document.getElementById('ledall').src = './img/allon.png';
		g_allled_flag = 1;
	}
	else
	{
		document.getElementById('ledall').src = './img/alloff.png';
		g_allled_flag = 0;
	}
}
	


function get_led_status()
{
	var xmlhttp_object = createXHR();
	var other_data = SUNHOME_LED_GET_CMD + ":" + "NULL";
	var URL = "/cgi-bin/deal_cmd.cgi?" + other_data;
	var xmlhttp_status;
	var data;
	
	if(xmlhttp_object)
	{
		xmlhttp_object.onreadystatechange = function()	//创建回调函数
		{
			if(xmlhttp_object.readyState==4)
			{
				if(200 == xmlhttp_object.status)
				{
					//alert(xmlhttp_object.responseText);
					data = xmlhttp_object.responseText.split("&");

					g_led_flag = parseInt(data[1]);
					display_change(g_led_flag);
				}
			}
		}
		
		xmlhttp_object.open("GET",URL,true);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();
	}
	else
	{
		alert('please check the Bowser!');
	}
}

function led_status_onload()
{
	all_check();
	get_led_status();
	//setInterval("get_led_status()", 2000);
}


function switch_led(led_number)
{
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/deal_cmd.cgi";
	var xmlhttp_status;
	var cmd1 = SUNHOME_LED_SET_CMD + ":" + led_number;
	var data;

	if(xmlhttp_object)
	{
	//	cmd1=encodeURI(cmd1);
	//	cmd1=encodeURI(cmd1);
		xmlhttp_object.onreadystatechange = function()	//创建回调函数
		{
			if(xmlhttp_object.readyState==4 && xmlhttp_object.status==200)
			{
				//alert("====----***"+xmlhttp_object.responseText);
				data = xmlhttp_object.responseText.split("&");

				g_led_flag = parseInt(data[1]);
				display_change(g_led_flag);
			}
		}
		xmlhttp_object.open("POST",URL,true);//false:同步;true:异步
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send(cmd1);	//以POST方式发送，最终将灯的状态传送给服务器，服务器端接收到该信息后去更新开发板中的灯
	}
	else
	{
		alert('please check the Bowser!');
	}
}


