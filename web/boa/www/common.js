
var g_url;	//�����洢��ǰ����ַ

var SUNHOME_LOGIN_CMD = 1;

var SUNHOME_LED_SET_CMD = 101;		/*����led������*/
var SUNHOME_LED_GET_CMD	= 102;		/*��ȡled��״̬����*/

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
	var url = window.location.href;//��ȡ��������ַ

	var tmp_last_index = url.lastIndexOf("/");/*�������һ��'/'  */
	g_url = url.substr(0, tmp_last_index);
	/*ͨ��substr()�õ���url�ĵ�һ���ַ���
	temp_last_index֮���һ���ַ���������url="192.168.1.123/login.html";
	��ôg_url="192.168.1.123" ������ȫ�ֱ���g_url�洢�˵�ǰ��ַ */
	//alert("g_url=="+g_url);
}

/*****************************************
����������ͨ�����Ĳ������ı�window.location.href����ת����Ӧ����ҳ
�������ͣ�0��1��2��3��4��5
����������
*******************************************/
function jump_page(index)
{
	switch(index)
	{
		case 0:
		{
			window.location.href = g_url+"/home.html";/* ��ת����ҳ */
		}
		break;
	
		case 1:
		{
			window.location.href = g_url+"/led_control.html";//��ת���ƵĿ�����ҳ
		}
		break;

		case 2:
		{
			window.location.href = g_url+"/send_message.html";//��ת�����Ͷ�����ҳ
		}
		break;

		case 3:
		{
			window.location.href = g_url+"/home_humiture.html";//��ת����ȡ��ʪ����ҳ
		}
		break;

		case 4:
		{
			window.location.href = g_url+":8000";//��ת����Ƶ�����ҳ
		}
		break;
		
		case 5:
		{
				
			window.location.href = g_url+"/home_mplayer.html";//��ת��mplayer���Ž��档
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
��������������XML���󣬲�����IE��firefox�������
�������ͣ�
�������������ش����õ�XML����
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