import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFDataFormat;
import org.apache.poi.ss.usermodel.*;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Hashtable;
import java.util.Scanner;

public class sqlite {
	static int Page = 0;
	static String Name = "";
	static String Filename = "";
	static Hashtable<String, table> database = new Hashtable();
	
    public static void main(String[] args) {
    	Scanner s = new Scanner(System.in);
    	String cmd;
    	 while(true) {
    		System.out.print(">>>");
         	cmd = s.nextLine();
         	readcmd(cmd);
         }
    }   
    
    public static void readcmd(String cmd) {
    	if(cmd.indexOf("open")>=0) {
    		String[] ss = cmd.split("\\s+");
    		Filename = ss[1];   //ÎÄµµÃû×Ö
    	}
    	else if(cmd.indexOf("get page")>=0) {
    		if(Filename == "") {
    			System.out.println("Please open xlsx file first");
    		}
    		String[] ss = cmd.split("\\s+");
    		String page = ss[2];
    		Page = Integer.getInteger(page);
    	}
    	else if(cmd.indexOf("create table")>=0) {
    		String[] ss = cmd.split("\\s+");
    		table t;
    		if(ss.length==2) {
    			t = new table(Filename);
    		}else {
    			t = new table(ss[2]);
    		}
    		t.Openfile(Filename, Page);
    		database.put(t.Name, t);
    	}
    	else if(cmd.indexOf("print table")>=0) {
    		String[] ss = cmd.split("\\s+");
    		String table = ss[2];
    		database.get(table).printout();
    	}
    }
    

}