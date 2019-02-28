package testCAD;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.event.MouseInputListener;
import javax.swing.filechooser.FileNameExtensionFilter;

import miniCAD.Shape;
import testCAD.Message;
import testCAD.Shapes;


class Listener implements MouseInputListener, ActionListener,MouseMotionListener ,MouseWheelListener{

	Shapes selected;
    Point nextpoint1;
    Point nextpoint2;
    Point original1 = new Point(0, 0);
    Point original2 = new Point(0, 0);
    Draw draw;
	
    public Listener(Draw draw) {
		this.draw = draw;
	}


	public Listener() {}


	private Shapes getSelected(Point p)
    {
        for(Shapes shapes: draw.shapes)
        {
            if(shapes.isSelected(p))
            {
                return shapes;
            }
        }
        return null;
    }
	
	@Override
	public void mouseClicked(MouseEvent e) {}

	@Override
	public void mouseEntered(MouseEvent e) {}

	@Override
	public void mouseExited(MouseEvent e) {}

	

	@Override
	public void mousePressed(MouseEvent e) {
	    nextpoint1=new Point(e.getX(),e.getY());
	    nextpoint2=new Point(e.getX(),e.getY());
	    switch (Message.state){
	    	case "select":
	           selected=getSelected(nextpoint1);
	           if(selected!=null){
	        	   original1=new Point(selected.p1.x,selected.p1.y);
	        	   original2=new Point(selected.p2.x,selected.p2.y);
	           }
	           break;
	    	case "line":
	    		draw.shapes.add(new Line(nextpoint1, nextpoint2));
	    		selected=null;
	    		break;
	    	case "square":
	    		draw.shapes.add(new Square(nextpoint1, nextpoint2));
	    		selected=null;
	    		break;
	    	case "circle":
	    		draw.shapes.add(new Circle(nextpoint1, nextpoint2));
	    		selected=null;
	    		break;
	    	case "delete":
	    		selected=getSelected(nextpoint1);
		        if(selected!=null){
		        	draw.shapes.remove(selected);
		        }
		        selected=null;
		        break;
	    	default:
	    		break;
	    }
	}
	
    private void operation(MouseEvent e)
    {
        if(Message.state.equals("idle")) {
            return;
        }
        
        if(Message.state.equals("delete")) {
        	Message.updateView();
            return;
        }
            
        if(!Message.state.equals("select") ) {
        	Shapes s=draw.shapes.get(draw.shapes.size()-1);
            s.p2.x=e.getX();
            s.p2.y=e.getY();
            Message.updateView();
        }
        else
        {
            if(selected!=null)
            {
                int deltaX=e.getX()-nextpoint1.x;
                int deltaY=e.getY()-nextpoint1.y;
                selected.p1.x=original1.x+deltaX;
                selected.p1.y=original1.y+deltaY;
                selected.p2.x=original2.x+deltaX;
                selected.p2.y=original2.y+deltaY;
                Message.updateView();
            }
        }
    }

	@Override
	public void mouseReleased(MouseEvent e) {
        operation(e);
	}

	@Override
	public void mouseDragged(MouseEvent e) {
        operation(e);
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		// TODO Auto-generated method stub
	}
	@Override
	public void actionPerformed(ActionEvent arg0) {
        String cmd=arg0.getActionCommand();
        switch (cmd)
        {
            case "select":
                Message.state="select";
                break;
            case "line":
            	Message.state="line";
                break;
            case "square":
            	Message.state="square";
                break;
            case "circle":
            	Message.state="circle";
                break;
            case "delete":
            	Message.state="delete";
                break;
            case "Gray":
            	Shapes s=draw.shapes.get(draw.shapes.size()-1);
            	s.color= Color.GRAY;
            	Message.updateView();
                break;
            case "White":
            	Shapes s1=draw.shapes.get(draw.shapes.size()-1);
            	s1.color= Color.WHITE;
            	Message.updateView();
                break;
            case "Blue":
            	Shapes s2=draw.shapes.get(draw.shapes.size()-1);
            	s2.color= Color.BLUE;
            	Message.updateView();
                break;
            case "Green":
            	Shapes s3=draw.shapes.get(draw.shapes.size()-1);
            	s3.color= Color.GREEN;
            	Message.updateView();
                break;
            case "Yellow": 
            	Shapes s4=draw.shapes.get(draw.shapes.size()-1);
            	s4.color= Color.YELLOW;
            	Message.updateView();
                break;
            case "Red":
            	Shapes s5=draw.shapes.get(draw.shapes.size()-1);
            	s5.color= Color.RED;
            	Message.updateView();
                break;
            case "Open":
                open();
                break;
            case "Save":
                save();
                break;
            default:
                Message.state="idle";
                break;
        }
			
		
	}
	
	

	private void save() {
        String fileName=JOptionPane.showInputDialog("please input a file name:");
        if(fileName==null||fileName.equals("")){
        	fileName = "a";
        }
        if(!fileName.endsWith(".cad")){
            fileName+=".cad";
        }

        JFileChooser saveFileChooser=new JFileChooser();
        saveFileChooser.setDialogTitle("Directory");
        saveFileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        int option=saveFileChooser.showOpenDialog(null);
        String path=null;
        if(JFileChooser.APPROVE_OPTION==option){
            path=saveFileChooser.getSelectedFile().getAbsolutePath();
        }
        
        try { //export
            ObjectOutputStream out=new ObjectOutputStream(new FileOutputStream(path+"\\"+fileName));
            out.writeObject(draw.shapes);
            out.close();
        }catch (IOException e){
            e.printStackTrace();
            JOptionPane.showMessageDialog(null,"Save failed!","Alert",JOptionPane.ERROR_MESSAGE);
        }
        JOptionPane.showMessageDialog(null,"Save successfully!","Success", JOptionPane.INFORMATION_MESSAGE);
		
	}

	private void open() {
        JFileChooser openFileChooser=new JFileChooser();
        openFileChooser.setDialogTitle("请选择文件路径");
        openFileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        FileNameExtensionFilter filter=new FileNameExtensionFilter("cad文件(*.cad)","cad");
        openFileChooser.setFileFilter(filter);

        int option=openFileChooser.showOpenDialog(null);
        String path=null;
        if(JFileChooser.APPROVE_OPTION==option) {
            path=openFileChooser.getSelectedFile().getAbsolutePath();
        }
        if(!path.endsWith("cad")){
            JOptionPane.showMessageDialog(null,"Please choose a cad file!","Alert", JOptionPane.ERROR_MESSAGE);
            return;
        }
        
        try { //import
            ObjectInputStream in=new ObjectInputStream(new FileInputStream(path));
            draw.shapes = (ArrayList<Shapes>)in.readObject();
            Message.updateView();
        }catch (Exception e){
            e.printStackTrace();
            JOptionPane.showMessageDialog(null,"Can not open the file!","Alert", JOptionPane.ERROR_MESSAGE);
        }
	}


	@Override
	public void mouseWheelMoved(MouseWheelEvent e) {
		//滚轮向前 缩小  滚轮向后 放大
		if( Message.state.equals("select") && selected!=null){
			selected.setSize(e.getWheelRotation());
			Message.updateView();
		}
	}
}
