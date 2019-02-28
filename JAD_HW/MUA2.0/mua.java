import java.util.*;
public class mua{
	public static Hashtable<String, String> v = new Hashtable();
	public static Hashtable<String, Integer> functions = new Hashtable();
	public static Hashtable<String, List<String>> lists = new Hashtable();
	//public static Vector v = new Vector();//store the objects
	public static List<String>  cmd= new ArrayList<String>();  //store the cmd
	public static void main(String []args) {
		functions.put("make", 2);
		functions.put("add", 2);
		functions.put("thing",1);
		functions.put("erase",1);
		functions.put("isname",1);
		functions.put("print",1);
		functions.put("read",0);
		functions.put("readlist",0);
		functions.put("add",2);
		functions.put("sub",2);
		functions.put("mul",2);
		functions.put("div",2);
		functions.put("mod",2);
		functions.put("eq",2);
		functions.put("gt",2);
		functions.put("lt",2);
	    functions.put("and",2);
	    functions.put("or",2);
	    functions.put("repeat",2);
	    functions.put("not",2);
	    functions.put("random",2);
	    functions.put("sqrt",1);
	    functions.put("output", 1);
	    functions.put("stop", 0);
		while(true) {
			System.out.print(">>>");
			Parse.Read();
			Parse.Interpret(cmd);
		}
		
	}

}