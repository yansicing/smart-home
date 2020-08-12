
var g_url;	//用来存储当前的网址

var SUNHOME_LOGIN_CMD = 1;

var SUNHOME_LED_SET_CMD = 101;		/*设置led灯命令*/
var SUNHOME_LED_GET_CMD	= 102;		/*获取led灯状态命令*/

var SUNHOME_TEMP_HUMI_GET_CMD = 201;

var SUNHOME_GPRS_SEND_MSG_CMD = 301;
var SUNHOME_GPRS_CALL_CMD = 302;
var SUNHOME_GPRS_HANG_UP_CMD = 303;

var SUNHOME_LOCAL_IP_SET_CMD = 401;
var SUNHOME_LOCAL_IP_GET_CMD = 402;

var SUNHOME_MPLAYER_PAUSE_CMD = 501;
var SUNHOME_MPLAYER_VOLUME_CMD = 502;
var SUNHOME_MPLAYER_CHANGESONG_CMD = 503;
var SUNHOME_MPLAYER_GET_SONGLIST_CMD = 504;
var SUNHOME_MPLAYER_START_CMD = 505;

function all_check()
{
	var url = window.location.href;//获取到整个网址

	var tmp_last_index = url.lastIndexOf("/");/*查找最后一个'/'  */
	g_url = url.substr(0, tmp_last_index);
	/*通过substr()得到从url的第一个字符到
	temp_last_index之间的一个字符串，例如url="192.168.1.123/login.html";
	那么g_url="192.168.1.123" ，最终全局变量g_url存储了当前网址 */
	//alert("g_url=="+g_url);
}

/*****************************************
功能描述：通过传的参数，改变window.location.href，跳转到相应的网页
参数类型：0、1、2、3、4、5
返回描述：
*******************************************/
function jump_page(index)
{
	switch(index)
	{
		case 0:
		{
			window.location.href = g_url+"/home.html";/* 跳转到主页 */
		}
		break;
	
		case 1:
		{
			window.location.href = g_url+"/led_control.html";//跳转到灯的控制网页
		}
		break;

		case 2:
		{
			window.location.href = g_url+"/send_message.html";//跳转到发送短信网页
		}
		break;

		case 3:
		{
			window.location.href = g_url+"/home_humiture.html";//跳转到获取温湿度网页
		}
		break;

		case 4:
		{
			window.location.href = g_url+":8000";//跳转到视频监控网页
		}
		break;
		
		case 5:
		{
				
			window.location.href = g_url+"/home_mplayer.html";//跳转到mplayer播放界面。
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

/*****************************************
功能描述：创建XML对象，并兼容IE，firefox等浏览器
参数类型：
返回描述：返回创建好的XML对象
*******************************************/
function createXHR()//
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