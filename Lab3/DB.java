package db;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;        //数据库连接实例
import java.sql.DriverManager;     //数据库驱动管理类，调用其静态方法getConnection并传入数据库的URL获得数据库连接实例
import java.sql.Statement;         //操作数据库要用到的类，主要用于执行SQL语句
import java.util.ArrayList;
import java.util.Calendar;
import java.sql.ResultSet;         //数据库查询结果集
import java.sql.SQLException;
import java.util.Date;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.text.SimpleDateFormat;

@SuppressWarnings("unused")

public class DB {
	
	private static String DRIVER_MYSQL = "com.mysql.jdbc.Driver";    //MySQL JDBC驱动字符串
    private static String URL = "jdbc:mysql://localhost:3306/lab3";
    private static Statement stmt;
    private Connection connection = null;
    
	public String connect()                         
	{
		 try{
	            Class.forName(DRIVER_MYSQL);     //加载JDBC驱动
	            connection = DriverManager.getConnection(URL,"root","1234");   //创建数据库连接对象
	            stmt = connection.createStatement();       //创建Statement对象
	            return "connect!";
	     }
		 catch (Exception e){
			 e.printStackTrace();
		 }
		 return "Success";
	}
	
	public String close()
	{
		try{
			connection.close();
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return "Success close";
	}
	
	/* 登陆 传入用户名和密码,若成功则返回用户ID,失败则返回-1 */
	public int checkUser(String sid,String password)
	{
		String temp = sid + " and password='" + password + "'";
		int ID ;
		if(sid.length()==6) ID = findID(temp);
		else ID = findAID(temp);
		
		return ID;
	}
	/* 尝试注册 如果注册失败则返回-1，否则返回新用户的ID */
	public int signUp(String sid,String password)
	{	int count = findID(sid);
		if(count != -1)	//已有同学号用户
		{
			return -1;
		}
		String ins = "insert student value(" + sid + ",'" + password + "')" ; 
		System.out.println(ins);
		try{
			stmt.execute(ins);
		}
		catch (SQLException e){
			e.printStackTrace();
			return -1;
		}
		return Integer.parseInt(sid);
	}
	
	
	/* 下面函数作用是在student表中找到符合条件的用户个数  */
	public int findID(String sid)
	{
		int ID = -1;
		String temp = "select sid from student where sid=" + sid ;
		System.out.println(temp);
		
		try {
			ResultSet rs = stmt.executeQuery(temp);
			while(rs.next())
	        {
	        	ID = rs.getInt(1);
	        }
		} 
		catch (SQLException e){e.printStackTrace();}
		return ID;
	}
	/* 下面函数作用是在admin表中找到符合条件的用户个数  */
	public int findAID(String aid)
	{
		int ID = -1;
		String temp = "select aid from admin where aid=" + aid ;
		ResultSet rs;
		try {
			rs = stmt.executeQuery(temp);
			while(rs.next())
	        {
	        	ID = rs.getInt(1);
	        }
		} 
		catch (SQLException e){e.printStackTrace();}
		
		return ID;
	}
	public int signUpA(String aid,String password)
	{
		int count = findAID(aid);
		if(count != -1)	//已有同学号用户
		{
			return -1;
		}
		String ins = "insert admin value(" + aid + ",'" + password + "')" ; 
		try{
			stmt.execute(ins);
		}
		catch (SQLException e){
			e.printStackTrace();
		}
		return Integer.parseInt(aid);
	}
	
	public String findNameID(String name)
	{
		String result = "";
		String s = "select sid from inf where name='" + name + "'";
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
			{
				result +=  "~" + rs.getString(1); 
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		if(result.length()!=0) return result.substring(1);
		else return result;
	}
	public String findName(String sid)
	{
		String result = "";
		String s = "select name from inf where sid=" + sid;
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
			{
				result = rs.getString(1); 
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	public String searchInf(String name)
	{
		String result = "";
		
		String s = "select * from inf where name=%" + name + "%";
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
			{
				result += "~" + rs.getString(1) + ":" + rs.getString(2) + ":" + rs.getString(3) + ":" + rs.getString(4); 
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	public String readInf(String sid)
	{
		String result = "";
		
		String s = "select * from inf where sid=" + sid;
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
			{
				result += "~" + rs.getString(1) + ":" + rs.getString(2) + ":" + rs.getString(3) + ":" + rs.getString(4); 
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	public String findCollege(String cname)
	{
		String result = "";
		return result;
	}
	//按照学号添加好友 如果不存在该用户 则返回-1 否则返回用户学号
	public void addFriend(String sid, String fid)
	{
			String i1 = "insert friend values(" + sid + "," + fid + ")" ;
			String i2 = "insert friend values(" + fid + "," + sid + ")" ;
			try{
				stmt.execute(i1);
				stmt.execute(i2);
			}
			catch (Exception e){
				e.printStackTrace();
			}
	}
	public String showFriend(String sid)
	{
		String result = "";
		String f = "select sidb,name from friend join inf where sidb = sid and sida=" + sid;
		try {
			ResultSet rs = stmt.executeQuery(f);
			while(rs.next())
	        {
				result += "~";
	        	result += rs.getString(1);
	        	result += "-" ;
	        	result += rs.getString(2);
	        }
		} 
		catch (SQLException e){e.printStackTrace();}
		if(result.length()>0)return result.substring(1);
		else return "";
	}
	public int deleteFriend(String sid, String fid)
	{
		int a = 1;
		String i1 = "delete from friend where sida=" + sid + " and sidb =" + fid ;
		String i2 = "delete from friend where sida=" + fid + " and sidb =" + sid ;
		try{
			stmt.execute(i1);
			stmt.execute(i2);
		}
		catch (Exception e){
			a = -1;
			e.printStackTrace();
		}
		return a;
	}
	//发布公告
	public int oneNotice(String aid, String topic, String text,String cid)
	{
		int NID = count("notice") + 1;
		
		String i = "insert notice value(" + NID + ",'" + topic + "','" + text + "'," + aid + ")";
		String j = "insert nsend value(" + NID + "," + cid + ")";
		try{
			stmt.execute(i);
			stmt.execute(j);
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return NID;
	}
	public String getNotice(String nid)
	{
		String result = "";
		
		String i = "select topic,text,aid from notice where nid=" + nid;
		try{
			ResultSet rs = stmt.executeQuery(i);
			while(rs.next())
			{
				result = rs.getString(1) + "~" + rs.getString(2);
			}
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return result;
	}
	public String showNotice(String cid)
	{
		String result = "";
		//此处利用了view视图
		//noticeboard = select nid, topic, cid from notice join nsend using(nid) ;
		String q = "select * from noticeboard where cid=" + cid;
		try {
			ResultSet rs = stmt.executeQuery(q);
			while(rs.next())
	        {
				result = result + "~";
	        	result += rs.getString(1) + "-";
	        	result += rs.getString(2) ;
	        }
		} 
		catch (SQLException e){e.printStackTrace();}
		return result;
	}
	
	public int sendMessage(String sid,String tosid,String topic,String text)
	{
		if(findID(tosid)==0) return -1;
		
		int mid = count("message");
		String im = "insert message value(" + mid + ",'" + topic + "','" + text + "')";
		String s = "insert send value(" + sid + "," + mid + ")";
		String r = "insert recv value(" + tosid + "," + mid + ")";
		try{
			stmt.execute(im);
			stmt.execute(s);
			stmt.execute(r);
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return mid;
	}
	
	//recv or send 为0时表示收件
	public String readMessage(String mid,String rors)
	{
		String s;
		if(Integer.parseInt(rors)==1)
		{
			s = "select mid,topic,text,sid from message join recv using(mid) where mid=" + mid;
		}
		else
		{
			s = "select mid,topic,text,sid from message join send using(mid) where mid=" + mid;
		}
		String result = "";
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
	        {
	        	result = rs.getString(1) + "~" + rs.getString(2) + "~" + rs.getString(3) + "~" + rs.getString(4);
	        }
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return result;
	}
	public String checkSend(String sid)
	{
		String result = "";
		//view sendbox select * from message join send using(mid)
		String s = "select mid,topic from sendbox where sid=" + sid;
		
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
	        {
	        	result += "~";
	        	result += rs.getString(1) + ":" + rs.getString(2);
	        }
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return result;
	}
	public String myCID(String sid)
	{
		String result = "";
		String s = "select cid from inf where sid=" + sid;
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
	        {
	        	result += rs.getString(1);
	        }
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return result;
	}
	public String checkRecv(String sid)
	{
		String result = "";
		String s = "select a.sid, mid,topic,text from (select * from send join message using(mid)) a join recv r using(mid) where r.sid=" + sid ;
		
		try{
			ResultSet rs = stmt.executeQuery(s);
			while(rs.next())
	        {
	        	result += "~";
	        	result += rs.getString(1) + ":" + rs.getString(2) + ":" + rs.getString(3) + ":" + rs.getString(4);
	        }
		}
		catch (Exception e){
			e.printStackTrace();
		}
		return result;
	}
	
	public int deleteSend(String mid)
	{
		String d ="delete from send where mid=" + mid;
		try{
			stmt.execute(d);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	public int cleanSend(String sid)
	{
		String d ="delete from send where sid=" + sid;
		try{
			stmt.execute(d);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	public int deleteRecv(String mid)
	{
		String d ="delete from recv where mid=" + mid;
		try{
			stmt.execute(d);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	public int cleanRecv(String sid)
	{
		String d ="delete from recv where sid=" + sid;
		try{
			stmt.execute(d);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int fullInf(String sid, String cname, String name, String age)
	{
		int cid = findCID(cname);
		if(cid==-1) return -1;
		
		int record = count("inf where sid = " + sid);
		if(record==1)
		{
			String ins1 = "update inf set cid=" + cid + " where sid=" + sid;
			String ins2 = "update inf set name='" + name + "' where sid=" + sid;
			String ins3 = "update inf set age=" + age + "where sid=" + sid;
			try{
				stmt.execute(ins1);
				stmt.execute(ins2);
				stmt.execute(ins3);
			}
			catch (SQLException e){
				e.printStackTrace();
			}
		}
		else
		{
			String ins = "insert inf value(" + sid + "," + cid +",'" + name + "'," + age + ")";
			try{
				stmt.execute(ins);
			}
			catch (SQLException e){
				e.printStackTrace();
			}
		}
		return Integer.parseInt(sid);
	}

	private int findCID(String cname)
	{
		int ID = -1;
		String temp = "select cid from college where cname='" + cname + "'" ;
		ResultSet rs;
		try {
			rs = stmt.executeQuery(temp);
			while(rs.next())
	        {
	        	ID = rs.getInt(1);
	        }
		} 
		catch (SQLException e){e.printStackTrace();}
		
		return ID;
	}
	
	public void deleteStudent(String id)
	{
		String i = "delete from student where sid=" + id;
		String j = "delete from inf where sid=" + id;
		String k = "delete from send where sid=" + id;
		String m = "delete from recv where sid=" + id;
		String n = "delete from friend where sida=" + id + " or sidb=" + id;
		try
		{
			stmt.execute(j);
			stmt.execute(k);
			stmt.execute(m);
			stmt.execute(n);
			
			stmt.execute(i);
		}
		catch(SQLException e)
		{
			e.printStackTrace();
		}
	}
	/* private函数 请勿直接使用或者修改！！！
	 * 用来查询个数 已有部分语句
	 * 传入参数直接为表名 则可用于查询当前已有元组个数
	 * 传入参数如果包含when，则可以完成筛选
	 */
	public int count(String tname) {
		ResultSet cnt = null;
		int n = 0;
		String temp = "select count(*) from " + tname ;
    	try
        {
            cnt = stmt.executeQuery(temp);
            while(cnt.next())
            {
            	n = cnt.getInt(1);
            }
        } 
    	catch (SQLException e){e.printStackTrace();}
    	return n;
    }
	
	public String collegeGroup()
	{
		String result = "";
		String q = "select cid,cname,count(*) from college join inf using(cid) group by cid,cname";
		try
        {
            ResultSet rs = stmt.executeQuery(q);
            while(rs.next())
            {
            	result += "~" + rs.getString(1) + "-" + rs.getString(2) + "-" + rs.getString(3) ;
            }
        } 
    	catch (SQLException e){e.printStackTrace();}	
		if(result.length()>0)return result.substring(1);
		else return result;
	}
}

// create unique index sinf on inf(sid)
// create index ninf on inf(name)
