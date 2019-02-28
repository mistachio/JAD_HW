import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Hashtable;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFDataFormat;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.DateUtil;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

public class table {
	String Name;
	
	static Hashtable<Integer, ArrayList> lines;
	table(String name){
		this.Name = name;
		lines = new Hashtable();
		
	}
	
	public static void Openfile(String filename, int Page) {
		File excelFile = new File(filename);
        XSSFWorkbook wb = null;
        try {
            wb = new XSSFWorkbook(new FileInputStream(excelFile));
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        XSSFSheet sheet = wb.getSheetAt(Page);
        int columnNum = 0;
        if (sheet.getRow(0) != null) {
        	columnNum = sheet.getRow(0).getLastCellNum()
        			- sheet.getRow(0).getFirstCellNum();
        }
        if (columnNum > 0) {
        	int index = 0;
        	for (Row row : sheet) {
        		String[] singleRow = new String[columnNum];
        		ArrayList<value> setRow = new ArrayList();
        		int n = 0;
        		for (int i = 0; i < columnNum; i++) {
        			Cell cell = row.getCell(i, Row.CREATE_NULL_AS_BLANK);
        			value v;
        			String vs;
        			if (cell.getCellType()==Cell.CELL_TYPE_NUMERIC) {  //数字格式
        				if (DateUtil.isCellDateFormatted(cell)) {
        					SimpleDateFormat sdf = null;
        					if (cell.getCellStyle().getDataFormat() == HSSFDataFormat
        							.getBuiltinFormat("h:mm")) {
        						sdf = new SimpleDateFormat("HH:mm");
        					} else {// 日期
        						sdf = new SimpleDateFormat("yyyy-MM-dd");
        					}
        					Date date = cell.getDateCellValue();
        					vs = sdf.format(date);
        					v = new Char(vs);
        				} else {
        					cell.setCellType(Cell.CELL_TYPE_STRING);
        					String temp = cell.getStringCellValue();
        					// 判断是否包含小数点，如果不含小数点，则转换为int类，如果含小数点，则转换为real类
        					if (temp.indexOf(".") > -1) {
        						singleRow[n] = String.valueOf(new Double(temp))
        								.trim();
        						vs = singleRow[n];
        						v = new Real(vs);
        					} else {
        						singleRow[n] = temp.trim();
        						vs = singleRow[n];
        						v = new Int(vs);
        					}
        				}
        			} else {    //字符串
        				singleRow[n] = cell.getStringCellValue().trim();
        				vs = singleRow[n];
        				v = new Char(vs);
        			}
        			setRow.add(v);
        			n++;
        		}
        		lines.put(index, setRow);
        		index++;
        	}
       }
	}

	public void printout() {
		if(lines.size()>1) {
			ArrayList<value> line = lines.get(0);
			System.out.println("-- -- -- -- -- -- -- -- -- -- -- -- -- -- --");
			System.out.print("field name"+":\t");  //第一行：字段名(index=0的信息)
			for(value v: line) {
				System.out.print(v.val+"\t"+"\t");
			}
			System.out.println("");
			lines.remove(0);
			for(int index: lines.keySet()) {     //第二行开始是列表内容
				System.out.print(index+":\t"+"\t");
				ArrayList<value> line1 = lines.get(index);
				for(value v: line1) {
					System.out.print(v.val+"\t"+"\t");
				}
				System.out.println("");
			}
			System.out.println("-- -- -- -- -- -- -- -- -- -- -- -- -- -- --");
			System.out.print("keys"+"\t"+"\t");   //最后一行，输出各列的类型
			line = lines.get(1);
			for(value v: line) {
				System.out.print(v.getClass()+"\t");
			}
			System.out.println("");
		}
	}

}
