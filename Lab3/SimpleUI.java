package db;

import java.text.SimpleDateFormat;

import java.util.Calendar;
import java.util.Scanner;

public class SimpleUI {
	
	static DB db1;
	static Scanner 	s = new Scanner(System.in); 
	static String	id;	  //id
	static String  	name; //学生姓名
	static String  	cid;
	static int 		type; //type=1为学生，=0为管理员
	
	public static void main(String[] args) {
		
		db1 = new DB();

		System.out.println(db1.connect());
				
		System.out.println("欢迎使用校内通信系统！今天是"+ getTime());

		signORlog();
		if(type==1)onlineStudent();
		else onlineAdmin();
		
		db1.close();
	}
	private static int signORlog()
	{
		while(true){
			System.out.println("已有账户登录请输入L，新用户注册请输入S：");
			String a = s.nextLine();
			if(a.equals("L")){
				login();
				return 0; }
			else if(a.equals("S")){
				signup();
				return 0; }
			else{
				System.out.println("错误指令！"); }
		}
	}
	
	private static void login()
	{
		while(true){
			System.out.print("账号：");
			id = s.next();
			System.out.print("密码：");
			String pw = s.next();
			
			int a = db1.checkUser(id,pw);
			if(a==-1) {
				System.out.println("错误信息！请重新输入！");
				continue;
			}
			else{
				if(id.length()==4) type = 0;
				else type = 1;
				return;
			}
		}
	}
	
	private static void signup()
	{
		while(true){
			String pw;
			while(true)
			{
				System.out.print("请输入账号(学生请使用6位学号)：");
				id = s.next();
				if(id.length()!=4 && id.length()!=6) {
					System.out.println("错误的账号！");
					continue;
				}
				else break;
			}
			while(true)
			{
				System.out.print("请输入密码：");
				pw = s.next();
				if(pw.length()==0) {
					System.out.println("密码不能为空！");
					continue;
				}
				else break;
			}
			
			int a;
			if(id.length()==6){
				a = db1.signUp(id,pw);
			}
			else a = db1.signUpA(id,pw);
			
			if(a==-1){
				System.out.println("已有同名用户！请尝试重新注册！");
				continue;
			}
			else{
				if(id.length()==4){
					type = 0;
					return ; }
				else{
					type = 1;
					//setInf();
					return ; }
			}
		}
	}
	private static void setInf()
	{	
		while(true)
		{
			System.out.print("请输入学院名：");
			String c = s.next();
			System.out.print("请输入姓名    ：");
			String n = s.next();
			System.out.print("请输入年龄    ：");
			String age = s.next();
			
			int a = db1.fullInf(id, c, n, age);
			if(a==-1){
				System.out.println("错误的学院名！");
				continue; }
			else {
				System.out.println("成功保存个人信息！");
				return; }
		}
	}
	private static void aboutFriend()
	{
		db1.close();
		System.out.println(db1.connect());
		System.out.println("我的好友");
		String result = db1.showFriend(id);
		if(result.length()==0) return ;
		String[] friends = result.split("~");
		for(int i=0;i<friends.length;i++)
		{
			String myf = friends[i] ;
			System.out.println(myf);
		}
	}

	private static void notice()
	{
		db1.close();
		System.out.println(db1.connect());
		String notices = db1.showNotice(cid);
		if(notices.length()==0){
			System.out.println("没有学院公告！");
			return ;
		}
		//System.out.println(notices);
		String[] m = notices.split("~");
		for(int i=1;i<m.length;i++)
		{
			System.out.println((i) + ". " + (m[i].split("-"))[1]);
		}
		while(true)
		{
			System.out.println("输入公告编号查看具体信息,输入0退出：");
			int a = s.nextInt();
			if(a==0) return;
			else if(a>=m.length){
				System.out.println("错误的公告编号！");
				continue;
			}
			else{
				String[] temp = m[a].split("-");
				String nid = temp[0];
				String n = db1.getNotice(nid);
				String[] tote = n.split("~");
				System.out.println("主题： " + tote[0]);
				System.out.println(tote[1]);
			}
		}
	}
	
	private static void findS()
	{
		db1.close();
		System.out.println(db1.connect());
		while(true){
			System.out.println("1:按照学号查询;2:按照姓名查询;3.退出;");
			int a;
			a = s.nextInt();
			
			if(a==3) return;
			else if(a==1){
				System.out.println("请输入学号：");
				String ida = s.next();
				
				String na = db1.findName(ida);
				if(na.length()==0){
					System.out.print("错误的学号");
					continue;
				}
				else{
					System.out.print(na + " 输入1添加对方为好友。其他数字退出");
					int mf = s.nextInt();
					if(mf==1){
						db1.addFriend(id, ida);
					}
				}	
			}
			else if(a==2){
				System.out.println("请输入姓名：");
				String ida = s.next();
				String na = db1.findNameID(ida);
				if(na.length()==0){
					System.out.print("不存在的用户！");
					continue;
				} 
				else{
					String[] idss = na.split("~");
					for(int i=0;i<idss.length;i++)
					{
						System.out.println(idss[i]);
					}
				}
			}
			else{
				System.out.print("错误命令！");
				continue;
			}	
		}
	}
	private static void sendBox()
	{
		db1.close();
		System.out.println(db1.connect());
		System.out.println("已发送内容：");
		String r = db1.checkSend(id);
		String[] rs = r.split("~");
		for(int i=1;i<rs.length;i++)
		{
			String[] temp = rs[i].split(":");
			System.out.println(i + temp[1]);
		}
		System.out.println("输入编号删除消息，输入0清空发件箱,，输入其他退出");
		int a = s.nextInt();
		if(a==0) db1.cleanSend(id);
		else if(a>=rs.length){
			return;
		}
		else{
			String[] m = rs[a].split(":");  
			db1.deleteSend(m[0]);
		}
	}
	private static void recvBox()
	{
		db1.close();
		System.out.println(db1.connect());
		System.out.println("已接收内容：");
		String r = db1.checkRecv(id);
		String[] rs = r.split("~");
		for(int i=1;i<rs.length;i++)
		{
			String[] temp = rs[i].split(":");
			System.out.println(i +"发送方: " + temp[0] + " 主题:" + temp[2]);
			System.out.println("内容：" + temp[3]);
		}
		System.out.println("输入编号删除消息，输入0清空收件箱，输入其他退出");
		int a = s.nextInt();
		if(a==0) db1.cleanRecv(id);
		else if(a>=rs.length){
			return;
		}
		else{
			String[] m = rs[a].split(":");  
			db1.deleteRecv(m[1]);
		}
	}
	private static void onlineStudent()
	{
		db1.close();
		System.out.println(db1.connect());
		name = db1.findName(id);
		System.out.println("欢迎登陆！" + name + "同学！");
		while(true){
			System.out.println("输入指令编号，进行操作：");
			System.out.println("1.修改个人信息");
			System.out.println("2.我的好友");
			System.out.println("3.查看学院公告");
			System.out.println("4.查询同学");
			System.out.println("5.发件箱");
			System.out.println("6.收件箱");
			System.out.println("7.发件");
			System.out.println("0.退出登录");
			
			int choice = s.nextInt();
			switch(choice){
			case 1:
				setInf();
				break;
			case 2:
				aboutFriend();				
				break;
			case 3:
				cid = db1.myCID(id);
				notice();
				break;
			case 4:
				findS();
				break;
			case 5:
				sendBox();
				break;
			case 6:
				recvBox();
				break;
			case 7:
				System.out.print("请输入收件人学号；");
				String rid = s.next();
				if(db1.findID(rid)==-1){
					System.out.println("错误的学号！");
					break;
				}
				else{
					String topic;
					while(true){
						System.out.print("主题：");
						topic = s.next();
						if(topic.length()!=0) break;
						System.out.println("主题不应该为空！");
					}
					System.out.print("内容：");
					String text = s.nextLine();
					text = s.nextLine();
					db1.sendMessage(id, rid, topic, text);
				}
				break;
			case 0:
				return;
			default:
				System.out.println("错误指令！");
			}
		}
	}
	
	private static void onlineAdmin()
	{
		db1.close();
		System.out.println(db1.connect());
		System.out.println("欢迎登陆！管理员" + id);
		
		while(true)
		{
			System.out.println("输入指令编号，进行操作：");
			System.out.println("1.查看学院信息");
			System.out.println("2.查看学院公告");
			System.out.println("3.发布学院公告");
			System.out.println("4.删除学生");
			System.out.println("0.退出登录");
			
			int choice = s.nextInt();
			switch(choice)
			{
			case 1:
				//此处一个分组查询
				String cg = db1.collegeGroup();
				String[] cs = cg.split("~");
				if(cg.length()==0){
					System.out.println("NO INFORMATION.");
				}
				for(int i=0;i<cs.length;i++)
				{
					String[] temp = cs[i].split("-");
					System.out.println("学院编号:" + temp[0] + " 学院名称:" + temp[1] + " 学院人数:" + temp[2]);
				}
				break;
			case 2:
				System.out.println("输入学院编号：");
				cid= s.next();
				if(db1.count("college where cid=" + cid)==0)
				{
					System.out.println("错误的学院编号！");
				}
				notice();
				break;
			case 3:
				System.out.println("输入学院编号：");
				cid= s.next();
				if(db1.count("college where cid=" + cid)==0)
				{
					System.out.println("错误的学院编号！");
				}
				String topic;
				while(true){
					System.out.print("主题：");
					topic = s.next();
					if(topic.length()!=0) break;
					System.out.println("主题不应该为空！");
				}
				System.out.println("内容：");
				String text = s.nextLine();
				text = s.nextLine();
				db1.oneNotice(id, topic, text,cid);
				break;
			case 4:
				System.out.print("输入需要删除的学生学号：");
				String aa = s.next();
				if((db1.findName(aa)).length()==0){
					System.out.println("错误的学生学号！"); }
				else{
					db1.deleteStudent(aa);
				}
				break;
			case 0:
				return;
			default:
				System.out.println("错误指令！");
			}
		}	
	}

	/* 获取时间 为八位 yyyyMMdd */
	private static String getTime() 
	{
		String date = new SimpleDateFormat("yyyyMMdd").format(Calendar.getInstance().getTime());
		return date;
	}
}
